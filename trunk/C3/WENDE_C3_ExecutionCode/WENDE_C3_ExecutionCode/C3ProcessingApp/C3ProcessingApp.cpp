/////////////////////////////////////////////////////////////////////////////////
// C3ProcessingApp.cpp : Defines the entry point for the console application.
/////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "CSharedStruct.h"
#include "ShmStructs.h"
#include <time.h>
#include <process.h>
#include "C3ProcessingConfiguration.h"

/////////////////////////////////////////////////////////////////////////////////
// Declare main functions
/////////////////////////////////////////////////////////////////////////////////
UINT WINAPI TrackThread (LPVOID pParam);		// main processing thread

int _tmain(int argc, _TCHAR* argv[])
{
	/////////////////////////////////////////////////////////////////////////////////
	// Read in the user configuration
	/////////////////////////////////////////////////////////////////////////////////
	C3ProcessingConfiguration config;

	/////////////////////////////////////////////////////////////////////////////////
	// Setup local variables
	/////////////////////////////////////////////////////////////////////////////////
	static char timeStr[256];								// Temporary time string
	static int cameraTrackMessageCount = 0;					// static packet count...
	bool readMessageSuccess = false;						// determines that the 
															// message was recived 
															// correctly

	/////////////////////////////////////////////////////////////////////////////////
	// Setup the shared memory
	/////////////////////////////////////////////////////////////////////////////////
	CSharedStruct<CAMERA_TRACK_MSG_SHM>		 m_CameraTracks;// shared memory data struct wrapper
	m_CameraTracks.Acquire(config.SHM_C3_CAMERA_TRACK,			// shared mem file name
						   config.SHM_C3_CAMERA_TRACK_MUTEX,		// shared mem mutex name
						   config.SHM_C3_CAMERA_TRACK_EVENT1,	// shared mem event name
						   config.SHM_C3_CAMERA_TRACK_EVENT2);	// shared mem event name
	if (m_CameraTracks.isServer()) m_CameraTracks->ShmInfo.Clients = 0;
	else m_CameraTracks->ShmInfo.Clients++;
	
	CSharedStruct<LASER_POINT_DIRECTION_SHM> m_LaserCommand;
	m_LaserCommand.Acquire(config.SHM_C3_LASER_POINTING,			// shared mem file name
						   config.SHM_C3_LASER_POINTING_MUTEX,	// shared mem mutex name
						   config.SHM_C3_LASER_POINTING_EVENT1,	// shared mem event name
						   config.SHM_C3_LASER_POINTING_EVENT2);	// shared mem event name
	if (m_LaserCommand.isServer()) m_LaserCommand->ShmInfo.Clients = 0;
	else m_LaserCommand->ShmInfo.Clients++;	

	CAMERA_TRACK_MSG_SHM					 inData;		// temporary holder of the current data 
	
	/////////////////////////////////////////////////////////////////////////////////
	// Spin and process the input messages and send commands
	/////////////////////////////////////////////////////////////////////////////////
	while(1)
	{
		// reset the success flag
		readMessageSuccess = false;

		/////////////////////////////////////////////////////////////////////////////////
		// Get Input messages
		/////////////////////////////////////////////////////////////////////////////////
		// Verify that the shm is setup and wait for event
		if (m_CameraTracks.isCreated() && m_CameraTracks.WaitForCommunicationEventServer() == WAIT_OBJECT_0)
		{
			// aquire the mutex ---> just for coding standard not really needed in our setup
			if (m_CameraTracks.WaitForCommunicationEventMutex() == WAIT_OBJECT_0)
			{
				// store the data in current struct
				inData = *m_CameraTracks.GetStruct();
				
				// Set the event to let server know (not needed in our setup)
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
				for (unsigned int ii = 0; ii < inData.ValidTracks; ii++)
				{
					printf("|-->Tack %d = [%d , %d]  \r\n",ii,inData.Tracks[ii].X,inData.Tracks[ii].Y);
				}
				printf("|-->Lasr = %d   \r\n",inData.ValidLasers);
				for (unsigned int ii = 0; ii < inData.ValidLasers; ii++)
				{
					printf("|-->Tack %d = [%d , %d]  \r\n",ii,inData.Lasers[ii].X,inData.Lasers[ii].Y);
				}
				printf("\r\n\r\n");
				readMessageSuccess = true;
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

		/////////////////////////////////////////////////////////////////////////////////
		// DO PROCESSING !!!!
		/////////////////////////////////////////////////////////////////////////////////
		// ...
		// ...
		// ... 

		/////////////////////////////////////////////////////////////////////////////////
		// Get Input messages
		/////////////////////////////////////////////////////////////////////////////////
		// Verify that the shm is setup and aquire the mutex 
		if (readMessageSuccess == true && m_LaserCommand.isCreated() &&
			m_LaserCommand.WaitForCommunicationEventMutex() == WAIT_OBJECT_0)
		{
			// set the output data
			m_LaserCommand->LaserOnOff   = true;
			m_LaserCommand->PacketNumber = cameraTrackMessageCount;
			m_LaserCommand->ProcessID    = m_LaserCommand.GetProcessID();
			m_LaserCommand->Time		 = inData.Time;
			
			m_LaserCommand->PointLocation.AZ = 20; //TODO FIX THIS VALUE TO ACTUAL
			m_LaserCommand->PointLocation.AZ = 21; //TODO FIX THIS VALUE TO ACTUAL

			// Set the event to let client know
			m_LaserCommand.SetEventClient();

			// release the mutex
			m_LaserCommand.ReleaseMutex();
		}
		else
		{
			// unable to get mutex???
		}
	}

	return 1L;
}
