#pragma once
#include "stdafx.h"
#include "C3AlertStates.h"


class C3ProcessingStates
{
public:
	static C3ProcessingStates& Instance()
	{
		// create singleto{}
		static C3ProcessingStates singleton;
		// return reference to singleton
		return singleton;
	}
	~C3ProcessingStates(void){};	
	
	void Set_Current_Alert(C3_Alert_Types eCurrentAlert);
	C3_Alert_Types Get_Current_Alert();

private:
	C3_Alert_Types m_eCurrentAlert;
	// Private constructor
	C3ProcessingStates(){};								
	//// Prevent copy-construction
	C3ProcessingStates(const C3ProcessingStates&);           
	//// Prevent assignment
	C3ProcessingStates& operator=(const C3ProcessingStates&);
	
};