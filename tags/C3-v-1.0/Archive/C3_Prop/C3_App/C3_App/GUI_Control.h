#include "stdafx.h"
#include "C3_User_Interface.h"
#include "Test_Driver.h"

// MP: Purpose of this class is to manage input to the C3_User_Interface.
// Just defined the operations as integer types for testing for now.

class GUI_Control
{
public:
	// Object life
	GUI_Control(void);
	~GUI_Control(void);

	// Status Indicators
	int Update_Camera_Indicators(int eCameraStatus);
	int Update_Laser_Indicators(int eLaserStatus);
	int Update_Rover_Indicators(int eRoverStatus);
	int Update_WENDE_Indicators(int eWendeStatus);

	// PPI Display
	int Update_PPI_Display(int ePPIData);

	// Live Video
	int Update_LiveVideo_Display(int eLiveData);
};