#pragma once

/////////////////////////////////////////////////////////////////////////////////
// Struct: enum types
/////////////////////////////////////////////////////////////////////////////////
typedef enum {
	C3_DISPLAY_STARTUP   = 1,
	CALIBRATION_INIT,
	CALIBRATION_IN_PROGRESS_1,	// send first point   
	CALIBRATION_IN_PROGRESS_2,	// send second point ---> receive first
	CALIBRATION_IN_PROGRESS_3,	// send third point	 ---> receive second  
	CALIBRATION_IN_PROGRESS_4,	// send fourth point ---> receive third
	CALIBRATION_IN_PROGRESS_5,	//                   ---> receive fourth   ---> calibrate
	CALIBRATION_FAILED,
	CALIBRATION_SUCCESS,
	TARGET_LEFT_PLAYING_FIELD,
	DTI_UPDATE,
	POC_STARTED,
	POC_ENTERED,
    POC_FINISHED,
	NUM_ALERTS_TYPES
}C3_Alert_Types; 
