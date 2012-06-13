#include "stdafx.h"
#include "LaserCommand.h"
#include "Utilties.h"

CLaserConfiguration::CLaserConfiguration(void)
{
}

CLaserConfiguration::CLaserConfiguration( BYTE *bytes)
{
	BytesToStatus(bytes);
}

CLaserConfiguration::~CLaserConfiguration(void)
{
}

void CLaserConfiguration::BytesToStatus( BYTE *bytes)
{
	int currLoc = 0;
	LaserConfiguration.PWM_AZ.MIN = CUtilities::BytesToInt(&bytes[currLoc]);
	currLoc += SIZE_OF_INT;
	LaserConfiguration.PWM_AZ.MIX = CUtilities::BytesToInt(&bytes[currLoc]);
	currLoc += SIZE_OF_INT;
	LaserConfiguration.PWM_EL.MIN = CUtilities::BytesToInt(&bytes[currLoc]);
	currLoc += SIZE_OF_INT;
	LaserConfiguration.PWM_EL.MIX = CUtilities::BytesToInt(&bytes[currLoc]);
	currLoc += SIZE_OF_INT;
	LaserConfiguration.Frequency = bytes[currLoc];
}

BYTE* CLaserConfiguration::StatusToBytes()
{
	int currLoc = 0;
	CUtilities::IntToBytes(&msgBytes[currLoc],LaserConfiguration.PWM_AZ.MIN);
	currLoc += SIZE_OF_INT;
	CUtilities::IntToBytes(&msgBytes[currLoc],LaserConfiguration.PWM_AZ.MAX);
	currLoc += SIZE_OF_INT;
	CUtilities::IntToBytes(&msgBytes[currLoc],LaserConfiguration.PWM_EL.MIN);
	currLoc += SIZE_OF_INT;
	CUtilities::IntToBytes(&msgBytes[currLoc],LaserConfiguration.PWM_EL.MAX);
	currLoc += SIZE_OF_INT;
	msgBytes[currLoc] = LaserConfiguration.Frequency;
	return msgBytes;
}
