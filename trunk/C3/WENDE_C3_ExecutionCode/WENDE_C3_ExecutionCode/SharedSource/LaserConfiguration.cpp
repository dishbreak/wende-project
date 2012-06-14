#include "stdafx.h"
#include "LaserConfiguration.h"
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
	LaserConfiguration.PWM_AZ.MIN = ntohl(CUtilities::BytesToInt(&bytes[currLoc]));
	currLoc += SIZE_OF_INT;
	LaserConfiguration.PWM_AZ.MAX = ntohl(CUtilities::BytesToInt(&bytes[currLoc]));
	currLoc += SIZE_OF_INT;
	LaserConfiguration.PWM_EL.MIN = ntohl(CUtilities::BytesToInt(&bytes[currLoc]));
	currLoc += SIZE_OF_INT;
	LaserConfiguration.PWM_EL.MAX = ntohl(CUtilities::BytesToInt(&bytes[currLoc]));
	currLoc += SIZE_OF_INT;
	LaserConfiguration.Frequency  = ntohl(CUtilities::BytesToInt(&bytes[currLoc]));
}

BYTE* CLaserConfiguration::StatusToBytes()
{
	int currLoc = 0;
	CUtilities::IntToBytes(&msgBytes[currLoc],htonl(LaserConfiguration.PWM_AZ.MIN));
	currLoc += SIZE_OF_INT;
	CUtilities::IntToBytes(&msgBytes[currLoc],htonl(LaserConfiguration.PWM_AZ.MAX));
	currLoc += SIZE_OF_INT;
	CUtilities::IntToBytes(&msgBytes[currLoc],htonl(LaserConfiguration.PWM_EL.MIN));
	currLoc += SIZE_OF_INT;
	CUtilities::IntToBytes(&msgBytes[currLoc],htonl(LaserConfiguration.PWM_EL.MAX));
	currLoc += SIZE_OF_INT;
	CUtilities::IntToBytes(&msgBytes[currLoc],htonl(LaserConfiguration.Frequency));
	return msgBytes;
}
