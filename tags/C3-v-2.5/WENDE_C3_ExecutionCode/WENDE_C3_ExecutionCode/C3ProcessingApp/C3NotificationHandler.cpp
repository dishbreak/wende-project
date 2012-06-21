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
void C3NotificationHandler::Set_Process_State(C3_Alert_Types nAlertType)
{
	/* Enter the critical section -- other threads are locked out */
	EnterCriticalSection(&cs);		
	/* Do some thread-safe processing! */

	if (nAlertType == C3_Alert_Types::CALIBRATION_INIT ||
		nAlertType == C3_Alert_Types::CALIBRATION_FAILED)
	{
		Set_IsCalibration(true);
		m_nAlertType = C3_Alert_Types::CALIBRATION_IN_PROGRESS_1;
	}
	else
	{
		m_nAlertType = nAlertType;
	}
	/* Leave the critical section -- other threads can now EnterCriticalSection() */
	LeaveCriticalSection(&cs);
};

void C3NotificationHandler::Set_DTI_Value(int nDTIValue)
{
	/* Enter the critical section -- other threads are locked out */
	EnterCriticalSection(&cs);		
	/* Do some thread-safe processing! */
	m_nDTIValue = nDTIValue;
	/* Leave the critical section -- other threads can now EnterCriticalSection() */
	LeaveCriticalSection(&cs);
};

void C3NotificationHandler::Set_Trial_Result(int nResult)
{
	/* Enter the critical section -- other threads are locked out */
	EnterCriticalSection(&cs);		
	/* Do some thread-safe processing! */
	m_nResult = nResult;
	/* Leave the critical section -- other threads can now EnterCriticalSection() */
	LeaveCriticalSection(&cs);
};

C3_Alert_Types C3NotificationHandler::Get_Process_State()
{
	/* Enter the critical section -- other threads are locked out */
	EnterCriticalSection(&cs);		
	/* Do some thread-safe processing! */
	C3_Alert_Types result = m_nAlertType;
	/* Leave the critical section -- other threads can now EnterCriticalSection() */
	LeaveCriticalSection(&cs);
	return result;
};

int C3NotificationHandler::Get_DTI_Value()
{
	/* Enter the critical section -- other threads are locked out */
	EnterCriticalSection(&cs);		
	/* Do some thread-safe processing! */
	int result = m_nDTIValue;
	/* Leave the critical section -- other threads can now EnterCriticalSection() */
	LeaveCriticalSection(&cs);
	return result;	
};

int C3NotificationHandler::Get_Trial_Result()
{
	/* Enter the critical section -- other threads are locked out */
	EnterCriticalSection(&cs);		
	/* Do some thread-safe processing! */
	int result = m_nResult;
	/* Leave the critical section -- other threads can now EnterCriticalSection() */
	LeaveCriticalSection(&cs);
	return result;	
};