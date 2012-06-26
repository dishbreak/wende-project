/////////////////////////////////////////////////////////////////////////////////
// C3ProcessingApp.cpp : Defines the entry point for the console application.
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
// Include directives
/////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CSharedStruct.h"
#include "ShmStructs.h"
#include <time.h>
#include <iostream>
#include <fstream>
#include <string>
#include <process.h>
#include "C3Configuration.h"
#include "C3FilterClass.h"
#include "C3TrackerManager.h"
#include "C3Utilities.h"
#include <vector>
#include <conio.h>
#include <stdio.h>
#include "C3NotificationHandler.h"
#include "Log.h"
/////////////////////////////////////////////////////////////////////////////////
// Using directives
/////////////////////////////////////////////////////////////////////////////////
using std::vector;
using std::cout;
using std::endl;
using std::ifstream;
using std::string;
/////////////////////////////////////////////////////////////////////////////////
// TEST functions
/////////////////////////////////////////////////////////////////////////////////
void TestKalmanFilter();
void TestTrackFilter();
void TestCalibration(HANDLE hconsole);
void TestNoMovement();
void TestXMovement();
/////////////////////////////////////////////////////////////////////////////////
// NON TEST FUNCTIONS
/////////////////////////////////////////////////////////////////////////////////
C3_TRACK_POINT_DOUBLE calibrate(C3_TRACK_POINT_DOUBLE*);
C3_TRACK_POINT_DOUBLE getCalibrationPointCommand();
void updateCalibrationState();
UINT WINAPI AlgorithmThread (LPVOID pParam);
void WriteDataToScreen(CAMERA_TRACK_MSG_SHM	inData, int cameraTrackMessageCount);
bool ReadInputMessage(CSharedStruct<CAMERA_TRACK_MSG_SHM> *shm,
					  CAMERA_TRACK_MSG_SHM                *trk,
					  int								  &cameraTrackMessageCount,
					  vector<C3_TRACK_POINT_DOUBLE>	      *roverPoints);
bool SendOutputMessage(bool										sendMessageSuccess,
					   bool										laserOnOff,
					   CAMERA_TRACK_MSG_SHM						inData,
					   C3_TRACK_POINT_DOUBLE					commandOut,
					   CSharedStruct<LASER_POINT_DIRECTION_SHM> *lCommand,
					   int										cameraTrackMessageCount);
bool SendNotification(CSharedStruct<ALGORITHM_INTERFACE_MSG_SHM> *notification);
bool SendPPINotification(CSharedStruct<PPI_DEBUG_MSG_SHM> *notification,
						 vector<C3_TRACK_POINT_DOUBLE>    *rPoints,
						 vector<C3_TRACK_POINT_DOUBLE>    *rPipPoints,
						 C3_TRACK_POINT_DOUBLE             laser);
