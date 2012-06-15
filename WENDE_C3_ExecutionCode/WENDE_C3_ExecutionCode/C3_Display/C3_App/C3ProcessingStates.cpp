#include "stdafx.h"
#include "C3ProcessingStates.h"
#include "C3AlertStates.h"

//C3ProcessingStates::C3ProcessingStates()
//{
//	
//}
//
//C3ProcessingStates::~C3ProcessingStates()
//{
//	
//}

void C3ProcessingStates::Set_Current_Alert(C3_Alert_Types eCurrentAlert)
{
	m_eCurrentAlert = eCurrentAlert;
};

C3_Alert_Types C3ProcessingStates::Get_Current_Alert()
{
	return m_eCurrentAlert;
};