// SocketManager.h: interface for the CSocketManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOCKETMANAGER_H__7403BD71_338A_4531_BD91_3D7E5B505793__INCLUDED_)
#define AFX_SOCKETMANAGER_H__7403BD71_338A_4531_BD91_3D7E5B505793__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SocketComm.h"
#include "PictureCtrl.h"

#define WM_UPDATE_CONNECTION	WM_APP+0x1234

typedef enum {
	C3_CAMERA_STATUS = 0,
	C3_CAMERA_TRACK  = 1,
	C3_CAMERA_IMAGE  = 2,
	C3_LASER_STATUS  = 4,
	C3_PACKET_TYPES  = 5 
} C3PacketType;

class CSocketManager : public CSocketComm  
{
public:
	CSocketManager();
	virtual ~CSocketManager();

	void SetMessageWindow(CEdit* pMsgCtrl);
	void SetPictureWindow(CPictureCtrl* picCtrl);
	void SetCameraMessageType(C3PacketType type);
	void AppendMessage(LPCTSTR strText, DWORD size);
public:

	virtual void OnDataReceived(const LPBYTE lpBuffer, DWORD dwCount);
	virtual void OnEvent(UINT uEvent, LPVOID lpvData);

protected:
	void DisplayData(const LPBYTE lpData, DWORD dwCount, const SockAddrIn& sfrom);
	CEdit			 *m_pMsgCtrl;
	C3PacketType m_cameraMsgType;
	CPictureCtrl	 *m_picCtrl;
	char			 *inData;
};

#endif // !defined(AFX_SOCKETMANAGER_H__7403BD71_338A_4531_BD91_3D7E5B505793__INCLUDED_)
