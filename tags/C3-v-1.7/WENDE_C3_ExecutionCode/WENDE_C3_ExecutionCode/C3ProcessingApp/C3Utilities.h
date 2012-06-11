#pragma once

#include "ShmStructs.h"
class C3Utilities
{
	public:
		static double EuclideanDistance(C3_TRACK_POINT p1, C3_TRACK_POINT p2);
		static double EuclideanDistance(C3_TRACK_POINT p1);
		static double EuclideanDistance(C3_TRACK_POINT_DOUBLE p1, C3_TRACK_POINT_DOUBLE p2);
		static double EuclideanDistance(C3_TRACK_POINT_DOUBLE p1);

};

