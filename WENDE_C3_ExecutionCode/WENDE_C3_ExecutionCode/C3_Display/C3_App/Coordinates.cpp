#include "stdafx.h"
#include "Coordinates.h"
#include <math.h>

//Make sure pointer starts as NULL
//Coordinates ^ Coordinates::coordsObj = nullptr;
CoordinatePair::CoordinatePair() {
	x = -100000; //Assumption: Contact 10km away from world center will not be  
	y = -100000; //            visible via the camera at initial state.
	IsOutOfBounds = false;
}

CoordinatePair::CoordinatePair(int xInput, int yInput){
	x = xInput;
	y = yInput;
	IsOutOfBounds = false;
}

CoordinatePair::CoordinatePair(CoordinatePair ^Other) {
	x = Other->x;
	y = Other->y;
	IsOutOfBounds = Other->IsOutOfBounds;
}

RatioPair::RatioPair() {
    x = 0.0;
    y = 0.0;
}

Coordinates::Coordinates(int TrackNum) {
	//initialize all coordinate pairs
	TotalTracks = TrackNum;
	WorldBounds = gcnew CoordinatePair(2000,2000);
	PixelBounds = gcnew CoordinatePair(300,300);
    WorldPxRatio = gcnew RatioPair();
    PixelShift = gcnew CoordinatePair();
    IsLaserValid = false;
    IsLaserLocated = false;
    LaserPixelCoords = gcnew CoordinatePair();
    LaserWorldCoords = gcnew CoordinatePair();
    LaserPixelLocation = gcnew CoordinatePair();
    LaserWorldLocation = gcnew CoordinatePair();
    CameraWorldLocation = gcnew CoordinatePair(0,-2000);
    
	CurWorldCoords = MakeCoordinatePairArray();
	OldWorldCoords = MakeCoordinatePairArray();
	PixelCoords = MakeCoordinatePairArray();
	PipWorldCoords = MakeCoordinatePairArray();
	PipPixelCoords = MakeCoordinatePairArray();

    //calculate the ratio between world and pixels
    WorldPxRatio->x = (float) 0.5 * PixelBounds->x / WorldBounds->x;
    WorldPxRatio->y = (float) -0.5 * PixelBounds->y / WorldBounds->y;
    //calculate the shift between world and pixels
    PixelShift->x = PixelBounds->x / 2;
    PixelShift->y = PixelBounds->y / 2;
    CameraPixelLocation = TranslateCoords(CameraWorldLocation);
}

Coordinates::~Coordinates() {

}

Coordinates ^ Coordinates::GetCoordinatesHandle() {
	if (!coordsObj) {
		coordsObj = gcnew Coordinates(SHM_MAX_TRACKS);
	}
	return coordsObj;
}

array<CoordinatePair^>^ Coordinates::MakeCoordinatePairArray() {
	array<CoordinatePair^>^ output = gcnew array<CoordinatePair^>(TotalTracks);
	for(int i = 0; i < TotalTracks; i++) {
		output[i] = gcnew CoordinatePair();
	}
	return output;
}

bool Coordinates::SetNewCoordinates(array<CoordinatePair^>^ InputSet, int NumValidTracks) {
	bool HasNewTrack = false;
	ValidTracks = NumValidTracks;
	for(int i = 0; i < TotalTracks ; i++) {
        if((InputSet[i]->x != CurWorldCoords[i]->x) || (InputSet[i]->y != CurWorldCoords[i]->y)) {
            HasNewTrack = true;
            OldWorldCoords[i]->x = CurWorldCoords[i]->x;
            OldWorldCoords[i]->y = CurWorldCoords[i]->y;
            CurWorldCoords[i]->x = InputSet[i]->x;
            CurWorldCoords[i]->y = InputSet[i]->y;     
        }
	}
	PixelCoords = TranslateCoords(CurWorldCoords);
	return HasNewTrack;
}


array<CoordinatePair^>^ Coordinates::GetNewCoordinatePair() {
	return PixelCoords;
}

