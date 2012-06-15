///////////////////////////////////////////////////////////////////////////////
//          File: C3Track.h
//    Created By: Benjamin K. Heiner
// Creation Date: 3-18-2012
//
//       Purpose: This file contains all functionality related to a single track.
//
//---Member Variables---
//	m_Filter			--> Alpha/Beta or Kalman filter for predictions
//  m_PredictionPoint	--> The last predicted point from above filter
//  m_historyPoints     --> History of camera perceived rover positions
//  m_isProsecuteTrack  --> Flag to determine wheter or not to point the laser
//  m_startTime         --> The time the track was started
//
//---Member Functions---
//
///////////////////////////////////////////////////////////////////////////////
#pragma once
#include "ShmStructs.h"
#include "C3FilterClass.h"
#include "C3CircularHistory.h"

class C3Track
{
	private:
		// Filter for tracking purposes
		C3FilterClass			m_filter;
		// Last predicted point
		C3_TRACK_POINT_DOUBLE	m_predictionPoint;
		// History of points for prosecution
		C3CircularHistory		m_historyPoints;
		// Flag to determine if laser commands are used
		bool					m_isProsecuteTrack;
		// The starting time of the track
		double					m_startTime;
		// The starting time of the track
		double					m_currTime;
		// the time at which the track leaves the playing field
		double					m_passTime;
		// The Distance to Illumination (DTI)
		double					m_DTI;
		// The Time To Illumination
		double					m_TTI;
		// the radius of the playing field
		double					m_playingFieldRadius;
		// store the total travel range
		double					m_travelRange;
	public:
		// Cononical Functions
		C3Track(const C3_TRACK_POINT_DOUBLE cameraRoverPosition, const double time);
		~C3Track(void);

	public:
		// Filter and predict next location....
		C3_TRACK_POINT_DOUBLE UpdateTrack(const C3_TRACK_POINT_DOUBLE cameraRoverPosition, 
										  const C3_TRACK_POINT_DOUBLE cameraLaserPosition,
										  const double time,
										  const C3_TRACK_POINT_DOUBLE laserOrigin);

		C3_TRACK_POINT_DOUBLE C3Track::getPointPropogatedToTime(int time);

	public:
		// returns the number of points contained in the history array
		unsigned int getNumHistoryPoints() const;
		// returns the last saved point containd in the history array
		C3_TRACK_POINT_DOUBLE getLastHistoryPoint() const;
		// returns the last prediction from the kalman filter
		C3_TRACK_POINT_DOUBLE getPredictionPoint() const;
		// Return the track current DTI
		double getDTI() const;
		// Return the track current TTI
		double getTTI() const;
		//
		double getSigma() const;
};