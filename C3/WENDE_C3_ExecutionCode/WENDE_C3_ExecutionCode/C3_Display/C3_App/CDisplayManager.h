#include "stdafx.h"
#include "C3_User_Interface.h"


class CDisplayManager
{
public: 
CDisplayManager(){};
~CDisplayManager(){};

int Update_Rover_PPI_Position(int x, int y);
int Update_Camera_Subsystem_Indicator(int nCameraStatus);
int Update_Camera_Communication_Indicator(enum eStatus);
int Update_Laser_Subsystem_Indicator(enum eStatus);
int Update_Laser_Communication_Indicator(enum eStatus);
int Update_Laser_Activity_Indicator(enum eStatus);
int Update_Rover_Acquired_Indicator(enum eStatus);
int Store_Latest_DTI(int nDTI);

};

