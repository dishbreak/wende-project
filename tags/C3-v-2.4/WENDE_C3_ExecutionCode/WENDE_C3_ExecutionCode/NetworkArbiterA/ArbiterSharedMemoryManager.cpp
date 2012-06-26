#include "StdAfx.h"
#include <stdio.h>						/* defines FILENAME_MAX */
#include <direct.h>
#define GetCurrentDir _getcwd
#include "ArbiterSharedMemoryManager.h"
#include <atlimage.h>

////////////////////////////////////////////////////////////////////////
// Defuatl contructor --- private as this is a singleton
////////////////////////////////////////////////////////////////////////
CArbiterSharedMemoryManager::CArbiterSharedMemoryManager(void)
{
	m_LaserCommand.Acquire("SHM_C3_LASER_STATUS",
						   "SHM_C3_LASER_STATUS_MUTEX",
						   "SHM_C3_LASER_STATUS_EVENT1",
						   "SHM_C3_LASER_STATUS_EVENT2");
	if (m_LaserCommand.isServer()) m_LaserCommand->ShmInfo.Clients = 0;
	else m_LaserCommand->ShmInfo.Clients++;

	C3LaserStatus.Acquire("SHM_C3_LASER_STATUS",
						  "SHM_C3_LASER_STATUS_MUTEX",
						  "SHM_C3_LASER_STATUS_EVENT1",
						  "SHM_C3_LASER_STATUS_EVENT2");
	if (C3LaserStatus.isServer()) C3LaserStatus->ShmInfo.Clients = 0;
	else C3LaserStatus->ShmInfo.Clients++;

	C3CameraStatus.Acquire("SHM_C3_CAMERA_STATUS",
						   "SHM_C3_CAMERA_STATUS_MUTEX",
						   "SHM_C3_CAMERA_STATUS_EVENT1",
						   "SHM_C3_CAMERA_STATUS_EVENT2");
	if (C3CameraStatus.isServer()) C3CameraStatus->ShmInfo.Clients = 0;
	else C3CameraStatus->ShmInfo.Clients++;

	C3CameraTracks.Acquire("SHM_C3_CAMERA_TRACK",
						   "SHM_C3_CAMERA_TRACK_MUTEX",
						   "SHM_C3_CAMERA_TRACK_EVENT1",
						   "SHM_C3_CAMERA_TRACK_EVENT2");
	if (C3CameraTracks.isServer()) C3CameraTracks->ShmInfo.Clients = 0;
	else C3CameraTracks->ShmInfo.Clients++;

	C3CameraImage.Acquire( "SHM_C3_CAMERA_IMAGE",
						   "SHM_C3_CAMERA_IMAGE_MUTEX",
						   "SHM_C3_CAMERA_IMAGE_EVENT1",
						   "SHM_C3_CAMERA_IMAGE_EVENT2");
	if (C3CameraImage.isServer()) C3CameraImage->ShmInfo.Clients = 0;
	else C3CameraImage->ShmInfo.Clients++;
}
////////////////////////////////////////////////////////////////////////
// Default Destructor
////////////////////////////////////////////////////////////////////////
CArbiterSharedMemoryManager::~CArbiterSharedMemoryManager(void)
{
}
////////////////////////////////////////////////////////////////////////
// Function: DecodeCameraStatusMessage_DEBUG
//  Purpose: This function decodes network traffic from camera
//           specifically decodes the status message.
////////////////////////////////////////////////////////////////////////
int CArbiterSharedMemoryManager::DecodeCameraStatusMessage_DEBUG(cameraStatus *ss, char* temp)
{
	// variables
	static char timeStr[256];
	static int  cameraStatusMessageCount = 0;
	// debug string for time
	time_t messageTime;// = ss->time();
	time( &messageTime ) ;	
	ss->set_time(messageTime);
	strftime(timeStr, 20, "%Y-%m-%d %H:%M:%S", localtime(&messageTime));
	// debug string for display 
	sprintf(temp, "+CAMERA STATUS MESSAGE(%d)\r\n", ++cameraStatusMessageCount);
		sprintf(temp, "%s|-->Laser  = %d   \r\n", temp,ss->laseron());
	    sprintf(temp, "%s|-->Time   = %s   \r\n", temp,timeStr);
		sprintf(temp, "%s|-->str = %s   \r\n", temp,ss->text().c_str());
		sprintf(temp, "%s|-->Status = %s   \r\n", temp,DecodeStatus((int)ss->status()).c_str());
		sprintf(temp, "%s\r\n\r\n", temp);

	return cameraStatusMessageCount;
}
////////////////////////////////////////////////////////////////////////
// Function: DecodeCameraStatusMessage
//  Purpose: This function decodes network traffic from camera
//           specifically decodes the status message.
////////////////////////////////////////////////////////////////////////
void CArbiterSharedMemoryManager::DecodeCameraStatusMessage(LPCTSTR strText, char* temp,DWORD size,__int64 startTime)
{
	// protobuf message
	cameraStatus ss;
	// decode the message
	ss.ParseFromArray(strText,size);
	//Prepare the debug print
	int packetNumber = DecodeCameraStatusMessage_DEBUG(&ss,temp);
	time_t messageTime;// = ss->time();
	time( &messageTime ) ;	
	// aquire the mutex
	if (C3CameraStatus.isCreated() &&  
		C3CameraStatus.WaitForCommunicationEventMutex() == WAIT_OBJECT_0)
	{
		// write the data
		C3CameraStatus->PacketNumber = packetNumber;
		C3CameraStatus->LaserOnOf	 = ss.laseron();
		C3CameraStatus->ProcessID    = C3CameraStatus.GetProcessID();
		C3CameraStatus->Status		 = ss.status();
		C3CameraStatus->SubsystemId  = 3;
		C3CameraStatus->Time		 = static_cast<DWORD>(ss.time());

		//end timing
		LARGE_INTEGER start;
		LARGE_INTEGER end;
		LARGE_INTEGER countsPerSecond;
		::QueryPerformanceCounter(&end);
		::QueryPerformanceFrequency(&countsPerSecond);
		start.QuadPart = startTime;
		double elapsed = (double)(end.QuadPart - start.QuadPart) / countsPerSecond.QuadPart;
		C3CameraStatus->startTime= startTime;

		// loops through clients and sends events
		int eventsToSend = C3CameraStatus->ShmInfo.Clients;
		for (int pp = 0; pp < eventsToSend; pp++)
		{
			// Set the event (GUI,processing, etc...)
			C3CameraStatus.SetEventServer();
		}
		// release the mutex
		C3CameraStatus.ReleaseMutex();
	}
}
////////////////////////////////////////////////////////////////////////
// Function: DecodeCameraTrackMessage_DEBUG
//  Purpose: This function decodes network traffic from camera
//           specifically decodes the track message.
////////////////////////////////////////////////////////////////////////
int CArbiterSharedMemoryManager::DecodeCameraTrackMessage_DEBUG(cameraTracks *tr, char* temp)
{
	// variables
	static char timeStr[256];
	static int cameraTrackMessageCount = 0;
	// debug string for display 
	sprintf(temp, "+CAMERA TRACK MESSAGE(%d)\r\n", ++cameraTrackMessageCount);
	sprintf(temp, "%s|-->Laser  = %d   \r\n", temp,tr->laseron());
	sprintf(temp, "%s|-->Status = %s   \r\n", temp,DecodeStatus((int)tr->status()).c_str());
	sprintf(temp, "%s|-->Tracks = %d   \r\n", temp,tr->target_size());
	for (int ii = 0; ii < tr->target_size(); ii++)
	{
		sprintf(temp, "%s|-->Tack %d = [%d , %d]  \r\n",  temp,ii,tr->target(ii).x(),tr->target(ii).y());
	}
	sprintf(temp, "%s|-->Laser = %d   \r\n", temp,tr->laser_size());
	for (int ii = 0; ii < tr->laser_size(); ii++)
	{
		sprintf(temp, "%s|-->Tack %d = [%d , %d]  \r\n", temp,ii,tr->laser(ii).x(),tr->laser(ii).y());
	}
	sprintf(temp, "%s\r\n\r\n", temp);

	return cameraTrackMessageCount;
}
////////////////////////////////////////////////////////////////////////
// Function: DecodeCameraTrackMessage
//  Purpose: This function decodes network traffic from camera
//           specifically decodes the status message.
////////////////////////////////////////////////////////////////////////
void CArbiterSharedMemoryManager::DecodeCameraTrackMessage(LPCTSTR strText, char* temp,DWORD size,__int64 startTime)
{
	// protobuf message
	cameraTracks tr;
	// decode the message
	tr.ParseFromArray(strText,size);
	//Prepare the debug print
	int packetNumebr = DecodeCameraTrackMessage_DEBUG(&tr,temp);
	// aquire the mutex
	if (C3CameraTracks.isCreated() &&  
		C3CameraTracks.WaitForCommunicationEventMutex() == WAIT_OBJECT_0)
	{
		// write the data
		C3CameraTracks->PacketNumber = packetNumebr;
		C3CameraTracks->LaserOnOf	 = tr.laseron();
		C3CameraTracks->ProcessID    = C3CameraTracks.GetProcessID();
		C3CameraTracks->Status		 = tr.status();
		C3CameraTracks->SubsystemId  = 3;
		C3CameraTracks->Time		 = static_cast<DWORD>(tr.time());
		C3CameraTracks->ValidTracks  = tr.target_size();
		
		//end timing
		LARGE_INTEGER start;
		LARGE_INTEGER end;
		LARGE_INTEGER countsPerSecond;
		::QueryPerformanceCounter(&end);
		::QueryPerformanceFrequency(&countsPerSecond);
		start.QuadPart = startTime;
		double elapsed = (double)(end.QuadPart - start.QuadPart) / countsPerSecond.QuadPart;
		C3CameraTracks->startTime= startTime;

		for (int ii = 0; ii < tr.target_size(); ii++)
		{
			C3CameraTracks->Tracks[ii].X = tr.target(ii).x();
			C3CameraTracks->Tracks[ii].Y = tr.target(ii).y();
		}
		C3CameraTracks->ValidLasers  =tr.laser_size();
		for (int ii = 0; ii < tr.laser_size(); ii++)
		{
			C3CameraTracks->Lasers[ii].X = tr.laser(ii).x();
			C3CameraTracks->Lasers[ii].Y = tr.laser(ii).y();
		}
		// loops through clients and sends events
		int eventsToSend = C3CameraTracks->ShmInfo.Clients;
		for (int pp = 0; pp < eventsToSend; pp++)
		{
			// Set the event (GUI,processing, etc...)
			C3CameraTracks.SetEventServer();
		}
		// release the mutex
		C3CameraTracks.ReleaseMutex();
	}
}
////////////////////////////////////////////////////////////////////////
// Function: DecodeCameraImageMessage_DEBUG
//  Purpose: This function decodes network traffic from camera
//           specifically decodes the track message.
////////////////////////////////////////////////////////////////////////
int CArbiterSharedMemoryManager::DecodeCameraImageMessage_DEBUG(cameraImage *im, char* temp)
{
	//variables
	static char timeStr[256];
	static int cameraImageMessageCount = 0;
	// debug string for time
	time_t messageTime;// = ss->time();
	time( &messageTime ) ;	
	im->set_time(messageTime);
	strftime(timeStr, 20, "%Y-%m-%d %H:%M:%S", localtime(&messageTime));
	// debug string for display 
	sprintf(temp, "+CAMERA IMAGE MESSAGE(%d)\r\n", ++cameraImageMessageCount);
	sprintf(temp, "%s|-->Time   = %s   \r\n", temp,timeStr);
	sprintf(temp, "%s\r\n\r\n", temp);

	return cameraImageMessageCount;
}
////////////////////////////////////////////////////////////////////////
// Function: RecreateImage
//  Purpose: This function recreates sent image and saves with unique 
//           name
////////////////////////////////////////////////////////////////////////
string CArbiterSharedMemoryManager::RecreateImage(cameraImage *im)
{
	static int oo = 0;
	oo = (oo==0)?1:0;
	// Current time
	static char timeStr[FILENAME_MAX];
	time_t messageTime;// = ss->time();
	time( &messageTime ) ;	
	im->set_time(messageTime);
	// uniquie file name
	char cCurrentPath[FILENAME_MAX];
	if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath) / sizeof(TCHAR)))
	{
		strcat(cCurrentPath,"");
	}
	else
	{
		strcat(cCurrentPath,"\\Images\\");
		mkdir(cCurrentPath);
	}
	cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */
	strftime(timeStr, FILENAME_MAX, "CameraImage-%Y-%m-%d-%H-%M-%S", localtime(&messageTime));
	sprintf(timeStr,"%s-%d-%d.bmp",timeStr,im->time(),oo);
	strcat(cCurrentPath,timeStr);
	CString saveName(cCurrentPath);
	// create the image
	// THIS IS SLOW AND SHOULD BE DONE BETTER....
	CImage img;
	img.Create(im->sizex() /*width*/, im->sizey()/*height*/, 24 /* bpp */, 0 /* No alpha channel */);
	int ll = 0;
	string st= im->imagedata();
	int maxL= st.size();
	// loop through the image rows
	for(int row = 0; row < min(im->sizey(),SHM_MAX_IMAGE_HEIGHT); row++)
	{
		// loop through the image cols
		for(int col = 0; col < min(im->sizex(),SHM_MAX_IMAGE_WIDTH); col++)
		{
			try
			{
				if (ll+2 < maxL)
				{
					//img.SetPixelRGB(col,row,st.at(ll+2),st.at(ll+1),st.at(ll+0));				
					if (oo==1)
						img.SetPixelRGB(col,row,0,st.at(ll+1),st.at(ll+0));				
					else if (00==2)
						img.SetPixelRGB(col,row,st.at(ll+2),0,st.at(ll+0));				
					else
						img.SetPixelRGB(col,row,st.at(ll+2),st.at(ll+1),0);				
				}
				else
				{
					img.SetPixelRGB(col,row,0,0,0);				
				}
				ll +=3;
			}
			catch(...)
			{

			}
		}
	}
	// save out the file
	img.Save(saveName);
	
	bool found = false;
	while(!found)
	{
		CFileStatus status;
		if( CFile::GetStatus( saveName, status ) )
		{
			found = true;
		}
	}
	//return the file name so that we know what to load.
	return saveName;
}
////////////////////////////////////////////////////////////////////////
// Function: DecodeCameraImageMessage
//  Purpose: This function decodes network traffic from camera
//           specifically decodes the status message.
////////////////////////////////////////////////////////////////////////
string CArbiterSharedMemoryManager::DecodeCameraImageMessage (LPCTSTR strText, char* temp,DWORD size,__int64 startTime)
{
	// protobuf message
	cameraImage im;
	// decode the message
	im.ParseFromArray(strText,size);
	//Prepare the debug print
	int packetNumebr = DecodeCameraImageMessage_DEBUG(&im,temp);
	// recreate the image
	string saveName  = RecreateImage(&im);
	time_t messageTime;// = ss->time();
	time( &messageTime ) ;	
	// write data to shm
	if (C3CameraImage.isCreated() &&  
	    C3CameraImage.WaitForCommunicationEventMutex() == WAIT_OBJECT_0)
	{
		// write the data
		C3CameraImage->PacketNumber = packetNumebr;
		C3CameraImage->ProcessID    = C3CameraImage.GetProcessID();
		C3CameraImage->Time			= static_cast<DWORD>(im.time());
		
		//end timing
		LARGE_INTEGER start;
		LARGE_INTEGER end;
		LARGE_INTEGER countsPerSecond;
		::QueryPerformanceCounter(&end);
		::QueryPerformanceFrequency(&countsPerSecond);
		start.QuadPart = startTime;
		double elapsed = (double)(end.QuadPart - start.QuadPart) / countsPerSecond.QuadPart;
		C3CameraImage->startTime= startTime;

		memcpy(C3CameraImage->imagePath,saveName.c_str(),saveName.size());
		// loops through clients and sends events
		int eventsToSend = C3CameraImage->ShmInfo.Clients;
		for (int pp = 0; pp < eventsToSend; pp++)
		{
			// Set the event (GUI,processing, etc...)
			C3CameraImage.SetEventServer();
		}
		// release the mutex
		C3CameraImage.ReleaseMutex();
	}
	// return path
	return saveName;
}
////////////////////////////////////////////////////////////////////////
// Function: DecodeLaserStatusMessage_DEBUG
//  Purpose: This function decodes network traffic from laser
//           specifically decodes the status message.
////////////////////////////////////////////////////////////////////////
int CArbiterSharedMemoryManager::DecodeLaserStatusMessage_DEBUG(CLaserStatus *ss, char* temp)
{
	// variables
	static int  laserStatusMessageCount = 0;
	// debug string for display 
	sprintf(temp, "+LASER STATUS MESSAGE(%d)\r\n", ++laserStatusMessageCount);
	sprintf(temp, "%s|-->Status = %s   \r\n", temp,DecodeStatus((int)ss->LaserStatus.status).c_str());
	sprintf(temp, "%s|-->AZ     = %d   \r\n", temp,(int)ss->LaserStatus.PWM_AZ);
	sprintf(temp, "%s|-->EL     = %d   \r\n", temp,(int)ss->LaserStatus.PWM_EL);
	sprintf(temp, "%s\r\n\r\n", temp);

	return laserStatusMessageCount;
}
////////////////////////////////////////////////////////////////////////
// Function: DecodeLaserStatusMessage
//  Purpose: This function decodes network traffic from laser
//           specifically decodes the status message.
////////////////////////////////////////////////////////////////////////
void CArbiterSharedMemoryManager::DecodeLaserStatusMessage(LPCTSTR strText, char* temp,DWORD size,__int64 startTime)
{
	
	//  message
	CLaserStatus ss((BYTE*)strText);

	// Prepare the debug print
	int packetNumber = DecodeLaserStatusMessage_DEBUG(&ss,temp);
	
	// aquire the mutex
	if (C3LaserStatus.isCreated() &&  
		C3LaserStatus.WaitForCommunicationEventMutex() == WAIT_OBJECT_0)
	{
		// write the data
		C3LaserStatus->PacketNumber = packetNumber;
		C3LaserStatus->ProcessID    = C3LaserStatus.GetProcessID();
		C3LaserStatus->SubsystemId  = 3;
		//end timing
		LARGE_INTEGER start;
		LARGE_INTEGER end;
		LARGE_INTEGER countsPerSecond;
		::QueryPerformanceCounter(&end);
		::QueryPerformanceFrequency(&countsPerSecond);
		start.QuadPart = startTime;
		double elapsed = (double)(end.QuadPart - start.QuadPart) / countsPerSecond.QuadPart;
		C3LaserStatus->startTime= startTime;

		C3LaserStatus->Status		= ss.LaserStatus.status;
		// loops through clients and sends events
		int eventsToSend = C3LaserStatus->ShmInfo.Clients;
		for (int pp = 0; pp < eventsToSend; pp++)
		{
			// Set the event (GUI,processing, etc...)
			C3LaserStatus.SetEventServer();
		}
		// release the mutex
		C3LaserStatus.ReleaseMutex();
	}
}
//DECODE THE STATUS
string CArbiterSharedMemoryManager::DecodeStatus(int status)
{
	string resultStatus;
	switch (status)
	{
		case  0: { resultStatus = "DOWN";			break;}
   	    case  1: { resultStatus = "READY";			break;}
	    case  2: { resultStatus = "OPERATIONAL";	break;}
		case  3: { resultStatus = "ERROR";			break;}
		case  4: { resultStatus = "FAILED";			break;}
		case  5:
		default: { resultStatus = "UNKOWN";			break;}
	}
	return resultStatus;
}