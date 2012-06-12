#include "StdAfx.h"
#include "C3TrackerManager.h"
#include "C3Utilities.h"
#include <algorithm>

using namespace std;

C3Tracker::C3Tracker(void)
	: m_maxDistance(5)
{
}

C3Tracker::~C3Tracker(void)
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

void C3Tracker::UpdateTracks(const vector<C3_TRACK_POINT> cameraRoverPositions, const double time)
{
	// variable for mapping positions to tracks
	map<unsigned int, C3_CORRELATE_struct> position2track;
		
	// If there is no existing tracks just pre create for the current frame
	if (this->m_tracks.empty() == true)
	{
		C3_CORRELATE_struct correlate;
		// Create a tracker for each new point
		for (unsigned int ii = 0; ii < cameraRoverPositions.size(); ii++)
		{
			// Add a new track for the ii camera rover position
			correlate.assignTrackIndex = AddTrack(cameraRoverPositions[ii],time)-1;
			correlate.dist = 0;
		}
	}
	else // ok so tracks exist... so lets correlate the positions
	{	
		// correlate the points to trackers.
		correlatePositions2Trackers(&position2track,cameraRoverPositions,time);
	}
	
	// iterator
	std::map<unsigned int, C3_CORRELATE_struct>::iterator iter;
    
	// loop through all items in the map..
    for (iter = position2track.begin(); iter != position2track.end(); iter++) 
	{
		// Filter the points
		//m_tracks[iter->second.assignTrackIndex]->UpdateTrack(cameraRoverPositions[iter->first],time);

		// Compute the azimuth and elevation information
		// TODO ... add in the code
	}
}
// Correlates a point with a tracker
void C3Tracker::correlatePositions2Trackers(map<unsigned int, C3_CORRELATE_struct> *position2track, 
											const vector<C3_TRACK_POINT> cameraRoverPositions, 
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
			// first check if the measurement is in the vicinity of the track, use 3 sigma around projected point //fc start
			//C3_TRACK_POINT_DOUBLE projectedPoint = m_tracks.cameraRoverPositions[ii].getPointPropogatedToTime(time);
			//double sigma = max(sqrt(cameraRoverPositions[ii].m_filter.P[1][1]),sqrt(cameraRoverPositions[ii].m_filter.P[1][1]));
			//if(min(3*sigma,5)>distanceFromTo(projectedPoint,m_tracks{jj]))//min(3*max(sqrt(track.P[1][1]),sqrt(track.P[2][2])),one foot)> sqrt(pow(xest-xmeas,2)+pow(yest-ymeas,2)))
			//{
				// if there is not enough history then only use the last position...
				if ( m_tracks[jj]->getNumHistoryPoints() <= 3 )
				{
					//correlate.dist = C3Utilities::EuclideanDistance(cameraRoverPositions[ii],m_tracks[jj]->getLastHistoryPoint());
				}
				// ok... we have the history points needed so lets coorelate using the 
				// prediction point (assumes velocity)
				else
				{
					//correlate.dist = C3Utilities::EuclideanDistance(cameraRoverPositions[ii],m_tracks[jj]->getPredictionPoint());
				}

				// assign to the closest tracker if within max distance and not in mapping list
				if (correlate.dist <= minValue && correlate.dist <= m_maxDistance && !isInMapping(position2track, jj))
				{
					correlate.assignTrackIndex = jj;				// ok so this is the tracker for the point
					minValue				   = correlate.dist;	// new min value for this point...
				}
			//} //fc end
			
			// else // if rejected than that means it is a new track and we call AddTrack
			
			
			
			// if rejected due to mapping then what
			// TODO ... determine which is better to use
			//else if (correlate.dist <= minValue && correlate.dist <= m_maxDistance)
			//{
			//	
			//}
		}

		// if there was no success then repeat
		if (correlate.assignTrackIndex == -1)
		{
			// Add a new track for the ii camera rover position
			correlate.assignTrackIndex = AddTrack(cameraRoverPositions[ii],time)-1;
			correlate.dist = 0;
		}

		// add the position to the list of mappings
		position2track->insert(pair<unsigned int, C3_CORRELATE_struct>(ii,correlate));
	}
}
unsigned int C3Tracker::AddTrack(const C3_TRACK_POINT cameraRoverPosition, const double time)
{
	// Create the new Track
	C3Track *track = NULL;

	// Determine if the tracker is within the playing field for initial point 
	double dist = C3Utilities::EuclideanDistance(cameraRoverPosition);

	// Check to see if the track is in the playing field
	if (dist > m_playingFieldRadius)	// outside the playing field
	{									// so do not track
		//track = new C3Track(cameraRoverPosition,time,false,2.0);
	}
	else								// inside the playing field
	{									// so track 
		//track = new C3Track(cameraRoverPosition,time,true,2.0);
	}

	// add the track to the array of tracks
	m_tracks.push_back(track);

	//TODO ... FIX THIS
	return m_tracks.size();
}


// we do this calculation alll the time... refactor to a method
double C3Tracker::distanceFromTo(C3_TRACK_POINT_DOUBLE meas1,C3_TRACK_POINT_DOUBLE meas2)
{

	return sqrt(pow(meas2.X -meas1.X,2)+pow(meas2.Y-meas1.Y,2));

}
// determine if a point has already been assigned to the tracker 
bool C3Tracker::isInMapping(const map<unsigned int, C3_CORRELATE_struct> *position2track, 
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