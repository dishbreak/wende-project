#include "stdafx.h"
#include "CSharedStruct.h"
#include "ShmStructs.h"
#include <time.h>
#include <process.h>
#include "CNetworkMonitor.h"
#include "CDisplayManager.h"

using namespace System;

UINT WINAPI TrackThread (LPVOID pParam);
UINT WINAPI ImageThread (LPVOID pParam);
UINT WINAPI LaserStatusThread (LPVOID pParam);
UINT WINAPI CameraStatusThread(LPVOID pParam);

void CNetworkMonitor::InitializeThread()
{
	HANDLE hThread1;
	UINT uiThreadId1 = 0;
	hThread1 = (HANDLE)_beginthreadex(NULL,				       // Security attributes
										0,					  // stack
									 CameraStatusThread,		// Thread proc
									 NULL,					  // Thread param
									 CREATE_SUSPENDED,		  // creation mode
									 &uiThreadId1);			  // Thread ID

	if ( NULL != hThread1)
	{
		//SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL);
		ResumeThread( hThread1 );
	}

	HANDLE hThread2;
	UINT uiThreadId2 = 0;
	hThread2 = (HANDLE)_beginthreadex(NULL,				       // Security attributes
										0,					  // stack
									 LaserStatusThread,   // Thread proc
									 NULL,					  // Thread param
									 CREATE_SUSPENDED,		  // creation mode
									 &uiThreadId2);			  // Thread ID

	if ( NULL != hThread2)
	{
		//SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL);
		ResumeThread( hThread2 );
	}

	HANDLE hThread3;
	UINT uiThreadId3 = 0;
	hThread3 = (HANDLE)_beginthreadex(NULL,				       // Security attributes
										0,					  // stack
									 TrackThread,   // Thread proc
									 NULL,					  // Thread param
									 CREATE_SUSPENDED,		  // creation mode
									 &uiThreadId3);			  // Thread ID

	if ( NULL != hThread3)
	{
		//SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL);
		ResumeThread( hThread3 );
	}

	HANDLE hThread4;
	UINT uiThreadId4 = 0;
	hThread4 = (HANDLE)_beginthreadex(NULL,				       // Security attributes
										0,					   // stack
									 ImageThread,			   // Thread proc
									 NULL,					   // Thread param
									 CREATE_SUSPENDED,		   // creation mode
									 &uiThreadId4);			   // Thread ID

	if ( NULL != hThread4)
	{
		//SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL);
		ResumeThread( hThread4 );
	}
}

