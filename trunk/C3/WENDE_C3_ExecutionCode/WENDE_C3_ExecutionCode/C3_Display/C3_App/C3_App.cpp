// C3_App.cpp : main project file.

#include "stdafx.h"
#include "C3_User_Interface.h"
#include "Test_Driver.h"
#include "CNetworkMonitor.h"
#include <process.h>

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
    
    //Ensure that the threads run
    while(1)
    {
//        Sleep (10000);
    }
	
	return 0;
}

