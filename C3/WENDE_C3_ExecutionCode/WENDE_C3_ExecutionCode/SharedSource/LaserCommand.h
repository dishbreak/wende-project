#pragma once

typedef struct
{
	int					PWM_AZ;
	int					PWM_EL;
	BYTE				isLaserOn;
} LASER_COMMAND_STRUCT;

class CLaserCommand
{
	public:
		CLaserCommand(void);
		CLaserCommand( BYTE *bytes);
		~CLaserCommand(void);

	public:
		LASER_COMMAND_STRUCT LaserStatus;
		void  BytesToStatus( BYTE *bytes);
		BYTE* StatusToBytes();
	
	private:
		BYTE msgBytes[sizeof(LASER_COMMAND_STRUCT)]; 
};
