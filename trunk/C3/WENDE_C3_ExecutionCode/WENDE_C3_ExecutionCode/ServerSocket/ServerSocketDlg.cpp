// ServerSocketDlg.cpp : implementation file
//

#include "stdafx.h"
#include <atlconv.h>
#include "ServerSocket.h"
#include "ServerSocketDlg.h"
#include "cameraMsgs.pb.h"
#include <string>
using namespace cameraMsgs;
using std::string;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int SOCK_TCP	= 0;
const int SOCK_UDP  = 1;

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerSocketDlg dialog

CServerSocketDlg::CServerSocketDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerSocketDlg::IDD, pParent)
	, m_CameraStatus(cameraMsgs::systemStatus::UNKNOWN)
	, m_imageName("")
{
	//{{AFX_DATA_INIT(CServerSocketDlg)
	m_strPort = _T("4444");
	
	m_nSockType = SOCK_TCP;	// default TCP
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CServerSocketDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CServerSocketDlg)
	//DDX_Control(pDX, IDC_TXT_MESSAGE, m_ctlMessage);
	DDX_Control(pDX, IDC_MESSAGE_LIST, m_ctlMsgList);
	DDX_Control(pDX, IDC_SVR_PORTINC, m_ctlPortInc);
	DDX_Text(pDX, IDC_SVR_PORT, m_strPort);
	DDX_Radio(pDX, IDC_TCP, m_nSockType);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LASER_STATUS, m_statusLaserOnCtrl);
	DDX_Control(pDX, IDC_BTN_SEND_IMAGE, m_CameraUnkownStatus);
	DDX_Control(pDX, IDC_EDIT1, m_CameraStatusTextCtrl);
	DDX_Control(pDX, IDC_TRACK_ENABLE_0, m_trackEnable0);
	DDX_Control(pDX, IDC_TRACK_ENABLE_1, m_trackEnable1);
	DDX_Control(pDX, IDC_TRACK_ENABLE_2, m_trackEnable2);
	DDX_Control(pDX, IDC_TRACK_ENABLE_3, m_trackEnable3);
	DDX_Control(pDX, IDC_TRACK_ENABLE_4, m_trackEnable4);
	DDX_Control(pDX, IDC_TRACK_ENABLE_5, m_trackEnable5);
	DDX_Control(pDX, IDC_TRACK_X_0, m_trackXEditBox0);
	DDX_Control(pDX, IDC_TRACK_X_1, m_trackXEditBox1);
	DDX_Control(pDX, IDC_TRACK_X_2, m_trackXEditBox2);
	DDX_Control(pDX, IDC_TRACK_X_3, m_trackXEditBox3);
	DDX_Control(pDX, IDC_TRACK_X_4, m_trackXEditBox4);
	DDX_Control(pDX, IDC_TRACK_X_5, m_trackXEditBox5);
	DDX_Control(pDX, IDC_TRACK_Y_0, m_trackYEditBox0);
	DDX_Control(pDX, IDC_TRACK_Y_1, m_trackYEditBox1);
	DDX_Control(pDX, IDC_TRACK_Y_2, m_trackYEditBox2);
	DDX_Control(pDX, IDC_TRACK_Y_3, m_trackYEditBox3);
	DDX_Control(pDX, IDC_TRACK_Y_4, m_trackYEditBox4);
	DDX_Control(pDX, IDC_TRACK_Y_5, m_trackYEditBox5);
	DDX_Control(pDX, IDC_LASER_ENABLE_6, m_laserEnable0);
	DDX_Control(pDX, IDC_LASER_X_0, m_laserXEditBox0);
	DDX_Control(pDX, IDC_LASER_Y_0, m_laserYEditBox0);
	DDX_Control(pDX, IDC_STATIC_PICTURE, m_picCtrl);
}

