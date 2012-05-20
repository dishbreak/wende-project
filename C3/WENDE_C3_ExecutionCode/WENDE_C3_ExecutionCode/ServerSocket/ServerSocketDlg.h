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
using std::string;
using namespace cameraMsgs;
#define MAX_CONNECTION		3

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
	void OnBtnSend(string strText, int portOffset, int size,  int type);

	// Generated message map functions
	//{{AFX_MSG(CServerSocketDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnStart();
	afx_msg void OnBtnStop();
	afx_msg void OnDestroy();
	afx_msg void OnBtnSendStatus();
	afx_msg void OnBtnSendImage();
	afx_msg void OnBtnSendTrack();
	//}}AFX_MSG
	afx_msg LRESULT OnUpdateConnection(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	CButton m_statusLaserOnCtrl;
	cameraMsgs::systemStatus m_CameraStatus;
	afx_msg void OnBnClickedCameraStatusDown();
	afx_msg void OnBnClickedCameraStatusReady();
	afx_msg void OnBnClickedCameraStatusOperational();
	afx_msg void OnBnClickedCameraStatusError();
	afx_msg void OnBnClickedCameraStatusFailed();
	afx_msg void OnBnClickedCameraStatusUnkown();
	CButton m_CameraUnkownStatus;
	CEdit m_CameraStatusTextCtrl;
	CButton m_trackEnable0;
	CButton m_trackEnable1;
	CButton m_trackEnable2;
	CButton m_trackEnable3;
	CButton m_trackEnable4;
	CButton m_trackEnable5;
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
	afx_msg void OnBnClickedTrackEnable0();
	afx_msg void OnBnClickedTrackEnable1();
	afx_msg void OnBnClickedTrackEnable2();
	afx_msg void OnBnClickedTrackEnable3();
	afx_msg void OnBnClickedTrackEnable4();
	afx_msg void OnBnClickedTrackEnable5();
	CButton m_laserEnable0;
	CEdit m_laserXEditBox0;
	CEdit m_laserYEditBox0;
	afx_msg void OnBnClickedLaserEnable6();
	afx_msg void OnBnClickedBtnSelectCameraImage();
	CString m_imageName;
	unsigned char bytes[SHM_MAX_IMAGE_SIZE];
	CPictureCtrl m_picCtrl;
	CImage  m_CameraImage;
	SIZE    tImageSize;
	void AddTrack(CButton *buttom, CEdit *x, CEdit *y, cameraTracks *track);
	void AddLaser(CButton *buttom, CEdit *x, CEdit *y, cameraTracks *track);
	afx_msg void OnBnClickedBtnSendLasserStatus();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERSOCKETDLG_H__BF055235_5494_4FFC_8289_20DBFD9503A8__INCLUDED_)
