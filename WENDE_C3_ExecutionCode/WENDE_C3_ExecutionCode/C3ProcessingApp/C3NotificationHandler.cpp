#include "stdafx.h"
#include "C3NotificationHandler.h"

//C3NotificationHandler ^ C3NotificationHandler::getC3NotificationHandler()
//{
//    if(!notificationHandler)
//    {
//        notificationHandler = gcnew C3NotificationHandler();
//    }
//    return notificationHandler;
//}

int C3NotificationHandler::Set_Alert_Type(int nAlertType)
{
	m_nAlertType = nAlertType;

	return 0;	
};

int C3NotificationHandler::Set_DTI_Value(int nDTIValue)
{
	m_nDTIValue = nDTIValue;

	return 0;		
};

int C3NotificationHandler::Set_Trial_Result(int nResult)
{
	m_nResult = nResult;

	return 0;	
};

int C3NotificationHandler::Get_Alert_Type()
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