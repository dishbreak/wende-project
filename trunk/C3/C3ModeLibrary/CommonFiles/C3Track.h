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
#include "C3Point.h"
#include "C3FilterClass.h"
#include "C3CircularHistory.h"

class C3Track
{
	private:
		// Filter for tracking purposes
		C3FilterClass		m_filter;
		// Last predicted point
		C3Point				m_predictionPoint;
		// History of points for prosecution
		C3CircularHistory	m_historyPoints;
		// Flag to determine if laser commands are used
		bool				m_isProsecuteTrack;
		// The starting time of the track
		unsigned int		m_startTime;

	public:
		// Cononical Functions
		C3Track(C3Point cameraRoverPosition, unsigned int time, bool isProsecuteTrack);
		~C3Track(void);

	public:
		// returns the number of points contained in the history array
		unsigned int getNumHistoryPoints() const;
		// returns the last saved point containd in the history array
		C3Point getLastHistoryPoint() const;
		// returns the last prediction from the kalman filter
		C3Point getPredicationPoint() const;

};