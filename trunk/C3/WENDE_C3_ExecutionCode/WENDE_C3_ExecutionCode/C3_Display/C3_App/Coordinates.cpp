#include "stdafx.h"
#include "Coordinates.h"

//Make sure pointer starts as NULL
Coordinates * Coordinates::coordsObj = 0;

Coordinates::Coordinates() {
	//initialize all coordinate pairs
	Coordinates::CurWorldCoords.x = 0;
	Coordinates::CurWorldCoords.y = 0;
	Coordinates::OldWorldCoords.x = 0;
	Coordinates::OldWorldCoords.y = 0;
	Coordinates::WorldBounds.x    = 5000;
	Coordinates::WorldBounds.y    = 5000;
	Coordinates::PixelBounds.x    = 300;
	Coordinates::PixelBounds.y    = 300;
}

Coordinates::~Coordinates() {

}

Coordinates * Coordinates::GetCoordinatesHandle() {
	if (!coordsObj) {
		coordsObj = new Coordinates();
	}
	return coordsObj;
}

void Coordinates::SetNewCoordinatePair(int xCoord, int yCoord) {
	//Only update if the coordinates are new
	if (xCoord != CurWorldCoords.x && yCoord != CurWorldCoords.y)
	{
		//Copy old coords out
		OldWorldCoords.x = CurWorldCoords.x;
		OldWorldCoords.y = CurWorldCoords.y;
		//Bring in new coords
		CurWorldCoords.x = xCoord;
		CurWorldCoords.y = yCoord;
		//Turn the new coords into Pixel Coords
		PixelCoords = Coordinates::TranslateCoords(CurWorldCoords);
	}
	
}

Coordinates::CoordinatePair Coordinates::GetNewCoordinatePair() {
	return Coordinates::PixelCoords;
}

Coordinates::CoordinatePair Coordinates::TranslateCoords(Coordinates::CoordinatePair WorldCoords) {
	Coordinates::CoordinatePair newCoords;
	float coordsRatio = 0.0;
	//Translate X
	coordsRatio = (float) WorldCoords.x / WorldBounds.x;
	newCoords.x = coordsRatio * PixelBounds.x;
	//Translate Y
	coordsRatio = 0.0;
	coordsRatio = (float) WorldCoords.y / WorldBounds.y;
	newCoords.y = coordsRatio * PixelBounds.y;
	return newCoords;
}