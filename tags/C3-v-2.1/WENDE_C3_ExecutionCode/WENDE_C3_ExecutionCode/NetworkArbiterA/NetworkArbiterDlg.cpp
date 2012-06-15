
// NetworkArbiterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NetworkArbiter.h"
#include "NetworkArbiterDlg.h"
#include "TinyXml\tinyxml.h"
#include "process.h"
#include "LaserCommand.h"
#include "LaserConfiguration.h"
#include "Utilties.h"
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
	DDX_Control(pDX, IDC_PWM_MIN, m_LaserPwmMin);
	DDX_Control(pDX, IDC_PWM_MAX, m_LaserPwmMax);
	DDX_Control(pDX, IDC_FREQ, m_LaserFreq);
	DDX_Control(pDX, IDC_C3_LASER_STATUS3, m_sendCont);
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
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_REGISTERED_MESSAGE( WMU_NOTIFY_TASKBAR_ICON, OnNotifyTaskbarIcon )
	ON_BN_CLICKED(IDC_C3_LASER_STATUS2, &CNetworkArbiterDlg::OnBnClickedC3LaserStatus2)
	ON_BN_CLICKED(IDC_C3_LASER_STATUS3, &CNetworkArbiterDlg::OnBnClickedC3LaserStatus3)
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

	AddTaskbarIcon();
	// The one and only window has been initialized, so show and update it.

	return TRUE;  // return TRUE  unless you set the focus to a control
}
void CNetworkArbiterDlg::AddTaskbarIcon()
{
   DWORD dwMessage = NIM_ADD;
   NOTIFYICONDATA   nid;

   nid.cbSize = sizeof(NOTIFYICONDATA); 
   nid.hWnd = GetSafeHwnd(); 
   nid.uID = 0; 
   nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP; 
   nid.uCallbackMessage = WMU_NOTIFY_TASKBAR_ICON; 
   nid.hIcon = GetIcon( TRUE );
   lstrcpyn( nid.szTip, AfxGetAppName(), sizeof( nid.szTip ) );

   ::Shell_NotifyIcon( dwMessage,  // message identifier 
                       &nid);      // pointer to structure
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
	port.Format(_T("%d"), C3Configuration::Instance().ConnectionArbiter.port);
	m_AddressControlValue = htonl(inet_addr(C3Configuration::Instance().ConnectionArbiter.ip.c_str()));
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
	port.Format(_T("%d"), C3Configuration::Instance().ConnectionArbiter.port);
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
			port.Format(_T("%d"), C3Configuration::Instance().ConnectionArbiter.port+ii);
			pThis->m_SocketObjectClients[ii].ConnectTo( C3Configuration::Instance().ConnectionArbiter.ip.c_str(), port, AF_INET, SOCK_STREAM); // TCP
			if (!pThis->m_SocketObjectClients[ii].IsOpen())
			{
				Sleep(1000);
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
	port.Format(_T("%d"), C3Configuration::Instance().ConnectionArbiter.port);
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
			port.Format(_T("%d"), C3Configuration::Instance().ConnectionArbiter.port+ii+SOCKET_COUNTS_CLIENTS);
			pThis->m_SocketObjectServer[ii].SetSmartAddressing( false );
			pThis->m_SocketObjectServer[ii].CreateSocket( port, AF_INET, SOCK_STREAM, 0); // TCP
		}
		// set to server!!!
		pThis->m_SocketObjectServer[ii].SetServerState( true );	// run as server
		// Now you may start the server/client thread to do the work for you...
		pThis->m_SocketObjectServer[ii].WatchComm();
		//
		pThis->m_cNetworkMonitor.InitializeThread(&pThis->m_SocketObjectServer[ii]);
	}
	pThis->m_hThreadServer = NULL;
	
	_endthreadex( 0 );
    
	return 1L;
} // end SocketThreadProc

void CNetworkArbiterDlg::OnBnClickedUpdateTcp()
{
	m_cNetworkMonitor.StopThreads();
	Sleep(5000);
	C3Configuration::Instance().WriteXMLFile();
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

	C3Configuration::Instance().ConnectionArbiter.ip = inet_ntoa(addr);
}

void CNetworkArbiterDlg::OnEnChangePort()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	m_PortCtrl.GetWindowTextA(m_AddressPort);
	C3Configuration::Instance().ConnectionArbiter.port = atoi(m_AddressPort);
}
void CNetworkArbiterDlg::C3LaserStatus()
{
	if(m_SocketObjectServer[0].IsOpen() == true)
	{
		stMessageProxy msgProxy;
		CLaserCommand pose;
		
		CString cTempAZ;									// serilize the message
		CString cTempEL;									// serilize the message

		m_LaserPWMAz.GetWindowTextA(cTempAZ);
		m_LaserPWMEl.GetWindowTextA(cTempEL);
		
		pose.LaserCommand.PWM_AZ = atoi(cTempAZ);
		pose.LaserCommand.PWM_EL = atoi(cTempEL);
		pose.LaserCommand.IsLaserOn = (m_LaserOnOff.GetCheck()==BST_CHECKED);

		int nLen = sizeof(LASER_COMMAND_STRUCT);
		memcpy(msgProxy.byData, T2CA((char*)pose.StatusToBytes()), nLen);
		
		unsigned char sizeArray[4];
		CUtilities::IntToBytes(sizeArray,htonl(nLen));
		unsigned char typeArray = (char)WENDE_MESSAGE_TYPE::LASER_COMMAND;

		m_SocketObjectServer[0].WriteComm(sizeArray , sizeof(unsigned char)*4, INFINITE);
		m_SocketObjectServer[0].WriteComm(&typeArray, sizeof(unsigned char)*1, INFINITE);
		m_SocketObjectServer[0].WriteComm(msgProxy.byData, nLen, INFINITE);
	}
}
void CNetworkArbiterDlg::OnBnClickedC3LaserStatus()
{
	C3LaserStatus();
	
}

