// C3_App.cpp : main project file.

#include "stdafx.h"
#include "C3_User_Interface.h"
#include "Test_Driver.h"
#include "CNetworkMonitor.h"

using namespace C3_App;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	// Networking thread. 
	CNetworkMonitor nm;
	nm.InitializeThread();

	// Show test form
	Test_Driver td;
	td.Show();

    Application::Run(gcnew C3_User_Interface());
	
	return 0;
}

