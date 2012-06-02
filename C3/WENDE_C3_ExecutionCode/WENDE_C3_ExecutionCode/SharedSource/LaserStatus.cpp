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
	LaserStatus.PWM_AZ = CUtilties::BytesToInt(&bytes[currLoc]);
	currLoc += SIZE_OF_INT;
	LaserStatus.PWM_EL = CUtilties::BytesToInt(&bytes[currLoc]);
	currLoc += SIZE_OF_INT;
	LaserStatus.status = (LASER_SYSTEM_STATUS)CUtilties::BytesToInt(&bytes[currLoc]);
}

BYTE* CLaserStatus::StatusToBytes()
{
	int currLoc = 0;
	CUtilties::IntToBytes(&msgBytes[currLoc],LaserStatus.PWM_AZ);
	currLoc += SIZE_OF_INT;
	CUtilties::IntToBytes(&msgBytes[currLoc],LaserStatus.PWM_EL);
	currLoc += SIZE_OF_INT;
	CUtilties::IntToBytes(&msgBytes[currLoc],(int)LaserStatus.status);
	return msgBytes;
}
