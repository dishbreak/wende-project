#include "stdafx.h"
#include "LaserCommand.h"
#include "Utilties.h"

CLaserCommand::CLaserCommand(void)
{
}

CLaserCommand::CLaserCommand( BYTE *bytes)
{
	BytesToStatus(bytes);
}

CLaserCommand::~CLaserCommand(void)
{
}

void CLaserCommand::BytesToStatus( BYTE *bytes)
{
	int currLoc = 0;
	LaserStatus.PWM_AZ = CUtilties::BytesToInt(&bytes[currLoc]);
	currLoc += SIZE_OF_INT;
	LaserStatus.PWM_EL = CUtilties::BytesToInt(&bytes[currLoc]);
	currLoc += SIZE_OF_BYTE;
	LaserStatus.isLaserOn = bytes[currLoc];
}

BYTE* CLaserCommand::StatusToBytes()
{
	int currLoc = 0;
	CUtilties::IntToBytes(&msgBytes[currLoc],LaserStatus.PWM_AZ);
	currLoc += SIZE_OF_INT;
	CUtilties::IntToBytes(&msgBytes[currLoc],LaserStatus.PWM_EL);
	currLoc += SIZE_OF_BYTE;
	msgBytes[currLoc] = LaserStatus.isLaserOn;
	return msgBytes;
}
