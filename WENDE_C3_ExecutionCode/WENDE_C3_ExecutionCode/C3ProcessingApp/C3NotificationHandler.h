#include "stdafx.h"
#include <windows.h>

/////////////////////////////////////////////////////////////////////////////////
// Struct: enum types
/////////////////////////////////////////////////////////////////////////////////
typedef enum {
	CALIBRATION_INIT    = 0,
	CALIBRATION_IN_PROGRESS_1,	// send first point   
	CALIBRATION_IN_PROGRESS_2,	// send second point ---> receive first
	CALIBRATION_IN_PROGRESS_3,	// send third point	 ---> receive second  
	CALIBRATION_IN_PROGRESS_4,	// send fourth point ---> receive third
	CALIBRATION_IN_PROGRESS_5,	//                   ---> receive fourth   ---> calibrate
	CALIBRATION_FAILED,
	CALIBRATION_SUCCESS,
	NUM_ALERTS_TYPES
}C3_Alert_Types; 

class C3NotificationHandler
{
public:
	//Singleton Manager for the shatred memory
	static C3NotificationHandler& Instance()
	{
		// create singleton
		static C3NotificationHandler singleton;
		// return reference to singleton
		return singleton;
	}	

	// Public access to modifiers
	void Set_Alert_Type(C3_Alert_Types nAlertType);
	void Set_DTI_Value(int nDTIValue);
	void Set_Trial_Result(int nResult);
	void Set_IsCalibration(bool isCalibration);

	C3_Alert_Types Get_Alert_Type();
	int Get_DTI_Value();
	int Get_Trial_Result();
	bool Get_IsCalibration();

private:
	C3NotificationHandler();
    ~C3NotificationHandler();
	C3NotificationHandler(const C3NotificationHandler&);
	C3NotificationHandler& operator=(const C3NotificationHandler&);

	//constructor made private to ensure this stays a singleton
private:	
	// Only modifiable via class
	C3_Alert_Types m_nAlertType;
	int m_nDTIValue;
	int m_nResult;
	bool m_isCalibration;
	CRITICAL_SECTION cs; /* This is the critical section object -- once initialized,
		     		    it cannot be moved in memory */
		             /* If you program in OOP, declare this as a non-static member in your class */

};