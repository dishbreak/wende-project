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
#define LED         4

// WENDE Laser Test Tool PC IP Address
byte server[] = { 192, 168, 1, 100 };
    
// Define client object
Client client(server, 4447);

// Blink delay value
int blinkDelay = 500;

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
  
  // Define the LED pin as an output
  pinMode(LED, OUTPUT);
  
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
  char data = 0; 
  if (client.available())
  {
    Serial.println("Data available!!");
    data = client.read();
    Serial.print(data);
  }
  else
  {
    Serial.println("Data not available!!");
  }
  
  // If the data value is not set to 0
  if( data != 0 )
  {
    // Strobe the LED the desired number of times
    strobeLED(data, blinkDelay);
  }
  
  // If we disconnect, try reconnecting
  //if (!client.connected())
  //{
    //client.connect();
    //Serial.println("Reconnecting");
  //}
}


