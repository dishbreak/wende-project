//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CSharedStruct_H__86467BA6_5AFA_11D3_863D_00A0244A9CA7__INCLUDED_)
#define AFX_CSharedStruct_H__86467BA6_5AFA_11D3_863D_00A0244A9CA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Aclapi.h"

template <class StructType>
class CSharedStruct
{
private:
	// file names
	char m_hSharedName[MAX_PATH];	// User provided Shared mem file name	
	char m_MutexName[MAX_PATH];		// user provided mutex1 name (Struct Data)
	char m_Event1[MAX_PATH];		// user provided server event name
	char m_Event2[MAX_PATH];		// user provided client event name
	// Shared Memory
	HANDLE		m_hFileMapping;		// Handle to shared memory file
	StructType *m_pvData;			// Pointer to shared memory
	DWORD		m_dwMaxDataSize;	// Size of shared memory struct
	DWORD		m_ProcessID;		// Process ID
	//Mutexes events
	HANDLE m_MutexStruct;			// Mutex to synchronize Client 
	HANDLE m_EventServer;			// Server informs client Avaliable Date
	HANDLE m_EventClient;			// Client informs server Data Read
	//Flags
	BOOL   m_Server;				// determines server
	BOOL   m_bCreated;				// determines created 

public:
	// Defualt Constructor (cononical)
	CSharedStruct();
	// Overide constructor (Should use)
	CSharedStruct(char *memName,
				  char *mutexName,
				  char *eventClientName,
				  char *eventServerName);
	// Default Constructor
	~CSharedStruct();
	// Releases all shm data and frees handles
	VOID Release();
	// starts shm, mutex, and events
	BOOL Acquire(char *memName,
				 char *mutexName,
				 char *eventClientName,
				 char *eventServerName);

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
	strcpy(m_hSharedName,"ApplicationSpecificSharedMem");
	strcpy(m_MutexName  ,"ApplicationSpecificMutex");
	strcpy(m_Event1     ,"MyFileEventObject1");
	strcpy(m_Event2     ,"MyFileEventObject2");
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
CSharedStruct<StructType>::CSharedStruct(char *memName,
										 char* mutexName,
										 char* eventClientName,
										 char* eventServerName)
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
BOOL CSharedStruct<StructType>::Acquire(char *memName,
										char *mutexName,
										char *eventClientName,
										char *eventServerName)
{
	//Check and see if already exists
	if (m_hFileMapping != NULL)
	{
		return FALSE;
	}

	// Try to create file mapping object (assume that this is the server)
	m_dwMaxDataSize = 0;
	m_hFileMapping = CreateFileMapping (INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(StructType), memName);

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
		m_hFileMapping = ::OpenFileMapping(FILE_MAP_ALL_ACCESS,FALSE,(LPCSTR)memName);
	} 
	// set server flag as this is the process that created the shm
	else 
	{
		m_Server = TRUE;
	}

	// --- BKH --- This is only pre-existing comment
	// --- BKH --- not sure on why use 
	// OK, if we're the first person to create this
	// file mapping object
	// we want to clear the ACL, so
	// anyone else can access this object
	//
	// If we don't do this, and a service creates the file mapping
	// User processes won't be able to access it.

	// So, we set the DACL to NULL, which effectively
	// grants Everyone All Access

	// More complicated ACLs are left as an exercise for the reader
	// (because I sure as hell can't figure them out!)
	SetNamedSecurityInfo(memName, SE_KERNEL_OBJECT, DACL_SECURITY_INFORMATION, 0, 0, (PACL) NULL, NULL);

	// get the size of the shm
	m_dwMaxDataSize = sizeof(StructType);

	// copy over the handle names
	strncpy(m_hSharedName, memName        , MAX_PATH - 1);
	strncpy(m_MutexName  , mutexName      , MAX_PATH - 1);
	strncpy(m_Event1     , eventClientName, MAX_PATH - 1);
	strncpy(m_Event2     , eventServerName, MAX_PATH - 1);
	
	// Map the shared mem
	m_pvData = (StructType *) MapViewOfFile( m_hFileMapping, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);

	//  verify that the map of the struct worked
	if (m_pvData == NULL)
	{
		CloseHandle(m_hFileMapping);
		return FALSE;
	}

	/*Create communication events*/
	m_MutexStruct	= ::CreateMutex(NULL,FALSE,      (LPCSTR)m_MutexName);
	m_EventServer	= ::CreateEvent(NULL,FALSE,FALSE,(LPCSTR)m_Event1);
	m_EventClient	= ::CreateEvent(NULL,FALSE,FALSE,(LPCSTR)m_Event2);

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
	DWORD result = WaitForSingleObject(m_EventServer,5000L);
	return result;
}
template <class StructType>
DWORD CSharedStruct<StructType>::WaitForCommunicationEventClient()
{
	DWORD result = WaitForSingleObject(m_EventClient,5000L);
	return result;
}
template <class StructType>
DWORD CSharedStruct<StructType>::WaitForCommunicationEventMutex()
{
	DWORD result = WaitForSingleObject(m_MutexStruct,5000L);
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

#endif