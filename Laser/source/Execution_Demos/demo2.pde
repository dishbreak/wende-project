#include <Client.h>
#include <Configuration.h>
#include <Debug.h>
#include <ParsedStream.h>
#include <Server.h>
#include <SpiUart.h>
#include <WiFly.h>
#include <WiFlyDevice.h>
#include <_Spi.h>

#define ON 1
#define OFF 0

// Arduino SPI pins
#define CS         10
#define MOSI       11
#define MISO       12
#define SCK        13
#define LASER_PIN   4

// WENDE Laser Test Tool PC IP Address
byte server[] = { 192, 168, 1, 100 };
    
// Define client object
Client client(server, 4447);

// Blink delay value
int blinkDelay = 500;
int laserCount = 0;

// Function used to turn the laser on and off
void laserCall(int laserSwitch)
{
  // Set the for loop based on the strobe count
  if(laserSwitch == 1)
  {
    //Turn the Laser ON
    digitalWrite(LASER_PIN, ON);
    Serial.print("Laser On");
  }
  else
  {
    //Turn the Laser OFF
    digitalWrite(LASER_PIN, OFF);
    Serial.print("Laser Off");
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
  
  // Define the LED pin as an output
  pinMode(LASER_PIN, OUTPUT);
  
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
  int data = 0;
  static int msgLength[4] = {0};
  static int azPWM[4] = {0};
  static int elPWM[4] = {0};
  static int laser_flag[4] = {0};
  
  if (client.available())
  {
    Serial.println("Data available!!");
    
    msgLength[0] = client.read();
    msgLength[1] = client.read();
    msgLength[2] = client.read();
    msgLength[3] = client.read();

    azPWM[0] = client.read();
    azPWM[1] = client.read();
    azPWM[2] = client.read();
    azPWM[3] = client.read();
    
    elPWM[0] = client.read();
    elPWM[1] = client.read();
    elPWM[2] = client.read();
    elPWM[3] = client.read();
    
    laser_flag[0] = client.read();
    laser_flag[1] = client.read();
    laser_flag[2] = client.read();
    laser_flag[3] = client.read();
    laserCall(laser_flag[0]);
  }
  else
  {
    Serial.println("Data not available!!");
  }
}


