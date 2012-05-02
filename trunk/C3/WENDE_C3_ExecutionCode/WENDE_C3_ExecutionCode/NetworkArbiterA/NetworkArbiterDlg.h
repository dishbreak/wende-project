
// NetworkArbiterDlg.h : header file
//

#pragma once
#include "stdafx.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "ConnectionSettings.h"
#include "tcpip/SocketManager.h"

#include "string.h"

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
public:
	afx_msg void OnIpnFieldchangedIpaddress1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

private:
	void WriteXMLFile();

private:
	ConnectionSettings m_Connection;
	
public:
	static const int SOCKET_COUNTS = 3;
	CIPAddressCtrl	m_AddressControl;
	DWORD			m_AddressControlValue;
	CString			m_AddressPort;
	CSocketManager  m_SocketObject[SOCKET_COUNTS];
	CEdit			m_pMsgCtrl;
};
