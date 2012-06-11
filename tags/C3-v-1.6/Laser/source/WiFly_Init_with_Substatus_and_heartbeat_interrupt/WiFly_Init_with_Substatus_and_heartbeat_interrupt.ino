#include <Configuration.h>
#include <Debug.h>
#include <ParsedStream.h>
#include <WiFly.h>
#include <_Spi.h>
#include <TimerOne.h>

#define ON 1
#define OFF 0

// Arduino SPI pins
#define CS 10
#define MOSI 11
#define MISO 12
#define SCK 13
#define LED 4

byte server[] = { 192, 168, 1, 100 }; // WENDE Laser Test Tool PC
    
Client client(server, 4440);

// Blink delay value
int blinkDelay = 500;

// Heartbeat ready to send
bool heartbeatReady = false;

// Interval between heartbeats (in microseconds)
// Use Timer1.setPeriod(period) to update this value
int heartbeatInterval = 5000000;

// Interrupt that sets the heartbeatReady to true, telling the loop to send the message
void setHeartbeatStatus()
{
	heartbeatReady = true;
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
  char data[5] = {0};
  static char laser_flag = 0;
  static char az_pwm[2] = {0};
  static char el_pwm[2] = {0};
  
  //To C3
  char return_string[6] = {0};
  char return_error = 0;
  char return_laser = 0;

  if (client.available())
  {
    Serial.println("Data available!!!");
		
	laser_flag = client.read();
	Serial.println(laser_flag);
	az_pwm[0] = client.read();
	Serial.println(az_pwm[0]);
	az_pwm[1] = client.read();
	Serial.println(az_pwm[1]);
	el_pwm[0] = client.read();
	Serial.println(el_pwm[0]);
	el_pwm[1] = client.read();
	Serial.println(el_pwm[1]);
	
	//data = client.read();
	//Serial.println(data);
    //Parse incoming data    
    //strncpy(laser_flag,data,0); 
    //strncpy(az_pwm,data,1); 
    //strncpy(el_pwm,data,3); 
  }
  else
  {
    Serial.println("Data not available");
  }
  //Send heartbeat every heartbeatInterval
  if (heartbeatReady)
  {
  	if (laser_flag != 0)
  	{
		//Send laser flag back to C3
    		return_laser = laser_flag;
  	}
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

    //strcpy(return_string,return_error);
    //strcat(return_string,return_laser);
    //strcat(return_string,az_pwm);
    //strcat(return_string,el_pwm);
    
    //Send the message back to C3
    client.write(return_string);

    //Reset heartbeat counter    
    heartbeatReady = false;
   }

}


bool check_error_flag()
{
  //This is where you check for error flag conditions.
  //Currently, it's always returning false.
  return false;
}

