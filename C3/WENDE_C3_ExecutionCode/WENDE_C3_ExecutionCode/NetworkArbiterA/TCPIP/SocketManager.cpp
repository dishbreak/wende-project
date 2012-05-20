//////////////////////////////////////////////////////////////////////
// SocketManager.cpp: implementation of the CSocketManager class.
// Modified By: Benjamin Kurt Heiner
//        Date: May 19 2012
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//Include directives
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <atlconv.h>
#include "SocketManager.h"
#include "ShmStructs.h"
#include <string>
#include "ArbiterSharedMemoryManager.h"
//////////////////////////////////////////////////////////////////////
// Setup std directives
//////////////////////////////////////////////////////////////////////
using std::string;
//////////////////////////////////////////////////////////////////////
// DEBUG TO GET FILE NAME
//////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSocketManager::CSocketManager()
: m_pMsgCtrl(NULL), m_MsgType(C3PacketType::C3_CAMERA_STATUS)
{
}

//////////////////////////////////////////////////////////////////////
// Destructor
//////////////////////////////////////////////////////////////////////
CSocketManager::~CSocketManager()
{	
	// makes sure that the indata is freed
	if (inData != NULL)
	{
		// free the data
		delete inData;
	}
}
//////////////////////////////////////////////////////////////////////
// Destructor
//////////////////////////////////////////////////////////////////////
void CSocketManager::DisplayData(const LPBYTE lpData, DWORD dwCount, const SockAddrIn& sfrom)
{
	// TODO FIX THIS
	char *inData = new char[dwCount];
	// correct the string to proper format
	#ifndef UNICODE
	USES_CONVERSION;
	memcpy(inData, A2CT((LPSTR)lpData), dwCount);
	#else
	MultiByteToWideChar(CP_ACP, 0, reinterpret_cast<LPCSTR>(lpData), dwCount, inData, dwCount+1 );
	#endif
	// process the message
	AppendMessage( inData, dwCount );
	// clean up memory
	delete inData;
}
//////////////////////////////////////////////////////////////////////
// Function: AppendMessage
//  purpose: Primary location for parsing the messages that come over
//           the network. Calls the shm singleton to put data into the 
//           shm and if valid attempts to display the message to the 
//           screen (only in debug mode).
//////////////////////////////////////////////////////////////////////
void CSocketManager::AppendMessage(LPCTSTR strText,DWORD size )
{
	// temporary display string
	static char temp [1024];
	// process the data according to the connection type
	switch(m_MsgType)
	{
		case C3PacketType::C3_CAMERA_STATUS: 
		{ 
			CArbiterSharedMemoryManager::Instance().DecodeCameraStatusMessage(strText,temp,size); 
			break; 
		}
		case C3PacketType::C3_CAMERA_TRACK:  
		{ 
			CArbiterSharedMemoryManager::Instance().DecodeCameraTrackMessage(strText,temp,size); 
			break; 
		}
		case C3PacketType::C3_CAMERA_IMAGE:  
		{ 
			CString loadName(CArbiterSharedMemoryManager::Instance().DecodeCameraImageMessage(strText,temp,size).c_str()); 
			m_picCtrl->Load(loadName);
			break; 
		}
		case C3PacketType::C3_LASER_STATUS:
		{
			CArbiterSharedMemoryManager::Instance().DecodeLaserStatusMessage(strText,temp,size); 
			break;
		}
		default: 
		{ 
			break; 
		}
	}
	// make sure that the control handle is valid before we attempt to display
	if (NULL == m_pMsgCtrl)
	{
		return;
	}
	// display to the debug screen
	HWND hWnd = m_pMsgCtrl->GetSafeHwnd();
	DWORD dwResult = 0;
	if (SendMessageTimeout(hWnd, WM_GETTEXTLENGTH, 0, 0, SMTO_NORMAL, 1000L, &dwResult) != 0)
	{
		int nLen = (int) dwResult;
		if (SendMessageTimeout(hWnd, EM_SETSEL, nLen, nLen, SMTO_NORMAL, 1000L, &dwResult) != 0)
		{
			if (SendMessageTimeout(hWnd, EM_REPLACESEL, FALSE, (LPARAM)temp, SMTO_NORMAL, 1000L, &dwResult) != 0)
			{
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////
// Function: SetCameraMessageType
//  purpose: Sets the message type for this socket manager
//////////////////////////////////////////////////////////////////////
void CSocketManager::SetCameraMessageType(C3PacketType type)
{
	m_MsgType = type;
}
//////////////////////////////////////////////////////////////////////
// Function: SetMessageWindow
//  purpose: Sets the message window for this socket
//////////////////////////////////////////////////////////////////////
void CSocketManager::SetMessageWindow(CEdit* pMsgCtrl)
{
	m_pMsgCtrl = pMsgCtrl;
}
//////////////////////////////////////////////////////////////////////
// Function: SetMessageWindow
//  purpose: Sets the picture window for the IM message
//////////////////////////////////////////////////////////////////////
void CSocketManager::SetPictureWindow(CPictureCtrl* picCtrl)
{
	m_picCtrl = picCtrl;
}
//////////////////////////////////////////////////////////////////////
// Function: AppendMessage
//  purpose: Process the receive messages
//////////////////////////////////////////////////////////////////////
void CSocketManager::OnDataReceived(const LPBYTE lpBuffer, DWORD dwCount)
{
	LPBYTE lpData = lpBuffer;
	SockAddrIn origAddr;
	stMessageProxy msgProxy;
	if (IsSmartAddressing())
	{
		dwCount = __min(sizeof(msgProxy), dwCount);
		memcpy(&msgProxy, lpBuffer, dwCount);
		origAddr = msgProxy.address;
		if (IsServer())
		{
			// broadcast message to all
			msgProxy.address.sin_addr.s_addr = htonl(INADDR_BROADCAST);
			WriteComm((const LPBYTE)&msgProxy, dwCount, 0L);
		}
		dwCount -= sizeof(msgProxy.address);
		lpData = msgProxy.byData;
	}

	// Display data to message list
	DisplayData( lpData, dwCount, origAddr );
}

///////////////////////////////////////////////////////////////////////////////
// OnEvent
// Send message to parent window to indicate connection status
///////////////////////////////////////////////////////////////////////////////
void CSocketManager::OnEvent(UINT uEvent, LPVOID lpvData)
{
	if (NULL == m_pMsgCtrl)
		return;

	CWnd* pParent = m_pMsgCtrl->GetParent();
	if (!::IsWindow( pParent->GetSafeHwnd()))
		return;
	string tempString; 
	switch( uEvent )
	{
		case EVT_CONSUCCESS:
			tempString = "Connection Established\r\n";
			AppendMessage( _T(tempString.c_str()), tempString.size());
			break;
		case EVT_CONFAILURE:
			tempString = "Connection Failed\r\n";
			AppendMessage( _T(tempString.c_str()), tempString.size());
			break;
		case EVT_CONDROP:
			tempString = "Connection Abandonned\r\n";
			AppendMessage( _T(tempString.c_str()), tempString.size());
			break;
		case EVT_ZEROLENGTH:
			tempString = "Zero Length Message\r\n";
			AppendMessage( _T(tempString.c_str()), tempString.size());
			break;
		default:
			tempString = "Unknown Socket event\n";
			TRACE(tempString.c_str());
			break;
	}
	pParent->PostMessage( WM_UPDATE_CONNECTION, uEvent, (LPARAM) this);
}
