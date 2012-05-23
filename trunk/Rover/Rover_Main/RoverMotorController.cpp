#include "RoverMotorController.h"
#include "RoverInterfaces.h"

//used for encoders
volatile long LM_count = 0; // Interrupt variables should be volatile (stored in RAM)
volatile long RM_count = 0;
long oldLMcount = 0;
long oldRMcount = 0;


/**
* RoverMotorRoutines: 
* Description: Main execution to control the motors for the rover
* @param left, left motor desired speed data - derived from the movement mode
* @param right, right motor desired speed data - derived from the movement mode
*/
void RoverMotorRoutines(motor_data* leftMotor, motor_data* rightMotor)
{
  static unsigned long lastMilli = 0;
  
  if (millis()-lastMilli >= LOOP_TIME) {
    lastMilli = millis();
    leftMotor->measuredSpeed = calcLeftMotorSpeed();
    rightMotor->measuredSpeed = calcRightMotorSpeed();

    leftMotor->PWM_val = calcPWM(leftMotor);
    rightMotor->PWM_val = calcPWM(leftMotor);
    setRightMotor(rightMotor->PWM_val);
    setLeftMotor(leftMotor->PWM_val);
  }
}

/**
* incLeftMotor: 
* Description: ISR to increment left motor encoder value
*/
void incLeftMotor()
{
  LM_count++;
}

/**
* incRightMotor: 
* Description: ISR to increment right motor encoder value
*/
void incRightMotor()
{
  RM_count++;
}

/**
* calcRightMotorSpeed: 
* Description: Calculate measured speed based on encoder counts
* @return measured speed
*/
int calcRightMotorSpeed()
{
  return (RM_count-oldRMcount)*SPEED_MULTIPLIER;
}

/**
* calcLeftMotorSpeed: 
* Description: Calculate measured speed based on encoder counts
* @return measured speed
*/
int calcLeftMotorSpeed()
{
  return (LM_count-oldLMcount)*SPEED_MULTIPLIER;
}

//PWM speed
int calcPWM(motor_data* motorData)
{
  float PIDdelta = 0;
  int error = 0;
  error = abs(motorData->targetSpeed)-abs(motorData->measuredSpeed); // calculate error
  motorData->cumError += error;     // calculate cumulative error
  PIDdelta = motorData->Kp*error-motorData->Kd*(error-motorData->lastError)+motorData->Ki*motorData->cumError;
  motorData->lastError = error; // set last error
  return constrain(motorData->PWM_val+int(PIDdelta),0,255); // constrain the new value to range
}
