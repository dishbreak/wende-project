#pragma once

typedef enum LASER_SYSTEM_STATUS 
{ 
	LASER_DOWN = 0,
	LASER_READY,
	LASER_OPERATIONAL,
	LASER_ERROR,
	LASER_FAILED,
	UNKNOWN,
};

typedef struct
{
	int					PWM_AZ;
	int					PWM_EL;
	LASER_SYSTEM_STATUS status;
} LASER_STATUS_STRUCT;

class CLaserStatus
{
	public:
		// default constructor
		CLaserStatus(void);
		// same as the StatusToBytes function (overide destructor)
		CLaserStatus( BYTE *bytes);
		// destructor
		~CLaserStatus(void);

	public:
		// data
		LASER_STATUS_STRUCT LaserStatus;
		// This message is used by the c3 team to decode
		// the status sent from the laser
		void  BytesToStatus( BYTE *bytes);
		// This function is used by the laser teamto send
		// status to the C3
		BYTE* StatusToBytes();
	
	private:
		BYTE msgBytes[sizeof(LASER_STATUS_STRUCT)]; 
};
