#include "RoverMovement.h"
#include "RoverMotorController.h"


// Move these to .h file
// Definitions for how each movement mode is represented with an int
#define fast_mode		1
#define slow_mode		2
#define crawl_stop_mode 3
#define pass_thru_mode  4
#define spiral_mode		5


/**
* RoverMovementRoutines: 
* inputs: int mode - The current movement mode
*         motor_data* leftMotor - current paramaters for the left motor (struct)
*         motor_data* rightMotor - current paramaters for the right motor (struct)
* outputs: leftMotor and rightMotor structs updated
* Description: Main execution of rover movement modes
*/
void RoverMovementRoutines(int mode, motor_data* leftMotor, motor_data* rightMotor) {

	static int init = 1; // if 1 we intitalize function
	//todo: make this random
	static int spin_time = 5; // time to spin in place (in ms)
	
	if (init == 1) {
		move_start = milli(); // get the current time and save it to track movement time (in ms)
		init = 0; // only init once
	}
	
	move time = milli() - move_start; // compute the current time spent moving
	
	//if we are in a mode that needs to spin and we have not spun long enough
	if ((mode == fast_mode || mode == slow_mode || mode == crawl_stop_mode) && (move_time < spin_time)) {
		//set motor paramaters so the rover will spin
	}
	else if (mode == spiral_mode) {
		// set motor parameters to spiral 
		// need to know current time
	}
	else if (mode == fast_mode) {
		//move straight at a fast speed
	}
	// else, we are in a mode that requires straight, slow movement
	else {
		// move staight at a slow speed
	}
	
}
