// SocketManager.cpp: implementation of the CSocketManager class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <atlconv.h>
//#include <Streams.h>
//#include "ServerSocket.h"
#include "SocketManager.h"

#include "cameraMsgs.pb.h"
#include <string>

using namespace cameraMsgs;
using std::string;

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
: m_pMsgCtrl(NULL), m_cameraMsgType(CameraPacketType::status)
{
	m_LaserCommand.Acquire("SHM_C3_LASER_STATUS",
						   "SHM_C3_LASER_STATUS_MUTEX",
						   "SHM_C3_LASER_STATUS_EVENT1",
						   "SHM_C3_LASER_STATUS_EVENT2");
	m_CameraStatus.Acquire("SHM_C3_CAMERA_STAUTS",
						   "SHM_C3_CAMERA_STATUS_MUTEX",
						   "SHM_C3_CAMERA_STATUS_EVENT1",
						   "SHM_C3_CAMERA_STATUS_EVENT2");
	m_CameraTracks.Acquire("SHM_C3_CAMERA_TRACK",
						   "SHM_C3_CAMERA_TRACK_MUTEX",
						   "SHM_C3_CAMERA_TRACK_EVENT1",
						   "SHM_C3_CAMERA_TRACK_EVENT2");
	m_CameraImage.Acquire( "SHM_C3_CAMERA_IMAGE",
						   "SHM_C3_CAMERA_IMAGE_MUTEX",
						   "SHM_C3_CAMERA_IMAGE_EVENT1",
						   "SHM_C3_CAMERA_IMAGE_EVENT2");
}

CSocketManager::~CSocketManager()
{

}


void CSocketManager::DisplayData(const LPBYTE lpData, DWORD dwCount, const SockAddrIn& sfrom)
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
	if (!sfrom.IsNull())
	{
		LONG  uAddr = sfrom.GetIPAddr();
		BYTE* sAddr = (BYTE*) &uAddr;
		int nPort = ntohs( sfrom.GetPort() ); // show port in host format...
		CString strAddr;
		// Address is stored in network format...
		strAddr.Format(_T("%u.%u.%u.%u (%d)>"),
					(UINT)(sAddr[0]), (UINT)(sAddr[1]),
					(UINT)(sAddr[2]), (UINT)(sAddr[3]), nPort);

		strData = strAddr + strData;
	}
	AppendMessage( strData );
}

