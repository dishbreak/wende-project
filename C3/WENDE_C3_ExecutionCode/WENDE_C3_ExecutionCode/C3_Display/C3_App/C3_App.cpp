// C3_App.cpp : main project file.

#include "stdafx.h"
#include "splash.h"
#include "C3_User_Interface.h"
#include "GUIConfiguration.h"
#include "Test_Driver.h"
#include "CNetworkMonitor.h"
#include <process.h>
#include "CC3AppControl.h"

using namespace C3_App;
//globals--for now
int roverContactX = 0;
int roverContactY = 0;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
    //  =======================================================================
    //  Display the splash screen using the overloaded construcutor
    //  =======================================================================
    //  Launch splash screen
    CSplash splash1(TEXT(".\\Splash.bmp"), RGB(128, 128, 128));
    splash1.ShowSplash();

	//Fist call to configuration singleton
	CGUIConfiguration::Instance();

	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Sleep(5000);
	splash1.CloseSplash();

	
	// Run up Controller and Processing
	// CC3AppControl ac;
	// ac.RunC3App();

	// Show test form
	Test_Driver td;
	if (CGUIConfiguration::Instance().isShowDebugPannel == true)
	{
		td.Show();
	}
	
	// Networking thread. 
	CNetworkMonitor nm;
	nm.InitializeThread();

    Application::Run(gcnew C3_User_Interface(&nm));
	
	return 0;
}

