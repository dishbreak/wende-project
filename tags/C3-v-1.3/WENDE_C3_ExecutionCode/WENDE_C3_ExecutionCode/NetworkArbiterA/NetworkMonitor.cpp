#include "StdAfx.h"
#include "NetworkMonitor.h"
#include <process.h>
#include "CSharedStruct.h"
#include "ShmStructs.h"
#include "C3Configuration.h"
#include "SocketComm.h"
#include "LaserCommand.h"
#include "Utilties.h"
UINT WINAPI LaserCommandThread (LPVOID pParam);

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
void CNetworkMonitor::InitializeThread(CSocketManager* socketObjectServer)
{
	if (!_isRunning)
	{
		m_SocketObjectServer = socketObjectServer;
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
										  LaserCommandThread,		// Thread proc
										  this,					  // Thread param
										  CREATE_SUSPENDED,		  // creation mode
										  &uiThreadId1);			  // Thread ID

		if ( NULL != hThread1)
		{
			//SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL);
			ResumeThread( hThread1 );
		}
	}
}

UINT WINAPI LaserCommandThread (LPVOID pParam)
{
	CNetworkMonitor* cNetworMonitor = (CNetworkMonitor*)pParam;
	bool isRunningInternal = cNetworMonitor->_isRunning;
	
	CSharedStruct<LASER_POINT_DIRECTION_SHM> m_LaserCommand;
	m_LaserCommand.Acquire(C3Configuration::Instance().SHM_C3_LASER_POINTING,			// shared mem file name
						   C3Configuration::Instance().SHM_C3_LASER_POINTING_MUTEX,	// shared mem mutex name
						   C3Configuration::Instance().SHM_C3_LASER_POINTING_EVENT1,	// shared mem event name
						   C3Configuration::Instance().SHM_C3_LASER_POINTING_EVENT2);	// shared mem event name
	if (m_LaserCommand.isServer()) m_LaserCommand->ShmInfo.Clients = 0;
	else m_LaserCommand->ShmInfo.Clients++;	

	while(isRunningInternal)
	{
		// aquire the mutex
		if (m_LaserCommand.isCreated() && m_LaserCommand.WaitForCommunicationEventServer() == WAIT_OBJECT_0)
		{
			if (m_LaserCommand.WaitForCommunicationEventMutex() == WAIT_OBJECT_0)
			{
				if (cNetworMonitor->m_SocketObjectServer!= NULL)
				{
					// Get data
					stMessageProxy msgProxy;
					CLaserCommand pose;
					pose.LaserStatus.PWM_AZ    = m_LaserCommand->PointLocation.AZ;
					pose.LaserStatus.PWM_EL    = m_LaserCommand->PointLocation.EL;
					pose.LaserStatus.isLaserOn = m_LaserCommand->LaserOnOff;
					int nLen = sizeof(LASER_COMMAND_STRUCT);
					memcpy(msgProxy.byData, T2CA((char*)pose.StatusToBytes()), nLen);
					unsigned char sizeArray[4];
					CUtilities::IntToBytes(sizeArray,htonl(nLen));
					cNetworMonitor->m_SocketObjectServer->WriteComm(sizeArray, sizeof(unsigned char)*4, INFINITE);
					cNetworMonitor->m_SocketObjectServer->WriteComm(msgProxy.byData, nLen, INFINITE);
				}
				// Set the event
				m_LaserCommand.SetEventClient();
				
				// release the mutex
				m_LaserCommand.ReleaseMutex();
			}
			else{/* unable to get mutex??? */}
		}
		else{}
		/* Enter the critical section -- other threads are locked out */
		EnterCriticalSection(&cNetworMonitor->cs);		
		/* Do some thread-safe processing! */
		isRunningInternal = cNetworMonitor->_isRunning;
		/* Leave the critical section -- other threads can now EnterCriticalSection() */
		LeaveCriticalSection(&cNetworMonitor->cs);
	}

	m_LaserCommand->ShmInfo.Clients--;	

	cNetworMonitor->m_SocketObjectServer = NULL;
	
	_endthreadex( 0 );
    
	return 1L;
}
