#include "stdafx.h"

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
	int Set_Alert_Type(int nAlertType);
	int Set_DTI_Value(int nDTIValue);
	int Set_Trial_Result(int nResult);
	int Get_Alert_Type();
	int Get_DTI_Value();
	int Get_Trial_Result();
private:
	C3NotificationHandler(){};
    ~C3NotificationHandler(){};
	C3NotificationHandler(const C3NotificationHandler&);
	C3NotificationHandler& operator=(const C3NotificationHandler&);

	//constructor made private to ensure this stays a singleton
private:	
	// Only modifiable via class
	int m_nAlertType;
	int m_nDTIValue;
	int m_nResult;
};