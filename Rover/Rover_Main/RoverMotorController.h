/**
*	RoverMotorController:
*		-Control the rover motors
*		-Interrupt Service Routines (ISR) for encoders
*	Author: Rover Team
*	Modified: 5/22/2012
*/

#ifndef RoverMotorController_h
#define RoverMotorController_h

#include "Arduino.h"

//defines
//loop time is far too slow...caused rover to waggle at 100
//dropped to 25
#define LOOP_TIME  25
#define SPEED_MULTIPLIER  ((1000/(3*53))*(204/LOOP_TIME))



//motor data structure
//Laser Detection data struct
typedef struct __motor_data {
  int PWM_val;
  int targetSpeed;
  int measuredSpeed;
  int cumError;
  int lastError;
  float Kp; // proportional gain value
  float Ki; // integrative gain value
  float Kd; // derivative gain value  
} motor_data;

//main rover motor controller routines
void RoverMotorRoutines(motor_data* leftMotor, motor_data* rightMotor);

//left/right encoder ISR
void incLeftMotor();
void incRightMotor();

//left/right motor speeds
int calcRightMotorSpeed();
int calcLeftMotorSpeed();

//Zero all counters
void zero_all_counts();

//PWM speed
int calcPWM(motor_data* motorData);

unsigned long getRightMotor();
unsigned long getLeftMotor();
unsigned long getminMotor();

#endif

