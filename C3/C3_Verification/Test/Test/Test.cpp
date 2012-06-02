// Test.cpp : main project file.

#include "stdafx.h"
#include "Form1.h"
#include "CVerification.h"
#include <string>

using namespace Test;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	CVerification cv;
	cv.Import_Test_Data_File();

	for(int i = 0; i < 20; i++)
	{
		string sRoverInX = cv.Get_Test_Data_Item(i, "RoverInX");
		string sRoverInY = cv.Get_Test_Data_Item(i, "RoverInY");
		string sLaserInX = cv.Get_Test_Data_Item(i, "LaserInX");
		string sLaserInY = cv.Get_Test_Data_Item(i, "LaserInY");
		string sLaserOutAz = cv.Get_Test_Data_Item(i, "LaserOutAz");
		string sLaserOutEl = cv.Get_Test_Data_Item(i, "LaserOutEl");
	}
	

	// Create the main window and run it
	Application::Run(gcnew Form1());
	return 0;
}
