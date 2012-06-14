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
#define AZ_SERVO_PIN 5
#define EL_SERVO_PIN 6

// Message Types
#define COMMAND_MSG 4
#define HEARTBEAT_MSG 5
#define CONFIG_MSG 6

// Lengths
#define COMMAND_MSG_LENGTH 12
#define CONFIG_MSG_LENGTH 20

// Error types
#define AZ_ERROR 0b00000001
#define EL_ERROR 0b00000010
#define LASER_ERROR 0b00000100

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

// Error flag
char return_error = 0;

// Interval between heartbeats (in milliseconds)
// Use Timer1.setPeriod(period) to update this value
int heartbeatInterval = 5000;

// Servos
Servo el_servo; 
Servo az_servo; 

// Interrupt that sets the heartbeatReady to true, telling the loop to send the message
void setHeartbeatStatus()
{
  heartbeatReady = true;
} 

// Process command message
void process_command_message(char command_msg_data[COMMAND_MSG_LENGTH])
{
  int laser_control;
  int az_control;
  int el_control;

  // Process AZ value
  az_control = ((int)command_msg_data[2] * 256) + command_msg_data[3];

  // Process EL value
  el_control = ((int)command_msg_data[6] * 256) + command_msg_data[7];

  // Process laser control
  laser_control = command_msg_data[11]; 
  
  // Check calues are valid and write them if they are
  if ((az_control >= AZ_MIN_PWM) && (az_control <= AZ_MAX_PWM))
	az_servo.writeTicks( az_control );
  else
	return_error = return_error | AZ_ERROR;
	
  if ((el_control >= EL_MIN_PWM) && (el_control <= EL_MAX_PWM))
	el_servo.writeTicks( el_control );
  else
	return_error = return_error | EL_ERROR;
	
  if ((laser_control == 0) || (laser_control == 1))
	digitalWrite( LASER_PIN, laser_control);
  else
	return_error = return_error | LASER_ERROR;
}

// Process config message
void process_config_message(char config_msg_data[CONFIG_MSG_LENGTH-1])
{

  int temp_heartbeat;
  int temp_min_az;
  int temp_min_el;
  int temp_max_az;
  int temp_max_el;
  
  // Check if value is greater than 0, indicating it should be updated

  // Get temp values
    temp_min_az = ((int)config_msg_data[2] * 256) + config_msg_data[3];
    temp_min_az = ((int)config_msg_data[6] * 256) + config_msg_data[7];
    temp_min_az = ((int)config_msg_data[10] * 256) + config_msg_data[11];
    temp_min_az = ((int)config_msg_data[14] * 256) + config_msg_data[15];
    temp_heartbeat = ((int)config_msg_data[18] * 256) + config_msg_data[19];

  // Set values
    AZ_MIN_PWM = temp_min_az;
	AZ_MAX_PWM = temp_max_az;
	EL_MIN_PWM = temp_min_el;
	EL_MAX_PWM = temp_max_el;
    heartbeatInterval = temp_heartbeat;
	Timer1.setPeriod(heartbeatInterval * 1000);

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
  Timer1.initialize(heartbeatInterval*1000);
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

void loop()
{

  //Index for receiving data
  int index;

  //From C3
  char message_type;
  char command_message_data[COMMAND_MSG_LENGTH] = {0};
  char config_message_data[CONFIG_MSG_LENGTH] = {0};

  //To C3
  char return_string[17] = {0};

  while (true)
  {

    if (client.available())
    {
      Serial.println("Data available!!!");
	  
	  // Discard message size
	  client.read();
	  client.read();
	  client.read();
	  client.read();
	  
      message_type = client.read();

      switch (message_type)
      {
      case COMMAND_MSG:
        for (index=0;index < COMMAND_MSG_LENGTH;index++)
        {
          command_message_data[index] = client.read();
		  //Serial.println(command_message_data[index],DEC);
        }
        process_command_message(command_message_data);
        break;
      case CONFIG_MSG:
        for (index=0;index < CONFIG_MSG_LENGTH;index++)
        {
          config_message_data[index] = client.read();
		  //Serial.println(command_message_data[index],DEC);
        }
        process_config_message(config_message_data);
        break;
      default:
        Serial.print("Invalid Message Type: ");
        Serial.println(message_type,DEC);
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
      //Create return message
	  // Size
	  return_string[0] = 0;
	  return_string[1] = 0;
	  return_string[2] = 0;
	  return_string[3] = 17;
	  
	  // Type
	  return_string[4] = HEARTBEAT_MSG;
	  
	  // Azimuth value
	  return_string[5] = 0;
	  return_string[6] = 0;
	  return_string[7] = (char)(digitalRead(AZ_SERVO_PIN) >> 8);
	  return_string[8] = (char)(digitalRead(AZ_SERVO_PIN) & 0x00FF);
	  
	  // Elevation value
	  return_string[9] = 0;
	  return_string[10] = 0;
	  return_string[11] = (char)(digitalRead(EL_SERVO_PIN) >> 8);
	  return_string[12] = (char)(digitalRead(EL_SERVO_PIN) & 0x00FF);
	  
	  // Laser status
	  return_string[13] = 0;
	  return_string[14] = (char)(digitalRead(LASER_PIN));
	  
	  // Error status
	  return_string[15] = 0;
	  return_string[16] = return_error;

      //Send the message back to C3
      client.write(return_string);

      //Reset heartbeat counter 
      heartbeatReady = false;
	  
	  //Reset error status
	  return_error = 0;
    }

  }
}



