#include "stdafx.h"
#include "CSharedStruct.h"
#include "ShmStructs.h"
#include <time.h>
#include <process.h>
#include "CNetworkMonitor.h"
#include "CDisplayManager.h"
#include "C3Configuration.h"
#include "C3ProcessingStates.h"
#include "Coordinates.h"
#include <fstream>
#include <iostream>

using namespace System;
using namespace std;

UINT WINAPI TrackThread (LPVOID pParam);
UINT WINAPI ImageThread (LPVOID pParam);
UINT WINAPI LaserStatusThread (LPVOID pParam);
UINT WINAPI CameraStatusThread(LPVOID pParam);
UINT WINAPI ProcessingInterfaceReceiveThread(LPVOID pParam);
UINT WINAPI ProcessingInterfaceTransmitThread(LPVOID pParam);
UINT WINAPI PPIDebugThread(LPVOID pParam);

CNetworkMonitor::CNetworkMonitor()
:_isRunning(false)
{
	functionArray[0] = TrackThread;
	functionArray[1] = ImageThread,
	functionArray[2] = LaserStatusThread,
	functionArray[3] = CameraStatusThread;
	functionArray[4] = ProcessingInterfaceReceiveThread;
	functionArray[5] = ProcessingInterfaceTransmitThread;
	functionArray[6] = PPIDebugThread;

	/* Initialize the critical section before entering multi-threaded context. */
	InitializeCriticalSection(&cs);
	InitializeCriticalSection(&statusLock);
	stLink.statusRecv= false;
	stLink.trackRecv = false;
}
CNetworkMonitor::~CNetworkMonitor()
{
	/* Release system object when all finished -- usually at the end of the cleanup code */
	DeleteCriticalSection(&cs);
	DeleteCriticalSection(&statusLock);
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
void CNetworkMonitor::StartProcessing()
{
	int ii = C3_NETWORK_THREAD_COUNT - 1;

	if ( NULL != threads[ii].handle)
	{
		ResumeThread( threads[ii].handle );
	}
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
		int ii = 0;
		for (; ii < C3_NETWORK_THREAD_COUNT-1;  ii++)
		{
			threads[ii].handle = (HANDLE)_beginthreadex( NULL,				       // Security attributes
				0,					       // stack
				functionArray[ii],		   // Thread proc
				this,					   // Thread param
				CREATE_SUSPENDED,		   // creation mode
				&threads[ii].uiThreadId);  // Thread ID

			if ( NULL != threads[ii].handle)
			{
				ResumeThread( threads[ii].handle );
			}
		}
		ii = C3_NETWORK_THREAD_COUNT - 1;
		threads[ii].handle = (HANDLE)_beginthreadex( NULL,				            // Security attributes
			0,					        // stack
			functionArray[ii],		    // Thread proc
			this,					    // Thread param
			CREATE_SUSPENDED,		    // creation mode
			&threads[ii].uiThreadId);   // Thread ID
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
			/* Enter the critical section -- other threads are locked out */
			EnterCriticalSection(&cNetworMonitor->statusLock);		
			cNetworMonitor->stLink.statusRecv= true;
			/* Leave the critical section -- other threads can now EnterCriticalSection() */
			LeaveCriticalSection(&cNetworMonitor->statusLock);
			if (m_CameraStatus.WaitForCommunicationEventMutex() == WAIT_OBJECT_0)
			{
				EnterCriticalSection(&cNetworMonitor->statusLock);		
			
				////set camera state
				dispman->Update_Camera_Subsystem_Indicator(m_CameraStatus->Status);
				dispman->Update_Camera_Communication_Indicator(1);

				if (dispman->Get_Laser_Activity() != m_CameraStatus->LaserOnOf)
				{
					dispman->Update_Laser_Activity_Indicator(m_CameraStatus->LaserOnOf);
				}

				dispman->Update_Overall_Status();

				// Set the event
				m_CameraStatus.SetEventClient();

				// release the mutex
				m_CameraStatus.ReleaseMutex();
				
				/* Leave the critical section -- other threads can now EnterCriticalSection() */
				LeaveCriticalSection(&cNetworMonitor->statusLock);
			}
			else
			{
				// unable to get mutex???
			}
		}
		else
		{
			/* Enter the critical section -- other threads are locked out */
			EnterCriticalSection(&cNetworMonitor->statusLock);		
			cNetworMonitor->stLink.statusRecv= false;
			if (cNetworMonitor->stLink.statusRecv==false && cNetworMonitor->stLink.trackRecv==false)
			{
				if (dispman->Get_Camera_Com_Status() != 0)
				{
					dispman->Update_Camera_Communication_Indicator(0);
					dispman->Update_Overall_Status();
				}
				else { /* no  update needed */}
			}
			/* Leave the critical section -- other threads can now EnterCriticalSection() */
			LeaveCriticalSection(&cNetworMonitor->statusLock);
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
				EnterCriticalSection(&cNetworMonitor->statusLock);		
			
				dispman->Update_Laser_Subsystem_Indicator(m_LaserStatus->Status);
				dispman->Update_Laser_Communication_Indicator(1);
				dispman->Update_Overall_Status();

				LARGE_INTEGER start;
				LARGE_INTEGER end;
				LARGE_INTEGER countsPerSecond;

				::QueryPerformanceCounter(&end);
				::QueryPerformanceFrequency(&countsPerSecond);
				start.QuadPart = m_LaserStatus->startTime;
				double elapsed = (double)(end.QuadPart - start.QuadPart) / countsPerSecond.QuadPart;

				// Set the event
				m_LaserStatus.SetEventClient();

				// release the mutex
				m_LaserStatus.ReleaseMutex();

				/* Leave the critical section -- other threads can now EnterCriticalSection() */
				LeaveCriticalSection(&cNetworMonitor->statusLock);
			}
			else { /* unable to get mutex??? */	}
		}
		else
		{
			EnterCriticalSection(&cNetworMonitor->statusLock);		
			if (dispman->Get_Laser_Com_Status() != 0)
			{
				dispman->Update_Laser_Communication_Indicator(0);
				dispman->Update_Overall_Status();
			}
			else { /* no  update needed */}
			/* Leave the critical section -- other threads can now EnterCriticalSection() */
			LeaveCriticalSection(&cNetworMonitor->statusLock);
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

	////get a handle to the CDisplayManager
	CDisplayManager ^dispman = CDisplayManager::getCDisplayManager();

	while(isRunningInternal)
	{
		// aquire the mutex
		if (m_CameraTracks.isCreated() && m_CameraTracks.WaitForCommunicationEventServer() == WAIT_OBJECT_0)
		{
			/* Enter the critical section -- other threads are locked out */
			EnterCriticalSection(&cNetworMonitor->statusLock);		
			cNetworMonitor->stLink.trackRecv= true;
			/* Leave the critical section -- other threads can now EnterCriticalSection() */
			LeaveCriticalSection(&cNetworMonitor->statusLock);
			if (m_CameraTracks.WaitForCommunicationEventMutex() == WAIT_OBJECT_0)
			{
				/* Enter the critical section -- other threads are locked out */
				EnterCriticalSection(&cNetworMonitor->statusLock);

				////set camera state
				dispman->Update_Camera_Subsystem_Indicator(m_CameraTracks->Status);
				dispman->Update_Camera_Communication_Indicator(1);
				if (dispman->Get_Laser_Activity() != m_CameraTracks->LaserOnOf)
				{
					dispman->Update_Laser_Activity_Indicator(m_CameraTracks->LaserOnOf);
				}
				dispman->Update_Overall_Status();

				sTrackMessage = *m_CameraTracks.GetStruct();

				LARGE_INTEGER start;
				LARGE_INTEGER end;
				LARGE_INTEGER countsPerSecond;

				::QueryPerformanceCounter(&end);
				::QueryPerformanceFrequency(&countsPerSecond);
				start.QuadPart = m_CameraTracks->startTime;
				double elapsed = (double)(end.QuadPart - start.QuadPart) / countsPerSecond.QuadPart;

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

				//Get laser dot
				bool HaveLaserPoint = false;
				if(sTrackMessage.ValidLasers > 0) HaveLaserPoint = true;
                CoordinatePair ^ newLaserPoint = gcnew CoordinatePair(sTrackMessage.Lasers[0].X, sTrackMessage.Lasers[0].Y);

				////get a handle to the CDisplayManager
				CDisplayManager ^dispman = CDisplayManager::getCDisplayManager();
				dispman->Update_Rover_PPI_Position(freshCoordinates, sTrackMessage.ValidTracks);
                dispman->Update_Laser_PPI_Position(newLaserPoint, HaveLaserPoint);

				if(sTrackMessage.ValidTracks >= 1)
					dispman->Update_Rover_Acquired_Indicator(1);
				else
					dispman->Update_Rover_Acquired_Indicator(0);

				/* Leave the critical section -- other threads can now EnterCriticalSection() */
				LeaveCriticalSection(&cNetworMonitor->statusLock);
			}
			else 
			{
			}
		}
		else 
		{ 
			/* Enter the critical section -- other threads are locked out */
			EnterCriticalSection(&cNetworMonitor->statusLock);		
			cNetworMonitor->stLink.trackRecv = false;
			if (cNetworMonitor->stLink.statusRecv==false && cNetworMonitor->stLink.trackRecv==false)
			{
				if (dispman->Get_Camera_Com_Status() != 0)
				{
					dispman->Update_Camera_Communication_Indicator(0);
					dispman->Update_Overall_Status();
				}
				else { /* no  update needed */}
			}
			/* Leave the critical section -- other threads can now EnterCriticalSection() */
			LeaveCriticalSection(&cNetworMonitor->statusLock);
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
				/* Enter the critical section -- other threads are locked out */
				EnterCriticalSection(&cNetworMonitor->statusLock);

				// Display data...
				sImagePath = m_CameraImage->imagePath;
				String ^ ssImagePath = gcnew String(sImagePath);

				CDisplayManager ^dispman = CDisplayManager::getCDisplayManager();
				dispman->Update_Live_Video_Feed(ssImagePath, m_CameraImage->startTime);

				// Set the event
				m_CameraImage.SetEventClient();

				// release the mutex
				m_CameraImage.ReleaseMutex();
				
				/* Leave the critical section -- other threads can now EnterCriticalSection() */
				LeaveCriticalSection(&cNetworMonitor->statusLock);
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

UINT WINAPI ProcessingInterfaceReceiveThread (LPVOID pParam)
{
	CNetworkMonitor* cNetworMonitor = (CNetworkMonitor*)pParam;
	bool isRunningInternal = cNetworMonitor->_isRunning;
	CSharedStruct<ALGORITHM_INTERFACE_MSG_SHM>		 m_ProcessingInterface;
	m_ProcessingInterface.Acquire(C3Configuration::Instance().SHM_C3_PROCESSING_STATUS,
		C3Configuration::Instance().SHM_C3_PROCESSING_STATUS_MUTEX,
		C3Configuration::Instance().SHM_C3_PROCESSING_STATUS_EVENT1,
		C3Configuration::Instance().SHM_C3_PROCESSING_STATUS_EVENT2);
	if (m_ProcessingInterface.isServer()) m_ProcessingInterface->ShmInfo.Clients = 0;
	else m_ProcessingInterface->ShmInfo.Clients++;

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
				/* Enter the critical section -- other threads are locked out */
				EnterCriticalSection(&cNetworMonitor->statusLock);

				nAlertType   = m_ProcessingInterface->AlertType;		// 1..n for different conditions: end of trial etc..
				nDTIValue    = m_ProcessingInterface->DTI;				// Actual DTI value
				nTrialResult = m_ProcessingInterface->POCResult;		// Pass / fail

				// Only call if the alert is relevant
				if(nAlertType != 0)
				{
					if((C3_Alert_Types)nAlertType == C3_Alert_Types::CALIBRATION_FAILED ||
						(C3_Alert_Types)nAlertType == C3_Alert_Types::CALIBRATION_SUCCESS )
					{
						// Update the Calibration button
						dispman->Update_Calibration_Reply(nAlertType);
					}

					if((C3_Alert_Types)nAlertType == C3_Alert_Types::TARGET_LEFT_PLAYING_FIELD )
					{
						// Patient left evac area
						dispman->Update_Notification_Panel(2);
					}

					// Call notification panel... trigger other events
					if((C3_Alert_Types)nAlertType == C3_Alert_Types::POC_FINISHED )
					{
						dispman->Store_Latest_DTI(nDTIValue, nTrialResult); 

						if(nTrialResult == 1)
							// Trial Success
							dispman->Update_Notification_Panel(1);
						else
							// Trial failure
							dispman->Update_Notification_Panel(3);
					}
				}			
				// release the mutex
				m_ProcessingInterface.ReleaseMutex();

				
				/* Leave the critical section -- other threads can now EnterCriticalSection() */
				LeaveCriticalSection(&cNetworMonitor->statusLock);
			}
			else { /* unable to get mutex??? */	}
		} 
		else 
		{ 
			/* Enter the critical section -- other threads are locked out */
			EnterCriticalSection(&cNetworMonitor->statusLock);

			/* loss of comm */
			// Only update if new notification (allows dismissal)
			if(4 != dispman->Get_Notification_Alert())
			{
				dispman->Set_Notification_Alert(4);
				dispman->Update_Notification_Panel(4);
			}

			/* Leave the critical section -- other threads can now EnterCriticalSection() */
			LeaveCriticalSection(&cNetworMonitor->statusLock);
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

UINT WINAPI ProcessingInterfaceTransmitThread(LPVOID pParam)
{
	CNetworkMonitor* cNetworMonitor = (CNetworkMonitor*)pParam;

	CSharedStruct<ALGORITHM_INTERFACE_MSG_SHM>		 m_ProcessingInterface;

	m_ProcessingInterface.Acquire(C3Configuration::Instance().SHM_C3_PROCESSING_STATUS,
		C3Configuration::Instance().SHM_C3_PROCESSING_STATUS_MUTEX,
		C3Configuration::Instance().SHM_C3_PROCESSING_STATUS_EVENT1,
		C3Configuration::Instance().SHM_C3_PROCESSING_STATUS_EVENT2);

	C3_Alert_Types eProcessingNotification;
	
	int ii = C3_NETWORK_THREAD_COUNT - 1;

	while(1)
	{
		if (m_ProcessingInterface.WaitForCommunicationEventMutex() == WAIT_OBJECT_0)
		{
			eProcessingNotification = C3ProcessingStates::Instance().Get_Current_Alert();

			if(eProcessingNotification == C3_Alert_Types::CALIBRATION_INIT)
			{
				// Ask Processing nicely to Init calibration
				m_ProcessingInterface->AlertType = C3_Alert_Types::CALIBRATION_INIT;
			}

			if(eProcessingNotification == C3_Alert_Types::POC_STARTED)
			{
				// Ask Processing nicely to Start trial
				m_ProcessingInterface->AlertType = C3_Alert_Types::POC_STARTED;
			}

			if(eProcessingNotification == C3_Alert_Types::POC_FINISHED)
			{
				// Ask Processing nicely End trial
				m_ProcessingInterface->AlertType = C3_Alert_Types::POC_FINISHED;
			}

			// Set the event
			m_ProcessingInterface.SetEventClient();	

			// release the mutex
			m_ProcessingInterface.ReleaseMutex();
			// suspend thread;
			if ( NULL != cNetworMonitor->threads[ii].handle)
			{
				SuspendThread( cNetworMonitor->threads[ii].handle );
			}
		}
	}

	_endthreadex( 0 );

	return 1L;
}

UINT WINAPI PPIDebugThread (LPVOID pParam)
{
	CNetworkMonitor* cNetworMonitor = (CNetworkMonitor*)pParam;
	bool isRunningInternal = cNetworMonitor->_isRunning;
	PPI_DEBUG_MSG_SHM sPPIMsg;
	CSharedStruct<PPI_DEBUG_MSG_SHM> m_PPIMsg;
	m_PPIMsg.Acquire(C3Configuration::Instance().SHM_C3_PROCESSING_DEBUG_STATUS,
		C3Configuration::Instance().SHM_C3_PROCESSING_DEBUG_MUTEX,
		C3Configuration::Instance().SHM_C3_PROCESSING_DEBUG_EVENT1,
		C3Configuration::Instance().SHM_C3_PROCESSING_DEBUG_EVENT2);
	if (m_PPIMsg.isServer()) m_PPIMsg->ShmInfo.Clients = 0;
	else m_PPIMsg->ShmInfo.Clients++;

	static char timeStr[512];
	static char temp[512];
	static int processingDebugMessageCount = 0;

	////get a handle to the CDisplayManager
	CDisplayManager ^dispman = CDisplayManager::getCDisplayManager();

	C3_TRACK_POINT cRoverLocationsCur[SHM_MAX_TRACKS]; 
	C3_TRACK_POINT cRoverLocationsPIP[SHM_MAX_TRACKS];
	C3_TRACK_POINT cLaserOrigin;

	while(isRunningInternal)
	{
		// aquire the mutex
		if (m_PPIMsg.isCreated() && m_PPIMsg.WaitForCommunicationEventServer() == WAIT_OBJECT_0)
		{
			if (m_PPIMsg.WaitForCommunicationEventMutex() == WAIT_OBJECT_0)
			{
				EnterCriticalSection(&cNetworMonitor->statusLock);		

				sPPIMsg = *m_PPIMsg.GetStruct();

				// Loop through list of valid tracks (determined by processing)
				for(unsigned int i = 0; i < sPPIMsg.NumberValid ; i ++) 
				{
					cRoverLocationsCur[i].X = sPPIMsg.RoverLocationsCur[i].X;
					cRoverLocationsCur[i].Y = sPPIMsg.RoverLocationsCur[i].Y;

					cRoverLocationsPIP[i].X = sPPIMsg.RoverLocationsPIP[i].X;
					cRoverLocationsPIP[i].Y = sPPIMsg.RoverLocationsPIP[i].Y;
				}

				cLaserOrigin = sPPIMsg.LaserOrigin;

				// Dispman stuff here //

				// Set the event
				m_PPIMsg.SetEventClient();

				// release the mutex
				m_PPIMsg.ReleaseMutex();

				/* Leave the critical section -- other threads can now EnterCriticalSection() */
				LeaveCriticalSection(&cNetworMonitor->statusLock);
			}
			else { /* unable to get mutex??? */	}
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