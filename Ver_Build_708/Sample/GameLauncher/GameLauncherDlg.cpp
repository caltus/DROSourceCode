// GameLauncherDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GameLauncher.h"
#include "GameLauncherDlg.h"
#include "../HspLauncherLib/HspLauncherLib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_PATH_LENGTH 1024
#define HSPLISTFILENAME			_T("\\HspL.exe")


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
// CGameLauncherDlg dialog

CGameLauncherDlg::CGameLauncherDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGameLauncherDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGameLauncherDlg)
	m_strTargetPath = _T("");
	m_strLauncherPath = _T("");
	m_strOption = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGameLauncherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGameLauncherDlg)
	DDX_Text(pDX, IDC_EDIT_TARGETPATH, m_strTargetPath);
	DDV_MaxChars(pDX, m_strTargetPath, 1024);
	DDX_Text(pDX, IDC_EDIT_LAUNCHERPATH, m_strLauncherPath);
	DDV_MaxChars(pDX, m_strLauncherPath, 1024);
	DDX_Text(pDX, IDC_EDIT_OPTION, m_strOption);
	DDV_MaxChars(pDX, m_strOption, 1024);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGameLauncherDlg, CDialog)
	//{{AFX_MSG_MAP(CGameLauncherDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_HSPL_START, OnBtnHsplStart)
	ON_BN_CLICKED(IDC_BTN_GAMESTART, OnBtnGamestart)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL  GetModulePath(HMODULE hModule, LPTSTR szPath, DWORD dwSize) 
{ 
    LPTSTR lastmark = NULL; 
    LPTSTR p = NULL; 
    DWORD  dwStrLen = 0; //length of the string that is copied to the buffer
  
//	assert( IsBadStringPtr( szPath, dwSize ) == 0 );

    if ( dwSize <= 0 ) 
	{
        return FALSE; 
	}

	// 버퍼길이로 계산 (ex 한글은 길이를 2로 계산)
    dwStrLen = GetModuleFileName(hModule, szPath, dwSize); 
   	if ( dwStrLen == 0) 
	{
		return FALSE; 
	}
    
	szPath[dwStrLen] = _T('\0'); 
    
	lastmark = NULL; 
	
	// 경로의 끝 글자.
	p = szPath + dwStrLen - 1;  
	

	while (*p) 
	{ 
        if (*p == _T('\\')) 
		{
            lastmark = p; 
			break;
		}

		// MBCS에서 문자에 '\'값과 같은값이 있을경우 오류발생.ex) '\' == 0x5c, '리' 0x83 0x5c
		// "lead bytes"는 항상 0x7F 보다 크다.
		// ex) Shift-JIS 인코딩에서는 (일본에서 가장 많이 사용되는 인코딩 방식) 0x81-0x9F 와 0xE0-0xFC 사이의 값은 캐릭터가 
		// 2바이트.
        p = CharPrev(szPath, p); 
    }

    if (lastmark) 
        *lastmark = _T('\0'); 

    return TRUE; 
} 

/////////////////////////////////////////////////////////////////////////////
// CGameLauncherDlg message handlers

BOOL CGameLauncherDlg::OnInitDialog()
{
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
	TCHAR szCurrentFilePath[MAX_PATH_LENGTH] = { 0, };

	// 현재 모듈경로 얻기
	if( GetModulePath( NULL, szCurrentFilePath, sizeof(szCurrentFilePath) ) == FALSE )
	{
		AfxMessageBox("GetModulePath fail");
		return FALSE;
	}
	TCHAR szLauncherPath[MAX_PATH_LENGTH] = { 0, };
	
	_stprintf(szLauncherPath, _T("%s%s"), szCurrentFilePath, HSPLISTFILENAME );
		
	m_strLauncherPath = szLauncherPath;
	m_strTargetPath = szCurrentFilePath;
	
	UpdateData(FALSE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGameLauncherDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CGameLauncherDlg::OnPaint() 
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

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGameLauncherDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CGameLauncherDlg::OnBtnHsplStart() 
{
	// TODO: Add your control notification handler code here
	DWORD dwRet = ERROR_SUCCESS;
	char szbuf[256] = { 0, };
	
	UpdateData(TRUE);

	// ================================================================
	// HackSheild Launcher(HspL.exe) Execute
	// =>The path of the HspL.exe must be same located in the GameRootPath 
	// ================================================================
	dwRet = _AhnHsp_StartLauncher( m_strLauncherPath ); // HspL full path.
	if( dwRet != ERROR_SUCCESS )
	{
		if( dwRet == ERROR_STARTLAUNCHER_WAITEVENT_FAIL )
		{
			sprintf(szbuf, "HackSheild not execute or the other reason[%x]", dwRet);
		}
		else
		{
			sprintf(szbuf, "_AhnHsp_StartLauncher fail : [%x]\n", dwRet);
		}

		AfxMessageBox(szbuf);
		return;	
	}
	
	AfxMessageBox("_AhnHsp_StartLauncher OK");
	return;
}

void CGameLauncherDlg::OnBtnGamestart() 
{
	// TODO: Add your control notification handler code here
	DWORD dwRet = ERROR_SUCCESS;
	char szFilePath[MAX_PATH] = { 0, }, szbuf[MAX_PATH] = { 0, };
	char szCommandLine[MAX_PATH] = { 0, };

	AHN_GAMEEXECINFO GameExecInfo;

	UpdateData( TRUE );
		
	// copy target file path.
	sprintf( GameExecInfo.szFilePath, "%s", m_strTargetPath );
	
	// copy game information.
	sprintf( GameExecInfo.szCommandLine, "%s", m_strOption);

	// ================================================================
	// Execute Game Program through the hackshield Launcher(HspL.exe)
	// ================================================================
	dwRet = _AhnHsp_StartGame( &GameExecInfo );
	if( dwRet != ERROR_SUCCESS )
	{
		
		if( dwRet == ERROR_STARTGAME_HSPL_NOTEXEC )
		{
			sprintf(szbuf, "Please execute HspL.exe", dwRet);
		}
		else
		{
			sprintf(szbuf, "_AhnHsp_StartGame fail : %x", dwRet);
		}

		AfxMessageBox(szbuf);

		return;
	}

	AfxMessageBox("_AhnHsp_StartGame OK ");
	return;
}


void CGameLauncherDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	_AhnHsp_CloseHandle();
}
