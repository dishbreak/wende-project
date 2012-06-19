#include "stdafx.h"
#include "C3Track.h"
#include "C3Utilities.h"
#include "C3Configuration.h"
#include <cmath>

using std::max;
using std::sqrt;

//////////////////////////////////////////////////////////////////////////////
// Funciton: C3Track
// Programmer: Benjamin Kurt Heiner
//
// Purpose: default constructor
//////////////////////////////////////////////////////////////////////////////
C3Track::C3Track(const C3_TRACK_POINT_DOUBLE cameraRoverPosition, 
				 const double time)
				 : m_startTime(time),
				   m_currTime(time),
				   m_DTI(C3Utilities::EuclideanDistance(cameraRoverPosition)),
				   m_TTI(0),
				   m_passTime(0),
				   m_travelRange(0)
{
	// set updated to false
	m_isUpdate = false;
	// Add to first history point
	m_lastUpdatePoint = cameraRoverPosition;

	// THE PLAYING FIELD RADIUS
	m_playingFieldRadius = C3Configuration::Instance().WENDE_PLAYING_FIELD_RADIUS;

	// DETERMINE IF THE TRACK STARTS INSIDE OF THE PLAYING FIELD
	if (m_playingFieldRadius > C3Utilities::EuclideanDistance(cameraRoverPosition))
	{
		m_isProsecuteTrack = true;
	}
	else
	{
		m_isProsecuteTrack = false;
	}
}

C3Track::~C3Track(void)
{
}

C3_TRACK_POINT_DOUBLE C3Track::getLastHistoryPoint() const
{
	//TODO FIX
	return m_lastUpdatePoint;
}


C3_TRACK_POINT_DOUBLE C3Track::getPredictionPoint() const
{
	return m_predictionPoint;
}

// find out what the tracks positions is estimated to be at time time
C3_TRACK_POINT_DOUBLE C3Track::getPointPropogatedToTime(double time)
{	
	return m_filter.GetPredictedPoint(time);
}

// Filter and predict next location....
C3_TRACK_POINT_DOUBLE C3Track::UpdateTrack(const C3_TRACK_POINT_DOUBLE cameraRoverPosition,
										   const C3_TRACK_POINT_DOUBLE cameraLaserPosition,
										   const double time,
										   const C3_TRACK_POINT_DOUBLE laserOrigin)
{
	C3_TRACK_POINT_DOUBLE result;

	// set updated to false
	m_isUpdate = true;

	// Get the update rate
	double updateRate = time - m_currTime;

	// Calculate Predicted Intercept Point (PIP)
	m_predictionPoint = m_filter.FilterInput(cameraRoverPosition,updateRate);

	// Update to the next time value using input
	m_currTime = time;

	// Update the DTI Value
	m_DTI = C3Utilities::EuclideanDistance(cameraRoverPosition);

	// Determine if inside playing field and valid
	if (m_DTI <= m_playingFieldRadius)
	{
		// set do  nothing states
		m_passTime	= time;			
		result.AZ	= 0;
		result.EL	= 0;
	}
	else
	{
		// Total travel range
		// THIS CAN BE REMOVED --- IF NEEDED
		m_travelRange = m_travelRange + C3Utilities::EuclideanDistance(cameraRoverPosition,m_lastUpdatePoint);

		// Calculate the TTI
		m_TTI		  = time - m_passTime;

		// Find Camera to Laser Translation and Rotation Parameters
		//double cameraTheta = atan2(playingFieldOrigin11-cameraOrigin11,playingFieldOrigin12-cameraOrigin12);
		//double laserTheta  = atan2(playingFieldOrigin11-laserOrigin11 ,playingFieldOrigin12-laserOrigin12 );
		//double theta       = laserTheta - cameraTheta;

		// Find Laser Origin in camera space
		//double laserOriginCameraSpace11 = laserOrigin11*cos(cameraTheta) - laserOrigin12*sin(cameraTheta);
		//double laserOriginCameraSpace12 = laserOrigin11*sin(cameraTheta) + laserOrigin12*cos(cameraTheta); 
		
		// camera to laser translation vector (Camera Coordinate system has Origin in playing field)
		//double cameraToLaserX = laserOriginCameraSpace11 - playingFieldOrigin11;
		//double cameraToLaserY = laserOriginCameraSpace12 - playingFieldOrigin12;

		//Alternative translation method(USE THIS ONE)
		//double bearing = atan2(laserOrigin.Y,laserOrigin.X);
		//double theta = -bearing + M_PI; // Revered order so the sign is correct. Clockwise positive, camera at 180.
		double theta =atan2(-laserOrigin.X,-laserOrigin.Y);
		double range = sqrt(pow(laserOrigin.X,2) + pow(laserOrigin.Y,2));
		double cameraToLaserX = laserOrigin.X;
		double cameraToLaserY = laserOrigin.Y;


		// Transform laser & rover postions into laser
		// translate camera x/y into relative X/Y coordinates system (relative to laser Origin)
		double relativeLaserX = cameraLaserPosition.X - cameraToLaserX;
		double relativeLaserY = cameraLaserPosition.Y - cameraToLaserY;
		double relativePipX   = m_predictionPoint.X - cameraToLaserX;
		double relativePipY   = m_predictionPoint.Y - cameraToLaserY;
		
		//Rotate relative x/y into local x/y coordinate system (Y axis goes from laser origin to playing field)
		double localLaserX = relativeLaserX*cos(theta) - relativeLaserY*sin(theta);
		double localLaserY = relativeLaserX*sin(theta) + relativeLaserY*cos(theta);
		double localLaserR = sqrt(relativeLaserX*relativeLaserX+relativeLaserY*relativeLaserY);
		double localPipX   = relativePipX*cos(theta) - relativePipY*sin(theta);
		double localPipY   = relativePipX*sin(theta) + relativePipY*cos(theta);
		double localPipR   = sqrt(relativePipX*relativePipX+relativePipY*relativePipY);

		double LaserHeight  = 1.5;//C3Configuration::Instance().LASER_HEIGHT; //convert to meters

		// transform local x/y into local az/el coordinate system
		double localLaserAz = atan2(localLaserX,localLaserY)*180/M_PI;
		double localLaserEl = atan2(localLaserR,LaserHeight)*180/M_PI;
		double localPipAz = atan2(localPipX,localPipY)*180/M_PI;
		double localPipEl = atan2(localPipR,LaserHeight)*180/M_PI;

		// calculate az/el
		double dAz = localPipAz - localLaserAz;
		double dEl = localPipEl - localLaserEl;

		result.AZ = dAz;
		result.EL = dEl;		
	}

	// Add to first history point
	m_lastUpdatePoint = cameraRoverPosition;

	return result;
}
// Return the track current DTI
double C3Track::getDTI() const
{
	return m_DTI;
}
//
double C3Track::getSigma() const
{
	double p11sqrt = sqrt(m_filter.m_kalman.P(1,1));
	double p22sqrt = sqrt(m_filter.m_kalman.P(2,2));
	double sigma = max(p11sqrt,p22sqrt);
	return sigma;
}
C3_TRACK_POINT_DOUBLE C3Track::getPredictedPoint() const
{
	return this->m_predictionPoint;
}
bool C3Track::isProsecute()const
{
	return this->m_isProsecuteTrack;
}