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
		CLaserStatus(void);
		CLaserStatus( BYTE *bytes);
		~CLaserStatus(void);

	public:
		LASER_STATUS_STRUCT LaserStatus;
		void  BytesToStatus( BYTE *bytes);
		BYTE* StatusToBytes();
	
	private:
		BYTE msgBytes[sizeof(LASER_STATUS_STRUCT)]; 
};
