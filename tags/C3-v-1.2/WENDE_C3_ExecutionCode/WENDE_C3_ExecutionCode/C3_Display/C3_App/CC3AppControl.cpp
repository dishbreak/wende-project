#include "stdafx.h"
#include "CC3AppControl.h"
#include <windows.h>
#include <shellapi.h>
#include <time.h>
#include <process.h>
#pragma comment(lib, "shell32.lib")

UINT WINAPI StartProcessingThread (LPVOID pParam);
UINT WINAPI StartControllerThread (LPVOID pParam);

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

	if ( NULL != hProcessingThread)
	{
		//SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL);
		ResumeThread( hProcessingThread );
	}

	HANDLE	hControllerThread;
	UINT	uiControllerThread = 0;
	hControllerThread = (HANDLE)_beginthreadex(NULL,			// Security attributes
										0,						// stack
										StartControllerThread,		// Thread proc
										NULL,					  // Thread param
										CREATE_SUSPENDED,		  // creation mode
										&uiControllerThread);			  // Thread ID

	if ( NULL != hControllerThread)
	{
		//SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL);
		ResumeThread( hControllerThread );
	}
}


UINT WINAPI StartProcessingThread (LPVOID pParam)
{
	ShellExecute(NULL,
		"open",
		"C:\\Users\\Mike Payne\\Desktop\\WENDE_C3_ExecutionCode\\WENDE_C3_ExecutionCode\\EXEs\\C3_App.exe",
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
		"C:\\Users\\Mike Payne\\Desktop\\WENDE_C3_ExecutionCode\\WENDE_C3_ExecutionCode\\EXEs\\C3_App.exe",
		NULL,
		0,
		SW_SHOWNORMAL);
	  
	_endthreadex( 0 );

	return 1L;
}