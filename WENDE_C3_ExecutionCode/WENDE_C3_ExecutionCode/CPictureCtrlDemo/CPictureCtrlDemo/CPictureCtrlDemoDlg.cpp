// CPictureCtrlDemoDlg.cpp : Implementierungsdatei
//

#include "stdafx.h"
#include "CPictureCtrlDemo.h"
#include "PictureCtrl.h"
#include "CPictureCtrlDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg-Dialogfeld für Anwendungsbefehl "Info"

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialogfelddaten
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung

// Implementierung
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


// CCPictureCtrlDemoDlg-Dialogfeld




CCPictureCtrlDemoDlg::CCPictureCtrlDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCPictureCtrlDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCPictureCtrlDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PICTURE, m_picCtrl);
}

BEGIN_MESSAGE_MAP(CCPictureCtrlDemoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CCPictureCtrlDemoDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CCPictureCtrlDemoDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CCPictureCtrlDemoDlg::OnBnClickedButton3)
/*	ON_BN_CLICKED(IDC_BUTTON4, &CCPictureCtrlDemoDlg::OnBnClickedButton4)*/
END_MESSAGE_MAP()


// CCPictureCtrlDemoDlg-Meldungshandler

BOOL CCPictureCtrlDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Hinzufügen des Menübefehls "Info..." zum Systemmenü.

	// IDM_ABOUTBOX muss sich im Bereich der Systembefehle befinden.
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

	// Symbol für dieses Dialogfeld festlegen. Wird automatisch erledigt
	//  wenn das Hauptfenster der Anwendung kein Dialogfeld ist
	SetIcon(m_hIcon, TRUE);			// Großes Symbol verwenden
	SetIcon(m_hIcon, FALSE);		// Kleines Symbol verwenden

	// TODO: Hier zusätzliche Initialisierung einfügen

	return TRUE;  // Geben Sie TRUE zurück, außer ein Steuerelement soll den Fokus erhalten
}

void CCPictureCtrlDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// Wenn Sie dem Dialogfeld eine Schaltfläche "Minimieren" hinzufügen, benötigen Sie 
//  den nachstehenden Code, um das Symbol zu zeichnen. Für MFC-Anwendungen, die das 
//  Dokument/Ansicht-Modell verwenden, wird dies automatisch ausgeführt.

void CCPictureCtrlDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // Gerätekontext zum Zeichnen

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Symbol in Clientrechteck zentrieren
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Symbol zeichnen
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// Die System ruft diese Funktion auf, um den Cursor abzufragen, der angezeigt wird, während der Benutzer
//  das minimierte Fenster mit der Maus zieht.
HCURSOR CCPictureCtrlDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CCPictureCtrlDemoDlg::OnBnClickedButton1()
{
	//Load an Image from File
	m_picCtrl.Load(CString(_T("pic1.jpg")));
}

void CCPictureCtrlDemoDlg::OnBnClickedButton2()
{
	//Load an Image from a Stream

	//Create the Stream
	IStream* pStream = NULL;
	if(CreateStreamOnHGlobal(NULL, TRUE, &pStream) == S_OK)
	{

		//Load the file ... Internet source ... etc.
		CFile picFile;
		if(picFile.Open(_T("pic2.gif"), CFile::modeRead | CFile::typeBinary))
		{
			BYTE pBuffer[1024];
			UINT uiRead = 0;

			while((uiRead = picFile.Read(pBuffer, sizeof(pBuffer))) != 0)
			{
				ULONG ulWritten = 0;
				pStream->Write(pBuffer, uiRead, &ulWritten);
			}

			//Load the Image
			m_picCtrl.Load(pStream);
		}
		while(pStream->Release());
	}
}

void CCPictureCtrlDemoDlg::OnBnClickedButton3()
{
	//Load from a Byte Stream

	//Load the file ... Internet source ... etc.
	CFile picFile;
	if(picFile.Open(_T("pic3.png"), CFile::modeRead | CFile::typeBinary))
	{
		BYTE* pBuffer = new BYTE[(unsigned int)picFile.GetLength()];
		if(pBuffer != NULL)
		{
			picFile.Read(pBuffer, (UINT)picFile.GetLength());

			//Load the Image
			m_picCtrl.Load(pBuffer, (size_t)picFile.GetLength());

			delete pBuffer;
		}
	}
}

// void CCPictureCtrlDemoDlg::OnBnClickedButton4()
// {
// 	//Load from a Resource
// 	m_picCtrl.Load(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP_PIC4), RT_BITMAP);
// }