BEGIN_MESSAGE_MAP(CServerSocketDlg, CDialog)
	//{{AFX_MSG_MAP(CServerSocketDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_START, OnBtnStart)
	ON_BN_CLICKED(IDC_BTN_STOP, OnBtnStop)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_SEND_STATUS, OnBtnSendStatus)
	ON_BN_CLICKED(IDC_BTN_SEND_IMAGE, OnBtnSendImage)
	ON_BN_CLICKED(IDC_BTN_SEND_TRACK, OnBtnSendTrack)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_UPDATE_CONNECTION, OnUpdateConnection)
	ON_BN_CLICKED(IDC_CAMERA_STATUS_DOWN, &CServerSocketDlg::OnBnClickedCameraStatusDown)
	ON_BN_CLICKED(IDC_CAMERA_STATUS_READY, &CServerSocketDlg::OnBnClickedCameraStatusReady)
	ON_BN_CLICKED(IDC_CAMERA_STATUS_OPERATIONAL, &CServerSocketDlg::OnBnClickedCameraStatusOperational)
	ON_BN_CLICKED(IDC_CAMERA_STATUS_ERROR, &CServerSocketDlg::OnBnClickedCameraStatusError)
	ON_BN_CLICKED(IDC_CAMERA_STATUS_Failed, &CServerSocketDlg::OnBnClickedCameraStatusFailed)
	ON_BN_CLICKED(IDC_CAMERA_STATUS_UNKOWN, &CServerSocketDlg::OnBnClickedCameraStatusUnkown)
	ON_BN_CLICKED(IDC_TRACK_ENABLE_0, &CServerSocketDlg::OnBnClickedTrackEnable0)
	ON_BN_CLICKED(IDC_TRACK_ENABLE_1, &CServerSocketDlg::OnBnClickedTrackEnable1)
	ON_BN_CLICKED(IDC_TRACK_ENABLE_2, &CServerSocketDlg::OnBnClickedTrackEnable2)
	ON_BN_CLICKED(IDC_TRACK_ENABLE_3, &CServerSocketDlg::OnBnClickedTrackEnable3)
	ON_BN_CLICKED(IDC_TRACK_ENABLE_4, &CServerSocketDlg::OnBnClickedTrackEnable4)
	ON_BN_CLICKED(IDC_TRACK_ENABLE_5, &CServerSocketDlg::OnBnClickedTrackEnable5)
	ON_BN_CLICKED(IDC_LASER_ENABLE_6, &CServerSocketDlg::OnBnClickedLaserEnable6)
	ON_BN_CLICKED(IDC_BTN_SELECT_CAMERA_IMAGE, &CServerSocketDlg::OnBnClickedBtnSelectCameraImage)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerSocketDlg message handlers

