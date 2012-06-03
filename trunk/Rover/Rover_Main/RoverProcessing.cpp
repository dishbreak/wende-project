#include "Arduino.h"
#include "RoverInterfaces.h"
#include "RoverProcessing.h"
#include "RoverLaserDetection.h"
#include "RoverUI.h"
#include "RoverMovement.h"
#include "RoverMotorController.h"
#include "RoverDemo.h"

//left and right motors
motor_data leftMotor;
motor_data rightMotor;

void RoverProcessingRoutines()
{
  static int roverState = 0;
  static boolean bSuccess = false;
  static boolean bMovementComplete = false;
  switch(roverState)
  {
    case ROVER_INIT_STATE:
      digitalWrite(ROVER_LED_PIN,bSuccess);
      //init rover here
      //Testing****
      //if(RoverUIRoutines())
      {
        roverState = ROVER_READY_STATE;
        //Testing****
        //roverMode = INPUT_SLOW_MODE;
        //roverMode = INPUT_FAST_MODE;
        //roverMode = INPUT_SPIRAL_MODE;
        //roverMode = INPUT_PASS_THROUGH_MODE;
        roverMode = INPUT_CRAWL_AND_STOP_MODE;
      }
      break;
      
    case ROVER_READY_STATE:
      
      //perform rover movement mode here
      //also check for failure here
      //if deemed a failure stop here and move to done state
      bMovementComplete = RoverMovementRoutines(roverMode, &leftMotor, &rightMotor);
      
      //perform laser detection check
      bSuccess = DetectionProcessing();
      
      if(bSuccess || bMovementComplete)
      {
        roverState = ROVER_DONE_STATE;
        //kill the motors
        leftMotor.targetSpeed = 0;
        rightMotor.targetSpeed = 0;
      }
      
      //perform motor controller logic
      RoverMotorRoutines(&leftMotor, &rightMotor);
        
      break;
      
    case ROVER_DONE_STATE:
//      Serial.println("Processing Done");
      //we are done with this trial
      //light LED according to success/failure
      digitalWrite(ROVER_LED_PIN,bSuccess);
      
      //perform motor controller logic
      //continued to allow for stopping without harming motors
      RoverMotorRoutines(&leftMotor, &rightMotor);
      
      break;
  }
}

