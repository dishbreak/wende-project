#ifndef COORDINATES_H
#define COORDINATES_H

#include "stdafx.h"

class Coordinates {
public:
	static Coordinates * GetCoordinatesHandle();
	void SetNewCoordinatePair(int xCoord, int yCoord);
	struct CoordinatePair {
		int x;
		int y;
	};
	//struct RatioPair {
	//	float x;
	//	float y;
	//}
	CoordinatePair GetNewCoordinatePair();
private:
	Coordinates();
	~Coordinates();
	CoordinatePair TranslateCoords(CoordinatePair);
	CoordinatePair CurWorldCoords;
	CoordinatePair OldWorldCoords;
	CoordinatePair PixelCoords;
	CoordinatePair PixelBounds;
	CoordinatePair WorldBounds;
	static Coordinates * coordsObj;
};

#endif