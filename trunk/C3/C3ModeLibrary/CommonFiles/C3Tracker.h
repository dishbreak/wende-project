///////////////////////////////////////////////////////////////////////////////
//          File: C3Tracker.h
//    Created By: Benjamin K. Heiner
// Creation Date: 3-18-2012
//
//       Purpose: This file contains all functionality related to a tracker 
//				  interfaces.
//
//---Member Variables---
//	m_tracks			  --> Contains all running tracks.
//  m_playingFieldRadius  --> Playing Field Radius.
//  m_PlayingFieldOrigion --> playing Field origion.
//
//---Member Functions---
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <vector>
#include <limits>

#include "C3Track.h"
#include "C3Point.h"

using std::vector;
using std::numeric_limits;

typedef struct C3_CORRELATE
{
	C3_CORRELATE()
	{
		dist		= 0.0; 
		assignTrackIndex = -1;
		assignPointIndex = -1;
	}

	double dist; 
	int    assignTrackIndex;
	int    assignPointIndex;
} C3_CORRELATE_struct;

class C3Tracker
{
	private:
		// Array of existing tracks
		vector<C3Track*> m_tracks;								// [1...N] where N is defined at creation
		// Playing Field origion
		C3Point			 m_PlayingFieldOrigion;					// [m,m]
		// Playing Field Radius
		double			 m_playingFieldRadius;					// (m)
		// Max Distance a point can be before it is ignorded
		double			 m_maxDistance;							// (m)

	public:
		C3Tracker(void);
		~C3Tracker(void);

	public:
		void UpdateTracks(vector<C3Point> cameraRoverPositions, unsigned int time);

	private:
		void AddTrack(C3Point cameraRoverPosition, unsigned int time);
};