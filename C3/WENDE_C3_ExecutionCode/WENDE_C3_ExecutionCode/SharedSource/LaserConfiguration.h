#pragma once

typedef struct
{
	int MIN;
	int MAX;
}LASER_LIMITS;
typedef struct
{
	LASER_LIMITS		PWM_AZ;
	LASER_LIMITS		PWM_EL;
	BYTE				Frequency;
} LASER_COMMAND_STRUCT;

class CLaserConfiguration
{
	public:
		// default constructor
		CLaserConfiguration(void);
		// same as the StatusToBytes function (overide destructor)
		CLaserConfiguration( BYTE *bytes);
		// destructor
		~CLaserConfiguration(void);

	public:
		// data
		LASER_COMMAND_STRUCT LaserConfiguration;
		// This function is used by the c3 team to send
		// command to the laser
		void  BytesToStatus( BYTE *bytes);
		// This message is used by the laser team to decode
		// the command sent from the c3
		BYTE* StatusToBytes();
	
	private:
		BYTE msgBytes[sizeof(LASER_COMMAND_STRUCT)]; 
};
