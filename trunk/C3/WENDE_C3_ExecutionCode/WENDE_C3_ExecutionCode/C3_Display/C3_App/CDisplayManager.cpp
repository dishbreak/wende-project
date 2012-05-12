#include "stdafx.h"
#include "CDisplayManager.h"
using namespace C3_App;

////////////////////////////////////////////////////////////////////////
// Description: Updates the PPI display when the new rover position is
//				sent through					
// Author:		Mike Payne
////////////////////////////////////////////////////////////////////////

int CDisplayManager::Update_Rover_PPI_Position(int x, int y)
{
	return 0;
}

////////////////////////////////////////////////////////////////////////
// Description: Updates the Camera subsystem indicator following a call 
//				to the static instance of the C3_User_Interface object					
// Author:		Mike Payne
////////////////////////////////////////////////////////////////////////

int CDisplayManager::Update_Camera_Subsystem_Indicator(int nCameraStatus)
{
	if(nCameraStatus == 0) 	// Status is OFFLINE
		C3_User_Interface::Instance->pbCameraStatus->Image = 
			System::Drawing::Image::FromFile ("Offline.jpg");

	if(nCameraStatus == 1) 	// Status is ONLINE
		C3_User_Interface::Instance->pbCameraStatus->Image = 
			System::Drawing::Image::FromFile ("Online.jpg");

	return 0;
}

////////////////////////////////////////////////////////////////////////
// Description: Updates the Camera communications indicator following a 
//				call to the static instance of the C3_User_Interface 
//				object					
// Author:		Vishal Kotcherlakota
////////////////////////////////////////////////////////////////////////
int CDisplayManager::Update_Camera_Communication_IndicatorI(int nCameraCommStatus)
{
	if (nCameraCommStatus == 0) //Status is OFFLINE
	{
		//Set Indicator Offline
		C3_User_Interface::Instance->pbCameraComms->Image = 
			System::Drawing::Image::FromFile ("Offline.jpg");
		//Set Subsystem Indicator Unknown
		//uncomment this when ready.
		//C3_User_Interface::Instance->pbCameraStatus->Image =
		//	System::Drawing::Image::FromFile ("Unknown.jpg");
	}
	if (nCameraCommStatus == 1)
		//Set Indicator Online
		C3_User_Interface::Instance->pbCameraComms->Image = 
			System::Drawing::Image::FromFile ("Online.jpg");
	return 0;
}


