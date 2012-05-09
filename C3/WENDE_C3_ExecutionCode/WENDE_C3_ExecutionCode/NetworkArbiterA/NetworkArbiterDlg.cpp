
// NetworkArbiterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NetworkArbiter.h"
#include "NetworkArbiterDlg.h"
#include "TinyXml\tinyxml.h"

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

	// Determine if file exists
	CFile theFile;
	char* szFileName = "appsettings.xml";

	CFileStatus status;
	if( !CFile::GetStatus( szFileName, status ) )
	{
		m_Connection.ip = "192.168.1.65";
		m_Connection.port = 4444;
		// Create the file if it does not exist
		//WriteXMLFile();
	}
		m_Connection.ip = "192.168.1.65";
		m_Connection.port = 4444;
	
	//// Read in the file
	//TiXmlDocument doc(szFileName);
	//bool loadOkay = doc.LoadFile();
	//if (loadOkay)
	//{
	//		
	//	TiXmlHandle hDoc(&doc);
	//	TiXmlElement* pElem;
	//	TiXmlHandle hRoot(0);
	//	///////////////
	//	// block: name
	//	///////////////
	//	pElem=hDoc.FirstChildElement().Element();
	//	// should always have a valid root but handle gracefully if it does
	//	if (pElem)
	//	{
	//		CString name=pElem->Value();
	//		// save this for later
	//		hRoot=TiXmlHandle(pElem);
	//		/////////////////////
	//		// block: connection
	//		/////////////////////
	//		pElem=hRoot.FirstChild("Connection").Element();
	//		if (pElem)
	//		{
	//			m_Connection.ip   = pElem->Attribute("ip");
	//			m_Connection.port = atoi(pElem->Attribute("port"));	
	//		}
	//		else
	//		{
	//			// ERROR ????
	//		}
	//	}
	//	else
	//	{
	//		// ERROR ????
	//	}
	//}
	//else
	//{
	//	// ERROR ????	
	//}

	// set the dialog controls..
	CString port;
	port.Format(_T("%d"), m_Connection.port);
	m_AddressControlValue = htonl(inet_addr(m_Connection.ip.c_str()));
	m_AddressPort         = port;

	// start thread to bring up com scokets
	RunThread();

	return 0;
}

void CNetworkArbiterDlg::WriteXMLFile()
{
	// Clean up possible memory leaks... --- FIX ---
	TiXmlDocument doc;  
	TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" );  
	doc.LinkEndChild( decl );  

	TiXmlElement * root = new TiXmlElement( "NetworkArbiter" );  
	doc.LinkEndChild( root );  

	TiXmlComment * comment1 = new TiXmlComment();
	comment1->SetValue("Settings for the WENDE C3 Arbiter     " );  
	root->LinkEndChild( comment1 );  
	TiXmlComment * comment2 = new TiXmlComment();
	comment2->SetValue("IP Address of the server              " );  
	root->LinkEndChild( comment2 );
	TiXmlComment * comment3 = new TiXmlComment();
	comment3->SetValue("Port is the starting port for messages" );  
	root->LinkEndChild( comment3 );  

	TiXmlElement * cxn = new TiXmlElement( "Connection" );  
	root->LinkEndChild( cxn );  
	cxn->SetAttribute("ip", m_Connection.ip.c_str());
	cxn->SetAttribute("port", m_Connection.port); // floating point attrib

	doc.SaveFile( "appsettings.xml" ); 
}
void CNetworkArbiterDlg::RunThread()
{
	HANDLE hThread;
	UINT uiThreadId = 0;
	hThread = (HANDLE)_beginthreadex(NULL,				       // Security attributes
										0,					  // stack
									 SocketStartThreadProc,   // Thread proc
									 this,					  // Thread param
									 CREATE_SUSPENDED,		  // creation mode
									 &uiThreadId);			  // Thread ID

	if ( NULL != hThread)
	{
		//SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL);
		ResumeThread( hThread );
		m_hThread = hThread;
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
UINT WINAPI CNetworkArbiterDlg::SocketStartThreadProc(LPVOID pParam)
{
	CNetworkArbiterDlg* pThis = reinterpret_cast<CNetworkArbiterDlg*>( pParam );
	// set the dialog controls..
	CString port;
	port.Format(_T("%d"), pThis->m_Connection.port);
	
	for (int ii = 0; ii < SOCKET_COUNTS; ii++)
	{
		switch(ii)
		{
			case 0: 
			{
				pThis->m_SocketObject[ii].SetCameraMessageType(CameraPacketType::status);
				break;
			}
			case 1: 
			{
				pThis->m_SocketObject[ii].SetCameraMessageType(CameraPacketType::track);
				break;
			}
			case 2: 
			{
				pThis->m_SocketObject[ii].SetCameraMessageType(CameraPacketType::image);
				break;
			}
			default:
			{
				break;
			}
		}
		// Set the display event
		pThis->m_SocketObject[ii].SetMessageWindow( &pThis->m_pMsgCtrl );

		while (!pThis->m_SocketObject[ii].IsOpen())
		{
			// To use TCP socket
			port.Format(_T("%d"), pThis->m_Connection.port+ii);
			pThis->m_SocketObject[ii].ConnectTo( pThis->m_Connection.ip.c_str(), port, AF_INET, SOCK_STREAM); // TCP
		}
		
		// Now you may start the server/client thread to do the work for you...
		pThis->m_SocketObject[ii].WatchComm();
	}
	
	pThis->m_hThread = NULL;
	
	_endthreadex( 0 );
    
	return 1L;
} // end SocketThreadProc

void CNetworkArbiterDlg::OnBnClickedUpdateTcp()
{
	// Stop the thread that is running....
	if ( NULL != m_hThread)
	{
		if(TerminateThread(m_hThread, 0) == FALSE)
		{
			// Could not force thread to exit -> call 'GetLastError()'
		}
	}
	// stop all COMS...
	for (int ii = 0; ii < SOCKET_COUNTS; ii++)
	{
		m_SocketObject[ii].StopComm();
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

	m_Connection.ip = inet_ntoa(addr);
}

void CNetworkArbiterDlg::OnEnChangePort()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	m_PortCtrl.GetWindowTextA(m_AddressPort);
	m_Connection.port = atoi(m_AddressPort);
}