array<CoordinatePair^>^ Coordinates::TranslateCoords(array<CoordinatePair^>^ WorldCoords) {
	array<CoordinatePair^>^ output = MakeCoordinatePairArray();
	for( int i=0; i<WorldCoords->Length; i++) {
		output[i] = TranslateCoords(WorldCoords[i]);
	}
	return output;
}

CoordinatePair^ Coordinates::TranslateCoords(CoordinatePair^ WorldCoords) {
	CoordinatePair^ newCoords = gcnew CoordinatePair();
	float coordsRatio = 0.0;
	//Translate X
    newCoords->x = (int) (WorldPxRatio->x * WorldCoords->x) + PixelShift->x;
	//Translate Y
    newCoords->y = (int) (WorldPxRatio->y * WorldCoords->y) + PixelShift->y;
	if ((abs(newCoords->x) > PixelBounds->x) || (abs(newCoords->y) > PixelBounds->y)) {
		newCoords->IsOutOfBounds = true;
	}
	return newCoords;
}

int Coordinates::GetValidTracks() {
	return ValidTracks;
}

int Coordinates::GetTotalTracks() {
	return TotalTracks;
}

CoordinatePair^ Coordinates::GetLaserPoint() {
    return LaserPixelCoords;
}

bool Coordinates::SetLaserPoint(CoordinatePair ^LaserPointUpdate, bool HasLaserPoint) {
    bool IsNewPoint = false;
    IsLaserValid = HasLaserPoint;
    if ((LaserPointUpdate->x != LaserWorldCoords->x) || (LaserPointUpdate->y != LaserWorldCoords->y)) {
        IsNewPoint = true;    
        LaserWorldCoords->x = LaserPointUpdate->x;
        LaserWorldCoords->y = LaserPointUpdate->y;
        LaserPixelCoords = TranslateCoords(LaserWorldCoords);
        if ((abs(LaserPixelCoords->x) > PixelBounds->x) || (abs(LaserPixelCoords->y) > PixelBounds->y)) {
            LaserPixelCoords->IsOutOfBounds = true;
        }
    }
    return IsNewPoint;
}

bool Coordinates::LaserPointIsValid() {
    return IsLaserValid;
}


array<CoordinatePair^>^ Coordinates::GetPipCoordinates() {
	return PipPixelCoords;
}

bool Coordinates::SetPipCoordinates(array<CoordinatePair^>^ PipCoordinateUpdates) {
	bool HasNewPips = false;
	for(int i = 0; i < TotalTracks; i++) {
		if( PipWorldCoords[i]->x != PipCoordinateUpdates[i]->x ||
			PipWorldCoords[i]->y != PipCoordinateUpdates[i]->y) {
				HasNewPips = true;
				PipWorldCoords[i]->x = PipCoordinateUpdates[i]->x;
				PipWorldCoords[i]->y = PipCoordinateUpdates[i]->y;
		}
	}
	PipPixelCoords = TranslateCoords(PipWorldCoords);
	return HasNewPips;
}

bool Coordinates::LaserIsLocated() {
    return IsLaserLocated;
}

CoordinatePair^ Coordinates::GetLaserLocation() {
	return LaserPixelLocation;
}

bool Coordinates::SetLaserLocation(CoordinatePair^ LaserLocation, bool HasLaserLocation) {
    bool HaveNewLocation = false;
    IsLaserLocated = HasLaserLocation;
    if (LaserWorldLocation->x != LaserLocation->x || 
        LaserWorldLocation->y != LaserLocation->y)
    {
        HaveNewLocation = true;
        LaserWorldLocation->x = LaserLocation->x;
        LaserWorldLocation->y = LaserLocation->y;
    }
    LaserPixelCoords = TranslateCoords(LaserWorldCoords);
    return HaveNewLocation;
}

CoordinatePair^ Coordinates::GetCameraLocation() {
    return CameraPixelLocation;
}
