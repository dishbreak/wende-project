//#ifdef STDAFX_INCLUDE
//#include "StdAfx.h"
//#endif
#include "C3Tracker.h"

C3Tracker::C3Tracker(void)
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
		for (unsigned int ii = 0; ii < cameraRoverPositions.size(); ii++)
		{
			C3Track *track = new C3Track(cameraRoverPositions[ii],time);
		}
	}
}