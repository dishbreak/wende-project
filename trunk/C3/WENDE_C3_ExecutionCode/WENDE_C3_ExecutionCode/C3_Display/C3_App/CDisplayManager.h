#include "stdafx.h"
#include "C3_User_Interface.h"
#include <string>

using namespace System;

class CDisplayManager
{
public: 
    //function that gets the handle to the singleton CDisplayManager
    static CDisplayManager * getCDisplayManager();
    int Update_Rover_PPI_Position(int x, int y);
    int Update_Camera_Subsystem_Indicator(int nCameraStatus);
    int Update_Camera_Communication_Indicator(int nCameraCommStatus);
    int Update_Laser_Subsystem_Indicator(int nLaserStatus);
    int Update_Laser_Communication_Indicator(int nLaserCommStatus);
    int Update_Laser_Activity_Indicator(int nLaserActivityStatus);
	int Update_Overall_Status(void);
    int Update_Rover_Acquired_Indicator(int nRoverAcqStatus);
    int Store_Latest_DTI(int nDTI);
	int Update_Live_Video_Feed(String ^ sImagePath);
	void Update_Notification_Panel(int nAlertID);

	// Accessor methods to determine overall status
	void Set_Camera_Status(int nCameraStatus);
	void Set_Laser_Status(int nLaserStatus);
	int Get_Camera_Status(void);
	int Get_Laser_Status(void);

//constructor made private to ensure this stays a singleton
private:
	CDisplayManager(){};
    ~CDisplayManager(){};
    static CDisplayManager * displayMgr;


};
