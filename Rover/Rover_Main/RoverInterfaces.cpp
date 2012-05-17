#include "RoverInterfaces.h"
#include "RoverMotorController.h"
#include "RoverDefaults.h"

/**
* setupInterfaces: 
* Description: Setup the input/output interfaces
*/
void setupInterfaces()
{
  //setup debug if enabled
  #if DEBUG == 1
    Serial.begin(9600);
  #endif
  //default analog setup
  analogReference(DEFAULT);
  
  //setup inputs
  pinMode(PHOTO_DETECTOR_PIN, INPUT);
  pinMode(AMBIENT_LIGHTING_PIN, INPUT);
  pinMode(ROVER_LED_PIN, INPUT);
  pinMode(ENCODER_R_PIN, INPUT);
  pinMode(ENCODER_L_PIN, INPUT);
  pinMode(INPUT_MODE_BIT0, INPUT);
  pinMode(INPUT_MODE_BIT1, INPUT);
  pinMode(INPUT_MODE_BIT2, INPUT);
  
  //setup outputs
  pinMode(MOTOR_R_PIN, OUTPUT);
  pinMode(MOTOR_L_PIN, OUTPUT);
  pinMode(MOTOR_R_EN, OUTPUT);
  pinMode(MOTOR_L_EN, OUTPUT);
  
  //set interrupt pull up resistors for consistancy
  digitalWrite(MOTOR_R_EN, HIGH);
  digitalWrite(MOTOR_L_EN, HIGH);
  
  //initialize interrupts
  attachInterrupt(0,              //timer0
                  incLeftMotor,   //ISR reference
                  FALLING);       //Falling edge
                  
  attachInterrupt(1,              //timer1
                  incRightMotor,  //ISR reference
                  FALLING);       //Falling edge
}

/**
* readLaserDetector: 
* Description: read the laser detector value
* @return current detector value
*/
int readLaserDetector()
{
  return analogRead(PHOTO_DETECTOR_PIN);
}

/**
* readLightSensor: 
* Description: read the light sensor value
* @return current lighting value
*/
int readLightSensor()
{
  return analogRead(AMBIENT_LIGHTING_PIN);
}

/**
* readRoverMode: 
* Description: read the rover mode
* @return current rover mode
*/
int readRoverMode()
{
  int bit0 = digitalRead(INPUT_MODE_BIT0);
  int bit1 = digitalRead(INPUT_MODE_BIT1);
  int bit2 = digitalRead(INPUT_MODE_BIT2);
  
  return (int) (bit0 + (bit1 << 1) + (bit2 << 2));
}
