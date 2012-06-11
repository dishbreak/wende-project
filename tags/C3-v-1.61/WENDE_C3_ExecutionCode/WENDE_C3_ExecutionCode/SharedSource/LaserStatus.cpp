#include "StdAfx.h"
#include "LaserStatus.h"
#include "Utilties.h"

CLaserStatus::CLaserStatus(void)
{
}

CLaserStatus::CLaserStatus( BYTE *bytes)
{
	BytesToStatus(bytes);
}

CLaserStatus::~CLaserStatus(void)
{
}

void CLaserStatus::BytesToStatus( BYTE *bytes)
{
	int currLoc = 0;
	LaserStatus.PWM_AZ = CUtilities::BytesToInt(&bytes[currLoc]);
	currLoc += SIZE_OF_INT;
	LaserStatus.PWM_EL = CUtilities::BytesToInt(&bytes[currLoc]);
	currLoc += SIZE_OF_INT;
	LaserStatus.status = (LASER_SYSTEM_STATUS)CUtilities::BytesToInt(&bytes[currLoc]);
}

BYTE* CLaserStatus::StatusToBytes()
{
	int currLoc = 0;
	CUtilities::IntToBytes(&msgBytes[currLoc],LaserStatus.PWM_AZ);
	currLoc += SIZE_OF_INT;
	CUtilities::IntToBytes(&msgBytes[currLoc],LaserStatus.PWM_EL);
	currLoc += SIZE_OF_INT;
	CUtilities::IntToBytes(&msgBytes[currLoc],(int)LaserStatus.status);
	return msgBytes;
}
