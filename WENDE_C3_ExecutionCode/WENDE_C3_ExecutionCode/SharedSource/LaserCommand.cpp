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
	LaserCommand.PWM_AZ = ntohl(CUtilities::BytesToInt(&bytes[currLoc]));
	currLoc += SIZE_OF_INT;
	LaserCommand.PWM_EL = ntohl(CUtilities::BytesToInt(&bytes[currLoc]));
	currLoc += SIZE_OF_INT;
	LaserCommand.IsLaserOn = ntohl(CUtilities::BytesToInt(&bytes[currLoc]));
}

BYTE* CLaserCommand::StatusToBytes()
{
	int currLoc = 0;
	CUtilities::IntToBytes(&msgBytes[currLoc],htonl(LaserCommand.PWM_AZ));
	currLoc += SIZE_OF_INT;
	CUtilities::IntToBytes(&msgBytes[currLoc],htonl(LaserCommand.PWM_EL));
	currLoc += SIZE_OF_INT;
	CUtilities::IntToBytes(&msgBytes[currLoc],htonl(LaserCommand.IsLaserOn));
	return msgBytes;
}
