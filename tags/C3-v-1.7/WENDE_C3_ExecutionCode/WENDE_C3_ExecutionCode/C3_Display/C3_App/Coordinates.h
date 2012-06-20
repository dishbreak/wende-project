#ifndef COORDINATES_H
#define COORDINATES_H

#include "stdafx.h"
#include "ShmStructs.h"

ref class CoordinatePair {
public:
	int x;
	int y;
	bool IsOutOfBounds;
	CoordinatePair();
	CoordinatePair(int xInput, int yInput);
	CoordinatePair(CoordinatePair ^Other);
	//CoordinatePair^ operator->();
};


ref class RatioPair {
public:
    float x;
    float y;
    RatioPair();
};

ref class Coordinates {
public:
	static Coordinates ^ GetCoordinatesHandle();
	array<CoordinatePair^>^ MakeCoordinatePairArray();
	bool SetNewCoordinates(array<CoordinatePair^>^ InputSet,int NumValidTracks);
	array<CoordinatePair^>^ GetNewCoordinatePair();
	int GetValidTracks();
	int GetTotalTracks();
private:
	bool SetNewCoordinatePair(CoordinatePair^ Input, CoordinatePair^ Current, CoordinatePair^ Prev);
	Coordinates(int TrackNum);
	~Coordinates();
	CoordinatePair^ TranslateCoords(CoordinatePair^);
	array<CoordinatePair^>^ TranslateCoords(array<CoordinatePair^>^ WorldCoords);
	array<CoordinatePair^>^ CurWorldCoords;
	array<CoordinatePair^>^ OldWorldCoords;
	array<CoordinatePair^>^ PixelCoords;
	CoordinatePair^ PixelBounds;
	CoordinatePair^ WorldBounds;
    CoordinatePair^ PixelShift;
    RatioPair^ WorldPxRatio;
	int ValidTracks;
	int TotalTracks;
	static Coordinates ^ coordsObj = nullptr;
};



#endif