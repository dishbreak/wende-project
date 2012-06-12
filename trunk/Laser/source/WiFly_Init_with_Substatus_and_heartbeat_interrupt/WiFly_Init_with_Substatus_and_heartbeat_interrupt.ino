#include <Configuration.h>
#include <Debug.h>
#include <ParsedStream.h>
#include <WiFly.h>
#include <_Spi.h>
#include <TimerOne.h>
#include <Servo.h>

#define ON 1
#define OFF 0

// Arduino SPI pins
#define CS 10
#define MOSI 11
#define MISO 12
#define SCK 13
#define LED 4
#define LASER_PIN 2
#define AZ_SERVO_PIN 8
#define EL_SERVO_PIN 9

// Message Types
#define CONTROL_MSG 1
#define CONFIG_MSG 2

// Lengths
#define CONTROL_MSG_LENGTH 6
#define CONFIG_MSG_LENGTH 7

// Min/Max PWM (in ticks)
int AZ_MIN_PWM = 2000;
int AZ_MAX_PWM = 4000;
int EL_MIN_PWM = 2000;
int EL_MAX_PWM = 4000;

byte server[] = { 192, 168, 1, 100 }; // WENDE Laser Test Tool PC

Client client(server, 4440);

// Blink delay value
int blinkDelay = 500;

// Heartbeat ready to send
bool heartbeatReady = false;

// Interval between heartbeats (in microseconds)
// Use Timer1.setPeriod(period) to update this value
long heartbeatInterval = 5000000;

// Servos
Servo el_servo; 
Servo az_servo; 

// Interrupt that sets the heartbeatReady to true, telling the loop to send the message
void setHeartbeatStatus()
{
  heartbeatReady = true;
} 

// Process control message
void process_control_message(char control_msg_data[CONTROL_MSG_LENGTH-1])
{
  int laser_control;
  int az_control;
  int el_control;

  // Process laser control
  laser_control = control_msg_data[0];

  // Process AZ value
  az_control = (control_msg_data[1] * 256) + control_msg_data[2];

  // Process EL value
  el_control = (control_msg_data[3] * 256) + control_msg_data[4];

  // Write values
  digitalWrite( LASER_PIN, laser_control);
  az_servo.writeTicks( az_control );
  el_servo.writeTicks( el_control );

}

// Process config message
void process_config_message(char config_msg_data[CONFIG_MSG_LENGTH-1])
{

  int temp_heartbeat;
  
  // Check if value is greater than 0, indicating it should be updated

  // Min AZ PWM
  if (config_msg_data[0] > 0)
    AZ_MIN_PWM = config_msg_data[0];

  // Max AZ PWM
  if (config_msg_data[1] > 0)
    AZ_MAX_PWM = config_msg_data[1];

  // Min EL PWM
  if (config_msg_data[2] > 0)
    EL_MIN_PWM = config_msg_data[2];

  // Max EL PWM
  if (config_msg_data[3] > 0)
    EL_MAX_PWM = config_msg_data[3];

  // Get new heartbeat interval
  temp_heartbeat = (config_msg_data[4] * 256) + config_msg_data[5];

  // Heartbeat interval
  if (temp_heartbeat > 0)
    heartbeatInterval = (long)temp_heartbeat * 1000;



}

// Function used to strobe the on-board LED
void strobeLED(int strobeCount, int ledDelay)
{
  // Set the for loop based on the strobe count
  for( int Inx=0; Inx < strobeCount; Inx++ )
  {
    // Strobe the LED on then off
    digitalWrite(LED,HIGH); //LED on
    delay(ledDelay);
    digitalWrite(LED,LOW); //LED off
    delay(ledDelay);
  }
}

void setup()
{
  // Initialize SPI pins
  pinMode(MOSI, OUTPUT);
  pinMode(MISO, INPUT);
  pinMode(SCK,OUTPUT);
  pinMode(CS,OUTPUT);
  digitalWrite(CS,HIGH); //disable device 

  // Initialize timer1
  Timer1.initialize(heartbeatInterval);
  Timer1.attachInterrupt(setHeartbeatStatus);

  // Define the LED pin as an output
  pinMode(LED, OUTPUT);

  Serial.begin(9600);
  SpiSerial.begin();

  WiFly.begin();

  if (!WiFly.join("WENDE")) {
    Serial.println("Association failed.");
  }

  Serial.println("Associated!");

  delay(5000);

  if (client.connect())
  {
    Serial.println("connected");
  }
  else
  {
    Serial.println("connection failed");
  }

  pinMode( LASER_PIN, OUTPUT );
  pinMode( AZ_SERVO_PIN, OUTPUT );
  pinMode( EL_SERVO_PIN, OUTPUT );

  el_servo.attach( EL_SERVO_PIN );
  az_servo.attach( AZ_SERVO_PIN );
}

//Assume the data in is 5 characters
//First character is Error Flag
//Second character is Laser On/Off
//Third character is Az PWM
//Fourth character is Az PWM
//Fifth character is El PWM
//Sixth character is El PWM

void loop()
{
  //Counter for heartbeat message
  static int hb_counter = 0;

  //Index for receiving data
  int index;

  //From C3
  char message_type;
  char data[5] = {
    0  };
  static char laser_flag = 0;
  static char az_pwm[2] = {
    0  };
  static char el_pwm[2] = {
    0  };
  char control_message_data[CONTROL_MSG_LENGTH-1] = {
    0  };
  char config_message_data[CONFIG_MSG_LENGTH-1] = {
    0  };

  //To C3
  char return_string[6] = {
    0  };
  char return_error = 0;
  char return_laser = 0;

  while (true)
  {

    if (client.available())
    {
      Serial.println("Data available!!!");

      //laser_flag = client.read();
      //Serial.println(laser_flag);
      //az_pwm[0] = client.read();
      //Serial.println(az_pwm[0]);
      //az_pwm[1] = client.read();
      //Serial.println(az_pwm[1]);
      //el_pwm[0] = client.read();
      //Serial.println(el_pwm[0]);
      //el_pwm[1] = client.read();
      //Serial.println(el_pwm[1]);

      message_type = client.read();

      switch (message_type)
      {
      case CONTROL_MSG:
        for (index=0;index < CONTROL_MSG_LENGTH;index++)
        {
          control_message_data[index] = client.read();
        }
        process_control_message(control_message_data);
        break;
      case CONFIG_MSG:
        for (index=0;index < CONFIG_MSG_LENGTH;index++)
        {
          config_message_data[index] = client.read();
        }
        process_config_message(config_message_data);
        break;
      default:
        Serial.print("Invalid Message Type: ");
        Serial.println(message_type);
        break;
      }


    }
    else
    {
      Serial.println("Data not available");
    }

    //Send heartbeat every heartbeatInterval
    if (heartbeatReady)
    {
      //Send laser status back to C3
      return_laser = digitalRead(LASER_PIN);
      if (check_error_flag())
      {
        return_error = 1;
      }

      //Create return message
      return_string[0] = return_error;
      return_string[1] = return_laser;
      return_string[2] = az_pwm[0];
      return_string[3] = az_pwm[1];
      return_string[4] = el_pwm[0];
      return_string[5] = el_pwm[1];

      //Send the message back to C3
      client.write(return_string);

      //Reset heartbeat counter 
      heartbeatReady = false;
    }

  }
}

bool check_error_flag()
{
  //This is where you check for error flag conditions.
  //Currently, it's always returning false.
  return false;
}




