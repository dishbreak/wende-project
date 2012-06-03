#include "stdafx.h"
#include "CDisplayManager.h"
#include <fstream>
#include <iostream> 

#include "Coordinates.h"


using namespace C3_App;
using namespace std;

//initialize static pointer to 0
CDisplayManager * CDisplayManager::displayMgr = 0;

// Class variable for determining overall status
int m_nCameraStatus;
int m_nLaserStatus;
int m_nLaserComStatus;
int m_nCameraComStatus;
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
	////for now, just dump everything to the globals.
	//extern int roverContactX;
	//extern int roverContactY;

	////only act on the information if it's new
	//if ((x != roverContactX) || (y != roverContactY))
	//{
	//	roverContactX = x;
	//	roverContactY = y;
	//	C3_User_Interface::Instance->pPPI->Invalidate();
	//}
	Coordinates* coordsObj = Coordinates::GetCoordinatesHandle();
	coordsObj->SetNewCoordinatePair(x, y);

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
			C3_User_Interface::Instance->OfflineInd;

		Set_Camera_Status(0);
	}
	// Status is ONLINE
	else 	
	{
		C3_User_Interface::Instance->pbCameraStatus->Image = 
			C3_User_Interface::Instance->OnlineInd;
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
	// Status is OFFLINE
	if((nLaserStatus == 0) || (nLaserStatus == 3) || (nLaserStatus == 4)) 		
	{
		C3_User_Interface::Instance->pbLaserStatus->Image = 
			C3_User_Interface::Instance->OfflineInd;

		Set_Laser_Status(0);
	}
	// Status is ONLINE
	else 	
	{
		C3_User_Interface::Instance->pbLaserStatus->Image = 
			C3_User_Interface::Instance->OnlineInd;

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
			C3_User_Interface::Instance->EnergizedInd;
	}

	if(nLaserActivityStatus == 0)
	{
		C3_User_Interface::Instance->pbLaserActivity->Image = 
			C3_User_Interface::Instance->InactiveInd;
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
	int nLaserComStatus = Get_Laser_Com_Status();
	int nCameraComStatus = Get_Camera_Com_Status();

	// Set status to ONLINE if camera and laser are OK
	if((nLaserSubsystemStatus == 1) && (nCameraSubsystemStatus == 1) && 
		(nLaserComStatus == 1) && (nCameraComStatus == 1))

		C3_User_Interface::Instance->pbOverallStatus->Image = 
			C3_User_Interface::Instance->OnlineInd;
	else
		C3_User_Interface::Instance->pbOverallStatus->Image = 
			C3_User_Interface::Instance->OfflineInd;

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
void CDisplayManager::Set_Camera_Com_Status(int nCameraComStatus)
{
	m_nCameraComStatus = nCameraComStatus;
}
int CDisplayManager::Get_Camera_Com_Status(void)
{
	return m_nCameraComStatus;
}
void CDisplayManager::Set_Laser_Com_Status(int nLaserComStatus)
{
	m_nLaserComStatus = nLaserComStatus;
}
int CDisplayManager::Get_Laser_Com_Status(void)
{
	return m_nLaserComStatus;
}



////////////////////////////////////////////////////////////////////////
// Description: Updates the Camera communications indicator following a 
//				call to the static instance of the C3_User_Interface 
//				object					
// Author:		Vishal Kotcherlakota
////////////////////////////////////////////////////////////////////////
int CDisplayManager::Update_Camera_Communication_Indicator(int nCameraCommStatus)
{
	// Status is OFFLINE
	if(nCameraCommStatus == 0) 		
	{
		C3_User_Interface::Instance->pbCameraComms->Image = 
			C3_User_Interface::Instance->OfflineInd;
		C3_User_Interface::Instance->pbCameraStatus->Image =
			C3_User_Interface::Instance->UnknownInd;
		Set_Camera_Com_Status(0);
		Set_Camera_Status(-1);
	}
	// Status is ONLINE
	else 	
	{
		C3_User_Interface::Instance->pbCameraComms->Image = 
			C3_User_Interface::Instance->OnlineInd;
		Set_Camera_Com_Status(1);
	}

	return 0;
}

int CDisplayManager::Update_Laser_Communication_Indicator(int nLaserCommStatus)
{
	// Status is OFFLINE
	if(nLaserCommStatus == 0) 		
	{
		C3_User_Interface::Instance->pbLaserComms->Image = 
			C3_User_Interface::Instance->OfflineInd;
		C3_User_Interface::Instance->pbLaserStatus->Image =
			C3_User_Interface::Instance->UnknownInd;
		Set_Laser_Com_Status(0);
		Set_Laser_Status(-1);
	}
	// Status is ONLINE
	else 	
	{
		C3_User_Interface::Instance->pbLaserComms->Image = 
			C3_User_Interface::Instance->OnlineInd;

		Set_Laser_Com_Status(1);
	}

	return 0;
}

int CDisplayManager::Update_Live_Video_Feed(String ^ sImagePath)
{
	System::Diagnostics::Debug::WriteLine(sImagePath);

	Bitmap^ bmCameraImage;

	// Use managed C to garbage collect automatically
	bmCameraImage = gcnew Bitmap(sImagePath);
	C3_User_Interface::Instance->pbLiveFeed->Image = bmCameraImage;
 		
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
////////////////////////////////////////////////////////////////////////
// Description: Disables all controls during POC demo				
// Author:		Mike Payne
////////////////////////////////////////////////////////////////////////
void CDisplayManager::Disable_All_Controls(void)
{
	C3_User_Interface::Instance->cmdExport->Enabled = false;
}



int CDisplayManager::Store_Latest_DTI(int nDTI, bool bPassed) {
	//get current date/time
	System::DateTime TimeStamp(0, System::DateTimeKind::Local);
	TimeStamp = System::DateTime::Now;
	//convert to meters
	float fDTI = (float) nDTI / 1000;
	//assemble fields
	System::String^ TimeField = 
		System::String::Concat( TimeStamp.ToShortDateString(),
								" ",
								TimeStamp.ToShortTimeString());
	System::String^ DtiField = fDTI.ToString();
	System::String^ PassField = bPassed.ToString();
	//append the row
	C3_User_Interface::Instance->dgvDtiLog->Rows->Add(
		TimeField, DtiField, PassField);
	return 0;
}
