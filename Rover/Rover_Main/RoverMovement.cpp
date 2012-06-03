#include "RoverMovement.h"
#include "RoverMotorController.h"
#include "RoverInterfaces.h"

/**
* RoverMovementRoutines: 
* inputs: int mode - The current movement mode
*         motor_data* leftMotor - current paramaters for the left motor (struct)
*         motor_data* rightMotor - current paramaters for the right motor (struct)
* outputs: leftMotor and rightMotor structs updated
* Description: Main execution of rove r movement modes
* @return whether movement is complete or not...complete means possible failure.
* Limitations: After running for 49.7 days the millis() function will overflow and 
* cause failure.
* Assumptions: Only one movement mode will be run between re-setting the processor.
*/
boolean RoverMovementRoutines(int mode, motor_data* leftMotor, motor_data* rightMotor) {
	
	static int init = 1; // if 1 we initialize function
	static unsigned long spin_time = 0; // time to spin in place (in ms)
	unsigned long move_time; // time since movement start (ms)
	static unsigned long move_start; // initial movement time (ms)
	unsigned long seed; // seed for the PRNG
	//Create spiral parameter variables
	static int Xspiral = 0;
        static boolean First_Mode_Entry = true;
	const int SpiralTightness = 200;
	
	
	if (init == 1) {
		init = 0; // only init once
		
		Xspiral = 0; //initalize for spiral mode
		
		// if we are in a mode that needs to spin, generate random number, else set to 0
		if(mode == INPUT_SPIRAL_MODE || mode == INPUT_PASS_THROUGH_MODE){
			spin_time = 0;
		}
		else{
			seed = (unsigned long)readLightSensor() + (unsigned long)readLaserDetector() + 
					(unsigned long)mode + millis() + micros(); // attempt to get some randomness...
			randomSeed(  seed  ); 
			spin_time = random(1, 20); // random number between 1 and 20 seconds 
			spin_time *= 1000; // convert to ms
		}
		
		// initialize motor struct parameters
		leftMotor->cumError = 0;
		leftMotor->lastError = 0;
		rightMotor->cumError = 0;
		rightMotor->lastError = 0;
		
		move_start = millis(); //get the current time and save it to track movement time (in ms)
	}
	
	
	//*** OVERFLOWs after 49 days, Consider updating***
	move_time = millis() - move_start; // compute the current time spent moving
	/* debug distance print
        Serial.print("Motor Distance ");	
        Serial.print(getRightMotor());
        Serial.print(" - ");
        Serial.println(getLeftMotor());	
	*/
	//if we are in a mode that needs to spin and we have not spun long enough
	if ((mode == INPUT_FAST_MODE || mode == INPUT_SLOW_MODE || mode == INPUT_CRAWL_AND_STOP_MODE) && (move_time < spin_time)) {
		// Left Motor
		leftMotor->Kp = fast_Kp; // proportional gain value
		leftMotor->Ki = fast_Ki; // integrative gain value
		leftMotor->Kd = fast_Kd; // derivative gain value
		leftMotor->targetSpeed = fast_speed;
		//Right Motor
		rightMotor->Kp = fast_Kp; // proportional gain value
		rightMotor->Ki = fast_Ki; // integrative gain value
		rightMotor->Kd = fast_Kd; // derivative gain value
		rightMotor->targetSpeed = -fast_speed;
	}
	
	// Spiral Mode
	else if (mode == INPUT_SPIRAL_MODE) {
		if (mode_over(getminMotor(), mode) == 0 || First_Mode_Entry){
			// set motor parameters for spiral
			// Left Motor
			leftMotor->Kp = fast_Kp; // proportional gain value
			leftMotor->Ki = fast_Ki; // integrative gain value
			leftMotor->Kd = fast_Kd; // derivative gain value
			leftMotor->targetSpeed = fast_speed;
			//Right Motor
			rightMotor->Kp = fast_Kp; // proportional gain value
			rightMotor->Ki = fast_Ki; // integrative gain value
			rightMotor->Kd = fast_Kd; // derivative gain value
			// compute the new speed for the  right wheel
			rightMotor->targetSpeed = (int)(127*atan((double)Xspiral/(double)SpiralTightness));
			
			Xspiral++; //increment spiral parameter for next run though
		
		}
		// time over, shutdown motors
		else{
			rightMotor->targetSpeed = 0;
			leftMotor->targetSpeed = 0;
			return 1;
		}
                First_Mode_Entry = false;
	}
	
	//move straight at a fast speed for fast mode
	else if (mode == INPUT_FAST_MODE) {
		if (mode_over(getminMotor(), mode) == 0 || First_Mode_Entry){
			// Left Motor
			leftMotor->Kp = fast_Kp; // proportional gain value
			leftMotor->Ki = fast_Ki; // integrative gain value
			leftMotor->Kd = fast_Kd; // derivative gain value
			leftMotor->targetSpeed = fast_speed;
			//Right Motor
			rightMotor->Kp = fast_Kp; // proportional gain value
			rightMotor->Ki = fast_Ki; // integrative gain value
			rightMotor->Kd = fast_Kd; // derivative gain value
			rightMotor->targetSpeed = fast_speed;
		}
		// time over, shutdown motors
		else{
			rightMotor->targetSpeed = 0;
			leftMotor->targetSpeed = 0;
			return 1;
		}
                First_Mode_Entry = false;
	}
	
	// else, we are in a mode that requires straight, slow movement (slow mode, crawl&stop, pass through)
	else {
		if(mode_over(getminMotor(), mode) == 0 || First_Mode_Entry){
			// Left Motor
			leftMotor->Kp = slow_Kp; // proportional gain value
			leftMotor->Ki = slow_Ki; // integrative gain value
			leftMotor->Kd = slow_Kd; // derivative gain value
			leftMotor->targetSpeed = slow_speed;
			//Right Motor
			rightMotor->Kp = slow_Kp; // proportional gain value
			rightMotor->Ki = slow_Ki; // integrative gain value
			rightMotor->Kd = slow_Kd; // derivative gain value
			rightMotor->targetSpeed = slow_speed;		
		}
		// time over, shutdown motors
		else{
			rightMotor->targetSpeed = 0;
			leftMotor->targetSpeed = 0;
			return 1;
		}
                First_Mode_Entry = false;
	}

    return 0;
}

/********************************************************************
name: 	mode_over
inputs: unsigned long move_time - time in ms the rover has spent moving
						not including in place spin
		int mode - the current movement mode the rovers is in

output: int - 0 if the trial is not past its max time, 1 if it is,
			 -1 if error
			 
description: checks if the Rover had exceeded the maximum movement 
			 time for it's current mode.
********************************************************************/
int mode_over(unsigned long move_time, int mode){

	int ret_val = 0;  // value to return
	int max_time = 0; // maximum time allocated for a given mode
	
	if(mode == INPUT_FAST_MODE){
		max_time = fast_stopdistance;
	}
	else if (mode == INPUT_SLOW_MODE){
		max_time = slow_stopdistance;
	}
	else if (mode == INPUT_CRAWL_AND_STOP_MODE){
		max_time = crawl_stopdistance;
	}
	else if (mode == INPUT_PASS_THROUGH_MODE){
		max_time = pass_stopdistance;
	}
	else if (mode == INPUT_SPIRAL_MODE){
		max_time = spiral_stopdistance;
	}
	else{ // invalid mode, exit function in error
		return -1;
	}
	
	if(move_time > max_time){
		return 1;
	}
	else{
		return 0;
	}
}
	
	
	
