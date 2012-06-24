#include <Configuration.h>
#include <Debug.h>
#include <ParsedStream.h>
#include <WiFly.h>
#include <_Spi.h>
#include <Servo.h>

#define ON 1
#define OFF 0

// Arduino SPI pins
#define CS         10
#define MOSI       11
#define MISO       12
#define SCK        13
#define LASER_PIN   4
#define AZ_SERVO_PIN 8
#define EL_SERVO_PIN 9

// Message Types
#define COMMAND_MSG 3
#define STATUS_MSG 4
#define CONFIG_MSG 5

// Lengths
#define COMMAND_MSG_LENGTH 12
#define CONFIG_MSG_LENGTH 20

// Error types
#define AZ_ERROR 0b00000001
#define EL_ERROR 0b00000010
#define LASER_ERROR 0b00000100

// Min/Max PWM (in ticks)
int AZ_MIN_PWM = 2350;
int AZ_MAX_PWM = 3850;
int EL_MIN_PWM = 2300;
int EL_MAX_PWM = 3625;

// WENDE Laser Test Tool PC IP Address
byte server[] = { 192, 168, 1, 100 };

// Define client object
Client client(server,4447);

// Define KPP Timers
long kppTime1 = 0;
long kppTime2 = 0;

// Error flag
char return_error = 2;

// Interval between heartbeats (in milliseconds)
int heartbeatInterval = 5000;

// Servos
Servo el_servo; 
Servo az_servo; 

// Process command message
void process_command_message(int command_msg_data[CONFIG_MSG_LENGTH+1])
{
  int laser_control;
  int az_control;
  int el_control;

  // Process AZ value
  az_control = (command_msg_data[1]<<24) + (command_msg_data[2]<<16) + (command_msg_data[3]<<8) + command_msg_data[4];

  // Process EL value
  el_control = (command_msg_data[5]<<24) + (command_msg_data[6]<<16) + (command_msg_data[7]<<8) + command_msg_data[8];

  // Process laser control
  laser_control = (command_msg_data[9]<<24) + (command_msg_data[10]<<16) + (command_msg_data[11]<<8) + command_msg_data[12];

//  Serial.println("Cmd-Az:");
//  Serial.print(az_control);
//  Serial.println("Cmd-El:");
//  Serial.print(el_control);
//  Serial.println("Cmd-Lsr:");
//  Serial.print(laser_control);

  kppTime2 = millis();
  Serial.print("KPP6-Processing Time: ");
  Serial.print(kppTime2-kppTime1, DEC);
  Serial.println("ms");
  // Check calues are valid and write them if they are
  if( az_control < AZ_MIN_PWM )
  {
    az_servo.writeTicks(AZ_MIN_PWM);
    return_error = 0;
  }
  else if( az_control > AZ_MAX_PWM )
  {
    az_servo.writeTicks(AZ_MAX_PWM);
    return_error = 0;
  }
  else
  {
    az_servo.writeTicks( az_control );
  }
	
  if( el_control < EL_MIN_PWM )
  {
    el_servo.writeTicks(EL_MIN_PWM);
    return_error = 3;
  }
  else if( el_control > EL_MAX_PWM )
  {
    el_servo.writeTicks(EL_MAX_PWM);
    return_error = 3;
  }
  else
  {
    el_servo.writeTicks( el_control );
  }
	
  if ((laser_control == 0) || (laser_control == 1))
  {
    digitalWrite( LASER_PIN, laser_control);
  }
  else
  {
    return_error = 5;
  }
}

// Process config message
void process_config_message(int config_msg_data[CONFIG_MSG_LENGTH+1])
{
  int temp_heartbeat;
  int temp_min_az;
  int temp_min_el;
  int temp_max_az;
  int temp_max_el;
  
  // Get min/max AZ/EL Servo PWM values
  temp_min_az = (config_msg_data[1]<<24) + (config_msg_data[2]<<16) + (config_msg_data[3]<<8) + config_msg_data[4];
  temp_max_az = (config_msg_data[5]<<24) + (config_msg_data[6]<<16) + (config_msg_data[7]<<8) + config_msg_data[8];
  temp_min_el = (config_msg_data[9]<<24) + (config_msg_data[10]<<16) + (config_msg_data[11]<<8) + config_msg_data[12];
  temp_max_el = (config_msg_data[13]<<24) + (config_msg_data[14]<<16) + (config_msg_data[15]<<8) + config_msg_data[16];

  // Get status/heartbeat message frequency (in ms)
  temp_heartbeat = (config_msg_data[17]<<24) + (config_msg_data[18]<<16) + (config_msg_data[19]<<8) + config_msg_data[20];

  // Set values
  AZ_MIN_PWM = temp_min_az;
  AZ_MAX_PWM = temp_max_az;
  EL_MIN_PWM = temp_min_el;
  EL_MAX_PWM = temp_max_el;
  heartbeatInterval = temp_heartbeat;
  
//  Serial.println(AZ_MIN_PWM);
//  Serial.println(AZ_MAX_PWM);
//  Serial.println(EL_MIN_PWM);
//  Serial.println(EL_MAX_PWM);
//  Serial.println(heartbeatInterval);
}

