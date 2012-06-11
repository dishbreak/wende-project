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
		// default constructor
		CLaserCommand(void);
		// same as the StatusToBytes function (overide destructor)
		CLaserCommand( BYTE *bytes);
		// destructor
		~CLaserCommand(void);

	public:
		// data
		LASER_COMMAND_STRUCT LaserStatus;
		// This function is used by the c3 team to send
		// command to the laser
		void  BytesToStatus( BYTE *bytes);
		// This message is used by the laser team to decode
		// the command sent from the c3
		BYTE* StatusToBytes();
	
	private:
		BYTE msgBytes[sizeof(LASER_COMMAND_STRUCT)]; 
};
