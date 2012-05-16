// SocketManager.h: interface for the CSocketManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOCKETMANAGER_H__7403BD71_338A_4531_BD91_3D7E5B505793__INCLUDED_)
#define AFX_SOCKETMANAGER_H__7403BD71_338A_4531_BD91_3D7E5B505793__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SocketComm.h"
#include "ShmStructs.h"
#include "CSharedStruct.h"
#include "PictureCtrl.h"

#define WM_UPDATE_CONNECTION	WM_APP+0x1234

typedef enum {
	status = 0,
	track = 1,
	image = 2
} CameraPacketType;
typedef union
{
	COLORREF c;
	struct
	{
#ifdef LITTLEENDIAN
		unsigned char cBlue;
		unsigned char cGreen;
		unsigned char cRed;
		unsigned char crsvd;
#else
		unsigned char crsvd;
		unsigned char cRed;
		unsigned char cGreen;
		unsigned char cBlue;
#endif
	} chars;
} UPixel;
class CSocketManager : public CSocketComm  
{
public:
	CSocketManager();
	virtual ~CSocketManager();

	void SetMessageWindow(CEdit* pMsgCtrl);
	void SetCameraMessageType(CameraPacketType type);
	void AppendMessage(LPCTSTR strText );
public:

	virtual void OnDataReceived(const LPBYTE lpBuffer, DWORD dwCount);
	virtual void OnEvent(UINT uEvent, LPVOID lpvData);

protected:
	void DisplayData(const LPBYTE lpData, DWORD dwCount, const SockAddrIn& sfrom);
	CEdit* m_pMsgCtrl;
	CameraPacketType m_cameraMsgType;
	CPictureCtrl* m_picCtrl;
	
private:
	void DecodeCameraStatusMessage(LPCTSTR strText, char* temp);
	void DecodeCameraTrackMessage(LPCTSTR strText, char* temp);
	void DecodeCameraImageMessage(LPCTSTR strText, char* temp);

	CSharedStruct<LASER_POINT_DIRECTION_SHM> m_LaserCommand;
	CSharedStruct<CAMERA_STATUS_MSG_SHM>	 m_CameraStatus;
	CSharedStruct<CAMERA_TRACK_MSG_SHM>		 m_CameraTracks;
	CSharedStruct<CAMERA_IMAGE_MSG_SHM>		 m_CameraImage;
};

#endif // !defined(AFX_SOCKETMANAGER_H__7403BD71_338A_4531_BD91_3D7E5B505793__INCLUDED_)
