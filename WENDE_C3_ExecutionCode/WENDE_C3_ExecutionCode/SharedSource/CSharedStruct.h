//
///     File Name: CSharedStruct.h (based on my VideoSharedMemory.h and template example from web)
///	   Programmer: Benjamin Kurt Heiner
///	 Date Created: 06/20/2007
/// Date Modified: --/--/----
///
///       Purpose: The folowin file contains all structures and classdes 
///                used for syncronized data messages.
///
///
///     -Structures-
///
///		User defined via templates
///
///		- Classes -
///
///	  CSharedMemory - holds all creation and controls functions for 
///                   the shared memory application

//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CSharedStruct_H__86467BA6_5AFA_11D3_863D_00A0244A9CA7__INCLUDED_)
#define AFX_CSharedStruct_H__86467BA6_5AFA_11D3_863D_00A0244A9CA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Aclapi.h"
#include <string>

using std::string;

#define SHM_TIME_OUT        5000L

template <class StructType>
class CSharedStruct
{
private:
	// file names
	string		m_hSharedName;			// User provided Shared mem file name	
	string		m_MutexName;			// user provided mutex1 name (Struct Data)
	string		m_Event1;				// user provided server event name
	string		m_Event2;				// user provided client event name

	// Shared Memory
	HANDLE		m_hFileMapping;			// Handle to shared memory file
	StructType *m_pvData;				// Pointer to shared memory
	DWORD		m_dwMaxDataSize;		// Size of shared memory struct
	DWORD		m_ProcessID;			// Process ID

	//Mutexes events
	HANDLE		m_MutexStruct;			// Mutex to synchronize Client 
	HANDLE		m_EventServer;			// Server informs client Avaliable Date
	HANDLE		m_EventClient;			// Client informs server Data Read
	
	//Flags
	BOOL		m_Server;				// determines server
	BOOL		m_bCreated;				// determines created 

public:
	// Defualt Constructor (cononical)
	CSharedStruct();
	// Overide constructor (Should use)
	CSharedStruct(string memName,
				  string mutexName,
				  string eventClientName,
				  string eventServerName);
	// Default Constructor
	~CSharedStruct();
	// Releases all shm data and frees handles
	VOID Release();
	// starts shm, mutex, and events
	BOOL Acquire(string memName,
				 string mutexName,
				 string eventClientName,
				 string eventServerName);
	// Allow access to shared memory data
	StructType *operator->();

	BOOL SetEventServer();
	BOOL SetEventClient();
	BOOL ReleaseMutex();

	DWORD WaitForCommunicationEventServer();
	DWORD WaitForCommunicationEventClient();
	DWORD WaitForCommunicationEventMutex();

	BOOL isCreated();
	BOOL isServer();

	DWORD GetProcessID();
	StructType* GetStruct()const;
};


template <class StructType>
StructType *CSharedStruct<StructType>::operator->()
{
	return m_pvData;
}

