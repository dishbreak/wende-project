#include "RoverMovement.h"
#include "RoverMotorController.h"
#include "RoverInterfaces.h"

/**
* RoverMovementRoutines: 
* inputs: int mode - The current movement mode
*         motor_data* leftMotor - current paramaters for the left motor (struct)
*         motor_data* rightMotor - current paramaters for the right motor (struct)
* outputs: leftMotor and rightMotor structs updated
* Description: Main execution of rover movement modes
*/
void RoverMovementRoutines(int mode, motor_data* leftMotor, motor_data* rightMotor) {
	
	static int init = 1; // if 1 we initialize function
	static unsigned long spin_time = 0; // time to spin in place (in ms)
	unsigned long move_time; // time since movement start (ms)
	unsigned long move_start; // initial movement time (ms)
	unsigned long seed; // seed for the PRNG
	
	if (init == 1) {
		init = 0; // only init once
		
		// if we are in a mode that needs to spin, generate random number
		if(mode == spiral_mode || mode == pass_thru_mode){
			spin_time = 0;
		}
		else{
			seed = (unsigned long)readLightSensor() + (unsigned long)readLaserDetector() + 
					(unsigned long)mode + millis() + micros(); // attempt to get some randomness...
			randomSeed(  seed  ); 
			spin_time = random(1, 20); // random number between 1 and 20 seconds 
			spin_time *= 1000; // convert to ms
		}
		
		// initialize motor struct parameters (rob - is this necessary here??)
		leftMotor->cumError = 0;
		leftMotor->lastError = 0;
		rightMotor->cumError = 0;
		rightMotor->lastError = 0;
		
		move_start = millis(); //get the current time and save it to track movement time (in ms)
	}
	
	move_time = millis() - move_start; // compute the current time spent moving
	
	
	
	
	//if we are in a mode that needs to spin and we have not spun long enough
	if ((mode == fast_mode || mode == slow_mode || mode == crawl_mode) && (move_time < spin_time)) {
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
	else if (mode == spiral_mode) {
		// set motor parameters to spiral 
		// need to know current time
	}
	//move straight at a fast speed
	else if (mode == fast_mode) {
		if (mode_over(move_time - spin_time, mode) == 0 ){
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
		}
	}
	
	// else, we are in a mode that requires straight, slow movement (slow mode, crawl&stop, pass through)
	else {
		if(mode_over(move_time - spin_time, mode) == 0){
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
		}
	}
	
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
	
	if(mode == fast_mode){
		max_time = fast_stoptime;
	}
	else if (mode == slow_mode){
		max_time = slow_stoptime;
	}
	else if (mode == crawl_mode){
		max_time = crawl_stoptime;
	}
	else if (mode == pass_thru_mode){
		max_time = pass_stoptime;
	}
	else if (mode == spiral_mode){
		max_time = spiral_stoptime;
	}
	else{ // invalid mode, exit function in error
		return = -1;
	}
	
	if(move_time > max_time){
		return 1;
	}
	else{
		return 0;
	}
}
	
	
	