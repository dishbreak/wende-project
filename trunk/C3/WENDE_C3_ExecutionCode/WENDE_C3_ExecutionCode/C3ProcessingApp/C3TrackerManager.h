///////////////////////////////////////////////////////////////////////////////
//          File: C3TrackerManager.h
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
//  m_maxDistance         --> Max Distance a point can be before it is ignorded
//
//---Member Functions---
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <vector>
#include <limits>
#include <map>

#include "C3Track.h"
#include "ShmStructs.h"

using std::map;
using std::pair;
using std::vector;
using std::numeric_limits;
using std::make_pair;
using std::min;
using std::max;

typedef struct C3_CORRELATE
{
	C3_CORRELATE()
	{
		dist			 = 0.0;	// initlize to 0 distance 
		assignTrackIndex = -1;	// assign a tracker
		isJustCreated    = true;// set to just created
	}

	double dist;				// the distance from the tracker point
	int    assignTrackIndex;	// the assigned tracker location
	bool   isJustCreated;		// flag for just created trackers
} C3_CORRELATE_struct;

class C3TrackerManager
{
	private:
		// Array of existing tracks
		vector<C3Track*> m_tracks;								// [1...N] where N is defined at creation
		// Playing Field origion
		C3_TRACK_POINT			 m_PlayingFieldOrigion;					// [m,m]
		// Playing Field Radius
		double			 m_playingFieldRadius;					// (m)
		// Max Distance a point can be before it is ignorded
		double			 m_maxDistance;							// (m)

	public:
		C3TrackerManager(void);
		~C3TrackerManager(void);

	public:
		// update tracks with input points
		C3_TRACK_POINT_DOUBLE UpdateTracks(const vector<C3_TRACK_POINT_DOUBLE> cameraRoverPositions,
											C3_TRACK_POINT_DOUBLE cameraLaserPosition, 
											const double time,
											const C3_TRACK_POINT_DOUBLE laserOrigin);
		// clear the tracks
		void ClearTracks();
		C3_TRACK_POINT_DOUBLE getPredictedPoint() const;
		int	GetDTI()const;
	private:
		// determine if a point has already been assigned to the tracker 
		bool isInMapping(const map<unsigned int, C3_CORRELATE_struct> *position2track, 
			             const unsigned int trackerNum);
		// Creates a new tracker
		unsigned int AddTrack(const C3_TRACK_POINT_DOUBLE cameraRoverPosition, 
			                  const double time);
		// Correlates a point with a tracker
		void correlatePositions2Trackers(map<unsigned int, C3_CORRELATE_struct> *position2track, 
			                             const vector<C3_TRACK_POINT_DOUBLE> cameraRoverPositions, 
										 const double time);
};