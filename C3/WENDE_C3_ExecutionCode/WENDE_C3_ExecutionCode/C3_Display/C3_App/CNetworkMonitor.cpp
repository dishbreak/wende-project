#include "stdafx.h"
#include "CSharedStruct.h"
#include "ShmStructs.h"
#include <time.h>
#include <process.h>
#include "CNetworkMonitor.h"
#include "CDisplayManager.h"
#include "C3Configuration.h"
#include "Coordinates.h"
#include <fstream>
#include <iostream>

using namespace System;
using namespace std;


UINT WINAPI TrackThread (LPVOID pParam);
UINT WINAPI ImageThread (LPVOID pParam);
UINT WINAPI LaserStatusThread (LPVOID pParam);
UINT WINAPI CameraStatusThread(LPVOID pParam);
UINT WINAPI ProcessingInterfaceThread(LPVOID pParam);

CNetworkMonitor::CNetworkMonitor()
:_isRunning(false)
{
	/* Initialize the critical section before entering multi-threaded context. */
	InitializeCriticalSection(&cs);
}
CNetworkMonitor::~CNetworkMonitor()
{
	/* Release system object when all finished -- usually at the end of the cleanup code */
	DeleteCriticalSection(&cs);
};
void CNetworkMonitor::StopThreads()
{
	/* Enter the critical section -- other threads are locked out */
	EnterCriticalSection(&cs);		
	/* Do some thread-safe processing! */
	_isRunning = false;
	/* Leave the critical section -- other threads can now EnterCriticalSection() */
	LeaveCriticalSection(&cs);
}
void CNetworkMonitor::InitializeThread()
{
	if (!_isRunning)
	{
		/* Enter the critical section -- other threads are locked out */
		EnterCriticalSection(&cs);		
		/* Do some thread-safe processing! */
		_isRunning = true;
		/* Leave the critical section -- other threads can now EnterCriticalSection() */
		LeaveCriticalSection(&cs);

		HANDLE hThread1;
		UINT uiThreadId1 = 0;
		hThread1 = (HANDLE)_beginthreadex(NULL,				       // Security attributes
											0,					  // stack
										 CameraStatusThread,		// Thread proc
										 this,					  // Thread param
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
										 this,					  // Thread param
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
										 this,					  // Thread param
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
											10000,					   // stack
										 ImageThread,			   // Thread proc
										 this,					   // Thread param
										 CREATE_SUSPENDED,		   // creation mode
										 &uiThreadId4);			   // Thread ID

		if ( NULL != hThread4)
		{
			//SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL);
			ResumeThread( hThread4 );
		}

		HANDLE hThread5;
		UINT uiThreadId5 = 0;
		hThread5 = (HANDLE)_beginthreadex(NULL,				       // Security attributes
											0,					   // stack
											ProcessingInterfaceThread,			   // Thread proc
											this,					   // Thread param
											CREATE_SUSPENDED,		   // creation mode
											&uiThreadId5);			   // Thread ID

		if ( NULL != hThread5)
		{
			//SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL);
			ResumeThread( hThread5 );
		}
	}
}

