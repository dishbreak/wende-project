// C3ProcessingApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "CSharedStruct.h"
#include "ShmStructs.h"
#include <time.h>
#include <process.h>

UINT WINAPI TrackThread (LPVOID pParam);

int _tmain(int argc, _TCHAR* argv[])
{
		HANDLE hThread2;
	UINT uiThreadId2 = 0;
	hThread2 = (HANDLE)_beginthreadex(NULL,				       // Security attributes
										0,					  // stack
									 TrackThread,   // Thread proc
									 NULL,					  // Thread param
									 CREATE_SUSPENDED,		  // creation mode
									 &uiThreadId2);			  // Thread ID

	if ( NULL != hThread2)
	{
		//SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL);
		ResumeThread( hThread2 );
	}

	while (1)
	{
		Sleep (10000);
	}
	return 0;
}

UINT WINAPI TrackThread (LPVOID pParam)
{
	static char timeStr[256];
	static int cameraTrackMessageCount = 0;

	CAMERA_TRACK_MSG_SHM inData; 
	CSharedStruct<CAMERA_TRACK_MSG_SHM>		 m_CameraTracks;
	m_CameraTracks.Acquire("SHM_C3_CAMERA_TRACK",
						   "SHM_C3_CAMERA_TRACK_MUTEX",
						   "SHM_C3_CAMERA_TRACK_EVENT1",
						   "SHM_C3_CAMERA_TRACK_EVENT2");
	while(1)
	{
		// aquire the mutex
		if (m_CameraTracks.isCreated() && m_CameraTracks.WaitForCommunicationEventServer() == WAIT_OBJECT_0)
		{
			if (m_CameraTracks.WaitForCommunicationEventMutex() == WAIT_OBJECT_0)
			{
				inData = *m_CameraTracks.GetStruct();
				// Set the event
				m_CameraTracks.SetEventClient();
				
				// release the mutex
				m_CameraTracks.ReleaseMutex();

				// Read the data
				printf("+CAMERA TRACK MESSAGE(%d)\r\n", ++cameraTrackMessageCount);
				printf("|-->Laser  = %d   \r\n",inData.LaserOnOf);
				time_t now = inData.Time;
				strftime(timeStr, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));
				printf("|-->Time   = %s   \r\n",timeStr);
				printf("|-->Status = %d   \r\n",inData.Status);
				printf("|-->Tracks = %d   \r\n",inData.ValidTracks);
				for (int ii = 0; ii < inData.ValidTracks; ii++)
				{
					printf("|-->Tack %d = [%d , %d]  \r\n",ii,inData.Tracks[ii].X,inData.Tracks[ii].Y);
				}
				printf("|-->Laser = %d   \r\n",inData.ValidLasers);
				for (int ii = 0; ii < inData.ValidLasers; ii++)
				{
					printf("|-->Tack %d = [%d , %d]  \r\n",ii,inData.Lasers[ii].X,inData.Lasers[ii].Y);
				}
				printf("\r\n\r\n");
				
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
