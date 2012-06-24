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
	int GetValidPips();
	int GetTotalTracks();
    CoordinatePair^ GetLaserPoint();
    bool SetLaserPoint(CoordinatePair ^LaserPointUpdate, bool HasLaserPoint);
    bool LaserPointIsValid();
	array<CoordinatePair^>^ GetPipCoordinates();
	bool SetPipCoordinates(array<CoordinatePair^>^ PipCoordinateUpdate, int NumValidTracks);
	bool LaserIsLocated();
	CoordinatePair^ GetLaserLocation();
	bool SetLaserLocation(CoordinatePair^ LaserLocation, bool HasLaserLocation);
    CoordinatePair^ GetCameraLocation();
	void CleanUp();

private:
    bool IsLaserValid;
    CoordinatePair^ LaserWorldCoords;
    CoordinatePair^ LaserPixelCoords;
	bool IsLaserLocated;
	CoordinatePair^ LaserWorldLocation;
	CoordinatePair^ LaserPixelLocation;
    CoordinatePair^ CameraWorldLocation;
    CoordinatePair^ CameraPixelLocation;
	Coordinates(int TrackNum);
	~Coordinates();
	CoordinatePair^ TranslateCoords(CoordinatePair^);
	array<CoordinatePair^>^ TranslateCoords(array<CoordinatePair^>^ WorldCoords);
	array<CoordinatePair^>^ CurWorldCoords;
	array<CoordinatePair^>^ OldWorldCoords;
	array<CoordinatePair^>^ PixelCoords;
	array<CoordinatePair^>^ PipWorldCoords;
	array<CoordinatePair^>^ PipPixelCoords;
	CoordinatePair^ PixelBounds;
	CoordinatePair^ WorldBounds;
    CoordinatePair^ PixelShift;
    RatioPair^ WorldPxRatio;
	int ValidTracks;
	int ValidPips;
	int TotalTracks;
	static Coordinates ^ coordsObj = nullptr;
};



#endif