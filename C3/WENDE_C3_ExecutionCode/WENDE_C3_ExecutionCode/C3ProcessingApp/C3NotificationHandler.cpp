#include "stdafx.h"
#include "C3NotificationHandler.h"

C3NotificationHandler::C3NotificationHandler()
{
	m_isCalibration = false;
	m_nDTIValue = 0;
	m_nResult = 0;
	/* Initialize the critical section before entering multi-threaded context. */
	InitializeCriticalSection(&cs);

};

C3NotificationHandler::~C3NotificationHandler()
{
	/* Release system object when all finished -- usually at the end of the cleanup code */
	DeleteCriticalSection(&cs);
};
void C3NotificationHandler::Set_IsCalibration(bool isCalibration)
{
	/* Enter the critical section -- other threads are locked out */
	EnterCriticalSection(&cs);		
	/* Do some thread-safe processing! */
	m_isCalibration = isCalibration;
	/* Leave the critical section -- other threads can now EnterCriticalSection() */
	LeaveCriticalSection(&cs);
}
bool C3NotificationHandler::Get_IsCalibration()
{
	/* Enter the critical section -- other threads are locked out */
	EnterCriticalSection(&cs);		
	/* Do some thread-safe processing! */
	bool result = m_isCalibration;
	/* Leave the critical section -- other threads can now EnterCriticalSection() */
	LeaveCriticalSection(&cs);
	//return result
	return result;
}
void C3NotificationHandler::Set_Alert_Type(C3_Alert_Types nAlertType)
{
	m_nAlertType = nAlertType;
};

void C3NotificationHandler::Set_DTI_Value(int nDTIValue)
{
	m_nDTIValue = nDTIValue;
};

void C3NotificationHandler::Set_Trial_Result(int nResult)
{
	m_nResult = nResult;
};

C3_Alert_Types C3NotificationHandler::Get_Alert_Type()
{
	return m_nAlertType;
};

int C3NotificationHandler::Get_DTI_Value()
{
	return m_nDTIValue;	
};

int C3NotificationHandler::Get_Trial_Result()
{
	return m_nResult;	
};