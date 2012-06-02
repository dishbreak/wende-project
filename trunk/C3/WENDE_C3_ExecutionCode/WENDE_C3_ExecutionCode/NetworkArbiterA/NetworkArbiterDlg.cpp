
// NetworkArbiterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NetworkArbiter.h"
#include "NetworkArbiterDlg.h"
#include "TinyXml\tinyxml.h"
#include "process.h"
#ifndef LASER_USE_PROTOBUF
#include "LaserCommand.h"
#include "Utilties.h"
#else
#include "laserMsgs.pb.h"
using namespace laserMsgs;
#endif
#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CNetworkArbiterDlg dialog




CNetworkArbiterDlg::CNetworkArbiterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNetworkArbiterDlg::IDD, pParent)
	, m_AddressControlValue(0)
	, m_AddressPort(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNetworkArbiterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, m_AddressControl);
	DDX_IPAddress(pDX, IDC_IPADDRESS1, m_AddressControlValue);
	DDX_Text(pDX, IDC_EDIT1, m_AddressPort);
	DDX_Control(pDX, IDC_MESSAGE_LIST, m_pMsgCtrl);
	DDX_Control(pDX, IDC_EDIT1, m_PortCtrl);
	DDX_Control(pDX, IDC_STATIC_PICTURE, m_picCtrl);
	DDX_Control(pDX, IDC_LASER_ON_OFF, m_LaserOnOff);
	DDX_Control(pDX, IDC_PWM_AZ, m_LaserPWMAz);
	DDX_Control(pDX, IDC_PWM_EL, m_LaserPWMEl);
}

BEGIN_MESSAGE_MAP(CNetworkArbiterDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_UPDATE_TCP, &CNetworkArbiterDlg::OnBnClickedUpdateTcp)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS1, &CNetworkArbiterDlg::OnIpnFieldchangedIpaddress1)
	ON_EN_CHANGE(IDC_EDIT1, &CNetworkArbiterDlg::OnEnChangePort)
	ON_BN_CLICKED(IDC_C3_LASER_STATUS, &CNetworkArbiterDlg::OnBnClickedC3LaserStatus)
	ON_BN_CLICKED(IDC_LASER_ON_OFF, &CNetworkArbiterDlg::OnBnClickedLaserOnOff)
END_MESSAGE_MAP()


// CNetworkArbiterDlg message handlers

BOOL CNetworkArbiterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CNetworkArbiterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CNetworkArbiterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CNetworkArbiterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

int CNetworkArbiterDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	

	// set the dialog controls..
	CString port;
	port.Format(_T("%d"), m_configArbiter.Connection.port);
	m_AddressControlValue = htonl(inet_addr(m_configArbiter.Connection.ip.c_str()));
	m_AddressPort         = port;
	//CString text;
	//text.Format(_T("%f"),100.0);
	//m_LaserPWMAz.SetWindowTextA(text);
	//m_LaserPWMEl.SetWindowTextA(text);

	// start thread to bring up com scokets
	RunThread();

	return 0;
}

