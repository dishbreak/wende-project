#pragma once

/////////////////////////////////////////////////////////////////////////////////
// Struct: enum types
/////////////////////////////////////////////////////////////////////////////////
typedef enum {
	C3_DISPLAY_STARTUP			=  1,
	CALIBRATION_INIT			=  2,
	CALIBRATION_IN_PROGRESS_1	=  3,	// send first point   
	CALIBRATION_IN_PROGRESS_2	=  4,	// send second point ---> receive first
	CALIBRATION_IN_PROGRESS_3	=  5,	// send third point	 ---> receive second  
	CALIBRATION_IN_PROGRESS_4	=  6,	// send fourth point ---> receive third
	CALIBRATION_IN_PROGRESS_5	=  7,	//                   ---> receive fourth   ---> calibrate
	CALIBRATION_FAILED			=  8,
	CALIBRATION_SUCCESS			=  9,
	TARGET_LEFT_PLAYING_FIELD	= 10,
	DTI_UPDATE					= 11,
	POC_STARTED					= 12,
	POC_ENTERED					= 13,
    POC_FINISHED				= 14,
	NUM_ALERTS_TYPES
}C3_Alert_Types; 
