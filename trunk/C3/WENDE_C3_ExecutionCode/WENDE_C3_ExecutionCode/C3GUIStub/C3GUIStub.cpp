// C3GUIStub.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CSharedStruct.h"
#include "ShmStructs.h"
#include <time.h>
//#include <Streams.h>
#include <process.h>

UINT WINAPI StatusThread(LPVOID pParam);
UINT WINAPI TrackThread (LPVOID pParam);
UINT WINAPI ImageThread (LPVOID pParam);


int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE hThread1;
	UINT uiThreadId1 = 0;
	hThread1 = (HANDLE)_beginthreadex(NULL,				       // Security attributes
										0,					  // stack
									 StatusThread,   // Thread proc
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
									 TrackThread,   // Thread proc
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
										0,					   // stack
									 ImageThread,			   // Thread proc
									 NULL,					   // Thread param
									 CREATE_SUSPENDED,		   // creation mode
									 &uiThreadId3);			   // Thread ID

	if ( NULL != hThread3)
	{
		//SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL);
		ResumeThread( hThread3 );
	}

	while (1)
	{
		Sleep (100);
	}
	
	return 0;
}
UINT WINAPI StatusThread (LPVOID pParam)
{
	CSharedStruct<CAMERA_STATUS_MSG_SHM>	 m_CameraStatus;
	m_CameraStatus.Acquire("SHM_C3_CAMERA_STAUTS",
						   "SHM_C3_CAMERA_STATUS_MUTEX",
						   "SHM_C3_CAMERA_STATUS_EVENT1",
						   "SHM_C3_CAMERA_STATUS_EVENT2");
	static char timeStr[512];
	static char temp[512];
	static int cameraStatusMessageCount = 0;

	while(1)
	{
		// aquire the mutex
		if (m_CameraStatus.isCreated() && m_CameraStatus.WaitForCommunicationEventServer() == WAIT_OBJECT_0)
		{
			if (m_CameraStatus.WaitForCommunicationEventMutex() == WAIT_OBJECT_0)
			{
				// Read the data
				sprintf(temp, "+CAMERA STATUS MESSAGE(%d)\r\n", ++cameraStatusMessageCount);
				sprintf(temp, "%s|-->Laser  = %d   \r\n", temp,m_CameraStatus->LaserOnOf);
				time_t now = m_CameraStatus->Time;
				strftime(timeStr, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));
				sprintf(temp, "%s|-->Time   = %s   \r\n", temp,timeStr);
				sprintf(temp, "%s|-->Status = %d   \r\n", temp,m_CameraStatus->Status);
				printf("%s\r\n\r\n", temp);

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
			// loss of comm
		}
	}

	_endthreadex( 0 );
    
	return 1L;
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
UINT WINAPI ImageThread (LPVOID pParam)
{
	CSharedStruct<CAMERA_IMAGE_MSG_SHM>		 m_CameraImage;
	m_CameraImage.Acquire( "SHM_C3_CAMERA_IMAGE",
						   "SHM_C3_CAMERA_IMAGE_MUTEX",
						   "SHM_C3_CAMERA_IMAGE_EVENT1",
						   "SHM_C3_CAMERA_IMAGE_EVENT2");	
	while(1)
	{
				// aquire the mutex
		if (m_CameraImage.isCreated() && m_CameraImage.WaitForCommunicationEventServer() == WAIT_OBJECT_0)
		{
			if (m_CameraImage.WaitForCommunicationEventMutex() == WAIT_OBJECT_0)
			{
				//// Read the data
				//LPBITMAPINFOHEADER  pdib = (LPBITMAPINFOHEADER) m_CameraImage->ImageData;
				//BITMAPFILEHEADER    hdr;
				//DWORD               dwSize;
				//// Initialize the bitmap header.
				//dwSize				= DibSize(pdib);
				//hdr.bfType          = BFT_BITMAP;
				//hdr.bfSize          = dwSize + sizeof(BITMAPFILEHEADER);
				//hdr.bfReserved1     = 0;
				//hdr.bfReserved2     = 0;
				//hdr.bfOffBits       = (DWORD)sizeof(BITMAPFILEHEADER) + pdib->biSize + DibPaletteSize(pdib);

				//CFile file;
				//if(!file.Open("test.bmp",CFile::modeWrite | CFile::modeCreate,NULL))
				//	return;

				//// Write the bitmap header and bitmap bits to the file.
				//file.Write((LPCVOID) &hdr, sizeof(BITMAPFILEHEADER));
				//file.Write(m_CameraImage.ReadFromSharedMemoryDataInfo(), dwSize);

				//file.Close();

				// Set the event
				m_CameraImage.SetEventClient();
				
				// release the mutex
				m_CameraImage.ReleaseMutex();
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