BOOL CServerSocketDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		int nVirtKey = (int) pMsg->wParam;
		if (nVirtKey == VK_ESCAPE)
			return TRUE;
		if (nVirtKey == VK_RETURN && (GetFocus()->m_hWnd  == m_ctlMessage.m_hWnd))
		{
			if (m_SocketManager[0].IsOpen() && m_SocketManager[1].IsOpen() && m_SocketManager[2].IsOpen())
			{
				string strTex = "TEST";
				OnBtnSend(strTex,0,strTex.size(),0);
			}
			return TRUE;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

///////////////////////////////////////////////////////////////////////////////
// PickNextAvailable : this is useful only for TCP socket
//void CServerSocketDlg::PickNextAvailable()
//{
//	m_pCurServer = NULL;
//	for(int i=0; i<MAX_CONNECTION; i++)
//	{
//		if (!m_SocketManager[i].IsOpen())
//		{
//			m_pCurServer = &m_SocketManager[i];
//			break;
//		}
//	}
//}


///////////////////////////////////////////////////////////////////////////////
// StartServer : Start the server
bool CServerSocketDlg::StartServer()
{
	bool bSuccess = false;
	int ipPort= atoi(m_strPort);
	CString port;
		
	if (m_nSockType == SOCK_TCP)
	{
		// no smart addressing - we use connection oriented
		for(int ii=0; ii<MAX_CONNECTION; ii++)
		{
			port.Format(_T("%d"), ipPort+ii);
			m_SocketManager[ii].SetSmartAddressing( false );
			bSuccess = m_SocketManager[ii].CreateSocket( port, AF_INET, SOCK_STREAM, 0); // TCP
		}
	}
	else
	{
		// no smart addressing - we use connection oriented
		for(int ii=0; ii<MAX_CONNECTION; ii++)
		{
			port.Format(_T("%d"), ipPort+ii);
			m_SocketManager[ii].SetSmartAddressing( true );
			bSuccess = m_SocketManager[ii].CreateSocket( port, AF_INET, SOCK_DGRAM, SO_BROADCAST); // UDP
		}
	}

	if (bSuccess && 
		m_SocketManager[0].WatchComm() && 
		m_SocketManager[1].WatchComm() &&
		m_SocketManager[2].WatchComm() )
	{
		for(int ii=0; ii<MAX_CONNECTION; ii++)
		{
			GetDlgItem(IDC_BTN_SEND_STATUS)->EnableWindow( TRUE );
			GetDlgItem(IDC_BTN_SEND_IMAGE)->EnableWindow( TRUE );
			GetDlgItem(IDC_BTN_SEND_TRACK)->EnableWindow( TRUE );
			GetDlgItem(IDC_BTN_STOP)->EnableWindow( TRUE );
			NextDlgCtrl();
			GetDlgItem(IDC_BTN_START)->EnableWindow( FALSE );
			GetDlgItem(IDC_TCP)->EnableWindow( FALSE );
			GetDlgItem(IDC_UDP)->EnableWindow( FALSE );
			CString strServer, strAddr;
			m_SocketManager[ii].GetLocalName( strServer.GetBuffer(256), 256);
			strServer.ReleaseBuffer();
			m_SocketManager[ii].GetLocalAddress( strAddr.GetBuffer(256), 256);
			strAddr.ReleaseBuffer();
			port.Format(_T("%d"), ipPort+ii);
			CString strMsg = _T("Server: ") + strServer;
					strMsg += _T(", @Address: ") + strAddr;
					strMsg += _T(" is running on port ") + port + CString("\r\n");
			m_SocketManager[ii].AppendMessage( strMsg );
		}
	}

	return bSuccess;
}

BOOL CServerSocketDlg::OnInitDialog()
{
	ASSERT( GetDlgItem(IDC_BTN_SEND_STATUS) != NULL );
	ASSERT( GetDlgItem(IDC_BTN_SEND_IMAGE) != NULL );
	ASSERT( GetDlgItem(IDC_BTN_SEND_TRACK) != NULL );
	ASSERT( GetDlgItem(IDC_BTN_START) != NULL );
	ASSERT( GetDlgItem(IDC_BTN_STOP) != NULL );

	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
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
	m_ctlPortInc.SetRange32( 2000, 4500);
	GetDlgItem(IDC_BTN_SEND_STATUS)->EnableWindow( FALSE );
	GetDlgItem(IDC_BTN_SEND_IMAGE)->EnableWindow( FALSE );
	GetDlgItem(IDC_BTN_SEND_TRACK)->EnableWindow( FALSE );
	GetDlgItem(IDC_BTN_STOP)->EnableWindow( FALSE );

	for(int i=0; i<MAX_CONNECTION; i++)
	{
		m_SocketManager[i].SetMessageWindow( &m_ctlMsgList );
		m_SocketManager[i].SetServerState( true );	// run as server
	}

	m_CameraUnkownStatus.SetCheck(true);
	//PickNextAvailable();
	m_trackXEditBox0.SetWindowTextA("0");
	m_trackYEditBox0.SetWindowTextA("0");
	m_trackXEditBox1.SetWindowTextA("0");
	m_trackYEditBox1.SetWindowTextA("0");
	m_trackXEditBox2.SetWindowTextA("0");
	m_trackYEditBox2.SetWindowTextA("0");
	m_trackXEditBox3.SetWindowTextA("0");
	m_trackYEditBox3.SetWindowTextA("0");
	m_trackXEditBox4.SetWindowTextA("0");
	m_trackYEditBox4.SetWindowTextA("0");
	m_trackXEditBox5.SetWindowTextA("0");
	m_trackYEditBox5.SetWindowTextA("0");
	m_laserXEditBox0.SetWindowTextA("0");
	m_laserYEditBox0.SetWindowTextA("0");
	m_trackXEditBox0.EnableWindow(false);
	m_trackYEditBox0.EnableWindow(false);
	m_trackXEditBox1.EnableWindow(false);
	m_trackYEditBox1.EnableWindow(false);
	m_trackXEditBox2.EnableWindow(false);
	m_trackYEditBox2.EnableWindow(false);
	m_trackXEditBox3.EnableWindow(false);
	m_trackYEditBox3.EnableWindow(false);
	m_trackXEditBox4.EnableWindow(false);
	m_trackYEditBox4.EnableWindow(false);
	m_trackXEditBox5.EnableWindow(false);
	m_trackYEditBox5.EnableWindow(false);
	m_laserXEditBox0.EnableWindow(false);
	m_laserYEditBox0.EnableWindow(false);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CServerSocketDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CServerSocketDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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

///////////////////////////////////////////////////////////////////////////////
// OnUpdateConnection
// This message is sent by server manager to indicate connection status
LRESULT CServerSocketDlg::OnUpdateConnection(WPARAM wParam, LPARAM lParam)
{
	UINT uEvent = (UINT) wParam;
	CSocketManager* pManager = reinterpret_cast<CSocketManager*>( lParam );

	// We need to do this only for TCP socket
	if (m_nSockType != SOCK_TCP)
		return 0L;

	if ( pManager != NULL)
	{
		// Server socket is now connected, we need to pick a new one
		if (uEvent == EVT_CONSUCCESS)
		{
			//PickNextAvailable();
			StartServer();
		}
		else if (uEvent == EVT_CONFAILURE || uEvent == EVT_CONDROP)
		{
			pManager->StopComm();
		/*	if (m_SocketManager[0] == NULL &&
				m_SocketManager[1] == NULL &&
				m_SocketManager[0] == NULL)
			{*/
				//PickNextAvailable();
				StartServer();
			//}
		}
	}

	return 1L;
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CServerSocketDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CServerSocketDlg::OnBtnStart() 
{
	UpdateData();

	StartServer();
}

void CServerSocketDlg::OnBtnStop() 
{
	// Disconnect all clients
	for(int i=0; i<MAX_CONNECTION; i++)
		m_SocketManager[i].StopComm();

	for(int i=0; i<MAX_CONNECTION; i++)
	{
		if (!m_SocketManager[i].IsOpen())
		{
			GetDlgItem(IDC_BTN_START)->EnableWindow( TRUE );
			PrevDlgCtrl();
			GetDlgItem(IDC_BTN_STOP)->EnableWindow( FALSE );
			GetDlgItem(IDC_TCP)->EnableWindow( TRUE );
			GetDlgItem(IDC_UDP)->EnableWindow( TRUE );
		}
	}
}


void CServerSocketDlg::OnBtnSendStatus()
{
	CString cstatus;									// serilize the message
	m_CameraStatusTextCtrl.GetWindowTextA(cstatus);

	// Get the current system time...
	time_t osBinaryTime;		// C run-time time (defined in <time.h>)
	time( &osBinaryTime ) ;		// Get the current time from the 
								// operating system.
	
	//Setup the camera message....
	cameraStatus status;
	status.set_laseron(m_statusLaserOnCtrl.GetCheck()==BST_CHECKED);	// set laser status
	status.set_status((cameraMsgs::systemStatus)m_CameraStatus);	// set camera status
	status.set_text(cstatus);				// set the camera text
	status.set_time(osBinaryTime);									// set the operational time
	string strText = status.SerializeAsString();
	OnBtnSend(strText,0,strText.size(),0);												// send the data
}
void CServerSocketDlg::OnBtnSendImage() 
{
	// Get the current system time...
	time_t osBinaryTime;		// C run-time time (defined in <time.h>)
	time( &osBinaryTime ) ;		// Get the current time from the 
								// operating system.
	
	//Setup the camera message....
	cameraImage image;
	image.set_time(osBinaryTime);									// set the operational time
	image.set_channels(3);											// should always be a RGB image
	image.set_sizex(tImageSize.cx);
	image.set_sizey(tImageSize.cy);
	image.set_imagedata(bytes, tImageSize.cx*tImageSize.cy*3);
	
	string strText = image.SerializeAsString();
	int size = image.ByteSize();
	OnBtnSend(strText,2,strText.size(),2);												// send the data
}
void CServerSocketDlg::OnBtnSendTrack() 
{
	// Get the current system time...
	time_t osBinaryTime;		// C run-time time (defined in <time.h>)
	time( &osBinaryTime ) ;		// Get the current time from the 
								// operating system.
	
	cameraTracks track;
	track.set_time(osBinaryTime);									// set the operational time
	track.set_laseron(m_statusLaserOnCtrl.GetCheck()==BST_CHECKED);	// set laser status
	
	AddTrack(&m_trackEnable0,&m_trackXEditBox0,&m_trackYEditBox0, &track);
	AddTrack(&m_trackEnable1,&m_trackXEditBox1,&m_trackYEditBox1, &track);
	AddTrack(&m_trackEnable2,&m_trackXEditBox2,&m_trackYEditBox2, &track);
	AddTrack(&m_trackEnable3,&m_trackXEditBox3,&m_trackYEditBox3, &track);
	AddTrack(&m_trackEnable4,&m_trackXEditBox4,&m_trackYEditBox4, &track);
	AddTrack(&m_trackEnable5,&m_trackXEditBox5,&m_trackYEditBox5, &track);
	AddLaser(&m_laserEnable0,&m_laserXEditBox0,&m_laserYEditBox0, &track);

	string strText = track.SerializeAsString();
	int size = track.ByteSize();
	OnBtnSend(strText,1,strText.size(),1);												// send the data
}
void CServerSocketDlg::AddLaser(CButton *buttom, CEdit *x, CEdit *y, cameraTracks *track)
{
	static CString tempString;
	if (buttom->GetCheck()==BST_CHECKED)
	{
		::cameraMsgs::track *cTrack = track->add_laser();
		x->GetWindowTextA(tempString);
		cTrack->set_x(atoi(tempString));
	    
		y->GetWindowTextA(tempString);
		cTrack->set_y(atoi(tempString));
	}
}

void CServerSocketDlg::AddTrack(CButton *buttom, CEdit *x, CEdit *y, cameraTracks *track)
{
	static CString tempString;
	int temp = 0;
	if (buttom->GetCheck()==BST_CHECKED)
	{
		::cameraMsgs::track *cTrack = track->add_target();
		
		x->GetWindowTextA(tempString);
		temp = atoi(tempString);
		cTrack->set_x(temp);

	    y->GetWindowTextA(tempString);
		temp = atoi(tempString);
		cTrack->set_y(temp);
	}
}

void CServerSocketDlg::OnBtnSend(string strText, int portOffset, int size,  int type) 
{
	int nLen = size;
	stMessageProxy msgProxy;

	if (nLen > 0)
	{
		USES_CONVERSION;
		//strText += _T("\r\n");
		if (m_nSockType == SOCK_UDP)
		{
			// send broadcast...
			msgProxy.address.CreateFrom(_T("255.255.255.255"), m_strPort);
			memcpy(msgProxy.byData, T2CA(strText.c_str()), nLen);
			nLen += msgProxy.address.Size();
		}
		else
		{
			nLen = __min(sizeof(msgProxy.byData)-1, nLen+1);
			memcpy(msgProxy.byData, T2CA(strText.c_str()), nLen);
		}

		// Send data to peer...
		if (m_nSockType == SOCK_UDP)
			m_SocketManager[portOffset].WriteComm((const LPBYTE)&msgProxy, nLen, INFINITE);
		else
		{
			unsigned char sizeArray[4] = {(nLen & 0xFF000000) >> 24,
										  (nLen & 0x00FF0000) >> 16,
										  (nLen & 0x0000FF00) >>  8,
										  (nLen & 0x000000FF) >>  0};
			m_SocketManager[portOffset].WriteComm(sizeArray, sizeof(unsigned char)*4, INFINITE);
			m_SocketManager[portOffset].WriteComm(msgProxy.byData, nLen, INFINITE);
		}
	}	
}

void CServerSocketDlg::OnDestroy() 
{
	for(int i=0; i<MAX_CONNECTION; i++)
	m_SocketManager[i].StopComm();

	CDialog::OnDestroy();
}


void CServerSocketDlg::OnBnClickedCameraStatusDown()
{
	m_CameraStatus = cameraMsgs::systemStatus::CAMERA_DOWN;
	// TODO: Add your control notification handler code here
}

void CServerSocketDlg::OnBnClickedCameraStatusReady()
{
	m_CameraStatus = cameraMsgs::systemStatus::CAMERA_READY;
	// TODO: Add your control notification handler code here
}

void CServerSocketDlg::OnBnClickedCameraStatusOperational()
{
	m_CameraStatus = cameraMsgs::systemStatus::CAMERA_OPERATIONAL;
	// TODO: Add your control notification handler code here
}

void CServerSocketDlg::OnBnClickedCameraStatusError()
{
	m_CameraStatus = cameraMsgs::systemStatus::CAMERA_ERROR;
	// TODO: Add your control notification handler code here
}

void CServerSocketDlg::OnBnClickedCameraStatusFailed()
{
	m_CameraStatus = cameraMsgs::systemStatus::CAMERA_FAILED;
	// TODO: Add your control notification handler code here
}

void CServerSocketDlg::OnBnClickedCameraStatusUnkown()
{
	m_CameraStatus = cameraMsgs::systemStatus::UNKNOWN;
	// TODO: Add your control notification handler code here
}

void CServerSocketDlg::OnBnClickedTrackEnable0()
{
	if(m_trackEnable0.GetCheck()==BST_CHECKED)
	{
		m_trackXEditBox0.EnableWindow(true);
		m_trackYEditBox0.EnableWindow(true);
	}
	else
	{
		m_trackXEditBox0.EnableWindow(false);
		m_trackYEditBox0.EnableWindow(false);
	}
}

void CServerSocketDlg::OnBnClickedTrackEnable1()
{
	if(m_trackEnable1.GetCheck()==BST_CHECKED)
	{
		m_trackXEditBox1.EnableWindow(true);
		m_trackYEditBox1.EnableWindow(true);
	}
	else
	{
		m_trackXEditBox1.EnableWindow(false);
		m_trackYEditBox1.EnableWindow(false);
	}
}

void CServerSocketDlg::OnBnClickedTrackEnable2()
{
	if(m_trackEnable2.GetCheck()==BST_CHECKED)
	{
		m_trackXEditBox2.EnableWindow(true);
		m_trackYEditBox2.EnableWindow(true);
	}
	else
	{
		m_trackXEditBox2.EnableWindow(false);
		m_trackYEditBox2.EnableWindow(false);
	}

}

void CServerSocketDlg::OnBnClickedTrackEnable3()
{
	if(m_trackEnable3.GetCheck()==BST_CHECKED)
	{
		m_trackXEditBox3.EnableWindow(true);
		m_trackYEditBox3.EnableWindow(true);
	}
	else
	{
		m_trackXEditBox3.EnableWindow(false);
		m_trackYEditBox3.EnableWindow(false);
	}
}

void CServerSocketDlg::OnBnClickedTrackEnable4()
{
	if(m_trackEnable4.GetCheck()==BST_CHECKED)
	{
		m_trackXEditBox4.EnableWindow(true);
		m_trackYEditBox4.EnableWindow(true);
	}
	else
	{
		m_trackXEditBox4.EnableWindow(false);
		m_trackYEditBox4.EnableWindow(false);
	}
}

void CServerSocketDlg::OnBnClickedTrackEnable5()
{
	if(m_trackEnable5.GetCheck()==BST_CHECKED)
	{
		m_trackXEditBox5.EnableWindow(true);
		m_trackYEditBox5.EnableWindow(true);
	}
	else
	{
		m_trackXEditBox5.EnableWindow(false);
		m_trackYEditBox5.EnableWindow(false);
	}
}

void CServerSocketDlg::OnBnClickedLaserEnable6()
{
	if(m_laserEnable0.GetCheck()==BST_CHECKED)
	{
		m_laserXEditBox0.EnableWindow(true);
		m_laserYEditBox0.EnableWindow(true);
	}
	else
	{
		m_laserXEditBox0.EnableWindow(false);
		m_laserYEditBox0.EnableWindow(false);
	}
}

void CServerSocketDlg::OnBnClickedBtnSelectCameraImage()
{
	// TODO: Add your control notification handler code here
	this->UpdateData();

	char strFilter[] = { "All Files (*.*)|*.*||" };

	CFileDialog FileDlg(TRUE, ".bcr", NULL, 0, strFilter);

	if( FileDlg.DoModal() == IDOK )
	{
		m_imageName = FileDlg.GetPathName();
		m_picCtrl.LoadFromFile(m_imageName);
		CImage  tImage;
		tImage.Load(m_imageName); // just change extension to load jpg
		
		int ll = 0;
		for(int row = 0; row < tImage.GetHeight(); row++)
		{
			for(int col = 0; col < tImage.GetWidth(); col++)
			{
				UPixel Pixel;
				Pixel.c=tImage.GetPixel(col,row);
				bytes[ll+0] = Pixel.chars.cRed;
				bytes[ll+1] = Pixel.chars.cGreen;
				bytes[ll+2] = Pixel.chars.cBlue;
				ll +=3;
			}
		}

		tImageSize.cx = tImage.GetWidth();
		tImageSize.cy = tImage.GetHeight();

		CImage img;
		img.Create(tImage.GetWidth(), tImage.GetHeight(), 24 /* bpp */, 0 /* No alpha channel */);
		ll =0;	
		int nPixel = 0;
		for(int row = 0; row < tImage.GetHeight(); row++)
		{
			for(int col = 0; col < tImage.GetWidth(); col++)
			{
				UPixel Pixel;
				Pixel.chars.cRed   = bytes[ll+0];
				Pixel.chars.cGreen = bytes[ll+1];
				Pixel.chars.cBlue  = bytes[ll+2];
				img.SetPixel(col,row,Pixel.c);
				ll +=3;
			}
		}
		img.Save("temp.bmp");
		return;
	}
	else
	{
		return;
	}
	this->UpdateData(FALSE);
}
