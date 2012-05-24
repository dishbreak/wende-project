/**
*	RoverUI:
*		-UI of the Rover, reads user selected modes
*	Author: Rover Team
*	Modified: 5/22/2012
*/

#ifndef RoverUI_h
#define RoverUI_h

#include "Arduino.h"

//states
#define UI_INIT_STATE  0  //init state, read inputs
#define UI_WAIT_STATE  1  //wait state
#define UI_READY_STATE 2  //ready state

extern int roverMode;

boolean RoverUIRoutines(void);
boolean RoverWait(boolean reset);
#endif