/////////////////////////////////////////////////////////////////////////////////
// MACROS
/////////////////////////////////////////////////////////////////////////////////
#define WAIT_MESSAGES    100
#define TICK_OFFSET      1086
#define TICKS_PER_DEGREE 20.6
#define DEGREES_TO_TICKS(DEG)(TICK_OFFSET+DEG*TICKS_PER_DEGREE)
#define MM_TO_M(x)(x/1000.0)
#define M_TO_MM(x)(x*1000.0)
#define MS_TO_S(x)(x/1000.0)
/////////////////////////////////////////////////////////////////////////////////
// Declare main functions
/////////////////////////////////////////////////////////////////////////////////
int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE hconsole = GetStdHandle (STD_OUTPUT_HANDLE);
	
	/////////////////////////////////////////////////////////////////////////////////
	// Setup local variables
	/////////////////////////////////////////////////////////////////////////////////
	bool readMessageSuccess = false;						// determines that the 
															// message was recived 
															// correctly
	bool sendMessageSuccess = false;						// determines if we send a message
	static int cameraTrackMessageCount = 0;					// static packet count...
	bool laserOnOff = false;
	int waitMessages = 0;									// the number of messages that we wait until using a measurment
	int calIndex = 0;										// number of points
	CAMERA_TRACK_MSG_SHM					 inData;		// temporary holder of the current data 
	vector<C3_TRACK_POINT_DOUBLE>			 roverPoints;	// the rover points
	vector<C3_TRACK_POINT_DOUBLE>			 rPoints;
	vector<C3_TRACK_POINT_DOUBLE>			 rPipPoints;
	vector<C3_TRACK_POINT_DOUBLE>			 avgPoints;
	C3_TRACK_POINT_DOUBLE					 testPoints[4];	// test points for calibration
	C3_TRACK_POINT_DOUBLE					 commandOut;	// the gimbal command out
	C3_TRACK_POINT_DOUBLE					 laserOrigin;	// the laser origion setup from the calibration
	C3_TRACK_POINT_DOUBLE					 laserPoint;	// temp contain for rover points
	C3TrackerManager						 tm;			// the tracker manager
	double theta=0.0;
	C3_Alert_Types							 state;

	memset((void*)&testPoints,0,sizeof(C3_TRACK_POINT_DOUBLE)*4);
	memset((void*)&commandOut,0,sizeof(C3_TRACK_POINT_DOUBLE));
	memset((void*)&laserOrigin,0,sizeof(C3_TRACK_POINT_DOUBLE));
	laserOrigin.X = C3Configuration::Instance().LaserOriginX;
	laserOrigin.Y = C3Configuration::Instance().LaserOriginY;
	memset((void*)&laserPoint,0,sizeof(C3_TRACK_POINT_DOUBLE));
	//TestKalmanFilter();
	//TestTrackFilter();
	//TestCalibration(hconsole);
	//TestNoMovement();
	//TestXMovement();
	//return 0;	
	/////////////////////////////////////////////////////////////////////////////////
	// Setup the shared memory
	/////////////////////////////////////////////////////////////////////////////////
	CSharedStruct<CAMERA_TRACK_MSG_SHM>		 m_CameraTracks;									// shared memory data struct wrapper
	m_CameraTracks.Acquire(C3Configuration::Instance().SHM_C3_CAMERA_TRACK,						// shared mem file name
						   C3Configuration::Instance().SHM_C3_CAMERA_TRACK_MUTEX,				// shared mem mutex name
						   C3Configuration::Instance().SHM_C3_CAMERA_TRACK_EVENT1,				// shared mem event name
						   C3Configuration::Instance().SHM_C3_CAMERA_TRACK_EVENT2);				// shared mem event name
	if (m_CameraTracks.isServer()) m_CameraTracks->ShmInfo.Clients = 0;
	else m_CameraTracks->ShmInfo.Clients++;
	
	CSharedStruct<LASER_POINT_DIRECTION_SHM> m_LaserCommand;
	m_LaserCommand.Acquire(C3Configuration::Instance().SHM_C3_LASER_POINTING,					// shared mem file name
						   C3Configuration::Instance().SHM_C3_LASER_POINTING_MUTEX,				// shared mem mutex name
						   C3Configuration::Instance().SHM_C3_LASER_POINTING_EVENT1,			// shared mem event name
						   C3Configuration::Instance().SHM_C3_LASER_POINTING_EVENT2);			// shared mem event name
	if (m_LaserCommand.isServer()) m_LaserCommand->ShmInfo.Clients = 0;
	else m_LaserCommand->ShmInfo.Clients++;	

	CSharedStruct<ALGORITHM_INTERFACE_MSG_SHM> m_DisplayNotification;
	m_DisplayNotification.Acquire(C3Configuration::Instance().SHM_C3_PROCESSING_STATUS,			// shared mem file name
								  C3Configuration::Instance().SHM_C3_PROCESSING_STATUS_MUTEX,	// shared mem mutex name
								  C3Configuration::Instance().SHM_C3_PROCESSING_STATUS_EVENT1,	// shared mem event name
								  C3Configuration::Instance().SHM_C3_PROCESSING_STATUS_EVENT2);	// shared mem event name
	if (m_DisplayNotification.isServer()) m_DisplayNotification->ShmInfo.Clients = 0;
	else m_DisplayNotification->ShmInfo.Clients++;

	CSharedStruct<PPI_DEBUG_MSG_SHM> m_PipNotification;
	m_PipNotification.Acquire(C3Configuration::Instance().SHM_C3_PROCESSING_DEBUG_STATUS,			// shared mem file name
						      C3Configuration::Instance().SHM_C3_PROCESSING_DEBUG_MUTEX,	// shared mem mutex name
							  C3Configuration::Instance().SHM_C3_PROCESSING_DEBUG_EVENT1,	// shared mem event name
						      C3Configuration::Instance().SHM_C3_PROCESSING_DEBUG_EVENT2);	// shared mem event name
	if (m_PipNotification.isServer()) m_PipNotification->ShmInfo.Clients = 0;
	else m_PipNotification->ShmInfo.Clients++;
	/////////////////////////////////////////////////////////////////////////////////
	// Thread
	/////////////////////////////////////////////////////////////////////////////////
	HANDLE hThread1;
	UINT uiThreadId1 = 0;
	hThread1 = (HANDLE)_beginthreadex(NULL,				      // Security attributes
										0,					  // stack
									  AlgorithmThread,		  // Thread proc
									  &m_DisplayNotification, // Thread param
									  CREATE_SUSPENDED,		  // creation mode
									  &uiThreadId1);  	      // Thread ID

	if ( NULL != hThread1)
	{
		//SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL);
		ResumeThread( hThread1 );
	}
	avgPoints.clear();
	/////////////////////////////////////////////////////////////////////////////////
	// Spin and process the input messages and send commands
	/////////////////////////////////////////////////////////////////////////////////
	while(1)
	{
		// reset the success flag
		sendMessageSuccess = false;				// set the write flag to false
		// Remove the previous points
		roverPoints.clear();					// clear points
		rPoints.clear();
		rPipPoints.clear();
		// send the points out
		readMessageSuccess =  ReadInputMessage(&m_CameraTracks,
			                                   &inData,
											   cameraTrackMessageCount,
											   &roverPoints);
		/////////////////////////////////////////////////////////////////////////////////
		// DO PROCESSING !!!!
		/////////////////////////////////////////////////////////////////////////////////
		if (readMessageSuccess)
		{
			////////////////////////
			// DO CALIBRATION!!!!!!
			////////////////////////
			if (C3NotificationHandler::Instance().Get_IsCalibration() == true)
			{
				if (C3Configuration::Instance().isSkipCalibration == false)
				{
					commandOut = getCalibrationPointCommand();
					laserOnOff = true;
					// wait until time passes...
					if (waitMessages >= WAIT_MESSAGES)
					{
						// make sure the camera saw the laser
						if (inData.ValidLasers != 0)
						{
							testPoints[calIndex].X = inData.Lasers[0].X;
							testPoints[calIndex].Y = inData.Lasers[0].Y;
						// Log laser commands for requirements sell-off
							FILE_LOG(logDEBUG) <<	"Laser Point: X = " << testPoints[calIndex].X << "\n";
							FILE_LOG(logDEBUG) <<	"Laser Point: Y = " << testPoints[calIndex].Y << "\n\n";
							// get next command
							commandOut = getCalibrationPointCommand();

							// save point (assumes single laser)
							
							calIndex++;

							// TODO ADD LOGIC TO CHECK THE LASER POINT ACTUALLY MOVED
							// ...
							// ...
							// ...
							if (C3NotificationHandler::Instance().Get_Process_State() == C3_Alert_Types::CALIBRATION_IN_PROGRESS_5)
							{	
								// do calibration
								// This is the line that has the calibration.
								laserOrigin = calibrate(testPoints);
								laserOrigin.X = MM_TO_M(laserOrigin.X);
								laserOrigin.Y = MM_TO_M(laserOrigin.Y);
								tm.ClearTracks();
								FILE_LOG(logDEBUG) <<	"Laser Origin: X   = " << laserOrigin.X << "\n";
								FILE_LOG(logDEBUG) <<	"Laser Origin: Y = " << laserOrigin.Y << "\n\n";
								//rest index
								calIndex    = 0;
								//2140 x
								//-700 y
								SendPPINotification(&m_PipNotification,&rPoints,&rPipPoints,laserOrigin);
								printf("****************\n");
								printf("****************\n");
								printf("[%5.5f,%5.5f]\n",laserOrigin.X,laserOrigin.Y);
								printf("****************\n");
								printf("****************\n");
								Sleep(4);
							}
							// walk the states
							updateCalibrationState();

							// set flag
							sendMessageSuccess = true;

							waitMessages = 0;
						}
						else
						{
							FILE_LOG(logDEBUG) <<	"NO LASER!!!!\n";
							// TODO ADD MAX COUNTER
							//C3NotificationHandler::Instance().Set_Process_State(C3_Alert_Types::CALIBRATION_FAILED);	
						}
					}
					else
					{						
						if (waitMessages ==  0)
						{
							// just send the same command since we have what we ne
							sendMessageSuccess = true;	
						}
						if (inData.ValidLasers == 0)
						{
							FILE_LOG(logDEBUG) <<	"NO LASER!!!!\n";
							// TODO ADD MAX COUNTER
							//C3NotificationHandler::Instance().Set_Process_State(C3_Alert_Types::CALIBRATION_FAILED);	
						}
						else
						{
							// increment count
							waitMessages++;
						}
					}
				}
				else
				{
					C3NotificationHandler::Instance().Set_Process_State(C3_Alert_Types::CALIBRATION_SUCCESS);	
					C3NotificationHandler::Instance().Set_IsCalibration(false);	
					SendPPINotification(&m_PipNotification,&rPoints,&rPipPoints,laserOrigin);
				}
			}
			////////////////////////
			// DO TRACKING!!!!!!
			////////////////////////
			else
			{
				state = C3NotificationHandler::Instance().Get_Process_State();
				if (state == C3_Alert_Types::POC_STARTED || 
					state == C3_Alert_Types::TARGET_LEFT_PLAYING_FIELD)
				{
					if (inData.ValidTracks != 0)
					{
						if (inData.ValidLasers != 0)
						{
							laserPoint.X = MM_TO_M(inData.Lasers[0].X);
							laserPoint.Y = MM_TO_M(inData.Lasers[0].Y);
							// time correct to second
							C3_TRACK_POINT_DOUBLE temp;
							temp = tm.UpdateTracks(roverPoints, laserPoint, MS_TO_S(inData.Time),laserOrigin);
							// TODO ITEM VERIFY THE LASER MACROS
							laserOnOff = true;//(commandOut.AZ != 0 && commandOut.EL != 0)? true : false;
							commandOut.AZ = (temp.AZ * TICKS_PER_DEGREE + commandOut.AZ);
							commandOut.EL = (temp.EL * TICKS_PER_DEGREE + commandOut.EL);
							if(abs(temp.AZ) >= 0.0001 && abs(temp.EL) >=0.001)
							{
								sendMessageSuccess = true;
							}	
							tm.getPIP(&rPoints,&rPipPoints);
							
							SendPPINotification(&m_PipNotification,&rPoints,&rPipPoints,laserOrigin);

							// Log laser commands for requirements sell-off
							FILE_LOG(logDEBUG) <<	"Laser Command: Azimuth   = " << commandOut.AZ << "\n";
							FILE_LOG(logDEBUG) <<	"Laser Command: Elevation = " << commandOut.EL << "\n\n";
						}
						else
						{
							FILE_LOG(logDEBUG) <<	"NO LASER!!!!\n";
							//sendMessageSuccess = true;
						}
					}
					else
					{
						sendMessageSuccess = false;
					}
					if (tm.GetDTI() > C3Configuration::Instance().WENDE_PLAYING_FIELD_RADIUS)
					{
						C3NotificationHandler::Instance().Set_Process_State(C3_Alert_Types::TARGET_LEFT_PLAYING_FIELD);
					}
				}
				else if (state == C3_Alert_Types::POC_FINISHED)
				{
					sendMessageSuccess == true;
					double value = tm.GetDTI();
					value = M_TO_MM(value);
					C3NotificationHandler::Instance().Set_DTI_Value((int)value);
					
					if (tm.GetDTI() <= C3Configuration::Instance().WENDE_FAILURE_LINE_RADIUS)
					{
						C3NotificationHandler::Instance().Set_Trial_Result(true);
					}
					else
					{
						C3NotificationHandler::Instance().Set_Trial_Result(false);
					}

					SendNotification(&m_DisplayNotification);
					Sleep(4000);
					C3NotificationHandler::Instance().Set_Process_State(C3_Alert_Types::CALIBRATION_SUCCESS);
					tm.ClearTracks();
					commandOut.AZ = DEGREES_TO_TICKS(90);
					commandOut.EL = DEGREES_TO_TICKS(90);
				}
			}
			
			////////////////////////
			// Send notifications
			////////////////////////
				SendNotification(&m_DisplayNotification);
		}
		////////////////////////
		// Send output message
			////////////////////////
		SendOutputMessage(sendMessageSuccess,
						  laserOnOff,
						  inData,
					      commandOut,
					      &m_LaserCommand,
					      cameraTrackMessageCount);
	}

	return 1L;
}
bool SendPPINotification(CSharedStruct<PPI_DEBUG_MSG_SHM> *notification,
						 vector<C3_TRACK_POINT_DOUBLE>    *rPoints,
						 vector<C3_TRACK_POINT_DOUBLE>    *rPipPoints,
						 C3_TRACK_POINT_DOUBLE             laser)
{
	bool result = false;
	////////////////////////
	// Send notifications
	////////////////////////
	if((*notification).isCreated() && 
		(*notification).WaitForCommunicationEventMutex() == WAIT_OBJECT_0)
	{
		std::cout << "Predictions diff = [";
		for (unsigned int ii = 0; ii < min(rPoints->size(),SHM_MAX_TRACKS); ii++)
		{
			(*notification)->RoverLocationsCur[ii].X = M_TO_MM((*rPoints)[ii].X);
			(*notification)->RoverLocationsCur[ii].Y = M_TO_MM((*rPoints)[ii].Y);
			(*notification)->RoverLocationsPIP[ii].X = M_TO_MM((*rPipPoints)[ii].X);
			(*notification)->RoverLocationsPIP[ii].Y = M_TO_MM((*rPipPoints)[ii].Y);
			if (min(rPoints->size(),SHM_MAX_TRACKS)-1 != ii)
			std::cout << C3Utilities::EuclideanDistance((*rPoints)[ii],(*rPipPoints)[ii]) << ", ";
			else
			std::cout << C3Utilities::EuclideanDistance((*rPoints)[ii],(*rPipPoints)[ii]);
		}
		std::cout << "]" << std::endl;

		(*notification)->ProcessID  = (*notification).GetProcessID();

		(*notification)->NumberValid  = rPoints->size();

		(*notification)->LaserOrigin.X = M_TO_MM(laser.X);
		(*notification)->LaserOrigin.Y = M_TO_MM(laser.Y);
		
		(*notification).SetEventServer();

		(*notification).ReleaseMutex();

		result = true;
	}
	
	return result;
}
bool SendNotification(CSharedStruct<ALGORITHM_INTERFACE_MSG_SHM> *notification)
{
	bool result = false;
	////////////////////////
	// Send notifications
	////////////////////////
	if((*notification).isCreated() && 
		(*notification).WaitForCommunicationEventMutex() == WAIT_OBJECT_0)
	{
		(*notification)->AlertType = C3NotificationHandler::Instance().Get_Process_State();
		(*notification)->DTI       = C3NotificationHandler::Instance().Get_DTI_Value();
		(*notification)->POCResult = C3NotificationHandler::Instance().Get_Trial_Result();

		(*notification).SetEventServer();

		(*notification).ReleaseMutex();

		result = true;
	}
	
	return result;
}
bool SendOutputMessage(bool										sendMessageSuccess,
					   bool										laserOnOff,
					   CAMERA_TRACK_MSG_SHM						inData,
					   C3_TRACK_POINT_DOUBLE					commandOut,
					   CSharedStruct<LASER_POINT_DIRECTION_SHM> *lCommand,
					   int										cameraTrackMessageCount)
{
	bool result = false;
	/////////////////////////////////////////////////////////////////////////////////
	// Send Output messages
	/////////////////////////////////////////////////////////////////////////////////
	// Verify that the shm is setup and aquire the mutex 
	if (sendMessageSuccess == true && (*lCommand).isCreated() &&
		(*lCommand).WaitForCommunicationEventMutex() == WAIT_OBJECT_0)
	{
		// set the output data
		(*lCommand)->LaserOnOff		  = laserOnOff;							
		(*lCommand)->PacketNumber	  = cameraTrackMessageCount;			
		(*lCommand)->ProcessID		  = (*lCommand).GetProcessID();	
		(*lCommand)->Time			  = inData.Time;						
		(*lCommand)->startTime		  = inData.startTime;	
		(*lCommand)->PointLocation.EL = commandOut.EL;			
		(*lCommand)->PointLocation.AZ = commandOut.AZ;			

		// Set the event to let client know
		(*lCommand).SetEventServer();

		// release the mutex
		(*lCommand).ReleaseMutex();

		result = true;
	}
	else
	{ 
		/* unable to get mutex??? */
		result = false;
	}
	return result;
}
bool ReadInputMessage(CSharedStruct<CAMERA_TRACK_MSG_SHM> *shm,
					  CAMERA_TRACK_MSG_SHM                *trk,
					  int                                 &cameraTrackMessageCount,
					  vector<C3_TRACK_POINT_DOUBLE>	      *roverPoints)
{
	bool readMessageSuccess = false;
	C3_TRACK_POINT_DOUBLE					 roverPoint;	// temp contain for rover points

	/////////////////////////////////////////////////////////////////////////////////
	// Get Input messages
	/////////////////////////////////////////////////////////////////////////////////
	// Verify that the shm is setup and wait for event
	if ((*shm).isCreated() && (*shm).WaitForCommunicationEventServer() == WAIT_OBJECT_0)
	{
		// aquire the mutex ---> just for coding standard not really needed in our setup
		if ((*shm).WaitForCommunicationEventMutex() == WAIT_OBJECT_0)
		{
			// store the data in current struct
			(*trk) = *(*shm).GetStruct();
			
			// Set the event to let server know (not needed in our setup)
			(*shm).SetEventClient();
			
			// release the mutex
			(*shm).ReleaseMutex();

			// write data to the screen
			//WriteDataToScreen((*trk),cameraTrackMessageCount);

			// Read the data
			for (unsigned int ii = 0; ii < (*trk).ValidTracks; ii++)
			{
				roverPoint.X = MM_TO_M((*trk).Tracks[ii].X);
				roverPoint.Y = MM_TO_M((*trk).Tracks[ii].Y);
				roverPoints->push_back(roverPoint);
			}

			// set the read message to true
			readMessageSuccess = true;
		}
		else{ /* unable to get mutex??? */}
	}
	else{ /* loss of comm*/	}

	return readMessageSuccess; 
}
C3_TRACK_POINT_DOUBLE getCalibrationPointCommand()
{
	C3_TRACK_POINT_DOUBLE command;
	switch(C3NotificationHandler::Instance().Get_Process_State())
	{
		case CALIBRATION_IN_PROGRESS_1:
			{
				command.AZ = DEGREES_TO_TICKS(90);
				command.EL = DEGREES_TO_TICKS(90);
				break;
			}
		case CALIBRATION_IN_PROGRESS_2:
			{
				command.AZ = DEGREES_TO_TICKS(90);
				command.EL = DEGREES_TO_TICKS(95);
				break;
			}
		case CALIBRATION_IN_PROGRESS_3:
			{
				command.AZ = DEGREES_TO_TICKS(95);
				command.EL = DEGREES_TO_TICKS(90);
				break;
			}
		case CALIBRATION_IN_PROGRESS_4:
			{
				command.AZ = DEGREES_TO_TICKS(95);
				command.EL = DEGREES_TO_TICKS(95);
				break;
			}
		case CALIBRATION_IN_PROGRESS_5:
			{
				command.AZ = DEGREES_TO_TICKS(95);
				command.EL = DEGREES_TO_TICKS(95);
				break;
			}
	}
	return command;
};
void updateCalibrationState()
{
	switch(C3NotificationHandler::Instance().Get_Process_State())
	{
		case CALIBRATION_IN_PROGRESS_1:
			{
				C3NotificationHandler::Instance().Set_Process_State(C3_Alert_Types::CALIBRATION_IN_PROGRESS_2);	
				break;
			}
		case CALIBRATION_IN_PROGRESS_2:
			{
				C3NotificationHandler::Instance().Set_Process_State(C3_Alert_Types::CALIBRATION_IN_PROGRESS_3);	
				break;
			}
		case CALIBRATION_IN_PROGRESS_3:
			{
				C3NotificationHandler::Instance().Set_Process_State(C3_Alert_Types::CALIBRATION_IN_PROGRESS_4);	
				break;
			}
		case CALIBRATION_IN_PROGRESS_4:
			{
				C3NotificationHandler::Instance().Set_Process_State(C3_Alert_Types::CALIBRATION_IN_PROGRESS_5);	
				break;
			}
		case CALIBRATION_IN_PROGRESS_5:
			{
				// set to success
				C3NotificationHandler::Instance().Set_Process_State(C3_Alert_Types::CALIBRATION_SUCCESS);	
				C3NotificationHandler::Instance().Set_IsCalibration(false);	

				break;
			}
	}
};

