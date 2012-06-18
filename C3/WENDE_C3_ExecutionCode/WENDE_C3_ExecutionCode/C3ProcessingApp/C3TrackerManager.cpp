#include "StdAfx.h"
#include "C3TrackerManager.h"
#include "C3Utilities.h"
#include "C3Configuration.h"
#include <algorithm>
#include <cmath>

C3TrackerManager::C3TrackerManager(void)
	: m_maxDistance(5)
{
}

C3TrackerManager::~C3TrackerManager(void)
{
	ClearTracks();
}

void C3TrackerManager::ClearTracks()
{
	// free the trackers
	for(int ii = m_tracks.size(); ii >=0; ii--)
	{
		C3Track *track = m_tracks[ii];

		delete track;

		m_tracks[ii] = NULL;
	}
	// clear the tracks vector
	m_tracks.clear();
}
///////////////////////////////////////////////////////////////////////////////////////
// Function: UpdateTracks
// Programmer: Benjamin Kurt Heiner
//
// Purpose: this function handles the updates of the various trackes given the input
//          time and rover points
///////////////////////////////////////////////////////////////////////////////////////
C3_TRACK_POINT_DOUBLE C3TrackerManager::UpdateTracks(const vector<C3_TRACK_POINT_DOUBLE> cameraRoverPositions, 
													 C3_TRACK_POINT_DOUBLE cameraLaserPosition, 
													 const double time,
													 const C3_TRACK_POINT_DOUBLE laserOrigin)
{
	// variable for mapping positions to tracks
	map<unsigned int, C3_CORRELATE_struct> position2track;
		
	// If there is no existing tracks just pre create for the current frame
	if (this->m_tracks.empty() == true)
	{
		// Create a tracker for each new point
		for (unsigned int ii = 0; ii < cameraRoverPositions.size(); ii++)
		{
			C3_CORRELATE_struct correlate;
			// Add a new track for the ii camera rover position
			correlate.assignTrackIndex = AddTrack(cameraRoverPositions[ii],time)-1;	// set the tracker ID
			// add to the map
			position2track.insert(make_pair(ii,correlate));
		}
	}
	else // ok so tracks exist... so lets correlate the positions
	{	
		// correlate the points to trackers.
		correlatePositions2Trackers(&position2track,cameraRoverPositions,time);
	}
	
	// iterator
	std::map<unsigned int, C3_CORRELATE_struct>::iterator iter;
    
	vector<C3_TRACK_POINT_DOUBLE> commands;
	// loop through all items in the map..
    for (iter = position2track.begin(); iter != position2track.end(); iter++) 
	{
		// check to see if the tracker was just created
		// if it has there is no reason to reupdate the tracker
		if ((*iter).second.isJustCreated == false)
		{
			//Filter the points
			commands.push_back(m_tracks[iter->second.assignTrackIndex]->UpdateTrack(cameraRoverPositions[iter->first],cameraLaserPosition,time,laserOrigin));
		}
	}
	// find the command to send out
	// TODO find a better way based on the age of the tracker
	bool found = false;
	C3_TRACK_POINT_DOUBLE result;
	
	result.AZ = 0;
	result.EL = 0;

	for (unsigned int yy=0; yy < commands.size() && !found; yy++)
	{
		if (commands[yy].AZ != 0 && commands[yy].EL != 0)
		{
			found = true;
			result = commands[yy];
		}
	}
	return result;
}
// Correlates a point with a tracker
void C3TrackerManager::correlatePositions2Trackers(map<unsigned int, C3_CORRELATE_struct> *position2track, 
											       const vector<C3_TRACK_POINT_DOUBLE> cameraRoverPositions, 
											       const double time)
{
	// loop over all the track positions
	for (unsigned int ii = 0; ii < cameraRoverPositions.size(); ii++)
	{
		C3_CORRELATE_struct correlate;
		double minValue    = numeric_limits<double>::max();
		
		// loop over all the camera measurements
		for (unsigned int jj = 0; jj < m_tracks.size(); ii++)
		{
			// first check if the measurement is in the vicinity of the track, 
			// use 3 sigma around projected point 
			double sigma = m_tracks[jj]->getSigma();
			double threshold = min(3.0*sigma,5.0);
			C3_TRACK_POINT_DOUBLE predPoint = m_tracks[jj]->getPointPropogatedToTime(time);
			correlate.dist = C3Utilities::EuclideanDistance(cameraRoverPositions[ii],predPoint);
			// make this a configuration item...
			if(threshold>correlate.dist)
			{
				// assign to the closest tracker if within max distance and not in mapping list
				if (correlate.dist <= minValue && correlate.dist <= m_maxDistance && !isInMapping(position2track, jj))
				{
					correlate.assignTrackIndex = jj;				// ok so this is the tracker for the point
					correlate.isJustCreated    = false;				// 
					minValue				   = correlate.dist;	// new min value for this point...
				}
			} 
		}

		// if there was no success then repeat
		if (correlate.assignTrackIndex == -1)
		{
			// Add a new track for the ii camera rover position
			correlate.assignTrackIndex = AddTrack(cameraRoverPositions[ii],time)-1;
		}

		// add the position to the list of mappings
		position2track->insert(make_pair(ii,correlate));
	}
}
////////////////////////////////////////////////////////////////////////////////////////
// Function: AddTrack
// Programmer: Benjamin Kurt Heiner
// 
// Purpose: this function creates a new track given a input rover location and time
////////////////////////////////////////////////////////////////////////////////////////
unsigned int C3TrackerManager::AddTrack(const C3_TRACK_POINT_DOUBLE cameraRoverPosition, const double time)
{
	// Create the new Track
	C3Track *track = NULL;

	// CREATE A NEW TRACK TO PUT INTO THE TRACKER MANAGER
	track = new C3Track(cameraRoverPosition,time);

	// add the track to the array of tracks
	m_tracks.push_back(track);

	// RETURN THE NUMBER OF TRACkS
	return m_tracks.size();
}
// determine if a point has already been assigned to the tracker 
bool C3TrackerManager::isInMapping(const map<unsigned int, C3_CORRELATE_struct> *position2track, 
							const unsigned int trackerNum)
{
	// flag...
	bool found = false;

	// iterator
	std::map<unsigned int, C3_CORRELATE_struct>::const_iterator iter;
    
	// loop through all items in the map..
    for (iter = position2track->begin(); iter != position2track->end() && found == false; iter++) 
	{
		if (iter->second.assignTrackIndex == trackerNum)
		{
			found = true;
		}
    }

	// returns if the tracker has already been used
	return found;
}
C3_TRACK_POINT_DOUBLE C3TrackerManager::getPredictedPoint() const
{
	return this->m_tracks[0]->getPredictedPoint();
}
