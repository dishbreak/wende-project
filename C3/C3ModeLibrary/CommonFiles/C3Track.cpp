//#ifdef STDAFX_INCLUDE
//#include "StdAfx.h"
//#endif
#include "C3Track.h"
C3Track::C3Track(C3Point cameraRoverPosition, 
				 unsigned int time, 
				 bool isProsecuteTrack)
				 : m_startTime(time),
				   m_isProsecuteTrack(isProsecuteTrack)
{
	// add to first history point
	this->m_historyPoints.addPosition(cameraRoverPosition);
	
	//
}

C3Track::~C3Track(void)
{
}

C3Point C3Track::getLastHistoryPoint() const
{
	return m_historyPoints.getLastHistoryPoint();
}

unsigned int C3Track::getNumHistoryPoints() const
{
	return m_historyPoints.getNumHistoryPoints();
}

C3Point C3Track::getPredicationPoint() const
{
	return m_predictionPoint;
}