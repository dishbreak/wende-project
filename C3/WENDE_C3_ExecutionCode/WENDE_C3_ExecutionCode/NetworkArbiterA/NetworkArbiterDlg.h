
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
	DECLARE_MESSAGE_MAP()
	static UINT WINAPI SocketStartThreadProc(LPVOID pParam);
	HANDLE      m_hThread;      // Thread handle

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

private:
	void RunThread();
	
public:
	static const int SOCKET_COUNTS = 3;
	CIPAddressCtrl	m_AddressControl;
	DWORD			m_AddressControlValue;
	CString			m_AddressPort;
	CSocketManager  m_SocketObject[SOCKET_COUNTS];
	CEdit			m_pMsgCtrl;
	afx_msg void OnBnClickedUpdateTcp();
	afx_msg void OnIpnFieldchangedIpaddress1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangePort();
	CEdit m_PortCtrl;
	CPictureCtrl m_picCtrl;

	C3ArbiterConfiguration m_configArbiter;
};