UINT WINAPI CameraStatusThread (LPVOID pParam)
{
	CNetworkMonitor* cNetworMonitor = (CNetworkMonitor*)pParam;
	bool isRunningInternal = cNetworMonitor->_isRunning;
	CSharedStruct<CAMERA_STATUS_MSG_SHM>	 m_CameraStatus;
	m_CameraStatus.Acquire(C3Configuration::Instance().SHM_C3_CAMERA_STATUS,
						   C3Configuration::Instance().SHM_C3_CAMERA_STATUS_MUTEX,
						   C3Configuration::Instance().SHM_C3_CAMERA_STATUS_EVENT1,
						   C3Configuration::Instance().SHM_C3_CAMERA_STATUS_EVENT2);
	if (m_CameraStatus.isServer()) m_CameraStatus->ShmInfo.Clients = 0;
	else m_CameraStatus->ShmInfo.Clients++;

	static char timeStr[512];
	static char temp[512];
	static int cameraStatusMessageCount = 0;

    ////get a handle to the CDisplayManager
    CDisplayManager ^dispman = CDisplayManager::getCDisplayManager();

	while(isRunningInternal)
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
				if (dispman->Get_Laser_Activity() != m_CameraStatus->LaserOnOf)
				{
					dispman->Update_Laser_Activity_Indicator(m_CameraStatus->LaserOnOf);
				}

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
			if (dispman->Get_Camera_Com_Status() != 0)
			{
				dispman->Update_Camera_Communication_Indicator(0);
				dispman->Update_Overall_Status();
			}
			else { /* no  update needed */}
		}
		/* Enter the critical section -- other threads are locked out */
		EnterCriticalSection(&cNetworMonitor->cs);		
		/* Do some thread-safe processing! */
		isRunningInternal = cNetworMonitor->_isRunning;
		/* Leave the critical section -- other threads can now EnterCriticalSection() */
		LeaveCriticalSection(&cNetworMonitor->cs);
	}

	_endthreadex( 0 );
    
	return 1L;
}
UINT WINAPI LaserStatusThread (LPVOID pParam)
{
	CNetworkMonitor* cNetworMonitor = (CNetworkMonitor*)pParam;
	bool isRunningInternal = cNetworMonitor->_isRunning;
	CSharedStruct<LASER_STATUS_MSG_SHM>	 m_LaserStatus;
	m_LaserStatus.Acquire(C3Configuration::Instance().SHM_C3_LASER_STATUS,
						  C3Configuration::Instance().SHM_C3_LASER_STATUS_MUTEX,
						  C3Configuration::Instance().SHM_C3_LASER_STATUS_EVENT1,
						  C3Configuration::Instance().SHM_C3_LASER_STATUS_EVENT2);
	if (m_LaserStatus.isServer()) m_LaserStatus->ShmInfo.Clients = 0;
	else m_LaserStatus->ShmInfo.Clients++;

	static char timeStr[512];
	static char temp[512];
	static int laserStatusMessageCount = 0;

    ////get a handle to the CDisplayManager
    CDisplayManager ^dispman = CDisplayManager::getCDisplayManager();

	while(isRunningInternal)
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
			else { /* unable to get mutex??? */	}
		}
		else
		{
			if (dispman->Get_Laser_Com_Status() != 0)
			{
				dispman->Update_Laser_Communication_Indicator(0);
				dispman->Update_Overall_Status();
			}
			else { /* no  update needed */}
		}
		/* Enter the critical section -- other threads are locked out */
		EnterCriticalSection(&cNetworMonitor->cs);		
		/* Do some thread-safe processing! */
		isRunningInternal = cNetworMonitor->_isRunning;
		/* Leave the critical section -- other threads can now EnterCriticalSection() */
		LeaveCriticalSection(&cNetworMonitor->cs);
	}

	_endthreadex( 0 );
    
	return 1L;
}
UINT WINAPI TrackThread (LPVOID pParam)
{
	CNetworkMonitor* cNetworMonitor = (CNetworkMonitor*)pParam;
	bool isRunningInternal = cNetworMonitor->_isRunning;
	CAMERA_TRACK_MSG_SHM sTrackMessage; 
	CSharedStruct<CAMERA_TRACK_MSG_SHM>		 m_CameraTracks;
	m_CameraTracks.Acquire(C3Configuration::Instance().SHM_C3_CAMERA_TRACK,
						   C3Configuration::Instance().SHM_C3_CAMERA_TRACK_MUTEX,
						   C3Configuration::Instance().SHM_C3_CAMERA_TRACK_EVENT1,
						   C3Configuration::Instance().SHM_C3_CAMERA_TRACK_EVENT2);
	if (m_CameraTracks.isServer()) m_CameraTracks->ShmInfo.Clients = 0;
	else m_CameraTracks->ShmInfo.Clients++;

	int x = 0;
	int y = 0;

	while(isRunningInternal)
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
				Coordinates ^ coordsObj = Coordinates::GetCoordinatesHandle();
				array<CoordinatePair^>^ freshCoordinates = coordsObj->MakeCoordinatePairArray();
				for(unsigned int i = 0; i < sTrackMessage.ValidTracks; i ++) {
					freshCoordinates[i]->x = sTrackMessage.Tracks[i].X;
					freshCoordinates[i]->y = sTrackMessage.Tracks[i].Y;
				}
				//x = sTrackMessage.Tracks->X; //x += 1;
				//y = sTrackMessage.Tracks->Y; //y += 1;

				////get a handle to the CDisplayManager
                CDisplayManager ^dispman = CDisplayManager::getCDisplayManager();
				dispman->Update_Rover_PPI_Position(freshCoordinates, sTrackMessage.ValidTracks);

				//This check is already built into DisplayManager
				//if(x >= 1 || y >= 1) dispman->Update_Rover_Acquired_Indicator(1);
			}
			else { /* unable to get mutex??? */	}
		}
		else { /* loss of comm */	}
		/* Enter the critical section -- other threads are locked out */
		EnterCriticalSection(&cNetworMonitor->cs);		
		/* Do some thread-safe processing! */
		isRunningInternal = cNetworMonitor->_isRunning;
		/* Leave the critical section -- other threads can now EnterCriticalSection() */
		LeaveCriticalSection(&cNetworMonitor->cs);
	}

	_endthreadex( 0 );
    
	return 1L;
}
UINT WINAPI ImageThread (LPVOID pParam)
{
	CNetworkMonitor* cNetworMonitor = (CNetworkMonitor*)pParam;
	bool isRunningInternal = cNetworMonitor->_isRunning;
	CSharedStruct<CAMERA_IMAGE_MSG_SHM>		 m_CameraImage;
	char * sImagePath;
	//System::String ^ sImagePath;
	m_CameraImage.Acquire(C3Configuration::Instance().SHM_C3_CAMERA_IMAGE,
						  C3Configuration::Instance().SHM_C3_CAMERA_IMAGE_MUTEX,
						  C3Configuration::Instance().SHM_C3_CAMERA_IMAGE_EVENT1,
						  C3Configuration::Instance().SHM_C3_CAMERA_IMAGE_EVENT2);

	if (m_CameraImage.isServer()) m_CameraImage->ShmInfo.Clients = 0;
	else m_CameraImage->ShmInfo.Clients++;

	while(isRunningInternal)
	{
				// aquire the mutex
		if (m_CameraImage.isCreated() && m_CameraImage.WaitForCommunicationEventServer() == WAIT_OBJECT_0)
		{
			if (m_CameraImage.WaitForCommunicationEventMutex() == WAIT_OBJECT_0)
			{
				// Display data...
				sImagePath = m_CameraImage->imagePath;
				String ^ ssImagePath = gcnew String(sImagePath);
				
				CDisplayManager ^dispman = CDisplayManager::getCDisplayManager();
				dispman->Update_Live_Video_Feed(ssImagePath);

				// Set the event
				m_CameraImage.SetEventClient();
				
				// release the mutex
				m_CameraImage.ReleaseMutex();


			}
			else { /* unable to get mutex??? */	}
		}
		else { /* loss of comm */ }
		/* Enter the critical section -- other threads are locked out */
		EnterCriticalSection(&cNetworMonitor->cs);		
		/* Do some thread-safe processing! */
		isRunningInternal = cNetworMonitor->_isRunning;
		/* Leave the critical section -- other threads can now EnterCriticalSection() */
		LeaveCriticalSection(&cNetworMonitor->cs);
	}

	_endthreadex( 0 );
    
	return 1L;
}

