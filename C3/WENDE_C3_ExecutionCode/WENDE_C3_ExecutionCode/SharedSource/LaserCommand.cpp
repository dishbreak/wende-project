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
	LaserStatus.PWM_AZ = CUtilities::BytesToInt(&bytes[currLoc]);
	currLoc += SIZE_OF_INT;
	LaserStatus.PWM_EL = CUtilities::BytesToInt(&bytes[currLoc]);
	currLoc += SIZE_OF_INT;
	LaserStatus.isLaserOn = bytes[currLoc];
}

BYTE* CLaserCommand::StatusToBytes()
{
	int currLoc = 0;
	CUtilities::IntToBytes(&msgBytes[currLoc],LaserStatus.PWM_AZ);
	currLoc += SIZE_OF_INT;
	CUtilities::IntToBytes(&msgBytes[currLoc],LaserStatus.PWM_EL);
	currLoc += SIZE_OF_INT;
	msgBytes[currLoc] = LaserStatus.isLaserOn;
	return msgBytes;
}
