
// NetworkArbiterDlg.h : header file
//

#pragma once
#include "stdafx.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "ConnectionSettings.h"
#include "tcpip/SocketManager.h"
#include "PictureCtrl.h"
#include "C3ArbiterConfiguration.h"
#include <string>
static const UINT WMU_NOTIFY_TASKBAR_ICON 
   = ::RegisterWindowMessage( _T("WMU_NOTIFY_TASKBAR_ICON") );

// CNetworkArbiterDlg dialog	
class CNetworkArbiterDlg : public CDialog
{
// Construction
public:
	CNetworkArbiterDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_NETWORKARBITER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnNotifyTaskbarIcon(  WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
	static UINT WINAPI SocketClientStartThreadProc(LPVOID pParam);
	static UINT WINAPI SocketServerStartThreadProc(LPVOID pParam);
	HANDLE      m_hThreadServer;      // Thread handle
	HANDLE      m_hThreadClient;      // Thread handle

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

private:
	void RunThread();
	
public:
	static const int SOCKET_COUNTS_CLIENTS = 3;
	static const int SOCKET_COUNTS_SERVERS = 1;
	CIPAddressCtrl	m_AddressControl;
	DWORD			m_AddressControlValue;
	CString			m_AddressPort;
	CSocketManager  m_SocketObjectClients[SOCKET_COUNTS_CLIENTS];
	CSocketManager  m_SocketObjectServer [SOCKET_COUNTS_SERVERS];
	CEdit			m_pMsgCtrl;
	afx_msg void OnBnClickedUpdateTcp();
	afx_msg void OnIpnFieldchangedIpaddress1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangePort();
	CEdit m_PortCtrl;
	CPictureCtrl m_picCtrl;

	C3ArbiterConfiguration m_configArbiter;
	afx_msg void OnBnClickedC3LaserStatus();
	afx_msg void OnBnClickedLaserOnOff();
	CButton m_LaserOnOff;
	CEdit m_LaserPWMAz;
	CEdit m_LaserPWMEl;
	void AddTaskbarIcon();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
