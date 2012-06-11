// CPictureCtrlDemoDlg.h : Headerdatei
//

#pragma once
#include "picturectrl.h"


// CCPictureCtrlDemoDlg-Dialogfeld
class CCPictureCtrlDemoDlg : public CDialog
{
// Konstruktion
public:
	CCPictureCtrlDemoDlg(CWnd* pParent = NULL);	// Standardkonstruktor

// Dialogfelddaten
	enum { IDD = IDD_CPICTURECTRLDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV-Unterstützung


// Implementierung
protected:
	HICON m_hIcon;

	// Generierte Funktionen für die Meldungstabellen
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
// 	afx_msg void OnBnClickedButton4();
	CPictureCtrl m_picCtrl;
};
