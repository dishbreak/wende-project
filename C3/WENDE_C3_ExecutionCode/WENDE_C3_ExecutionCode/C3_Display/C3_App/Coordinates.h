#include "stdafx.h"

class Coordinates {
public:
	static Coordinates * GetCoordinatesHandle();
	void SetNewCoordinatePair(int,int);
	struct CoordinatePair {
		int x;
		int y;
	};
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