void CNetworkArbiterDlg::RunThread()
{
	HANDLE hThreadClient;
	UINT uiThreadIdClient = 0;
	hThreadClient = (HANDLE)_beginthreadex(NULL,                    // Security attributes
								     0,  					  // stack
									 SocketClientStartThreadProc,   // Thread proc
									 this,					  // Thread param
									 CREATE_SUSPENDED,		  // creation mode
									 &uiThreadIdClient);			  // Thread ID

	if ( NULL != hThreadClient)
	{
		//SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL);
		ResumeThread( hThreadClient );
		m_hThreadClient = hThreadClient;
	}

	HANDLE hThreadServer;
	UINT uiThreadIdServer = 0;
	hThreadServer = (HANDLE)_beginthreadex(NULL,                    // Security attributes
								     0,  					  // stack
									 SocketServerStartThreadProc,   // Thread proc
									 this,					  // Thread param
									 CREATE_SUSPENDED,		  // creation mode
									 &uiThreadIdServer);			  // Thread ID

	if ( NULL != hThreadServer)
	{
		//SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL);
		ResumeThread( hThreadServer );
		m_hThreadServer = hThreadServer;
	}
}
///////////////////////////////////////////////////////////////////////////////
// SocketThreadProc
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//     Socket Thread function.  This function is the main thread for socket
//     communication - Asynchronous mode.
// PARAMETERS:
//     LPVOID pParam : Thread parameter - a CSocketComm pointer
// NOTES:
///////////////////////////////////////////////////////////////////////////////
UINT WINAPI CNetworkArbiterDlg::SocketClientStartThreadProc(LPVOID pParam)
{
	CNetworkArbiterDlg* pThis = reinterpret_cast<CNetworkArbiterDlg*>( pParam );
	// set the dialog controls..
	CString port;
	port.Format(_T("%d"), pThis->m_configArbiter.Connection.port);
	// setup the client sockets
	for (int ii = 0; ii < SOCKET_COUNTS_CLIENTS; ii++)
	{
		// Set the packet type
		pThis->m_SocketObjectClients[ii].SetMessageType((C3PacketType)ii);
		// Set the display event
		pThis->m_SocketObjectClients[ii].SetMessageWindow( &pThis->m_pMsgCtrl );
		pThis->m_SocketObjectClients[ii].SetPictureWindow( &pThis->m_picCtrl );

		while (!pThis->m_SocketObjectClients[ii].IsOpen())
		{
			// To use TCP socket
			port.Format(_T("%d"), pThis->m_configArbiter.Connection.port+ii);
			pThis->m_SocketObjectClients[ii].ConnectTo( pThis->m_configArbiter.Connection.ip.c_str(), port, AF_INET, SOCK_STREAM); // TCP
			if (!pThis->m_SocketObjectClients[ii].IsOpen())
			{
				Sleep(5);
			}
		}
		
		// Now you may start the server/client thread to do the work for you...
		pThis->m_SocketObjectClients[ii].WatchComm();
	}
	pThis->m_hThreadClient = NULL;
	
	_endthreadex( 0 );
    
	return 1L;
} // end SocketThreadProc
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//     Socket Thread function.  This function is the main thread for socket
//     communication - Asynchronous mode.
// PARAMETERS:
//     LPVOID pParam : Thread parameter - a CSocketComm pointer
// NOTES:
///////////////////////////////////////////////////////////////////////////////
UINT WINAPI CNetworkArbiterDlg::SocketServerStartThreadProc(LPVOID pParam)
{
	CNetworkArbiterDlg* pThis = reinterpret_cast<CNetworkArbiterDlg*>( pParam );
	// set the dialog controls..
	CString port;
	port.Format(_T("%d"), pThis->m_configArbiter.Connection.port);
	// setup the server sockets
	for (int ii = 0; ii < SOCKET_COUNTS_SERVERS; ii++)
	{
		switch(ii)
		{
			case 0: 
			{
				pThis->m_SocketObjectServer[ii].SetMessageType(C3PacketType::C3_LASER_STATUS);
				break;
			}
			//case #:
			//...
			//...
			//...
			default:
			{
				break;
			}
		}
		// Set the display event
		pThis->m_SocketObjectServer[ii].SetMessageWindow( &pThis->m_pMsgCtrl );
		pThis->m_SocketObjectServer[ii].SetPictureWindow( &pThis->m_picCtrl );
		while (!pThis->m_SocketObjectServer[ii].IsOpen())
		{
			// To use TCP socket
			port.Format(_T("%d"), pThis->m_configArbiter.Connection.port+ii+SOCKET_COUNTS_CLIENTS);
			pThis->m_SocketObjectServer[ii].SetSmartAddressing( false );
			pThis->m_SocketObjectServer[ii].CreateSocket( port, AF_INET, SOCK_STREAM, 0); // TCP
		}
		// set to server!!!
		pThis->m_SocketObjectServer[ii].SetServerState( true );	// run as server
		// Now you may start the server/client thread to do the work for you...
		pThis->m_SocketObjectServer[ii].WatchComm();
	}
	pThis->m_hThreadServer = NULL;
	
	_endthreadex( 0 );
    
	return 1L;
} // end SocketThreadProc

