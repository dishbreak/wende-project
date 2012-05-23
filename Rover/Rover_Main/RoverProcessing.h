/**
*	RoverProcessing:
*		-"Brain" of the Rover, decision making processing
*	Author: Rover Team
*	Modified: 2/22/2012
*/

#ifndef RoverProcessing_h
#define RoverProcessing_h

#include "Arduino.h"

#define ROVER_INIT_STATE  0
#define ROVER_READY_STATE  1
#define ROVER_DONE_STATE  2

void RoverProcessingRoutines(void);
#endif
