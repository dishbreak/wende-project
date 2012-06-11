#include "stdafx.h"
#include "C3_User_Interface.h"
#include <string>
#include "Coordinates.h"

using namespace System;
using namespace System::Drawing;

ref class CDisplayManager
{
public: 
    //function that gets the handle to the singleton CDisplayManager
    static CDisplayManager ^ getCDisplayManager();
    int Update_Rover_PPI_Position(array<CoordinatePair^>^ inputCoords, int NumValidTracks);
    int Update_Camera_Subsystem_Indicator(int nCameraStatus);
    int Update_Camera_Communication_Indicator(int nCameraCommStatus);
    int Update_Laser_Subsystem_Indicator(int nLaserStatus);
    int Update_Laser_Communication_Indicator(int nLaserCommStatus);
    int Update_Laser_Activity_Indicator(int nLaserActivityStatus);
	int Update_Overall_Status(void);
    int Update_Rover_Acquired_Indicator(int nRoverAcqStatus);
    int Store_Latest_DTI(int nDTI, bool bPassed);
	int Update_Live_Video_Feed(String ^ sImagePath, __int64 nStartTime);
	int Update_Calibration_Reply(int nAlertID);
	void Update_Notification_Panel(int nAlertID);
	void Disable_All_Controls(void);

	// Accessor methods to determine overall status
	void Set_Camera_Status(int nCameraStatus);
	void Set_Laser_Status(int nLaserStatus);
	void Set_Laser_Com_Status(int nLaserComStatus);
	void Set_Camera_Com_Status(int nCameraComStatus);
	void Set_Laser_Activity(int nLaserActivityStatus);
	void Set_Calibrated_Status(int nCalibratedStatus);
	int Get_Camera_Status(void);
	int Get_Laser_Status(void);
	int Get_Laser_Com_Status(void);
	int Get_Camera_Com_Status(void);
	int Get_Laser_Activity(void);
	int Get_Calibrated_Status(void);

//constructor made private to ensure this stays a singleton
private:
	CDisplayManager();
    ~CDisplayManager(){};
    // Class variable for determining overall status
    int m_nCameraStatus;
    int m_nLaserStatus;
    int m_nLaserComStatus;
    int m_nCameraComStatus;
    int m_nLaserActivityStatus;
    int m_OverStatus;
	int m_CalibratedStatus;
    static CDisplayManager ^ displayMgr = nullptr;
};

