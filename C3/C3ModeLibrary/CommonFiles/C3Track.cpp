//#ifdef STDAFX_INCLUDE
//#include "StdAfx.h"
//#endif
#include "C3Track.h"

C3Track::C3Track(void)
{
}

C3Track::~C3Track(void)
{
}

C3Track::C3Track(C3Point cameraRoverPosition, unsigned int time)
{
	// add to first history point
	this->m_historyPoints.push_back(cameraRoverPosition);


}
