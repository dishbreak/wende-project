
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
}

BEGIN_MESSAGE_MAP(CNetworkArbiterDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS1, &CNetworkArbiterDlg::OnIpnFieldchangedIpaddress1)
	ON_WM_CREATE()
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


void CNetworkArbiterDlg::OnIpnFieldchangedIpaddress1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
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
		// Create the file if it does not exist
		WriteXMLFile();
	}
	
	// Read in the file
	TiXmlDocument doc(szFileName);
	bool loadOkay = doc.LoadFile();
	if (loadOkay)
	{
			
		TiXmlHandle hDoc(&doc);
		TiXmlElement* pElem;
		TiXmlHandle hRoot(0);
		///////////////
		// block: name
		///////////////
		pElem=hDoc.FirstChildElement().Element();
		// should always have a valid root but handle gracefully if it does
		if (pElem)
		{
			CString name=pElem->Value();
			// save this for later
			hRoot=TiXmlHandle(pElem);
			/////////////////////
			// block: connection
			/////////////////////
			pElem=hRoot.FirstChild("Connection").Element();
			if (pElem)
			{
				m_Connection.ip   = pElem->Attribute("ip");
				m_Connection.port = atoi(pElem->Attribute("port"));	
			}
			else
			{
				// ERROR ????
			}
		}
		else
		{
			// ERROR ????
		}
	}
	else
	{
		// ERROR ????	
	}

	// set the dialog controls..
	CString port;
	port.Format(_T("%d"), m_Connection.port);
	m_AddressControlValue = htonl(inet_addr(m_Connection.ip.c_str()));
	m_AddressPort         = port;

	for (int ii = 0; ii < 1/*SOCKET_COUNTS*/; ii++)
	{
		switch(ii)
		{
			case 1: 
			{
				m_SocketObject[ii].SetCameraMessageType(CameraPacketType::status);
				break;
			}
			case 2: 
			{
				m_SocketObject[ii].SetCameraMessageType(CameraPacketType::track);
				break;
			}
			case 3: 
			{
				m_SocketObject[ii].SetCameraMessageType(CameraPacketType::image);
				break;
			}
			default:
			{
				break;
			}
		}
		// Set the display event
		m_SocketObject[ii].SetMessageWindow( &m_pMsgCtrl );

		while (!m_SocketObject[ii].IsOpen())
		{
			// To use TCP socket
			port.Format(_T("%d"), m_Connection.port+ii);
			m_SocketObject[ii].ConnectTo( m_Connection.ip.c_str(), port, AF_INET, SOCK_STREAM); // TCP
		}
		
		// Now you may start the server/client thread to do the work for you...
		m_SocketObject[ii].WatchComm();
	}
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
	cxn->SetAttribute("ip", "192.168.1.70");
	cxn->SetAttribute("port", 4444); // floating point attrib

	doc.SaveFile( "appsettings.xml" ); 
}