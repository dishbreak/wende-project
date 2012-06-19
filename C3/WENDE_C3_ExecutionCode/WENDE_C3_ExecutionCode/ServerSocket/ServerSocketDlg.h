// ServerSocketDlg.h : header file
//

#if !defined(AFX_SERVERSOCKETDLG_H__BF055235_5494_4FFC_8289_20DBFD9503A8__INCLUDED_)
#define AFX_SERVERSOCKETDLG_H__BF055235_5494_4FFC_8289_20DBFD9503A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SocketManager.h"
#include "cameraMsgs.pb.h"
#include "afxwin.h"
#include "atlimage.h"
#include "PictureCtrl.h"
#include "ShmStructs.h"
#include <string>
#include "LaserStatus.h"
#include <iostream>
#include <fstream>


using std::string;
using std::ifstream;
using std::string;
using namespace cameraMsgs;

#define MAX_CONNECTION		4

/////////////////////////////////////////////////////////////////////////////
// CServerSocketDlg dialog

class CServerSocketDlg : public CDialog
{
// Construction
public:
	CServerSocketDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CServerSocketDlg)
	enum { IDD = IDD_SERVERSOCKET_DIALOG };
	CEdit	m_ctlMessage;
	CEdit	m_ctlMsgList;
	CSpinButtonCtrl	m_ctlPortInc;
	CString	m_strPort;
	int		m_nSockType;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerSocketDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	CSocketManager m_SocketManager[MAX_CONNECTION];
	//CSocketManager* m_pCurServer;

	//void PickNextAvailable();
	bool StartServer();
	void OnBtnSend(const char* strText, int portOffset, int size,  int type);

	// Generated message map functions
	//{{AFX_MSG(CServerSocketDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnStart();
	afx_msg void OnBtnStop();
	afx_msg void OnDestroy();
	
	//}}AFX_MSG
	afx_msg LRESULT OnUpdateConnection(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCameraStatusDown();
	afx_msg void OnBnClickedCameraStatusReady();
	afx_msg void OnBnClickedCameraStatusOperational();
	afx_msg void OnBnClickedCameraStatusError();
	afx_msg void OnBnClickedCameraStatusFailed();
	afx_msg void OnBnClickedCameraStatusUnkown();
	afx_msg void OnBtnSendStatus();
	afx_msg void OnBtnSendImage();
	afx_msg void OnBtnSendTrack();
	afx_msg void OnBnClickedLaserEnable6();
	afx_msg void OnBnClickedBtnSelectCameraImage();
	afx_msg void OnBnClickedBtnSendStopAndCrawl();
	afx_msg void OnBnClickedTrackEnable0();
	afx_msg void OnBnClickedTrackEnable1();
	afx_msg void OnBnClickedTrackEnable2();
	afx_msg void OnBnClickedTrackEnable3();
	afx_msg void OnBnClickedTrackEnable4();
	afx_msg void OnBnClickedTrackEnable5();
	afx_msg void OnBnClickedBtnSendLasserStatus();
	afx_msg void OnBnClickedLaserStatusDown();
	afx_msg void OnBnClickedLaserStatusFailed();
	afx_msg void OnBnClickedLaserStatusError();
	afx_msg void OnBnClickedLaserStatusReady();
	afx_msg void OnBnClickedLaserStatusUnkown();
	afx_msg void OnBnClickedLaserStatusOperational();
	afx_msg void OnBnClickedBtnSendStatusCont();
	afx_msg void OnBnClickedBtnSendTrack2();
	afx_msg void OnBnClickedBtnSendImage2();
	afx_msg void OnBnClickedBtnSendLaserStatus();
	
	CEdit m_CameraStatusTextCtrl;
	CEdit m_trackXEditBox0;
	CEdit m_trackXEditBox1;
	CEdit m_trackXEditBox2;
	CEdit m_trackXEditBox3;
	CEdit m_trackXEditBox4;
	CEdit m_trackXEditBox5;
	CEdit m_trackYEditBox0;
	CEdit m_trackYEditBox1;
	CEdit m_trackYEditBox2;
	CEdit m_trackYEditBox3;
	CEdit m_trackYEditBox4;
	CEdit m_trackYEditBox5;
	
	CEdit m_laserXEditBox0;
	CEdit m_laserYEditBox0;
	CString m_imageName;
	unsigned char bytes[SHM_MAX_IMAGE_SIZE];
	CPictureCtrl m_picCtrl;
	CImage  m_CameraImage;
	SIZE    tImageSize;
	void AddTrack(CButton *buttom, CEdit *x, CEdit *y, cameraTracks *track);
	void AddLaser(CButton *buttom, CEdit *x, CEdit *y, cameraTracks *track);
	void SendFile();
	CEdit m_LaserStatusText;
	cameraMsgs::systemStatus m_CameraStatus;
	LASER_SYSTEM_STATUS m_LaserStatus;
	HANDLE m_timerHandleCameraStatus;
	HANDLE m_timerHandleCameraTrack;
	HANDLE m_timerHandleCameraImage;
	HANDLE m_timerHandleLaserStatus;
	HANDLE m_timerHandleSlowCrawl;
	HANDLE m_timerHandleSlow;
	HANDLE m_timerHandleFast;
	HANDLE m_timerHandleSpiral;
	HANDLE m_timerHandleFail;
	CButton m_laserEnable0;
	CButton m_CameraStatusButton;
	CButton m_CameraTrackButton;
	CButton m_CameraImageButton;
	CButton m_LaserStatusButton;
	CButton m_trackEnable0;
	CButton m_trackEnable1;
	CButton m_trackEnable2;
	CButton m_trackEnable3;
	CButton m_trackEnable4;
	CButton m_trackEnable5;
	CButton m_CameraUnkownStatus;
	CButton m_statusLaserOnCtrl;	
	CButton m_StopCrawl;
	ifstream myfile;
	afx_msg void OnBnClickedBtnSendSlow();
	CButton m_TrialSlow;
	afx_msg void OnBnClickedBtnSendFast();
	CButton m_TrialFast;
	afx_msg void OnBnClickedBtnSendSpiral();
	CButton m_TrialSpiral;
	afx_msg void OnBnClickedBtnSendFail();
	CButton m_TrialFail;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERSOCKETDLG_H__BF055235_5494_4FFC_8289_20DBFD9503A8__INCLUDED_)