void setup()
{
  // Initialize SPI pins
  pinMode(MOSI, OUTPUT);
  pinMode(MISO, INPUT);
  pinMode(SCK,OUTPUT);
  pinMode(CS,OUTPUT);
  digitalWrite(CS,HIGH); //disable device 
  
  // Define the LED pin as an output
  pinMode(LASER_PIN, OUTPUT);
  pinMode( AZ_SERVO_PIN, OUTPUT );
  pinMode( EL_SERVO_PIN, OUTPUT );

  el_servo.attach( EL_SERVO_PIN );
  az_servo.attach( AZ_SERVO_PIN );

  // Init the Servo Positions
  az_servo.writeTicks(3070);
  el_servo.writeTicks(3290);
  
  Serial.begin(9600);
  SpiSerial.begin();

  delay(5000);

  if (client.connect())
  {
    Serial.println("connected");
  }
  else
  {
    Serial.println("connection failed");
  }
}

void loop()
{
  // Heartbeat timers
  static long heartbeatTimer1 = 0;
  static long heartbeatTimer2 = 0;
  
  // Reset KPP timers
  kppTime1 = 0;
  kppTime2 = 0;

  //From C3
  char message_type = 0;
  int msg_size = 0;
  char command_message_data[COMMAND_MSG_LENGTH] = {0};
  char config_message_data[CONFIG_MSG_LENGTH] = {0};
  int msg_in[CONFIG_MSG_LENGTH+1] = {0};

  //To C3
  static char return_string[17] = {0};
  
//  while(true)
//  {
   
    int data = 0;
    if (client.available())
    {
      kppTime1 = millis();
      Serial.println("Data available");

      // Discard message size
      msg_size = (client.read()<<24) + (client.read()<<16) + (client.read()<<8) + client.read();
//      Serial.println("MsgSize: ");
//      Serial.println(msg_size,DEC);

      for(int lnInx = 0; lnInx<msg_size+1; lnInx++)
      {
        while(!client.available()){}
        msg_in[lnInx] = client.read();
//        data = client.read();
//        Serial.println(data,DEC);
      }

//    Serial.print("MsgTyp: ");
//    Serial.println(msg_in[0], DEC);

      switch (msg_in[0])
      {
        case COMMAND_MSG:
          process_command_message(msg_in);
          break;
        case CONFIG_MSG:
          process_config_message(msg_in);
          break;
        default:
          Serial.print("Invalid Message Type: ");
          Serial.println(msg_in[0],DEC);
          break;
        }
    }
    else
    {
      //Serial.println("Data not available!!");
    }
    
    heartbeatTimer2 = millis();
	
    //Send heartbeat every heartbeatInterval
    if ((heartbeatTimer2 - heartbeatTimer1) >= heartbeatInterval)
    {
      //Serial.println("Sending heartbeat...");
      //Create return message
      return_string[0] = 0;
      return_string[1] = 0;
      return_string[2] = 0;
      return_string[3] = 12;
  	  
      // Type
      return_string[4] = 4;

      int AzTicks = az_servo.readTicks();
      // Azimuth value
      return_string[5] = 0;
      return_string[6] = 0;
      return_string[7] = (AzTicks & 0x0000FF00) >> 8;
      return_string[8] = AzTicks & 0x000000FF;

      int ElTicks = el_servo.readTicks();
      // Elevation value
      return_string[9] = 0;
      return_string[10] = 0;
      return_string[11] = (ElTicks & 0x0000FF00) >> 8;
      return_string[12] = ElTicks & 0x000000FF;
  	  
      // Laser status
      return_string[13] = 0;
      return_string[14] = 0;
      return_string[15] = 0;
      return_string[16] = return_error;
  
      //Send the message back to C3
      //client.write(return_string);
      for(int lnI = 0; lnI < 17; lnI++)
      {
        client.write(return_string[lnI]);
      }
	  
      heartbeatTimer1 = heartbeatTimer2;
             	  
      //Reset error status
      return_error = 2;
    }
  //}

  if(!client.connected())
  {
    //client.stop();
    client.connect();
  }
}

