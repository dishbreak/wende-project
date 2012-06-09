#include "stdafx.h"
#include "CDisplayManager.h"
#include <fstream>
#include <iostream> 
#include "Notification.h"
#include "Notifier.h"
#include "Coordinates.h"
#include "C3AlertStates.h"
using namespace C3_App;
using namespace std;



CDisplayManager::CDisplayManager() {
    m_nCameraComStatus = 0;
    m_nCameraStatus = 0;
    m_nLaserActivityStatus = 0;
    m_nLaserComStatus = 0;
    m_nLaserStatus = 0;
    m_OverStatus = 0;
}
////////////////////////////////////////////////////////////////////////
// Description: Returns a pointer to the CDisplayManager. If there is no
//              CDisplayManager, one is new'd and returned.
// Author:      Vishal Kotcherlakota
////////////////////////////////////////////////////////////////////////
CDisplayManager ^ CDisplayManager::getCDisplayManager()
{
    if(!displayMgr)
    {
        displayMgr = gcnew CDisplayManager();
    }
    return displayMgr;
}
////////////////////////////////////////////////////////////////////////
// Description: Updates the PPI display when the new rover position is
//				sent through					
// Author:		Mike Payne
////////////////////////////////////////////////////////////////////////

int CDisplayManager::Update_Rover_PPI_Position(array<CoordinatePair^>^ inputCoords, int NumValidTracks)
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
	Coordinates ^ coordsObj = Coordinates::GetCoordinatesHandle();
	//if any of the coordinates are different, invalidate the PPI display.
	if(coordsObj->SetNewCoordinates(inputCoords, NumValidTracks)) {
		C3_User_Interface::Instance->pPPI->Invalidate();
	}

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
		if (Get_Camera_Status() != 0)
		{
			C3_User_Interface::Instance->Update_Camera_Subsystem_Indicator(
				C3_User_Interface::Instance->OfflineInd);
		}
		Set_Camera_Status(0);
	}
	// Status is ONLINE
	else 	
	{
		if (Get_Camera_Status() != 1)
		{
			C3_User_Interface::Instance->Update_Camera_Subsystem_Indicator(
				C3_User_Interface::Instance->OnlineInd);
		}
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
		if (Get_Laser_Status() != 0)
		{
			C3_User_Interface::Instance->Update_Laser_Subsystem_Indicator(
				C3_User_Interface::Instance->OfflineInd);
		}
		Set_Laser_Status(0);
	}
	// Status is ONLINE
	else 	
	{
		if (Get_Laser_Status() != 1)
		{
			C3_User_Interface::Instance->Update_Laser_Subsystem_Indicator(
				C3_User_Interface::Instance->OnlineInd);
		}
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
	if (Get_Laser_Activity() != nLaserActivityStatus)
	{
		if(nLaserActivityStatus == 1)
		{
			C3_User_Interface::Instance->Update_Laser_Activity_Indicator( 
				C3_User_Interface::Instance->EnergizedInd);
		}
		else
		{
			C3_User_Interface::Instance->Update_Laser_Activity_Indicator(
				C3_User_Interface::Instance->InactiveInd);
		}
		Set_Laser_Activity(nLaserActivityStatus);
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
	int nLaserSubsystemStatus  = Get_Laser_Status();
	int nCameraSubsystemStatus = Get_Camera_Status();
	int nLaserComStatus        = Get_Laser_Com_Status();
	int nCameraComStatus       = Get_Camera_Com_Status();

	// Set status to ONLINE if camera and laser are OK
	if((nLaserSubsystemStatus == 1) && 
	   (nCameraSubsystemStatus == 1) && 
	   (nLaserComStatus == 1) && 
	   (nCameraComStatus == 1))
	{
	   if (m_OverStatus != 1)
	   {
	   		C3_User_Interface::Instance->Update_Overall_Status_Indicator( 
				C3_User_Interface::Instance->OnlineInd);
	   }
	   m_OverStatus = 1;
	}
	else
	{
		if (m_OverStatus != 0)
		{
			C3_User_Interface::Instance->Update_Overall_Status_Indicator( 
				C3_User_Interface::Instance->OfflineInd);
		}
		m_OverStatus = 0;
	}
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
void CDisplayManager::Set_Laser_Activity(int nLaserActivityStatus)
{
	m_nLaserActivityStatus = nLaserActivityStatus;
}
void CDisplayManager::Set_Camera_Com_Status(int nCameraComStatus)
{
	m_nCameraComStatus = nCameraComStatus;
}
int CDisplayManager::Get_Laser_Activity(void)
{
	return m_nLaserActivityStatus;
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
		if (Get_Camera_Com_Status() != 0)
		{
			C3_User_Interface::Instance->Update_Camera_Comm_Indicator(
				C3_User_Interface::Instance->OfflineInd);
		}
		if (Get_Camera_Status() != -1)
		{
			C3_User_Interface::Instance->Update_Camera_Comm_Indicator(
				C3_User_Interface::Instance->UnknownInd);
		}
		Set_Camera_Com_Status(0);
		Set_Camera_Status(-1);
	}
	// Status is ONLINE
	else 	
	{
		if (Get_Camera_Com_Status() != 1)
		{
			C3_User_Interface::Instance->Update_Camera_Comm_Indicator(
				C3_User_Interface::Instance->OnlineInd);
		}
		Set_Camera_Com_Status(1);
	}

	return 0;
}

int CDisplayManager::Update_Laser_Communication_Indicator(int nLaserCommStatus)
{
	// Status is OFFLINE
	if(nLaserCommStatus == 0) 		
	{
		if (Get_Laser_Com_Status() != 0)
		{
			C3_User_Interface::Instance->Update_Laser_Comm_Indicator( 
				C3_User_Interface::Instance->OfflineInd);
		}
		if (Get_Laser_Status() != -1)
		{
			C3_User_Interface::Instance->Update_Laser_Comm_Indicator(
				C3_User_Interface::Instance->UnknownInd);
		}
		Set_Laser_Com_Status(0);
		Set_Laser_Status(-1);
	}
	// Status is ONLINE
	else 	
	{
		if (Get_Laser_Com_Status() != 1)
		{
			C3_User_Interface::Instance->Update_Laser_Comm_Indicator(
				C3_User_Interface::Instance->OnlineInd);
		}
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
	C3_User_Interface::Instance->Update_Live_Feed_Panel(bmCameraImage);
 		
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
	Notification::NotifyMesg mesgType;

	switch(nAlertID)
	{
		case 1:

			// If alert status = 1 (Rover stopped before failure)			
			mesgType = Notification::NotifyMesg::TrialSuccess;
			break;

		case 2:
			
			// If alert status = 2 (Contact is > 0.7m away from centre)
			mesgType = Notification::NotifyMesg::PatientLeftEvacArea;
			break;

		case 3:

			// If alert status = 3 (Rover not stopped before failure line)
			mesgType = Notification::NotifyMesg::TrialFailed;
			break;

		case 4:

			// If alert status = 4 (calibration failed) or subsystems do not work

			mesgType = Notification::NotifyMesg::SystemNonOperational;
			break;

		case 5:

			// If alert status = 5 (calibration success) & subsystems work

			if(m_nCameraStatus == 1 && m_nLaserStatus ==1)
				mesgType = Notification::NotifyMesg::SystemOperational;
			else
				mesgType = Notification::NotifyMesg::SystemNonOperational;

			break;
	}
	
	Notification newNote(mesgType);
	C3_User_Interface::Instance->Update_Notification_Panel(newNote.NotifyText, newNote.bgColor, newNote.fgColor);
}
////////////////////////////////////////////////////////////////////////
// Description: Disables all controls during POC demo				
// Author:		Mike Payne
////////////////////////////////////////////////////////////////////////
void CDisplayManager::Disable_All_Controls(void)
{
	C3_User_Interface::Instance->cmdExport->Enabled = false;
}



int CDisplayManager::Store_Latest_DTI(int nDTI, bool bPassed) 
{
	//get current date/time
	System::DateTime TimeStamp(0, System::DateTimeKind::Local);
	TimeStamp = System::DateTime::Now;
	//convert to meters
	float fDTI = (float) nDTI / 1000;
	//Pass/Fail?

	//assemble fields
	System::String^ TimeField = 
		System::String::Concat( TimeStamp.ToShortDateString(),
								" ",
								TimeStamp.ToShortTimeString());
	System::String^ DtiField = fDTI.ToString();
	System::String^ PassField;
	if (bPassed) { PassField = "PASS"; }
	else { PassField = "FAIL"; }
	//append the row

	C3_User_Interface::Instance->Update_Table(TimeField, DtiField, PassField);

	return 0;
}


int CDisplayManager::Update_Rover_Acquired_Indicator(int nRoverAcqStatus)
{
	if(nRoverAcqStatus == 1)
	{
		C3_User_Interface::Instance->Update_Rover_Acquired_Indicator( 
			C3_User_Interface::Instance->AcquiredInd);
	}
	else
	{
		C3_User_Interface::Instance->Update_Rover_Acquired_Indicator( 
			C3_User_Interface::Instance->UnknownNsInd);
	}
	return 0;
}

int CDisplayManager::Update_Calibration_Reply(int nAlertID) {
	switch(nAlertID) 
	{
		case C3_Alert_Types::CALIBRATION_FAILED:
			{
				Update_Notification_Panel(4);
				break;
			}
		case C3_Alert_Types::CALIBRATION_SUCCESS:
			{
				Update_Notification_Panel(5);
				break;
			}
		default:
			{
				break;
			}
	}
	C3_User_Interface::Instance->Update_Calibration_Button((C3_Alert_Types)nAlertID);
	return 0;
}