// Calibration Method
C3_TRACK_POINT_DOUBLE calibrate(C3_TRACK_POINT_DOUBLE *points){

	C3_TRACK_POINT_DOUBLE laserPosition;
	double m1;
	if ((points[1].X - points[0].X) != 0)
	{
		m1 = ((points[1].Y-points[0].Y) / (points[1].X - points[0].X));
	}
	else
	{
		m1 = 0.0;
	}
	double m2;
	if ((points[3].X - points[2].X) != 0)
	{
		m2 = ((points[3].Y-points[2].Y) / (points[3].X - points[2].X));	 
	}
	else
	{
		m2 = 0.0;
	}
	double b1 = points[0].Y-m1*points[0].X; 
	double b2 = points[3].Y-m2*points[3].X;
	 
	if ((m1-m2) != 0)
	{
		laserPosition.X = (b2-b1) / (m1-m2); 
	}
	else
	{
		laserPosition.X = 0.0; 
	}
	laserPosition.Y = laserPosition.X*m1+b1;
		
	return laserPosition;

}
// algorithm thread
UINT WINAPI AlgorithmThread (LPVOID pParam)
{
	CSharedStruct<ALGORITHM_INTERFACE_MSG_SHM> m_DisplayNotification;
	m_DisplayNotification.Acquire(C3Configuration::Instance().SHM_C3_PROCESSING_STATUS,			// shared mem file name
								  C3Configuration::Instance().SHM_C3_PROCESSING_STATUS_MUTEX,	// shared mem mutex name
								  C3Configuration::Instance().SHM_C3_PROCESSING_STATUS_EVENT1,	// shared mem event name
								  C3Configuration::Instance().SHM_C3_PROCESSING_STATUS_EVENT2);	// shared mem event name
	while(1)
	{
		if (m_DisplayNotification.isCreated() && m_DisplayNotification.WaitForCommunicationEventClient() == WAIT_OBJECT_0)
		{
			if (m_DisplayNotification.WaitForCommunicationEventMutex() == WAIT_OBJECT_0)
			{
				// read state
				C3NotificationHandler::Instance().Set_Process_State((C3_Alert_Types)m_DisplayNotification->AlertType);	
				// release mutex
				m_DisplayNotification.ReleaseMutex();
			}
		}
	}
	_endthreadex( 0 );
    
	return 1L;
}
void WriteDataToScreen(CAMERA_TRACK_MSG_SHM	inData, int cameraTrackMessageCount)
{
	//char timeStr[256];								// Temporary time string
	//// Read the data
	//printf("+CAMERA TRACK MESSAGE(%d)\r\n", ++cameraTrackMessageCount);
	//printf("|-->Laser  = %d   \r\n",inData.LaserOnOf);
	//time_t now = inData.Time;
	//strftime(timeStr, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));
	//printf("|-->Time   = %s   \r\n",timeStr);
	//printf("|-->Status = %d   \r\n",inData.Status);
	//printf("|-->Tracks = %d   \r\n",inData.ValidTracks);
	//for (unsigned int ii = 0; ii < inData.ValidTracks; ii++)
	//{
	//	printf("|-->Tack %d = [%d , %d]  \r\n",ii,inData.Tracks[ii].X,inData.Tracks[ii].Y);
	//}
	//printf("|-->Lasr = %d   \r\n",inData.ValidLasers);
	//for (unsigned int ii = 0; ii < inData.ValidLasers; ii++)
	//{
	//	printf("|-->Tack %d = [%d , %d]  \r\n",ii,inData.Lasers[ii].X,inData.Lasers[ii].Y);
	//}
	//printf("\r\n\r\n");
}
/////////////////////////////////////////////////////////////////////////////////
// TEST functions
/////////////////////////////////////////////////////////////////////////////////
void TestKalmanFilter()
{
	vector<string> files;
	files.insert(files.begin(),"TEST_5.txt");
	files.insert(files.begin(),"TEST_4.txt");
	files.insert(files.begin(),"TEST_3.txt");
	files.insert(files.begin(),"TEST_2.txt");
	files.insert(files.begin(),"TEST_1.txt");
	for (unsigned int jj = 0; jj < files.size(); jj++)
	{
		printf("TEST FILE --- %d ---\n",jj);
		C3FilterClass  kalman;
		
		C3_TRACK_POINT_DOUBLE testPoint;
		C3_TRACK_POINT_DOUBLE procResult;
		C3_TRACK_POINT_DOUBLE fileResult;

		double         testUpdateRate = 0.250;
		double         MAX_ERROR      = 0.0000005;

		// verify test output 1 (matlab move in x only)
		ifstream myfile (files[jj].c_str());
		for (int ii = 0; myfile.is_open() && ii < 100; ii ++)
		{
			myfile >> testPoint.X >> testPoint.Y >> fileResult.X >> fileResult.Y;
			procResult = kalman.FilterInput(testPoint,testUpdateRate);
			assert(abs(fileResult.X-procResult.X) < MAX_ERROR && 
				   abs(fileResult.Y-procResult.Y) < MAX_ERROR);
			printf("Iteration (%d)::Difference [X,Y]= [%f,%f]\n", ii,abs(fileResult.X-procResult.X),abs(fileResult.Y-procResult.Y));
		}
		myfile.close();
	}
}
void TestNoMovement()
{
	C3_TRACK_POINT_DOUBLE roverPoint;
	roverPoint.X = 0.0;
	roverPoint.Y = 0.0;
	C3_TRACK_POINT_DOUBLE laserPoint;
	laserPoint.X = 0.0;
	laserPoint.Y = 0.0;
	C3_TRACK_POINT_DOUBLE laserOrigin;
	laserOrigin.X = -2.0;
	laserOrigin.Y =  3.0;

	C3Track  track(roverPoint,0.0);
	
	C3_TRACK_POINT_DOUBLE procResult;
	int            time			= 0.0;
	double         MAX_ERROR    = 0.0000005;

	for (int ii = 0; ii < 100; ii++)
	{
		procResult = track.UpdateTrack(roverPoint,laserPoint,time,laserOrigin);
		time += 250;	
		assert(procResult.AZ == 0 && procResult.EL == 0);
		printf("Iteration (%d)::Difference [X,Y]= [%f,%f]\n", ii,procResult.X,procResult.Y);
	}
}
void TestXMovement()
{
	C3_TRACK_POINT_DOUBLE laserPoint;
	laserPoint.X = 0.0;
	laserPoint.Y = 0.0;
	C3_TRACK_POINT_DOUBLE laserOrigin;
	laserOrigin.X = 0.0;
	laserOrigin.Y =  0.0;

	C3TrackerManager  track;
	
	C3_TRACK_POINT_DOUBLE procResult;
	double         time			= 0.0;
	int			   MOVEMENT     = 21;
	vector<C3_TRACK_POINT_DOUBLE> t;
	C3_TRACK_POINT_DOUBLE testPoint;
	
	double         testUpdateRate = 0.250;
	double         MAX_ERROR      = 0.0000005;

	// verify test output 1 (matlab move in x only)
	ifstream myfile ("TEST_5.txt");
	for (int ii = 0; myfile.is_open() && ii < 100; ii ++)
	{
		myfile >> testPoint.X >> testPoint.Y >> procResult.X >> procResult.Y;
		
		t.clear();
		t.push_back(testPoint);
		procResult = track.UpdateTracks(t,laserPoint,time,laserOrigin);
		time += 250.0/1000.0;	
		printf("Iteration (%d)::in [X,Y]= [%f,%f] out = [%f,%f]\n", ii,testPoint.X,testPoint.Y,
			track.getPredictedPoint().X,track.getPredictedPoint().Y);
	}
}
void TestTrackFilter()
{
	vector<string> files;
	files.insert(files.begin(),"TestTrack_fast.txt");
	files.insert(files.begin(),"TestTrack_slow.txt");
	files.insert(files.begin(),"TestTrack_stopCrawl.txt");
	files.insert(files.begin(),"TestTrack_spiral.txt");
	for (unsigned int jj = 0; jj < files.size(); jj++)
	{
		printf("TEST FILE --- %d ---\n",jj);
		C3_TRACK_POINT_DOUBLE roverPoint;
		roverPoint.X = 0.0;
		roverPoint.Y = 0.0;

		C3Track  track(roverPoint,0.0);
		
		C3_TRACK_POINT_DOUBLE laserPoint;
		C3_TRACK_POINT_DOUBLE procResult;
		C3_TRACK_POINT_DOUBLE fileResult;

		double         time			= 0.0;
		double         MAX_ERROR    = 0.0000005;

		// verify test output 1 (matlab move in x only)
		ifstream myfile (files[jj].c_str());
		C3_TRACK_POINT_DOUBLE laserOrigin;
		laserOrigin.X = -2;
		laserOrigin.Y =  3;
		for (int ii = 0; myfile.is_open() && ii < 100; ii ++)
		{
			myfile >> roverPoint.X >> roverPoint.Y >> laserPoint.X >> laserPoint.Y >> time;
			procResult = track.UpdateTrack(roverPoint,laserPoint,time,laserOrigin);
			myfile >> fileResult.AZ >> fileResult.EL;
			/*assert(abs(fileResult.AZ-procResult.AZ) < MAX_ERROR && 
				   abs(fileResult.EL-procResult.EL) < MAX_ERROR);*/
			printf("Iteration (%d)::Difference [X,Y]= [%f,%f]\n", ii,abs(fileResult.X-procResult.X),abs(fileResult.Y-procResult.Y));
		}
		myfile.close();
	}
}
// Calibration Unit Test
void TestCalibration(HANDLE hconsole)
{
	char files[4][256] = {{"TEST_CAL_1.txt"},
					      {"TEST_CAL_2.txt"},
	                      {"TEST_CAL_3.txt"},
					      {"TEST_CAL_4.txt"}};
	C3_TRACK_POINT_DOUBLE testPoints[4];
	C3_TRACK_POINT_DOUBLE procResult;
	C3_TRACK_POINT_DOUBLE fileResult;
	
	procResult.X = 0;
	procResult.Y = 0;

	double         MAX_ERROR      = 0.005;

	for (unsigned int jj = 0; jj < 4; jj++)
	{
		printf("TEST CALIBRATION FILE --- %d (%s)---\n",jj,files[jj]);
		
		ifstream myfile (files[jj]);
		if ( myfile.is_open() )
		{
			myfile >> testPoints[0].X >> testPoints[0].Y >>testPoints[1].X >> testPoints[1].Y >>testPoints[2].X >> testPoints[2].Y >>testPoints[3].X >> testPoints[3].Y >> fileResult.X >> fileResult.Y;
			procResult = calibrate(testPoints); //get laser data
			if (abs(fileResult.X-procResult.X) < MAX_ERROR && abs(fileResult.Y-procResult.Y) < MAX_ERROR)
			{
				SetConsoleTextAttribute(hconsole,FOREGROUND_GREEN);
				printf("PASS!!!!!!!::Difference [Xlaser,Ylaser]= [%f,%f]\n",abs(fileResult.X-procResult.X),abs(fileResult.Y-procResult.Y));
			}
			else
			{
				SetConsoleTextAttribute(hconsole,FOREGROUND_RED);
				printf("FAILED!!!!!::Difference [Xlaser,Ylaser]= [%f,%f]\n",abs(fileResult.X-procResult.X),abs(fileResult.Y-procResult.Y));		
			}
			SetConsoleTextAttribute(hconsole,FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
			
			myfile.close();
		}
	}
}