void CSocketManager::DecodeCameraStatusMessage(LPCTSTR strText, char* temp)
{
	static char timeStr[256];
	static int cameraStatusMessageCount = 0;
	string s((LPCTSTR)strText);
	cameraStatus ss;

	ss.ParseFromString(s);

	strcpy(temp,strText);
	sprintf(temp, "+CAMERA STATUS MESSAGE(%d)\r\n", ++cameraStatusMessageCount);
	sprintf(temp, "%s|-->Laser  = %d   \r\n", temp,ss.laseron());
	time_t now = ss.time();
	strftime(timeStr, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));
	sprintf(temp, "%s|-->Time   = %s   \r\n", temp,timeStr);
	sprintf(temp, "%s|-->Status = %d   \r\n", temp,ss.status());
	sprintf(temp, "%s\r\n\r\n", temp);

	// aquire the mutex
	if (m_CameraStatus.isCreated() &&  
		m_CameraStatus.WaitForCommunicationEventMutex() == WAIT_OBJECT_0)
	{
		// write the data
		m_CameraStatus->PacketNumber = cameraStatusMessageCount;
		m_CameraStatus->LaserOnOf	 = ss.laseron();
		m_CameraStatus->ProcessID    = m_CameraStatus.GetProcessID();
		m_CameraStatus->Status		 = ss.status();
		m_CameraStatus->SubsystemId  = 3;
		m_CameraStatus->Time		 = ss.time();
		m_CameraStatus->ValidChars	 = 0; 
		// Set the event
		m_CameraStatus.SetEventServer();
		// release the mutex
		m_CameraStatus.ReleaseMutex();
	}
}
void CSocketManager::DecodeCameraTrackMessage(LPCTSTR strText, char* temp)
{
	static char timeStr[256];
	static int cameraTrackMessageCount = 0;
	string s((LPCTSTR)strText);
	cameraTracks tr;

	tr.ParseFromString(s);

	sprintf(temp, "+CAMERA TRACK MESSAGE(%d)\r\n", ++cameraTrackMessageCount);
	sprintf(temp, "%s|-->Laser  = %d   \r\n", temp,tr.laseron());
	time_t now = tr.time();
	strftime(timeStr, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));
	sprintf(temp, "%s|-->Time   = %s   \r\n", temp,timeStr);
	sprintf(temp, "%s|-->Status = %d   \r\n", temp,tr.status());
	sprintf(temp, "%s|-->Tracks = %d   \r\n", temp,tr.target_size());
	for (int ii = 0; ii < tr.target_size(); ii++)
	{
		sprintf(temp, "%s|-->Tack %d = [%d , %d]  \r\n",  temp,ii,tr.target(ii).x(),tr.target(ii).y());
	}
	sprintf(temp, "%s|-->Laser = %d   \r\n", temp,tr.laser_size());
	for (int ii = 0; ii < tr.laser_size(); ii++)
	{
		sprintf(temp, "%s|-->Tack %d = [%d , %d]  \r\n", temp,ii,tr.laser(ii).x(),tr.laser(ii).y());
	}
	sprintf(temp, "%s\r\n\r\n", temp);
	
	// aquire the mutex
	if (m_CameraTracks.isCreated() &&  
		m_CameraTracks.WaitForCommunicationEventMutex() == WAIT_OBJECT_0)
	{
		// write the data
		m_CameraTracks->PacketNumber = cameraTrackMessageCount;
		m_CameraTracks->LaserOnOf	 = tr.laseron();
		m_CameraTracks->ProcessID    = m_CameraTracks.GetProcessID();
		m_CameraTracks->Status		 = tr.status();
		m_CameraTracks->SubsystemId  = 3;
		m_CameraTracks->Time		 = tr.time();
		m_CameraTracks->ValidTracks  = tr.target_size();
		for (int ii = 0; ii < tr.target_size(); ii++)
		{
			m_CameraTracks->Tracks[ii].X = tr.target(ii).x();
			m_CameraTracks->Tracks[ii].Y = tr.target(ii).y();
		}
		m_CameraTracks->ValidLasers  =tr.laser_size();
		for (int ii = 0; ii < tr.laser_size(); ii++)
		{
			m_CameraTracks->Lasers[ii].X = tr.laser(ii).x();
			m_CameraTracks->Lasers[ii].Y = tr.laser(ii).y();
		}

		// Set the event (GUI)
		m_CameraTracks.SetEventServer();
		// Set the event (Processing)
		m_CameraTracks.SetEventServer();
		// release the mutex
		m_CameraTracks.ReleaseMutex();
	}
}
void CSocketManager::DecodeCameraImageMessage(LPCTSTR strText, char* temp)
{
	string s((LPCTSTR)strText);
	cameraImage im;

	im.ParseFromString(s);
	
	string sss = im.imagedata();
	CImage img;
	img.Create(im.sizex(), im.sizey(), 24 /* bpp */, 0 /* No alpha channel */);
	int ll =0;	
	int nPixel = 0;
	for(int row = 0; row < im.sizey(); row++)
	{
		for(int col = 0; col < im.sizex(); col++)
		{
			UPixel Pixel;
			Pixel.chars.cRed   = sss[ll+0];
			Pixel.chars.cGreen = sss[ll+1];
			Pixel.chars.cBlue  = sss[ll+2];
			img.SetPixel(col,row,Pixel.c);
			ll +=3;
		}
	}
	img.Save("temp.bmp");

	// Read the data
	//LPBITMAPINFOHEADER  pdib = (LPBITMAPINFOHEADER) NULL;
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

	////sprintf(temp, "Laser=%d Time=%I64d Status=%d\n\n", ss.laseron(),ss.time(),ss.status());
}
void CSocketManager::AppendMessage(LPCTSTR strText )
{
	if (NULL == m_pMsgCtrl)
		return;

	char temp [1024];
	
	switch(m_cameraMsgType)
	{
		case CameraPacketType::status: { DecodeCameraStatusMessage(strText,temp); break; }
		case CameraPacketType::track:  { DecodeCameraTrackMessage(strText,temp); break; }
		case CameraPacketType::image:  { DecodeCameraImageMessage(strText,temp); break; }
		default: { break; }
	}

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
			if (SendMessageTimeout(hWnd, EM_REPLACESEL, FALSE, (LPARAM)temp, SMTO_NORMAL, 1000L, &dwResult) != 0)
			{
			}
		}
	}
}
void CSocketManager::SetCameraMessageType(CameraPacketType type)
{
	m_cameraMsgType = type;
}

void CSocketManager::SetMessageWindow(CEdit* pMsgCtrl)
{
	m_pMsgCtrl = pMsgCtrl;
}


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
