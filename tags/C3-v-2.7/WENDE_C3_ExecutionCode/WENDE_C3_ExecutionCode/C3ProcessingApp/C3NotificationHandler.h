#include "stdafx.h"
#include <windows.h>
#include "C3AlertStates.h"

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
	void Set_Process_State(C3_Alert_Types nAlertType);
	void Set_DTI_Value(int nDTIValue);
	void Set_Trial_Result(int nResult);
	void Set_IsCalibration(bool isCalibration);

	C3_Alert_Types Get_Process_State();
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