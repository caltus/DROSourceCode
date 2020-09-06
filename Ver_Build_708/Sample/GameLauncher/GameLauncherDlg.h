// GameLauncherDlg.h : header file
//

#if !defined(AFX_GAMELAUNCHERDLG_H__D01E8AF1_4021_4F8D_BFD5_4FADB4D16E45__INCLUDED_)
#define AFX_GAMELAUNCHERDLG_H__D01E8AF1_4021_4F8D_BFD5_4FADB4D16E45__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CGameLauncherDlg dialog

class CGameLauncherDlg : public CDialog
{
// Construction
public:
	CGameLauncherDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CGameLauncherDlg)
	enum { IDD = IDD_GAMELAUNCHER_DIALOG };
	CString	m_strTargetPath;
	CString	m_strLauncherPath;
	CString	m_strOption;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGameLauncherDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CGameLauncherDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnHsplStart();
	afx_msg void OnBtnGamestart();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GAMELAUNCHERDLG_H__D01E8AF1_4021_4F8D_BFD5_4FADB4D16E45__INCLUDED_)
