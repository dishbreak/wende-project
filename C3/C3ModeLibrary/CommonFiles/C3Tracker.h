#pragma once
#include <vector>
#include "C3Track.h"
#include "C3Point.h"
using std::vector;

class C3Tracker
{
	private:
		vector<C3Track*> m_tracks;

	public:
		C3Tracker(void);
		~C3Tracker(void);

	public:
		void UpdateTracks(vector<C3Point> cameraRoverPositions, unsigned int time);
};