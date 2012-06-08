#include "stdafx.h"
#include "CC3AppControl.h"
#include "C3Configuration.h"
#include <windows.h>
#include <shellapi.h>
#include <time.h>
#include <process.h>
#pragma comment(lib, "shell32.lib")

UINT WINAPI StartProcessingThread (LPVOID pParam);
UINT WINAPI StartControllerThread (LPVOID pParam);
UINT WINAPI StartDriverThread (LPVOID pParam);

void CC3AppControl::RunC3App()
{
	HANDLE	hProcessingThread;
	UINT	uiProcessingThread = 0;
	hProcessingThread = (HANDLE)_beginthreadex(NULL,			// Security attributes
										0,						// stack
										StartProcessingThread,		// Thread proc
										NULL,					  // Thread param
										CREATE_SUSPENDED,		  // creation mode
										&uiProcessingThread);			  // Thread ID

	if ( NULL != hProcessingThread){ResumeThread( hProcessingThread );}

	HANDLE	hControllerThread;
	UINT	uiControllerThread = 0;
	hControllerThread = (HANDLE)_beginthreadex(NULL,			// Security attributes
										0,						// stack
										StartControllerThread,		// Thread proc
										NULL,					  // Thread param
										CREATE_SUSPENDED,		  // creation mode
										&uiControllerThread);			  // Thread ID

	if ( NULL != hControllerThread){ResumeThread( hControllerThread );}

	HANDLE	hDriverThread;
	UINT	uiDriverThread = 0;
	hDriverThread = (HANDLE)_beginthreadex(NULL,			// Security attributes
										0,						// stack
										StartDriverThread,		// Thread proc
										NULL,					  // Thread param
										CREATE_SUSPENDED,		  // creation mode
										&uiDriverThread);			  // Thread ID

	if ( NULL != hDriverThread){ResumeThread( hDriverThread );}
}


UINT WINAPI StartProcessingThread (LPVOID pParam)
{
	ShellExecute(NULL,
		"open",
		C3Configuration::Instance().processingStartPath.c_str(),
		NULL,
		0,
		SW_SHOWNORMAL);
	   
	_endthreadex( 0 );

	return 1L;
}

UINT WINAPI StartControllerThread (LPVOID pParam)
{
	ShellExecute(NULL,
		"open",
		C3Configuration::Instance().arbiterStartPath.c_str(),
		NULL,
		0,
		SW_SHOWNORMAL);
	  
	_endthreadex( 0 );

	return 1L;
}
UINT WINAPI StartDriverThread (LPVOID pParam)
{
	ShellExecute(NULL,
		"open",
		C3Configuration::Instance().driverStartPath.c_str(),
		NULL,
		0,
		SW_SHOWNORMAL);

	_endthreadex( 0 );

	return 1L;
}