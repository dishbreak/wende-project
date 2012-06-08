#include "stdafx.h"
#include "C3Track.h"
#include "C3Utilities.h"
#include "C3Configuration.h"
#include <cmath>

C3Track::C3Track(const C3_TRACK_POINT_DOUBLE cameraRoverPosition, 
				 const double time)
				 : m_startTime(time),
				   m_currTime(0),
				   m_DTI(C3Utilities::EuclideanDistance(cameraRoverPosition)),
				   m_TTI(0),
				   m_passTime(0),
				   m_travelRange(0)
{
	m_playingFieldRadius = C3Configuration::Instance().WENDE_PLAYING_FIELD_RADIUS;

	if (m_playingFieldRadius < C3Utilities::EuclideanDistance(cameraRoverPosition))
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
	return C3_TRACK_POINT_DOUBLE();//m_historyPoints.getLastHistoryPoint();
}

unsigned int C3Track::getNumHistoryPoints() const
{
	return m_historyPoints.getNumHistoryPoints();
}

C3_TRACK_POINT_DOUBLE C3Track::getPredictionPoint() const
{
	return m_predictionPoint;
}

// Filter and predict next location....
C3_TRACK_POINT_DOUBLE C3Track::UpdateTrack(const C3_TRACK_POINT_DOUBLE cameraRoverPosition,
										   const C3_TRACK_POINT_DOUBLE cameraLaserPosition,
										   const double time)
{
	C3_TRACK_POINT_DOUBLE result;

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
		m_travelRange = m_travelRange + C3Utilities::EuclideanDistance(cameraRoverPosition,this->m_historyPoints.getLastHistoryPoint());

		// Calculate the TTI
		m_TTI		  = time - m_passTime;

		// TODO find camera/laser origin... FIX ... calibration items
		double laserOrigin11       = -2.0;
		double laserOrigin12       =  3.0;
		double cameraOrigin11      =  1.0;
		double cameraOrigin12	    = -2.0;
		double playingFieldOrigin11 =  0.0;
		double playingFieldOrigin12 =  0.0;

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
		double theta = M_PI - atan2(laserOrigin12,laserOrigin11); // yea I used a constant included by cmath that goes to 20 decimal places to 1up you so what..wanna fight about it?
		double range = sqrt(pow(laserOrigin11,2) + pow(laserOrigin12,2));
		double cameraToLaserX = -range*sin(theta);
		double cameraToLaserY = -range*cos(theta);


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
		double localPipX   = relativePipX*cos(theta) - relativePipX*sin(theta);
		double localPipY   = relativePipX*sin(theta) + relativePipX*cos(theta);
		double localPipR   = sqrt(relativePipX*relativePipX+relativePipY*relativePipY);

		// TODO ... Configuration for local laser height... fix
		double LaserHeight  = 5; //convert to meters

		// transform local x/y into local az/el coordinate system
		double localLaserAz = atan2(localLaserX,localLaserY)*180/M_PI;
		double localLaserEl = atan2(localLaserR,LaserHeight)*180/M_PI;
		double localPipAz = atan2(localPipX,localPipY)*180/M_PI;
		double localPipEl = atan2(localPipR,LaserHeight)*180/M_PI;

		// calculate az/el
		double dAz = localPipAz - localLaserAz;
		double dEl = localPipEl - localLaserEl;

		// Remove --- TODOD ---- FIX
		result.AZ = dAz/updateRate;
		result.EL = dEl/updateRate;		
	}
	
	// Add to first history point
	this->m_historyPoints.addPosition(cameraRoverPosition);

	return m_predictionPoint;
}
// Return the track current DTI
double C3Track::getDTI() const
{
	return m_DTI;
}