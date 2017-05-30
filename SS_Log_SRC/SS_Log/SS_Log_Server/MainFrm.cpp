// MainFrm.cpp : implementation of the CMainFrame class
// Version 3.02

#include "stdafx.h"
#include "SS_Log_Server.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
    m_bFirstShowHasBeenHidden = FALSE;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndDlgBar.Create(this, IDR_MAINFRAME,
		CBRS_ALIGN_TOP, AFX_IDW_DIALOGBAR))
	{
		TRACE0("Failed to create dialogbar\n");
		return -1;		// fail to create
    }


	if (!m_wndReBar.Create(this) ||
		!m_wndReBar.AddBar(&m_wndToolBar) ||
		!m_wndReBar.AddBar(&m_wndDlgBar))
	{
		TRACE0("Failed to create rebar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
/*
	// TODO: Remove this if you don't want tool tips
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);
*/
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
    LONG nNegVisible = (~WS_VISIBLE);
    cs.style = cs.style & nNegVisible;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CFrameWnd::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here

}

LRESULT CMainFrame::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
    LPWINDOWPOS lpwp = NULL;

    switch( message )
    {
    case WM_WINDOWPOSCHANGING:
        lpwp = (LPWINDOWPOS) lParam;
        if( lpwp->flags & SWP_SHOWWINDOW && m_bFirstShowHasBeenHidden == FALSE )
        {
            lpwp->flags &= ~SWP_SHOWWINDOW;
            m_bFirstShowHasBeenHidden = TRUE;
        }
        break;

    default:
        break;
    }

	return CFrameWnd::DefWindowProc(message, wParam, lParam);
}

void CMainFrame::OnClose()
{
	// Here we will eliminate a memory leak in the pipe listening
    // thread in SSLogConnection.  That thread may be sitting there
    // waiting for a connection.  We should connect to it and send
    // a message telling the thread to shutdown.
    HANDLE hPipe = INVALID_HANDLE_VALUE;
    TCHAR szPipeName[MAX_PATH];
    SS_String sServerName;
    CString csServerName;
    GetWindowText(csServerName);
    sServerName = csServerName;
    if( sServerName.Find(_T(" -- Pipe Closed")) == _T("") ) // is listening thread still running?
    {
        _stprintf( szPipeName, _T("\\\\.\\pipe\\%s"), sServerName.GetBufferConst() );

        hPipe = ::CreateFile(szPipeName, GENERIC_WRITE, 0, NULL,
            OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if( hPipe != INVALID_HANDLE_VALUE )
        {
            SS_String sMessage;
            DWORD dwFilter = LOGTYPE_LOGTOWINDOW|LOGTYPE_LOGTOFILE|LOGTYPE_LOGTOEVENTLOG;
            sMessage.Format(_T("%d\t%s\t%s\t%d\t%d\t%s"),
                MSGTYPE_SHUTDOWN_SERVER, _T(""), _T(""), 0, dwFilter, _T(""));

            DWORD dwBytesWritten;
            ::WriteFile( hPipe, sMessage.GetBufferConst(), sMessage.Length()+1,
                &dwBytesWritten, NULL );
            ::CloseHandle(hPipe);
        }
    }

	CFrameWnd::OnClose();
}
