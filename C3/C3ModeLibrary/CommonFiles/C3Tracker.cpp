//#ifdef STDAFX_INCLUDE
//#include "StdAfx.h"
//#endif

#include "C3Tracker.h"
#include "C3Utilities.h"

C3Tracker::C3Tracker(void)
	: m_maxDistance(5)
{
}

C3Tracker::~C3Tracker(void)
{
}

void C3Tracker::UpdateTracks(vector<C3Point> cameraRoverPositions, unsigned int time)
{
	// If there is no existing tracks just pre create for the current frame
	if (this->m_tracks.empty() == true)
	{
		// Create a tracker for each new point
		for (unsigned int ii = 0; ii < cameraRoverPositions.size(); ii++)
		{
			// Add a new track for the ii camera rover position
			AddTrack(cameraRoverPositions[ii],time);
		}
	}
	else // ok so tracks exist... so lets correlate the positions
	{
		C3_CORRELATE_struct correlate;
		double minValue    = numeric_limits<double>::max();

		// loop over all the positions
		for (unsigned int ii = 0; ii < cameraRoverPositions.size(); ii++)
		{
			// loop over all the points
			for (unsigned int jj = 0; jj < m_tracks.size(); ii++)
			{
				// if there is not enough history then only use the last position...
				if ( m_tracks[jj]->getNumHistoryPoints() <= 2 )
				{
					correlate.dist = C3Utilities::EuclideanDistance(cameraRoverPositions[ii],m_tracks[jj]->getLastHistoryPoint());
				}
				// ok... we have the history points needed so lets coorelate using the 
				// prediction point (assumes velocity)
				else
				{
					correlate.dist = C3Utilities::EuclideanDistance(cameraRoverPositions[ii],m_tracks[jj]->getPredicationPoint());
				}

				// assign to the closest tracker if with max distance
				if (correlate.dist <= minValue  && correlate.dist <= m_maxDistance)
				{
					correlate.assignTrackIndex = jj;				// ok so this is the tracker for the point
					correlate.assignPointIndex = ii;				// ok so this is the point for the tracker
					minValue				   = correlate.dist;	// new min value for this point...
				}
			}
		}
	}
}

void C3Tracker::AddTrack(C3Point cameraRoverPosition, unsigned int time)
{
	// Create the new Track
	C3Track *track = NULL;

	// Determine if the tracker is within the playing field for initial point 
	double dist = C3Utilities::EuclideanDistance(cameraRoverPosition);

	// Check to see if the track is in the playing field
	if (dist > m_playingFieldRadius)	// outside the playing field
	{									// so do not track
		track = new C3Track(cameraRoverPosition,time,false);
	}
	else								// inside the playing field
	{									// so track 
		track = new C3Track(cameraRoverPosition,time,true);
	}
}