// FUNCTION NAME: CSharedStruct
//       PURPOSE: default constructor for shared memory application
//
//          NOTE: Sould not use... but can
template <class StructType>
CSharedStruct<StructType>::CSharedStruct()	// DEFAULT CONSTRUCTOR (CONONICAL)
:m_hFileMapping(NULL),						// SET MAP HANDLE TO NULL
 m_bCreated(FALSE),							// SET CREATED STATUS TO FALSE
 m_Server(FALSE),							// SET SERVER STATUS TO FALSE
 m_MutexStruct(NULL),						// SET MUTEX HANDLE TO NULL
 m_EventServer(NULL),						// SET EVENT HANDLE TO NULL
 m_EventClient(NULL)						// SET EVENT HANDLE TO NULL
{
	m_hFileMapping  = NULL;
	m_ProcessID		= ::GetCurrentProcessId();
	m_hSharedName   = "ApplicationSpecificSharedMem";
	m_MutexName     = "ApplicationSpecificMutex";
	m_Event1        = "MyFileEventObject1";
	m_Event2        = "MyFileEventObject2";
}
// FUNCTION NAME: CSharedStruct
//       PURPOSE: default destructor for shared memory application
template <class StructType>
CSharedStruct<StructType>::~CSharedStruct()
{
	// clean up handles and shared memory
	Release();
}
// FUNCTION NAME: CSharedStruct
//       PURPOSE: overide constructor for shared memory application
//
//    Parameters: 
//		char* memName
//		char* mutexName 
//		char* eventClientName
//		char* eventServerName
//       
//    NOTE: Sould be used!!!!
template <class StructType>
CSharedStruct<StructType>::CSharedStruct(string memName,
										 string mutexName,
										 string eventClientName,
										 string eventServerName)
{
	// get process id
	m_ProcessID		= ::GetCurrentProcessId();
	// Call defualt constructor for init
	CSharedStruct();
	// Setup shared memory
	Acquire(memName,
		    mutexName,
		    eventClientName,
		    eventServerName);
}
// FUNCTION NAME: CSharedStruct
//       PURPOSE: unmapsthe shm and releases events, shm, and mutex
template <class StructType>
VOID CSharedStruct<StructType>::Release()
{
	// unmap the data
	if (m_pvData != NULL)
	{
		UnmapViewOfFile(m_pvData);
		m_pvData = NULL;
	}
	// Clean up the file handle
	if (m_hFileMapping != NULL)
	{
		CloseHandle(m_hFileMapping);
	}
	// Clean up the mutex handle
	if (m_MutexStruct != NULL)
	{
		CloseHandle(m_MutexStruct);
	}
	// Clean up the server handle
	if (m_EventServer != NULL)
	{
		CloseHandle(m_EventServer);
	}
	// Clean up the client handle
	if (m_EventClient != NULL)
	{
		CloseHandle(m_EventClient);
	}	
}
// FUNCTION NAME: CSharedStruct
//       PURPOSE: Sets up shared mem, maps mem, creates mutex, creates events
template <class StructType>
BOOL CSharedStruct<StructType>::Acquire(string memName,
										string mutexName,
										string eventClientName,
										string eventServerName)
{
	//Check and see if already exists
	if (m_hFileMapping != NULL)
	{
		return FALSE;
	}

	// Try to create file mapping object (assume that this is the server)
	m_dwMaxDataSize = 0;
	m_hFileMapping = CreateFileMapping (INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(StructType), memName.c_str());

	// OK... so we could not create the shm
	// so attempt to connect to the shm
	int err = GetLastError();
	if (m_hFileMapping == NULL)
	{
		return FALSE;
	}
		
	// attach to mem
	if (err == ERROR_ALREADY_EXISTS) 
	{
		m_hFileMapping = ::OpenFileMapping(FILE_MAP_ALL_ACCESS,FALSE,(LPCSTR)memName.c_str());
	} 
	// set server flag as this is the process that created the shm
	else 
	{
		m_Server = TRUE;
	}

	// get the size of the shm
	m_dwMaxDataSize = sizeof(StructType);

	// copy over the handle names
	m_hSharedName = memName;
	m_MutexName   = mutexName;
	m_Event1      = eventClientName;
	m_Event2      = eventServerName;
	
	// Map the shared mem
	m_pvData = (StructType *) MapViewOfFile( m_hFileMapping, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);

	//  verify that the map of the struct worked
	if (m_pvData == NULL)
	{
		CloseHandle(m_hFileMapping);
		return FALSE;
	}

	/*Create communication events*/
	m_MutexStruct	= ::CreateMutex(NULL,FALSE,      (LPCSTR)m_MutexName.c_str());
	m_EventServer	= ::CreateEvent(NULL,FALSE,FALSE,(LPCSTR)m_Event1.c_str());
	m_EventClient	= ::CreateEvent(NULL,FALSE,FALSE,(LPCSTR)m_Event2.c_str());

	// set to created
	m_bCreated = true;

	return TRUE;
}
template <class StructType>
BOOL CSharedStruct<StructType>::SetEventServer()
{
	BOOL success = ::SetEvent(m_EventServer);
	
	if (success == 0)
	{
		int err = GetLastError();
		//TODO ADD DEBUG
	}

	return success;
}
template <class StructType>
BOOL CSharedStruct<StructType>::SetEventClient()
{
	BOOL success = ::SetEvent(m_EventClient);

	if (success == 0)
	{
		int err = GetLastError();
		//TODO ADD DEBUG
	}

	return success;
}
template <class StructType>
BOOL CSharedStruct<StructType>::ReleaseMutex()
{
	BOOL success = ::ReleaseMutex(m_MutexStruct);
	
	if (success == 0)
	{
		int err = GetLastError();
		//TODO ADD DEBUG
	}

	return success;
}
template <class StructType>
DWORD CSharedStruct<StructType>::WaitForCommunicationEventServer()
{
	DWORD result = WaitForSingleObject(m_EventServer,SHM_TIME_OUT);
	
	switch (result)
	{
		case WAIT_ABANDONED:
			{
				break;
			}
		case WAIT_OBJECT_0:
			{
				break;
			}
		case WAIT_TIMEOUT:
			{
				break;
			}
		case WAIT_FAILED:
			{
				break;
			}
		default:
			{
				break;
			}
	}
	return result;
}
template <class StructType>
DWORD CSharedStruct<StructType>::WaitForCommunicationEventClient()
{
	DWORD result = WaitForSingleObject(m_EventClient,SHM_TIME_OUT);
	
	switch (result)
	{
		case WAIT_ABANDONED:
			{
				break;
			}
		case WAIT_OBJECT_0:
			{
				break;
			}
		case WAIT_TIMEOUT:
			{
				break;
			}
		case WAIT_FAILED:
			{
				break;
			}
		default:
			{
				break;
			}
	}
	return result;
}
template <class StructType>
DWORD CSharedStruct<StructType>::WaitForCommunicationEventMutex()
{
	DWORD result = WaitForSingleObject(m_MutexStruct,SHM_TIME_OUT);

	switch (result)
	{
		case WAIT_ABANDONED:
			{
				break;
			}
		case WAIT_OBJECT_0:
			{
				break;
			}
		case WAIT_TIMEOUT:
			{
				break;
			}
		case WAIT_FAILED:
			{
				break;
			}
		default:
			{
				break;
			}
	}
	return result;
}
template <class StructType>
BOOL CSharedStruct<StructType>::isCreated()
{
	return this->m_bCreated;
}
template <class StructType>
BOOL CSharedStruct<StructType>::isServer()
{
	return this->m_Server;
}
template <class StructType>
DWORD CSharedStruct<StructType>::GetProcessID()
{
	return this->m_ProcessID;
}
template <class StructType>
StructType* CSharedStruct<StructType>::GetStruct()const
{
	return m_pvData;
}
#endif