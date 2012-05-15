///
///     File Name: VideoSharedMemory.h
///	   Programmer: Benjamin Kurt Heiner
///	 Date Created: 06/20/2007
/// Date Modified: --/--/----
///
///       Purpose: The folowin file contains all structures and classdes 
///                used for syncronized telemetry and video.
///
///
///     -Structures-
///
///   TelemetryInformation - Holds Mixed Telemetry Info
///	  UTCInformation       - UTC time for packet arrival
///   SharedMemoryStruct   - Contains structure of shared memory file
///	  Image Information    - Contains alll image information provided 
///                          to the client
///
///		- Classes -
///
///	  CSharedMemory - holds all creation and controls functions for 
///                   the shared memory application

#ifndef VIDEOSHAREDMEMORY_H
#define VIDEOSHAREDMEMORY_H

#include "../stdafx.h"

/// Total Number of Handles in shared memory app
/// handel 1: shared memory mutex (used in server and client)
/// handle 2: event from server (used)
/// handle 3: event from client (noty used)
#define HANDLE_COUNT 5

/// 1024w * 1024h * 3p
#define MAX_PIXEL_COUNT 3145728
#define SHM_TIME_OUT        5000L
#pragma pack(1)
typedef struct Image
{
	unsigned int  nChannels;     
	unsigned int  depth;         
	unsigned int  width;         
	unsigned int  height;        
	unsigned int  size;			 
	unsigned char imageData[MAX_PIXEL_COUNT];
}Image;

// Holds Mixed Telemetry Info
#pragma pack(1)		/// byte align
typedef struct TelemetryInformation
{
	float Roll;
	float Pitch;
	float TrueHeading;
	float GroundTrack;
	float Altitude;
	float GPSLat;
	float GPSLon;
	float GimbalAZ;
	float GimbalEL;
	float CHFoV;
	float Airspeed;
	unsigned short SynchStatus;	
}TelemetryInformation;

#pragma pack(1)		/// byte align
typedef struct UTCInformation
{
	unsigned char Year;
	unsigned char Month;
	unsigned char Day;
	unsigned char Hours;
	unsigned char Minutes;
	unsigned char Seconds; 
	unsigned int  Miliseconds;
}UTCInformation;

#pragma pack(1)		/// byte align
typedef struct SharedMemoryStruct
{
	/* Writing process ID */ 
	DWORD ProcessID;		
	/* Agent Number*/
	unsigned short agent;
	/* hold image information */
	Image image;
	/* UTC time of frame */
	UTCInformation UTCTime;
	/* UTC time of packet */
	UTCInformation UTCData;

	TelemetryInformation Telemetry;
}SharedMemoryStruct;


class CSharedMemory
{
public:
	// default constructor
	CSharedMemory();
	// default destructor
	~CSharedMemory();
	//
	BOOL CreateSharedMemory();
	void SetFileNames(CString MemName, CString MutexName1,CString MutexName2, CString Event1, CString Event2,CString Event3);
	// 
	BOOL IsServer(){return m_Server;}
	BOOL IsSharedMemCreated(){return m_Shared;}
	///
	/// Write Shared Mem
	///
	BOOL WriteImageToSharedMemory(LPCVOID imageData, 
		                          unsigned int size,			
								  unsigned short agent,			
								  unsigned int height    = 480, 
								  unsigned int width     = 640, 
								  unsigned int nChannels = 3, 
								  unsigned int depth     = 32);
	BOOL WriteImageToSharedMemory(BYTE *imageData, 
		                          unsigned int size, 
								  unsigned short agent,			
								  unsigned int height    = 480, 
								  unsigned int width     = 640, 
								  unsigned int nChannels = 3, 
								  unsigned int depth     = 32);
	BOOL CSharedMemory::WriteUTCdataToSharedMemory   (UTCInformation UTC,BOOL type = false);
	BOOL CSharedMemory::WriteTelemDataToSharedMemory (TelemetryInformation UTC);
	///
	/// Read Shared Mem
	///
	unsigned int    ReadFromSharedMemoryChannelInfo();
	unsigned short  ReadFromSharedMemoryAgentNum();
	unsigned int    ReadFromSharedMemoryDepthInfo();
	unsigned int    ReadFromSharedMemoryHeightInfo();
	unsigned int    ReadFromSharedMemoryWidthInfo();
	unsigned char*  ReadFromSharedMemoryDataInfo();
	UTCInformation* ReadFromSharedMemoryUTCInfo(BOOL type = FALSE);
	TelemetryInformation* ReadFromSharedMemoryTelemInfo();
	DWORD           ReadFromSharedMemoryProcessID();
	//communication events
	DWORD WaitForCommunicationEvents();
	DWORD WaitForCommunicationEventServerTelem();
	DWORD WaitForCommunicationEventServerVideo();
	DWORD WaitForCommunicationEventClient();
	DWORD WaitForCommunicationEventMutexTelem ();
	DWORD WaitForCommunicationEventMutexVideo ();
	//
	void ReleaseMutexServerTelem();
	void ReleaseMutexServerVideo();
	void SetEventClient();
	void SetEventServerTelem();
	void SetEventServerVideo();

private:
	// file names
	CString m_MemName;				// User provided Shared mem file name
    CString m_MutexName1;			// user provided mutex1 name (telem)
	CString m_MutexName2;			// user provided mutex2 name (video)
	CString m_Event1;				// user provided server event name
	CString m_Event2;				// user provided client event name
	CString m_Event3;				// user provided client event name
	// Shared Memory
	HANDLE				m_MapFile;	// handle to shared memory file
	SharedMemoryStruct *m_SMenPtr;	// pointer to shared memory
	DWORD				m_ProcessID;// Process ID

	//Mutexes events
	HANDLE m_MutexTelem;		// Mutex to synchronize Client Telem
	HANDLE m_MutexVideo;		// m_Comunicationm[1] = Client informs server
	HANDLE m_EventServerTelem;	// m_Comunicationm[2] = Server informs client telem packet
	HANDLE m_EventServerVideo;	// m_Comunicationm[3] = Server informs client video packet
	HANDLE m_EventClient;		// m_Comunicationm[4] = Mutex to synchronize Client Video

	//Flags
	BOOL   m_Client;				// determines client
	BOOL   m_Server;				// determines server
	//! Bool.
	/*! Represents status of shared memory creation */
	BOOL   m_Shared;				// 
};
#endif