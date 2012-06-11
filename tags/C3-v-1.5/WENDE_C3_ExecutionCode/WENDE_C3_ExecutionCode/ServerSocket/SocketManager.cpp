// SocketManager.cpp: implementation of the CSocketManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <atlconv.h>
#include "ServerSocket.h"
#include "SocketManager.h"
#include "LaserCommand.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/*
const UINT EVT_CONSUCCESS = 0x0000;	// Connection established
const UINT EVT_CONFAILURE = 0x0001;	// General failure - Wait Connection failed
const UINT EVT_CONDROP	  = 0x0002;	// Connection dropped
const UINT EVT_ZEROLENGTH = 0x0003;	// Zero length message
*/

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSocketManager::CSocketManager()
: m_pMsgCtrl(NULL)
{

}

CSocketManager::~CSocketManager()
{

}


void CSocketManager::DisplayData(const LPBYTE lpData, DWORD dwCount, const SockAddrIn& sfrom,__int64 startTime)
{
	CString strData;
#ifndef UNICODE
	USES_CONVERSION;
	memcpy(strData.GetBuffer(dwCount), A2CT((LPSTR)lpData), dwCount);
	strData.ReleaseBuffer(dwCount);
#else
	MultiByteToWideChar(CP_ACP, 0, reinterpret_cast<LPCSTR>(lpData), dwCount, strData.GetBuffer(dwCount+1), dwCount+1 );
	strData.ReleaseBuffer(dwCount);
#endif
	// variables
	char temp[512];
	static int  laserCommandMessageCount = 0;

	CLaserCommand command((BYTE*)A2CT((LPSTR)lpData));

	// debug string for display 
	sprintf(temp, "+LASER COMMAND MESSAGE(%d)\r\n", ++laserCommandMessageCount);
	sprintf(temp, "%s|->isLaserOn = %d\r\n", temp,command.LaserStatus.isLaserOn);
	sprintf(temp, "%s|->AZ        = %d\r\n", temp,command.LaserStatus.PWM_AZ);
	sprintf(temp, "%s|->EL        = %d\r\n", temp,command.LaserStatus.PWM_EL);
	sprintf(temp, "%s\r\n\r\n", temp);


	AppendMessage( temp );
}


void CSocketManager::AppendMessage(LPCTSTR strText )
{
	if (NULL == m_pMsgCtrl)
		return;
/*
	if (::IsWindow( m_pMsgCtrl->GetSafeHwnd() ))
	{
		int nLen = m_pMsgCtrl->GetWindowTextLength();
		m_pMsgCtrl->SetSel(nLen, nLen);
		m_pMsgCtrl->ReplaceSel( strText );
	}
*/
	HWND hWnd = m_pMsgCtrl->GetSafeHwnd();
	DWORD dwResult = 0;
	if (SendMessageTimeout(hWnd, WM_GETTEXTLENGTH, 0, 0, SMTO_NORMAL, 1000L, &dwResult) != 0)
	{
		int nLen = (int) dwResult;
		if (SendMessageTimeout(hWnd, EM_SETSEL, nLen, nLen, SMTO_NORMAL, 1000L, &dwResult) != 0)
		{
			if (SendMessageTimeout(hWnd, EM_REPLACESEL, FALSE, (LPARAM)strText, SMTO_NORMAL, 1000L, &dwResult) != 0)
			{
			}
		}

	}
}


void CSocketManager::SetMessageWindow(CEdit* pMsgCtrl)
{
	m_pMsgCtrl = pMsgCtrl;
}


void CSocketManager::OnDataReceived(const LPBYTE lpBuffer, DWORD dwCount,__int64 startTime)
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
	DisplayData( lpData, dwCount, origAddr, startTime );
}

///////////////////////////////////////////////////////////////////////////////
// OnEvent
// Send message to parent window to indicate connection status
void CSocketManager::OnEvent(UINT uEvent, LPVOID lpvData)
{
	if (NULL == m_pMsgCtrl)
		return;

	CWnd* pParent = m_pMsgCtrl->GetParent();
	if (!::IsWindow( pParent->GetSafeHwnd()))
		return;

	switch( uEvent )
	{
		case EVT_CONSUCCESS:
			AppendMessage( _T("Connection Established\r\n") );
			break;
		case EVT_CONFAILURE:
			AppendMessage( _T("Connection Failed\r\n") );
			break;
		case EVT_CONDROP:
			AppendMessage( _T("Connection Abandonned\r\n") );
			break;
		case EVT_ZEROLENGTH:
			AppendMessage( _T("Zero Length Message\r\n") );
			break;
		default:
			TRACE("Unknown Socket event\n");
			break;
	}

	pParent->PostMessage( WM_UPDATE_CONNECTION, uEvent, (LPARAM) this);

}
