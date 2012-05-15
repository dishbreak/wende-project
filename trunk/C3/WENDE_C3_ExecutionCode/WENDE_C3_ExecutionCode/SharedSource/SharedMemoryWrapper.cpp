#include "StdAfx.h"
#include "SharedMemoryWrapper.h"
//! Default Constructor (canonical Function)
/*!
*/
CSharedMemory::CSharedMemory():m_MapFile(NULL),m_Server(FALSE),m_Shared(FALSE)
{
	m_ProcessID		= ::GetCurrentProcessId();
	m_MemName		= "ApplicationSpecificSharedMem";
	m_MutexName1	= "ApplicationSpecificFree1";
	m_MutexName2	= "ApplicationSpecificFree2";
	m_Event1		= "MyFileEventObject1";
	m_Event2		= "MyFileEventObject2";
	m_Event3		= "MyFileEventObject3";
}
//! SetFileNames
//  Purpose 
/*!
*/
void CSharedMemory::SetFileNames(CString MemName, CString MutexName1, CString MutexName2, CString Event1, CString Event2, CString Event3 )
{
	m_MemName		= MemName;
	m_MutexName1	= MutexName1;
	m_MutexName2	= MutexName2;
	m_Event1		= Event1;
	m_Event2		= Event2;
	m_Event3		= Event3;
}
//! default destructor (canonical Function)
/*!
*/
CSharedMemory::~CSharedMemory()
{
	UnmapViewOfFile(m_SMenPtr);

	try
	{
		if (m_MutexTelem)
			CloseHandle(m_MutexTelem);
		if (m_MutexVideo)
			CloseHandle(m_MutexVideo);
		if (m_EventServerTelem)
			CloseHandle(m_EventServerTelem);
		if (m_EventServerVideo)
			CloseHandle(m_EventServerVideo);
		if (m_EventClient)
			CloseHandle(m_EventClient);

	if (m_MapFile != NULL)
		CloseHandle(m_MapFile);
	}
	catch(...)
	{
		TRACE("UNABLE TO DELETE ITEMS");
	}
}
//! Instantiates Shared Memory
/*!
This function is used to create the shared memory space. The function is used for both client and server applications
*/
BOOL CSharedMemory::CreateSharedMemory()
{
	/*Check and see if already exists*/
	if (m_MapFile != NULL)
	{
		return FALSE;
	}

	// Try to create file mapping object (assume that this is the server)
	m_MapFile = ::CreateFileMapping((HANDLE)0xFFFFFFFF, NULL,PAGE_READWRITE,0,sizeof(SharedMemoryStruct),(LPCSTR)m_MemName);
	if (GetLastError() == ERROR_ALREADY_EXISTS) 
	{
		m_MapFile = ::OpenFileMapping(FILE_MAP_ALL_ACCESS,FALSE,(LPCSTR)m_MemName);
	} 
	else 
	{
		m_Server = TRUE;
	}

	/*Set read write access to memory*/
	m_SMenPtr = (SharedMemoryStruct*)::MapViewOfFile(m_MapFile,FILE_MAP_ALL_ACCESS,0,0,sizeof(SharedMemoryStruct));

	if (m_Server)
	{
		m_SMenPtr->ProcessID=m_ProcessID;
	}

	/*Create communication events*/
	m_MutexTelem		= ::CreateMutex(NULL,FALSE,      (LPCSTR)m_MutexName1);
	m_MutexVideo		= ::CreateMutex(NULL,FALSE,      (LPCSTR)m_MutexName2);
	m_EventServerTelem  = ::CreateEvent(NULL,FALSE,FALSE,(LPCSTR)m_Event1);
	m_EventServerVideo  = ::CreateEvent(NULL,FALSE,FALSE,(LPCSTR)m_Event2);
	m_EventClient		= ::CreateEvent(NULL,FALSE,FALSE,(LPCSTR)m_Event3);
	
	m_Shared = TRUE;

	return TRUE;
}
// Writes to memory
BOOL CSharedMemory::WriteImageToSharedMemory(BYTE *imageData, unsigned int size, unsigned short agent, unsigned int height, unsigned int width, unsigned int nChannels, unsigned int depth)
{
	m_SMenPtr->image.nChannels=nChannels;
	m_SMenPtr->image.depth    =depth; 
	m_SMenPtr->image.width    =width;
	m_SMenPtr->image.height   =height;
	m_SMenPtr->image.size     =size;
	m_SMenPtr->agent = agent;
	if (size < MAX_PIXEL_COUNT)
		memcpy(m_SMenPtr->image.imageData,imageData,size);
	else
		memcpy(m_SMenPtr->image.imageData,imageData,MAX_PIXEL_COUNT);	
	return true;
}
BOOL CSharedMemory::WriteTelemDataToSharedMemory (TelemetryInformation telemetry)
{
	memcpy(&m_SMenPtr->Telemetry, &telemetry,sizeof(TelemetryInformation));
	return true;
}
BOOL CSharedMemory::WriteUTCdataToSharedMemory (UTCInformation UTC, BOOL type)
{
	if (type == TRUE)
        memcpy(&m_SMenPtr->UTCData, &UTC,sizeof(UTCInformation));
	else 
		memcpy(&m_SMenPtr->UTCTime, &UTC,sizeof(UTCInformation));
	return true;
}
DWORD CSharedMemory::WaitForCommunicationEvents()
{
	HANDLE handles[HANDLE_COUNT];

	handles[0] = m_MutexTelem;
	handles[1] = m_MutexVideo;
	handles[2] = m_EventServerTelem;
	handles[3] = m_EventServerVideo;
	handles[4] = m_EventClient;

	return WaitForMultipleObjects(HANDLE_COUNT,			// number of objects in array
								    handles,				// array of objects
								    FALSE,					// wait for any
								    INFINITE);		// indefinite wait
}
DWORD CSharedMemory::ReadFromSharedMemoryProcessID()
{
	return m_SMenPtr->ProcessID;
}
unsigned int CSharedMemory::ReadFromSharedMemoryChannelInfo()
{
	return m_SMenPtr->image.nChannels;
}
unsigned int CSharedMemory::ReadFromSharedMemoryDepthInfo()
{
	return m_SMenPtr->image.depth;
}
unsigned int CSharedMemory::ReadFromSharedMemoryHeightInfo()
{
	return m_SMenPtr->image.height;
}
unsigned int CSharedMemory::ReadFromSharedMemoryWidthInfo()
{
	return m_SMenPtr->image.width;
}
unsigned char* CSharedMemory::ReadFromSharedMemoryDataInfo()
{
	return (unsigned char*)m_SMenPtr->image.imageData;
}
///
DWORD CSharedMemory::WaitForCommunicationEventServerTelem()
{
	DWORD result = WaitForSingleObject(m_EventServerTelem,SHM_TIME_OUT);
	return result;
}
DWORD CSharedMemory::WaitForCommunicationEventServerVideo()
{
	DWORD result = WaitForSingleObject(m_EventServerVideo,SHM_TIME_OUT);
	return result;
}
DWORD CSharedMemory::WaitForCommunicationEventClient()
{
	DWORD result = WaitForSingleObject(m_EventClient,SHM_TIME_OUT);
	return result;
}
DWORD CSharedMemory::WaitForCommunicationEventMutexVideo()
{
	DWORD result = WaitForSingleObject(m_MutexVideo,SHM_TIME_OUT);
	return result;
}
DWORD CSharedMemory::WaitForCommunicationEventMutexTelem()
{
	DWORD result = WaitForSingleObject(m_MutexTelem,SHM_TIME_OUT);
	return result;
}
///
UTCInformation* CSharedMemory::ReadFromSharedMemoryUTCInfo(BOOL type)
{
	if (type == TRUE)
        return &m_SMenPtr->UTCData;
	else
		return &m_SMenPtr->UTCTime;
}
TelemetryInformation* CSharedMemory::ReadFromSharedMemoryTelemInfo()
{
	return &m_SMenPtr->Telemetry;
}
unsigned short CSharedMemory::ReadFromSharedMemoryAgentNum()
{
	return m_SMenPtr->agent;
}
///
void CSharedMemory::ReleaseMutexServerVideo()
{
	::ReleaseMutex(m_MutexVideo);
}
void CSharedMemory::ReleaseMutexServerTelem()
{
	::ReleaseMutex(m_MutexTelem);
}
///
void CSharedMemory::SetEventServerTelem()
{
	::SetEvent(m_EventServerTelem);
}
void CSharedMemory::SetEventServerVideo()
{
	::SetEvent(m_EventServerVideo);
}
void CSharedMemory::SetEventClient()
{
	::SetEvent(m_EventClient);
}