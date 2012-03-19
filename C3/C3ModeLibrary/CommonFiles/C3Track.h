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
//  m_ProsecuteTrack    --> Flag to determine wheter or not to point the laser
//  m_time              --> The time the track was started
//
//---Member Functions---
//
///////////////////////////////////////////////////////////////////////////////
#pragma once
#include <vector>
#include "C3Point.h"
#include "C3FilterClass.h"
using std::vector;

class C3Track
{
	private:
		// Filter for tracking purposes
		C3FilterClass   m_Filter;
		// Last predicted point
		C3Point			m_PredictionPoint;
		// History of points for prosecution
		vector<C3Point>	m_historyPoints;
		// Flag to determine if laser commands are used
		bool			m_ProsecuteTrack;
		// The starting time of the track
		unsigned int    m_time;

	public:
		C3Track(void);
		C3Track(C3Point cameraRoverPosition, unsigned int time);
		~C3Track(void);
};