UINT WINAPI CameraStatusThread (LPVOID pParam)
{
	CSharedStruct<CAMERA_STATUS_MSG_SHM>	 m_CameraStatus;
	m_CameraStatus.Acquire("SHM_C3_CAMERA_STATUS",
						   "SHM_C3_CAMERA_STATUS_MUTEX",
						   "SHM_C3_CAMERA_STATUS_EVENT1",
						   "SHM_C3_CAMERA_STATUS_EVENT2");
	if (m_CameraStatus.isServer()) m_CameraStatus->ShmInfo.Clients = 0;
	else m_CameraStatus->ShmInfo.Clients++;

	static char timeStr[512];
	static char temp[512];
	static int cameraStatusMessageCount = 0;

    ////get a handle to the CDisplayManager
    CDisplayManager *dispman = CDisplayManager::getCDisplayManager();

	while(1)
	{
		// aquire the mutex
		if (m_CameraStatus.isCreated() && m_CameraStatus.WaitForCommunicationEventServer() == WAIT_OBJECT_0)
		{
			if (m_CameraStatus.WaitForCommunicationEventMutex() == WAIT_OBJECT_0)
			{
                ////set camera state
                dispman->Update_Camera_Subsystem_Indicator(m_CameraStatus->Status);
				dispman->Update_Camera_Communication_Indicator(1);

				// TODO: Remove once laser interface added.
				dispman->Update_Laser_Activity_Indicator(m_CameraStatus->LaserOnOf);
				dispman->Update_Overall_Status();

				// Set the event
				m_CameraStatus.SetEventClient();
				
				// release the mutex
				m_CameraStatus.ReleaseMutex();
			}
			else
			{
				// unable to get mutex???
			}
		}
		else
		{
			dispman->Update_Camera_Communication_Indicator(0);
			dispman->Update_Overall_Status();
		}
	}

	_endthreadex( 0 );
    
	return 1L;
}
UINT WINAPI LaserStatusThread (LPVOID pParam)
{
	CSharedStruct<LASER_STATUS_MSG_SHM>	 m_LaserStatus;
	m_LaserStatus.Acquire(	"SHM_C3_LASER_STATUS",
							"SHM_C3_LASER_STATUS_MUTEX",
							"SHM_C3_LASER_STATUS_EVENT1",
							"SHM_C3_LASER_STATUS_EVENT2");
	if (m_LaserStatus.isServer()) m_LaserStatus->ShmInfo.Clients = 0;
	else m_LaserStatus->ShmInfo.Clients++;

	static char timeStr[512];
	static char temp[512];
	static int laserStatusMessageCount = 0;

    ////get a handle to the CDisplayManager
    CDisplayManager *dispman = CDisplayManager::getCDisplayManager();

	while(1)
	{
		// aquire the mutex
		if (m_LaserStatus.isCreated() && m_LaserStatus.WaitForCommunicationEventServer() == WAIT_OBJECT_0)
		{
			if (m_LaserStatus.WaitForCommunicationEventMutex() == WAIT_OBJECT_0)
			{
				dispman->Update_Laser_Subsystem_Indicator(m_LaserStatus->Status);
				dispman->Update_Laser_Communication_Indicator(1);
				dispman->Update_Overall_Status();

				// Set the event
				m_LaserStatus.SetEventClient();
				
				// release the mutex
				m_LaserStatus.ReleaseMutex();
			}
			else
			{
				// unable to get mutex???
			}
		}
		else
		{
			dispman->Update_Laser_Communication_Indicator(0);
			dispman->Update_Overall_Status();
		}
	}

	_endthreadex( 0 );
    
	return 1L;
}
UINT WINAPI TrackThread (LPVOID pParam)
{
	CAMERA_TRACK_MSG_SHM sTrackMessage; 
	CSharedStruct<CAMERA_TRACK_MSG_SHM>		 m_CameraTracks;
	m_CameraTracks.Acquire("SHM_C3_CAMERA_TRACK",
						   "SHM_C3_CAMERA_TRACK_MUTEX",
						   "SHM_C3_CAMERA_TRACK_EVENT1",
						   "SHM_C3_CAMERA_TRACK_EVENT2");
	if (m_CameraTracks.isServer()) m_CameraTracks->ShmInfo.Clients = 0;
	else m_CameraTracks->ShmInfo.Clients++;

	int x = 0;
	int y = 0;

	while(1)
	{
		// aquire the mutex
		if (m_CameraTracks.isCreated() && m_CameraTracks.WaitForCommunicationEventServer() == WAIT_OBJECT_0)
		{
			if (m_CameraTracks.WaitForCommunicationEventMutex() == WAIT_OBJECT_0)
			{
				sTrackMessage = *m_CameraTracks.GetStruct();
				// Set the event
				m_CameraTracks.SetEventClient();
				
				// release the mutex
				m_CameraTracks.ReleaseMutex();

				// Will need to build this up to handle multiple tracks
				x = sTrackMessage.Tracks->X;
				y = sTrackMessage.Tracks->Y;

				////get a handle to the CDisplayManager
                CDisplayManager *dispman = CDisplayManager::getCDisplayManager();
				dispman->Update_Rover_PPI_Position(x, y);
			}
			else
			{
				// unable to get mutex???
			}
		}
		else
		{
			// loss of comm
		}
	}

	_endthreadex( 0 );
    
	return 1L;
}
UINT WINAPI ImageThread (LPVOID pParam)
{
	CSharedStruct<CAMERA_IMAGE_MSG_SHM>		 m_CameraImage;
	char * sImagePath;
	//System::String ^ sImagePath;
	m_CameraImage.Acquire( "SHM_C3_CAMERA_IMAGE",
						   "SHM_C3_CAMERA_IMAGE_MUTEX",
						   "SHM_C3_CAMERA_IMAGE_EVENT1",
						   "SHM_C3_CAMERA_IMAGE_EVENT2");

	if (m_CameraImage.isServer()) m_CameraImage->ShmInfo.Clients = 0;
	else m_CameraImage->ShmInfo.Clients++;

	while(1)
	{
				// aquire the mutex
		if (m_CameraImage.isCreated() && m_CameraImage.WaitForCommunicationEventServer() == WAIT_OBJECT_0)
		{
			if (m_CameraImage.WaitForCommunicationEventMutex() == WAIT_OBJECT_0)
			{
				// Display data...
				sImagePath = m_CameraImage->imagePath;
				String ^ ssImagePath = gcnew String(sImagePath);

				CDisplayManager *dispman = CDisplayManager::getCDisplayManager();

				dispman->Update_Live_Video_Feed(ssImagePath);

				// Set the event
				m_CameraImage.SetEventClient();
				
				// release the mutex
				m_CameraImage.ReleaseMutex();

				delete ssImagePath;
			}
			else
			{
				// unable to get mutex???
			}
		}
		else
		{
			// loss of comm
		}
	}

	_endthreadex( 0 );
    
	return 1L;
}