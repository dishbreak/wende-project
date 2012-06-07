#include "stdafx.h"
#include "Coordinates.h"

//Make sure pointer starts as NULL
//Coordinates ^ Coordinates::coordsObj = nullptr;
CoordinatePair::CoordinatePair() {
	x = 0;
	y = 0;
}

CoordinatePair::CoordinatePair(int xInput, int yInput){
	x = xInput;
	y = yInput;
}

CoordinatePair::CoordinatePair(CoordinatePair ^Other) {
	x = Other->x;
	y = Other->y;
}

//CoordinatePair^ CoordinatePair::operator->() {
//	return this;
//}

Coordinates::Coordinates(int TrackNum) {
	//initialize all coordinate pairs
	TotalTracks = TrackNum;
	WorldBounds = gcnew CoordinatePair(2000,2000);
	PixelBounds = gcnew CoordinatePair(300,300);
	//CoordinatePair PixelBounds(300,300);
	CurWorldCoords = MakeCoordinatePairArray();
	OldWorldCoords = MakeCoordinatePairArray();
	PixelCoords = MakeCoordinatePairArray();
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
		//bool TrackChanged = SetNewCoordinatePair(InputSet[i], CurWorldCoords[i], OldWorldCoords[i]);
	}
	PixelCoords = TranslateCoords(CurWorldCoords);
	return HasNewTrack;
}

bool Coordinates::SetNewCoordinatePair(CoordinatePair^ Input, CoordinatePair^ Current, CoordinatePair^ Prev) {
	//Only update if the coordinates are new
	bool IsNewTrack = false;
	if ((Input->x != Current->x) || (Input->y != Current->y)) {
		IsNewTrack = true;
		//Prev = CoordinatePair(Current);
		//Current = CoordinatePair(Input);
	}
	return IsNewTrack;
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
	coordsRatio = (float) WorldCoords->x / WorldBounds->x;
	newCoords->x = (int) (coordsRatio * PixelBounds->x);
	//Translate Y
	coordsRatio = 0.0;
	coordsRatio = (float) WorldCoords->y / WorldBounds->y;
	newCoords->y = (int) (coordsRatio * PixelBounds->y);
	return newCoords;
}

int Coordinates::GetValidTracks() {
	return ValidTracks;
}

int Coordinates::GetTotalTracks() {
	return TotalTracks;
}
