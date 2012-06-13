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
	LaserCommand.PWM_AZ = CUtilities::BytesToInt(&bytes[currLoc]);
	currLoc += SIZE_OF_INT;
	LaserCommand.PWM_EL = CUtilities::BytesToInt(&bytes[currLoc]);
	currLoc += SIZE_OF_INT;
	LaserCommand.isLaserOn = bytes[currLoc];
}

BYTE* CLaserCommand::StatusToBytes()
{
	int currLoc = 0;
	CUtilities::IntToBytes(&msgBytes[currLoc],LaserCommand.PWM_AZ);
	currLoc += SIZE_OF_INT;
	CUtilities::IntToBytes(&msgBytes[currLoc],LaserCommand.PWM_EL);
	currLoc += SIZE_OF_INT;
	msgBytes[currLoc] = LaserCommand.isLaserOn;
	return msgBytes;
}
