#include "StdAfx.h"
#include "C3Track.h"
#include "C3Utilities.h"

// Filter and predict next location....
C3_TRACK_POINT UpdateTrack(const C3_TRACK_POINT cameraRoverPosition, const unsigned int time);

C3Track::C3Track(const C3_TRACK_POINT cameraRoverPosition, 
				 const double time, 
				 const bool isProsecuteTrack,
				 const double playingFieldRadius)
				 : m_startTime(time),
				   m_isProsecuteTrack(isProsecuteTrack),
				   m_DTI(C3Utilities::EuclideanDistance(cameraRoverPosition)),
				   m_TTI(0),
				   m_passTime(0),
				   m_playingFieldRadius(playingFieldRadius)
{
}

C3Track::~C3Track(void)
{
}

C3_TRACK_POINT C3Track::getLastHistoryPoint() const
{
	return m_historyPoints.getLastHistoryPoint();
}

unsigned int C3Track::getNumHistoryPoints() const
{
	return m_historyPoints.getNumHistoryPoints();
}

C3_TRACK_POINT C3Track::getPredicationPoint() const
{
	return m_predictionPoint;
}

// Filter and predict next location....
C3_TRACK_POINT C3Track::UpdateTrack(const C3_TRACK_POINT cameraRoverPosition, const double time)
{
	// add to first history point
	this->m_historyPoints.addPosition(cameraRoverPosition);

	// Update the DTI Value
	m_DTI = C3Utilities::EuclideanDistance(cameraRoverPosition);

	//if (m_DTI <= playingFieldRadius)
	//{
	//	azimuthElevation.Azimuth	= 0;
	//	azimuthElevation.Elevation	= 0;
	//	m_passTime					= -1;
	//}
	//else
	//{


	//}

	// Update the tracker filter
	return m_filter.FilterInput(cameraRoverPosition,time);
}
// Return the track current DTI
double C3Track::getDTI() const
{
	return m_DTI;
}
// Commanded Laser Azimuth and Elevation
C3_TRACK_POINT C3Track::CommandLaser()
{
	C3_TRACK_POINT track;
	return track;
}