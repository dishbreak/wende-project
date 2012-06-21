#include "stdafx.h"
#include "GUI_Control.h"


GUI_Control::GUI_Control(void)
{

}

GUI_Control::~GUI_Control(void)
{

}

// MP: Only provided implementation for one operation so far.

int GUI_Control::Update_Camera_Indicators(int eCameraStatus)
{
	// MP: I implemented a wrapper function to change the textbox text, 
	// but when you call this, it doesn't do anything. Probably because
	// it's creating a new instance - aside from the one displaying.
	C3_App::C3_User_Interface ^f = gcnew C3_App::C3_User_Interface();

	f->change_button_text();
	return 0;
}

