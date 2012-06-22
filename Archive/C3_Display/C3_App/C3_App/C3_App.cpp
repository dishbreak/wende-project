// C3_App.cpp : main project file.

#include "stdafx.h"
#include "C3_User_Interface.h"
#include "Test_Driver.h"
#include "GUI_Control.h"

using namespace C3_App;

// This currently involves 4 classes - C3_App, C3_User_Interface, 
// Test_Driver and GUI_Control. The idea is that C3_App spawns the 
// C3_User_Interface and shows the test panel, when you click the button
// on the test panel, it should then call through to the C3_User_Interface. 
// I have not gotten this to work yet though. 

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	// Show test form
	Test_Driver td;
	td.Show();

	// MP: Tried the below code, but couldn't get it to work
	// C3_User_Interface ^f = gcnew C3_User_Interface;
	// f->change_button_text();

	// Create the main window and run it
	// MP: This thread continues to run until you close the window

    Application::Run(gcnew C3_User_Interface());


	return 0;
}
