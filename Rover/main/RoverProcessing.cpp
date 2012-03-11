#include "Arduino.h"
#include "RoverProcessing.h"
#include "RoverDefaults.h"
#include "RoverLaserDetection.h"
#include "RoverDemo.h"

void RoverProcessingRoutines()
{
	static int curr_state = USER_INPUT_STATE;
	
	if(ROVER_DEMO_MODE == 1)
	{
		curr_state = ROVER_DEMO_STATE;
	}
	
	//state machine...
	switch (curr_state)
	{
		case ROVER_DEMO_STATE:
			//execute rover demos...
			//RoverLightSensingDemo();
                        RoverLaserDetectionDemo();
			return; //do not want to do anything else here

                        return;
	}
//        DetectionProcessing();
	//now update the ADC values
	//set the PWM for the motors
}
