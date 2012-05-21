#include "stdafx.h"
#include "CDisplayManager.h"
using namespace C3_App;

//initialize static pointer to 0
CDisplayManager * CDisplayManager::displayMgr = 0;

// Class variable for determining overall status
int m_nCameraStatus;
int m_nLaserStatus;
////////////////////////////////////////////////////////////////////////
// Description: Returns a pointer to the CDisplayManager. If there is no
//              CDisplayManager, one is new'd and returned.
// Author:      Vishal Kotcherlakota
////////////////////////////////////////////////////////////////////////
CDisplayManager * CDisplayManager::getCDisplayManager()
{
    if(!displayMgr)
    {
        displayMgr = new CDisplayManager();
    }
    return displayMgr;
}
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
	// Status is OFFLINE
	if((nCameraStatus == 0) || (nCameraStatus == 3) || (nCameraStatus == 4)) 	
	{
		C3_User_Interface::Instance->pbCameraStatus->Image = 
			System::Drawing::Image::FromFile ("Offline.png");

		Set_Camera_Status(0);
	}
	// Status is ONLINE
	else 	
	{
		C3_User_Interface::Instance->pbCameraStatus->Image = 
			System::Drawing::Image::FromFile ("Online.png");

		Set_Camera_Status(1);
	}

	return 0;
}
////////////////////////////////////////////////////////////////////////
// Description: Updates the Laser subsystem indicator following a call 
//				to the static instance of the C3_User_Interface object					
// Author:		Mike Payne
////////////////////////////////////////////////////////////////////////
int CDisplayManager::Update_Laser_Subsystem_Indicator(int nLaserStatus)
{
	if(nLaserStatus >= 3) 	// TODO: Change after laser interface added
	{
		C3_User_Interface::Instance->pbLaserStatus->Image = 
			System::Drawing::Image::FromFile ("Offline.png");

		Set_Laser_Status(0);
	}

	if(nLaserStatus <= 2) 	// TODO: Change after laser interface added
	{
		C3_User_Interface::Instance->pbLaserStatus->Image = 
			System::Drawing::Image::FromFile ("Online.png");

		Set_Laser_Status(1);
	}

	return 0;
}
////////////////////////////////////////////////////////////////////////
// Description: Updates the Laser Activity indicator following a call 
//				to the static instance of the C3_User_Interface object					
// Author:		Mike Payne
////////////////////////////////////////////////////////////////////////
int CDisplayManager::Update_Laser_Activity_Indicator(int nLaserActivityStatus)
{
	if(nLaserActivityStatus == 1)
	{
		C3_User_Interface::Instance->pbLaserActivity->Image = 
			System::Drawing::Image::FromFile ("Energized.png");
	}

	if(nLaserActivityStatus == 0)
	{
		C3_User_Interface::Instance->pbLaserActivity->Image = 
			System::Drawing::Image::FromFile ("Inactive.png");
	}

	return 0;
}
////////////////////////////////////////////////////////////////////////
// Description: Updates the overall WENDE status as an aggregation of  
//				subsystem status'				
// Author:		Mike Payne
////////////////////////////////////////////////////////////////////////
int CDisplayManager::Update_Overall_Status(void)
{
	int nLaserSubsystemStatus = Get_Laser_Status();
	int nCameraSubsystemStatus = Get_Camera_Status();

	// Set status to ONLINE if camera and laser are OK
	if((nLaserSubsystemStatus == 1) && (nCameraSubsystemStatus == 1))
		C3_User_Interface::Instance->pbOverallStatus->Image = 
			System::Drawing::Image::FromFile ("Online.png");
	else
		C3_User_Interface::Instance->pbOverallStatus->Image = 
			System::Drawing::Image::FromFile ("Offline.png");

	return 0;
}
void CDisplayManager::Set_Camera_Status(int nCameraStatus)
{
	m_nCameraStatus = nCameraStatus;
}
int CDisplayManager::Get_Camera_Status(void)
{
	return m_nCameraStatus;
}
void CDisplayManager::Set_Laser_Status(int nLaserStatus)
{
	m_nLaserStatus = nLaserStatus;
}
int CDisplayManager::Get_Laser_Status(void)
{
	return m_nLaserStatus;
}
////////////////////////////////////////////////////////////////////////
// Description: Updates the Camera communications indicator following a 
//				call to the static instance of the C3_User_Interface 
//				object					
// Author:		Vishal Kotcherlakota
////////////////////////////////////////////////////////////////////////
int CDisplayManager::Update_Camera_Communication_Indicator(int nCameraCommStatus)
{
	if (nCameraCommStatus == 0) //Status is OFFLINE
	{
		//Set Indicator Offline
		C3_User_Interface::Instance->pbCameraComms->Image = 
			System::Drawing::Image::FromFile ("Offline.png");
		//Set Subsystem Indicator Unknown
		//uncomment this when ready.
		C3_User_Interface::Instance->pbCameraStatus->Image =
			System::Drawing::Image::FromFile ("Unknown.png");
	}
	if (nCameraCommStatus == 1)
		//Set Indicator Online
		C3_User_Interface::Instance->pbCameraComms->Image = 
			System::Drawing::Image::FromFile ("Online.png");
	return 0;
}

int CDisplayManager::Update_Live_Video_Feed(String ^ sImagePath)
{
	C3_User_Interface::Instance->pbLiveFeed->Image = 
		System::Drawing::Image::FromFile(sImagePath);
	return 0;
}

////////////////////////////////////////////////////////////////////////
// Description: Updates the notifications panel following a 
//				call to the static instance of the C3_User_Interface 
//				object - passing in Alert ID					
// Author:		Mike Payne
////////////////////////////////////////////////////////////////////////
void CDisplayManager::Update_Notification_Panel(int nAlertID)
{
	switch(nAlertID)
	{
		case 1:
			C3_User_Interface::Instance->tbAlertsPanel->Text = 
				L"INFORMATION: TRIAL SUCCESS";
		case 2:
			C3_User_Interface::Instance->tbAlertsPanel->Text = 
				L"ALERT: PATIENT HAS LEFT EVAC AREA!";
		case 3:
			C3_User_Interface::Instance->tbAlertsPanel->Text = 
				L"ALERT: TRIAL FAILED";
		case 4:
			C3_User_Interface::Instance->tbAlertsPanel->Text = 
				L"WARNING: WENDE SYSTEM NOT OPERATIONAL";
		default:
			C3_User_Interface::Instance->tbAlertsPanel->Text = 
				L"INFORMATION: WENDE SYSTEM OPERATIONAL";
	}
}
