#include "RoverMotorController.h"
#include "RoverInterfaces.h"

//used for encoders
volatile boolean bLeftFwd = true;
volatile boolean bRightFwd = true;

volatile unsigned long LM_count = 0; // Interrupt variables should be volatile (stored in RAM)
volatile unsigned long RM_count = 0;

unsigned long oldLMcount = 0;
unsigned long oldRMcount = 0;


/**
* RoverMotorRoutines: 
* Description: Main execution to control the motors for the rover
* @param left, left motor desired speed data - derived from the movement mode
* @param right, right motor desired speed data - derived from the movement mode
*/
void RoverMotorRoutines(motor_data* leftMotor, motor_data* rightMotor)
{
  static unsigned long lastMilli = 0;
  //***OVERFLOW Condition not handled, Consider updating***  
  if (millis()-lastMilli >= LOOP_TIME) {
       
    if(leftMotor->targetSpeed < 0)
    {
      leftMotor->targetSpeed = abs(leftMotor->targetSpeed);
      
      if(bLeftFwd)
      {
        bLeftFwd = false;
        LM_count = 0;
        oldLMcount = 0;
        leftMotor->cumError = 0;
        leftMotor->lastError = 0;
      }
    }
    else if(leftMotor->targetSpeed > 0 && !bLeftFwd)
    {
      bLeftFwd = true;
      LM_count = 0;
      oldLMcount = 0;
      leftMotor->cumError = 0;
      leftMotor->lastError = 0;
    }
    
    if(rightMotor->targetSpeed < 0)
    {
      rightMotor->targetSpeed = abs(rightMotor->targetSpeed);
      
      if(bRightFwd)
      {
        bRightFwd = false;
        
        RM_count = 0;
        oldRMcount = 0;
        rightMotor->cumError = 0;
        rightMotor->lastError = 0;
      }
    }
    else if(rightMotor->targetSpeed > 0 && !bRightFwd)
    {
      bRightFwd = true;
      RM_count = 0;
      oldRMcount = 0;
      rightMotor->cumError = 0;
      rightMotor->lastError = 0;
    }
    
    lastMilli = millis();
    leftMotor->measuredSpeed = calcLeftMotorSpeed();
    rightMotor->measuredSpeed = calcRightMotorSpeed();

    leftMotor->PWM_val = calcPWM(leftMotor);
    rightMotor->PWM_val = calcPWM(rightMotor);
    setRightMotor(bRightFwd, (rightMotor->targetSpeed!=0) * rightMotor->PWM_val);
    setLeftMotor(bLeftFwd, (leftMotor->targetSpeed!=0) * leftMotor->PWM_val);
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
* getLeftMotor: 
* Description: ISR to returns left motor encoder value
*/
unsigned long getLeftMotor()
{
  return(LM_count);
}

/**
* getRightMotor: 
* Description: ISR to returns right motor encoder value
*/
unsigned long getRightMotor()
{
  return(RM_count);
}

/**
* getMinMotor: 
* Description: returns motor encoder value that is smallest
*/
unsigned long getminMotor()
{
  if (LM_count > RM_count)
    return(RM_count);
  else
    return(LM_count);
}


void zero_all_counts()
{
  LM_count = 0;
  oldLMcount = 0;
  RM_count = 0;
  oldRMcount = 0;
}

/**
* calcRightMotorSpeed: 
* Description: Calculate measured speed based on encoder counts
* @return measured speed
*/
int calcRightMotorSpeed()
{
  int SpeedValue;
  SpeedValue = (RM_count-oldRMcount)*SPEED_MULTIPLIER;
  oldRMcount = RM_count;
  return SpeedValue;
}

/**
* calcLeftMotorSpeed: 
* Description: Calculate measured speed based on encoder counts
* @return measured speed
*/
int calcLeftMotorSpeed()
{
  int SpeedValue;
  SpeedValue = (LM_count-oldLMcount)*SPEED_MULTIPLIER;
  oldLMcount = LM_count;
  return SpeedValue;
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