UINT WINAPI ProcessingInterfaceThread (LPVOID pParam)
{
	CNetworkMonitor* cNetworMonitor = (CNetworkMonitor*)pParam;
	bool isRunningInternal = cNetworMonitor->_isRunning;
	CSharedStruct<ALGORITHM_INTERFACE_MSG_SHM>		 m_ProcessingInterface;
	m_ProcessingInterface.Acquire(C3Configuration::Instance().SHM_C3_PROCESSING_STATUS,
								C3Configuration::Instance().SHM_C3_PROCESSING_STATUS_EVENT1,
								C3Configuration::Instance().SHM_C3_PROCESSING_STATUS_EVENT2,
								C3Configuration::Instance().SHM_C3_PROCESSING_STATUS_MUTEX);
	
	CDisplayManager ^dispman = CDisplayManager::getCDisplayManager();
	int nDTIValue = 0;
	int nTrialResult = false;
	int nAlertType = 1;

	while(isRunningInternal)
	{
		// aquire the mutex
		if (m_ProcessingInterface.isCreated() && m_ProcessingInterface.WaitForCommunicationEventServer() == WAIT_OBJECT_0)
		{
			if (m_ProcessingInterface.WaitForCommunicationEventMutex() == WAIT_OBJECT_0)
			{
				nAlertType = m_ProcessingInterface->AlertType;		// 1..n for different conditions: end of trial etc..
				nDTIValue = m_ProcessingInterface->DTI;				// Actual DTI value
				nTrialResult = m_ProcessingInterface->POCResult;	// Pass / fail

				// Only call if the alert is relevant
				if(nAlertType != 0)
				{
					dispman->Update_Notification_Panel(4);
					// Call notification panel... trigger other events
					if(nDTIValue > 0)
					{
						dispman->Store_Latest_DTI(nDTIValue, nTrialResult); 
					}
				}

				// Set the event
				m_ProcessingInterface.SetEventClient();
			
				// release the mutex
				m_ProcessingInterface.ReleaseMutex();
			}
			else { /* unable to get mutex??? */	}
		}
		else { /* loss of comm */						dispman->Update_Notification_Panel(4);}
		/* Enter the critical section -- other threads are locked out */
		EnterCriticalSection(&cNetworMonitor->cs);		
		/* Do some thread-safe processing! */
		isRunningInternal = cNetworMonitor->_isRunning;
		/* Leave the critical section -- other threads can now EnterCriticalSection() */
		LeaveCriticalSection(&cNetworMonitor->cs);
	}

	_endthreadex( 0 );
    
	return 1L;
}