void CNetworkArbiterDlg::OnBnClickedLaserOnOff()
{
	// TODO: Add your control notification handler code here
}

void CNetworkArbiterDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
	   // remove the taskbar icon
   DWORD dwMessage = NIM_DELETE;
   NOTIFYICONDATA   nid;

   nid.cbSize = sizeof(NOTIFYICONDATA);
   nid.uID = 0;
   nid.hWnd = GetSafeHwnd();

   ::Shell_NotifyIcon( dwMessage,  // message identifier 
                       &nid);      // pointer to structure

	CDialog::OnDestroy();	
}

void CNetworkArbiterDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if( SIZE_MINIMIZED  == nType )
	{
	  ShowWindow( SW_HIDE );
	}
}
LRESULT CNetworkArbiterDlg::OnNotifyTaskbarIcon(  WPARAM wParam, LPARAM lParam )
{

	UINT uID = (UINT)wParam;       // this is the ID you assigned to your taskbar icon
	UINT uMouseMsg = (UINT)lParam; // mouse messages

	switch (uMouseMsg)
   {
	case WM_LBUTTONDOWN:
      break;
   case WM_LBUTTONDBLCLK:
      if( IsIconic() )
      {
         ShowWindow( SW_RESTORE );
      }
      ShowWindow( SW_SHOW );
      SetForegroundWindow();
      break;
	case WM_RBUTTONDOWN:
      {
         CMenu* pMenu = GetMenu();
         if( pMenu )
         {
            CMenu *pSubMenu = NULL;
            pSubMenu = pMenu->GetSubMenu( 0 );
            {
              SetForegroundWindow(); // *** little patch here ***				
			  POINT pointCursor;			   
			  ::GetCursorPos( &pointCursor );			   
			  pSubMenu->TrackPopupMenu( TPM_RIGHTALIGN, 
                                         pointCursor.x, pointCursor.y, 
                                         this );
            }
         }
      }
      break;
	case WM_RBUTTONDBLCLK:
      break;
	case WM_MOUSEMOVE:
      break;
   }
   return 0L;
}
void CNetworkArbiterDlg::OnBnClickedC3LaserStatus2()
{
	if(m_SocketObjectServer[0].IsOpen() == true)
	{
		stMessageProxy msgProxy;
		CLaserConfiguration config;

		CString cTempMIN;									// serilize the message
		CString cTempMAX;									// serilize the message
		CString cTempFREQ;									// serilize the message

		m_LaserPwmMin.GetWindowTextA(cTempMIN);
		m_LaserPwmMax.GetWindowTextA(cTempMAX);
		m_LaserFreq.GetWindowTextA(cTempFREQ);
			
		config.LaserConfiguration.PWM_AZ.MIN = atoi(cTempMIN);
		config.LaserConfiguration.PWM_AZ.MAX = atoi(cTempMAX);
		config.LaserConfiguration.PWM_EL.MIN = atoi(cTempMIN);
		config.LaserConfiguration.PWM_EL.MAX = atoi(cTempMAX);
		config.LaserConfiguration.Frequency  = atoi(cTempFREQ);

		int nLen = sizeof(LASER_CONFIG_STRUCT);
		memcpy(msgProxy.byData, T2CA((char*)config.StatusToBytes()), nLen);
		
		unsigned char sizeArray[4];
		CUtilities::IntToBytes(sizeArray,htonl(nLen));
		unsigned char typeArray = (char)WENDE_MESSAGE_TYPE::LASER_CONFIG;

		m_SocketObjectServer[0].WriteComm(sizeArray , sizeof(unsigned char)*4, INFINITE);
		m_SocketObjectServer[0].WriteComm(&typeArray, sizeof(unsigned char)*1, INFINITE);
		m_SocketObjectServer[0].WriteComm(msgProxy.byData, nLen, INFINITE);
	}
}
void CALLBACK TimerProcLaserCommand(void* lpParametar, BOOLEAN TimerOrWaitFired)
{
	// This is used only to call QueueTimerHandler
	// Typically, this function is static member of CTimersDlg
	CNetworkArbiterDlg* obj = (CNetworkArbiterDlg*) lpParametar;
	obj->C3LaserStatus();
} 
void CNetworkArbiterDlg::OnBnClickedC3LaserStatus3()
{
	static bool isStart = true;
	if (isStart == true)
	{
		isStart = false;
		m_sendCont.SetWindowTextA("Stop Send Command");
		BOOL success = ::CreateTimerQueueTimer(	&m_timerHandleLaserCommand,
												NULL,
												TimerProcLaserCommand,
												this,
												0,
												250,
												WT_EXECUTEINTIMERTHREAD);
	}
	else
	{
		m_sendCont.SetWindowTextA("Send Laser Command (cont.)");
		isStart = true;
		// destroy the timer
		DeleteTimerQueueTimer(NULL, m_timerHandleLaserCommand, NULL);
		Sleep(5000);
		CloseHandle (m_timerHandleLaserCommand);
	}
}