void CNetworkArbiterDlg::OnBnClickedUpdateTcp()
{
	m_configArbiter.WriteXMLFile();
	// Stop the thread that is running....
	if ( NULL != m_hThreadClient)
	{
		if(TerminateThread(m_hThreadClient, 0) == FALSE)
		{
			// Could not force thread to exit -> call 'GetLastError()'
		}
	}
	if ( NULL != m_hThreadServer)
	{
		if(TerminateThread(m_hThreadServer, 0) == FALSE)
		{
			// Could not force thread to exit -> call 'GetLastError()'
		}
	}
	// stop all COMS...
	for (int ii = 0; ii < SOCKET_COUNTS_CLIENTS; ii++)
	{
		m_SocketObjectClients[ii].StopComm();
	}
	// stop all COMS...
	for (int ii = 0; ii < SOCKET_COUNTS_SERVERS; ii++)
	{
		m_SocketObjectServer[ii].StopComm();
	}
	// start all COMS with new data...
	RunThread();
}

void CNetworkArbiterDlg::OnIpnFieldchangedIpaddress1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	DWORD temp = 0;
	m_AddressControl.GetAddress(temp);
	m_AddressControlValue = temp;
	struct in_addr addr;
	addr.s_addr = htonl((long)temp);

	m_configArbiter.Connection.ip = inet_ntoa(addr);
}

void CNetworkArbiterDlg::OnEnChangePort()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	m_PortCtrl.GetWindowTextA(m_AddressPort);
	m_configArbiter.Connection.port = atoi(m_AddressPort);
}

void CNetworkArbiterDlg::OnBnClickedC3LaserStatus()
{
	if(m_SocketObjectServer[0].IsOpen() == true)
	{
		stMessageProxy msgProxy;
		#ifndef LASER_USE_PROTOBUF
		CLaserCommand pose;
		CString cTemp;									// serilize the message
		m_LaserPWMAz.GetWindowTextA(cTemp);
		pose.LaserStatus.PWM_AZ = atoi(cTemp);
		m_LaserPWMEl.GetWindowTextA(cTemp);
		pose.LaserStatus.PWM_EL = atoi(cTemp);
		pose.LaserStatus.isLaserOn = (m_LaserOnOff.GetCheck()==BST_CHECKED);
		int nLen = sizeof(LASER_COMMAND_STRUCT);
		memcpy(msgProxy.byData, T2CA((char*)pose.StatusToBytes()), nLen);
		#else
		// time
		time_t seconds;
		seconds = time (NULL);
		laserPose pose;
		pose.set_time((DWORD)seconds);
		pose.set_laseron(m_LaserOnOff.GetCheck()==BST_CHECKED);
		CString cTemp;									// serilize the message
		m_LaserPWMAz.GetWindowTextA(cTemp);
		pose.mutable_target()->set_pulseaz(atoi(cTemp));
		m_LaserPWMEl.GetWindowTextA(cTemp);
		pose.mutable_target()->set_pulseel(atoi(cTemp));
		string strText = pose.SerializeAsString();
		USES_CONVERSION;
		int nLen = __min(sizeof(msgProxy.byData)-1, strText.size()+1);
		memcpy(msgProxy.byData, T2CA(strText.c_str()), nLen);
		#endif
	
		unsigned char sizeArray[4];
		CUtilities::IntToBytes(sizeArray,nLen);
		m_SocketObjectServer[0].WriteComm(sizeArray, sizeof(unsigned char)*4, INFINITE);
		m_SocketObjectServer[0].WriteComm(msgProxy.byData, nLen, INFINITE);
	}
}

void CNetworkArbiterDlg::OnBnClickedLaserOnOff()
{
	// TODO: Add your control notification handler code here
}
