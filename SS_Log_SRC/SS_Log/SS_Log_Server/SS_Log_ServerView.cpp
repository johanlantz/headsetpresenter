// SS_Log_ServerView.cpp : implementation of the SS_Log_ServerView class
// Version 3.02

#include "stdafx.h"
#include <winspool.h>
#include "SS_Log_Server.h"

#include "SS_Log_Server.h"
#include "SS_Log_ServerDoc.h"
#include "SS_Log_ServerView.h"
#include "SS_FilterSettingsDlg.h"
#include "SSFilterSettings.h"
#include "MainFrm.h"
#include "SS_String.h"
#include "SS_RegistryKey.h"
#include "CPage.h"
#include "SSLogDestination.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern SS_Log_ServerApp theApp;


struct FindMatchingInOutStruct
{
    SS_Log_ServerView* pWnd;
    UINT iItem;
};

DWORD WINAPI FindMatchingInOutThread( LPVOID pParam )
{
    UINT nr = TRUE;

    Sleep(100);
    FindMatchingInOutStruct* pInfo = (FindMatchingInOutStruct*)pParam;
    BOOL bMatchFound = FALSE;
    CListCtrl* pListCtrl = &(pInfo->pWnd->GetListCtrl());
    INT i = 0;
    SS_String sMsg1;
    SS_String sMsg2;
    SS_String sID1;
    SS_String sID2;
    pListCtrl->GetItemText(pInfo->iItem, SSLW_COLUMN_MESSAGE, sMsg1.GetBuffer(256), 256);
    sMsg1.ReleaseBuffer();
    sID1 = sMsg1.Find(_T("ID: "));

    if( sID1 != "" )
    {
        for( i=0; i<pListCtrl->GetItemCount(); i++ )
        {
            pListCtrl->GetItemText(i, SSLW_COLUMN_MESSAGE, sMsg2.GetBuffer(256), 256);
            sMsg2.ReleaseBuffer();
            sID2 = sMsg2.Find(_T("ID: "));
            if( sID1 == sID2 && sMsg1 != sMsg2 )
            {
                bMatchFound = TRUE;
                break;
            }
        }
    }

    if( bMatchFound == TRUE )
    {
        UINT nState = pListCtrl->GetItemState(i, LVIF_STATE);
        nState |= LVIS_SELECTED;
        pListCtrl->SetItemState(i, nState, LVIF_STATE|LVIS_SELECTED);
    }

    delete pInfo;
    pInfo = NULL;

    return nr;
}


// ----------------------------------------------------------------------- //
//  Function:		EnumWindowsProc()
//  Author:			Steve Schaneville
//  Parameters:		hwnd [in] - Handle to the found window (see MSDN).
//                  lParam [out] - pointer to an HWND object so we can
//                  return the found-window's window handle.
//  Return Values:	(see MSDN)
//  Notes:			Looking for an instance of "Microsoft Visual C++".
//                  If we find it, return the associated window handle
//                  via the lParam parameter.
// ----------------------------------------------------------------------- //
BOOL CALLBACK EnumWindowsProc( HWND hwnd, LPARAM lParam )
{
    HWND* phWnd = (HWND*)lParam;

    TCHAR szText[256];
    TCHAR* szTextBuff = szText;
    DWORD dwResult = 0;
    LRESULT status;
    status = SendMessageTimeout(hwnd, WM_GETTEXT, 256, (LPARAM) &szText[0], SMTO_ABORTIFHUNG, 1000, &dwResult); // new
    //GetWindowText(hwnd, szText, 256);

    while( TRUE )
    {
        int nResult = strcspn( szTextBuff, "Microsoft Visual C++" );
        if( _tcslen(szTextBuff) == (UINT)nResult )
            break;

        szTextBuff += nResult;
        if( !_tcsncmp(szTextBuff, "Microsoft Visual C++", _tcslen("Microsoft Visual C++")) )
        {
            *phWnd = hwnd;
            return FALSE;
        }

        szTextBuff++;
    }

    return TRUE;
}

// helps us pass some information to the ListCtrlSortItems() function
typedef struct tagSortItemsInfo
{
    CListCtrl*  pListCtrl;
    int         nColumn;
} SORT_ITEMS_INFO, *LPSORT_ITEMS_INFO;

// this helps us sort any column reverse or forward when the user clicks a
// column heading multiple times.  Out of laziness, the variable is used for
// all columns instead of having a separate variable for each column.
BOOL g_bReverseSort = TRUE;

// ----------------------------------------------------------------------- //
//  Function:		ListCtrlSortItems()
//  Author:			Steve Schaneville
//  Parameters:		(see MSDN)
//  Return Values:	(see MSDN)
//  Notes:			We are sorting the columns here when the user clicks
//                  on a column heading.  There are different comparisons
//                  depending on which column is clicked.
// ----------------------------------------------------------------------- //
int CALLBACK ListCtrlSortItems(LPARAM lParam1, LPARAM lParam2,
                         LPARAM lParamSort)
{
    LPSORT_ITEMS_INFO pSII = (LPSORT_ITEMS_INFO)lParamSort;
    CListCtrl* pList = pSII->pListCtrl;

    LVFINDINFO fi;
    fi.flags = LVFI_PARAM;
    fi.lParam = lParam1;
    int nItem1 = pList->FindItem(&fi);

    fi.flags = LVFI_PARAM;
    fi.lParam = lParam2;
    int nItem2 = pList->FindItem(&fi);

    if( -1 == nItem1 || -1 == nItem2 )
        return 0;

    CString cs1 = pSII->pListCtrl->GetItemText(nItem1, pSII->nColumn);
    CString cs2 = pSII->pListCtrl->GetItemText(nItem2, pSII->nColumn);

    int nResult = 0;
    switch( pSII->nColumn )
    {
    case SSLW_COLUMN_TIMEDATE:
        {
            SYSTEMTIME st1;
            SYSTEMTIME st2;
            FILETIME ft1;
            FILETIME ft2;

            st1.wHour   = (WORD)(_ttoi(cs1.Mid( 0,2).GetBuffer(10)));
            st1.wMinute = (WORD)(_ttoi(cs1.Mid( 3,2).GetBuffer(10)));
            st1.wSecond = (WORD)(_ttoi(cs1.Mid( 6,2).GetBuffer(10)));
            st1.wDay    = (WORD)(_ttoi(cs1.Mid(10,2).GetBuffer(10)));
            st1.wMonth  = (WORD)(_ttoi(cs1.Mid(13,2).GetBuffer(10)));
            st1.wYear   = (WORD)(_ttoi(cs1.Mid(16,4).GetBuffer(10)));
            st1.wMilliseconds = 0;

            st2.wHour   = (WORD)(_ttoi(cs2.Mid( 0,2).GetBuffer(10)));
            st2.wMinute = (WORD)(_ttoi(cs2.Mid( 3,2).GetBuffer(10)));
            st2.wSecond = (WORD)(_ttoi(cs2.Mid( 6,2).GetBuffer(10)));
            st2.wDay    = (WORD)(_ttoi(cs2.Mid(10,2).GetBuffer(10)));
            st2.wMonth  = (WORD)(_ttoi(cs2.Mid(13,2).GetBuffer(10)));
            st2.wYear   = (WORD)(_ttoi(cs2.Mid(16,4).GetBuffer(10)));
            st2.wMilliseconds = 0;

            SystemTimeToFileTime( &st1, &ft1 );
            SystemTimeToFileTime( &st2, &ft2 );

            nResult = CompareFileTime( &ft1, &ft2 );
        }
    	break;
    case SSLW_COLUMN_LINE:
    case SSLW_COLUMN_ENTRY:
        {
            int nLine1 = _ttoi(cs1.GetBuffer(10));
            int nLine2 = _ttoi(cs2.GetBuffer(10));
            nResult = nLine1 < nLine2 ? -1 : 1;
            if( nLine1 == nLine2 )
                nResult = 0;
        }
        break;
    case SSLW_COLUMN_MESSAGE:
    case SSLW_COLUMN_FILE:
    case SSLW_COLUMN_LEVEL:
        {
            nResult = cs1.Compare(cs2);
        }
        break;
    default:
        break;
    }

    // reverse the order if it is time
    if( g_bReverseSort )
        if( nResult )
            nResult = nResult == -1 ? 1 : -1;

        return nResult;
}


/////////////////////////////////////////////////////////////////////////////
// SS_Log_ServerView

IMPLEMENT_DYNCREATE(SS_Log_ServerView, CListView)

BEGIN_MESSAGE_MAP(SS_Log_ServerView, CListView)
//{{AFX_MSG_MAP(SS_Log_ServerView)
	ON_NOTIFY_REFLECT(LVN_INSERTITEM, OnInsertItem)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnClick)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDoubleClick)
	ON_COMMAND(ID_EDIT_FILTERSETTINGS, OnEditFilterSettings)
	ON_COMMAND(ID_VIEW_REFRESH, OnViewRefresh)
	ON_COMMAND(ID_EDIT_LISTVIEW_FILTER, OnEditListviewFilter)
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnItemChanged)
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGING, OnItemChanging)
	//}}AFX_MSG_MAP
// Standard printing commands
ON_COMMAND(ID_FILE_PRINT, CListView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_DIRECT, CListView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_PREVIEW, CListView::OnFilePrintPreview)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SS_Log_ServerView construction/destruction

SS_Log_ServerView::SS_Log_ServerView() :
    m_bInitialized(FALSE), m_pFilterSettings(NULL), m_pFilterSettingsDlg(NULL),
    m_bProcessingInOutMatches(FALSE)
{
    // TODO: add construction code here
}

SS_Log_ServerView::~SS_Log_ServerView()
{
	delete m_pFilterSettingsDlg;
	delete m_pFilterSettings;
}

BOOL SS_Log_ServerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
    //  the CREATESTRUCT cs
    cs.style |= WS_CHILD|WS_VISIBLE|LVS_REPORT;
    cs.style &= ~LVS_SINGLESEL;
    cs.dwExStyle |= LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|LVS_EX_TRACKSELECT;

	return CListView::PreCreateWindow(cs);
}

void SS_Log_ServerView::Serialize(CArchive& ar)
{
	UNREFERENCED_PARAMETER(ar);
}

void SS_Log_ServerView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	SS_String sKeyPath = GetDocument()->RegKey();
	SS_String sKeyCopyFrom = sKeyPath;
	sKeyPath ^= SSLOG_REG_LISTVIEW_SUBKEY;
	sKeyCopyFrom ^= SSLOG_REG_FILTERS_SUBKEY;
	delete m_pFilterSettings;
	m_pFilterSettings = new SSFilterSettings(sKeyPath, TRUE);
	m_pFilterSettings->InitFilters(sKeyCopyFrom);

    // create our columns and the pipe thread
    if( !m_bInitialized )
	{
        GetListCtrl().InsertColumn(SSLW_COLUMN_ENTRY, _T("ID"), LVCFMT_LEFT, 45, -1);
        GetListCtrl().InsertColumn(SSLW_COLUMN_LEVEL, _T("Level"), LVCFMT_LEFT, 100, -1);
		GetListCtrl().InsertColumn(SSLW_COLUMN_MESSAGE, _T("Message"), LVCFMT_LEFT, 350, -1);
		GetListCtrl().InsertColumn(SSLW_COLUMN_TIMEDATE, _T("Time, Date"), LVCFMT_LEFT, 170, -1);
		GetListCtrl().InsertColumn(SSLW_COLUMN_FILE, _T("File"), LVCFMT_LEFT, 200, -1);
		GetListCtrl().InsertColumn(SSLW_COLUMN_LINE, _T("Line #"), LVCFMT_LEFT, 60, -1);

        GetListCtrl().SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|
                                       LVS_EX_INFOTIP|LVS_EX_ONECLICKACTIVATE);

		m_bInitialized = TRUE;
	}
}

/////////////////////////////////////////////////////////////////////////////
// SS_Log_ServerView printing

BOOL SS_Log_ServerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
    return DoPreparePrinting(pInfo);
}

void SS_Log_ServerView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	UNREFERENCED_PARAMETER(pDC);

	// TODO: add extra initialization before printing
    pInfo->SetMaxPage(GetListCtrl().GetItemCount()/30+1);
    LPDEVMODE dm = pInfo->m_pPD->GetDevMode();
    dm->dmOrientation = DMORIENT_LANDSCAPE;
}

void SS_Log_ServerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// SS_Log_ServerView diagnostics

#ifdef _DEBUG
void SS_Log_ServerView::AssertValid() const
{
	CListView::AssertValid();
}

void SS_Log_ServerView::Dump(CDumpContext& dc) const
{
    CListView::Dump(dc);
}

SS_Log_ServerDoc* SS_Log_ServerView::GetDocument() // non-debug version is inline
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(SS_Log_ServerDoc)));
    return (SS_Log_ServerDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// SS_Log_ServerView message handlers


void SS_Log_ServerView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	UNREFERENCED_PARAMETER(pSender);
	UNREFERENCED_PARAMETER(lHint);
	UNREFERENCED_PARAMETER(pHint);
}

// ----------------------------------------------------------------------- //
//  Function:		SS_Log_ServerView::OnInsertItem()
//  Author:			Steve Schaneville
//  Parameters:		(see MSDN)
//  Return Values:	(see MSDN)
//  Notes:			here we just increment the "item count" displayed
//                  in the status bar.
// ----------------------------------------------------------------------- //
void SS_Log_ServerView::OnInsertItem(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    UNREFERENCED_PARAMETER(pNMListView);

    // TODO: Add your control notification handler code here
    TCHAR szStatus[256];
    sprintf( szStatus, _T("Log Entry Count: %d"), GetListCtrl().GetItemCount() );
    int i;
    i = ((CMainFrame*)GetTopLevelFrame())->GetStatusBar()->SetPaneText(ID_SEPARATOR, szStatus);

    *pResult = 0;
}


void SS_Log_ServerView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: Add your specialized code here and/or call the base class
    if(pDC->IsPrinting())
    {
        TCHAR szPrinter[MAX_PATH];
        //GetDefaultPrinter(szPrinter, &dwSize);
        GetProfileString( "windows", "device", "", szPrinter, MAX_PATH );
        _tcstok(szPrinter, ",");

        lpDevMode = ChangeDevMode(m_hWnd,szPrinter);
        pDC->ResetDC(lpDevMode);
    }
    CListView::OnPrepareDC(pDC, pInfo);
}

LPVOID SS_Log_ServerView::MyAllocMem(DWORD cb)
{
    return VirtualAlloc(NULL, cb, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
}

LPDEVMODE SS_Log_ServerView::ChangeDevMode(HWND hWnd, char *pDevice)
{
   HANDLE      hPrinter;
   LPDEVMODE   pDevMode;
   DWORD       dwNeeded, dwRet;

   /* Start by opening the printer */
   if (!OpenPrinter(pDevice, &hPrinter, NULL))
       return NULL;

   /*
    * Step 1:
    * Allocate a buffer of the correct size.
    */
   dwNeeded = DocumentProperties(hWnd,
       hPrinter,       /* handle to our printer */
       pDevice,        /* Name of the printer */
       NULL,           /* Asking for size so */
       NULL,           /* these are not used. */
       0);             /* Zero returns buffer size. */
   pDevMode = (LPDEVMODE)malloc(dwNeeded);

   /*
    * Step 2:
    * Get the default DevMode for the printer and
    * modify it for our needs.
    */
   dwRet = DocumentProperties(hWnd,
       hPrinter,
       pDevice,
       pDevMode,       /* The address of the buffer to fill. */
       NULL,           /* Not using the input buffer. */
       DM_OUT_BUFFER); /* Have the output buffer filled. */
   if (dwRet != IDOK)
   {
       /* if failure, cleanup and return failure */
       free(pDevMode);
       ClosePrinter(hPrinter);
       return NULL;
   }

   /*
   * Make changes to the DevMode which are supported.
   */
   if (pDevMode->dmFields & DM_ORIENTATION)
   {
       /* if the printer supports paper orientation, set it*/
       pDevMode->dmOrientation = DMORIENT_LANDSCAPE;
   }
   /*
   * Step 3:
   * Merge the new settings with the old.
   * This gives the driver a chance to update any private
   * portions of the DevMode structure.
   */
   dwRet = DocumentProperties(hWnd,
       hPrinter,
       pDevice,
       pDevMode,       /* Reuse our buffer for output. */
       pDevMode,       /* Pass the driver our changes. */
       DM_IN_BUFFER |  /* Commands to Merge our changes and */
       DM_OUT_BUFFER); /* write the result. */

   /* Done with the printer */
   ClosePrinter(hPrinter);

   if (dwRet != IDOK)
   {
       /* if failure, cleanup and return failure */
       free(pDevMode);
       return NULL;
   }

   /* return the modified DevMode structure */
   return pDevMode;

}

// ----------------------------------------------------------------------- //
//  Function:		SS_Log_ServerView::OnColumnClick()
//  Author:			Steve Schaneville
//  Parameters:		(see MSDN)
//  Return Values:	(see MSDN)
//  Notes:			This is for sorting.  See the global ListCtrlSortItems()
//                  function at the top of this file.
// ----------------------------------------------------------------------- //
void SS_Log_ServerView::OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    // TODO: Add your control notification handler code here
    SORT_ITEMS_INFO sii;
    sii.nColumn = pNMListView->iSubItem;
    sii.pListCtrl = &GetListCtrl();

    g_bReverseSort = !g_bReverseSort;
    GetListCtrl().SortItems(ListCtrlSortItems, (LPARAM)&sii);

    *pResult = 0;
}

// ----------------------------------------------------------------------- //
//  Function:		SS_Log_ServerView::OnDoubleClick()
//  Author:			Steve Schaneville
//  Parameters:		(see MSDN)
//  Return Values:	(see MSDN)
//  Notes:			here we are going to send a message to an open instance
//                  of MSDEV to open a file and go to the correct line number.
//                  If there is no open instance of MSDEV, open one.  If
//                  the SS_Log_AddIn is not installed, let the user know
//                  and offer to install it.  Do the install.
// ----------------------------------------------------------------------- //
void SS_Log_ServerView::OnDoubleClick(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    // prepare the message for the MSDEV window
    TCHAR szCommandLine[MAX_PATH];
    sprintf( szCommandLine, _T("\"%s\";%s"),
        GetListCtrl().GetItemText(pNMListView->iItem, SSLW_COLUMN_FILE).GetBuffer(10),
        GetListCtrl().GetItemText(pNMListView->iItem, SSLW_COLUMN_LINE).GetBuffer(10));

	// Check to see if the SS_Log_AddIn is properly installed.  Prompt the
    // user for installation if necessary.
    BOOL bShowMessage = FALSE;
    SS_RegistryKey key(HKEY_CURRENT_USER);
    if( key.OpenKey("Software\\Microsoft\\DevStudio\\6.0\\AddIns\\SS_Log_AddIn.DSAddIn.1") )
    {
        TCHAR szMessage[1024];
        LoadString(AfxGetInstanceHandle(), IDS_INSTALL_SS_LOG_ADDIN_PROMPT, szMessage, 1024);
        int nMBResult = MessageBox(szMessage, "Install SS_Log_AddIn", MB_YESNO);
        if( IDNO == nMBResult ) // user responded "yes"?
        {
            *pResult = 0;
            return;
        }
        else
        {
            bShowMessage = TRUE;

            TCHAR szDesc[1024];
            LoadString(AfxGetInstanceHandle(), IDS_ADDIN_DESCRIPTION, szDesc, 1024);

            // open the regkey that contains the system root for either NT or Win98
            key.OpenKey(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion");
            if(!key.IsOpen())
                key.OpenKey(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion");

            TCHAR* szSystemRoot = NULL;
            if(key.IsOpen())
            {
                DWORD dwSize = NULL;
                key.QueryValue("SystemRoot", (unsigned char**)&szSystemRoot, &dwSize);

                // did we find the SystemRoot?
                if( dwSize )
                {
                    TCHAR szFile[MAX_PATH];
                    _tcscpy(szFile, szSystemRoot);
                    _tcscat(szFile, _T("\\SS_Log_AddIn.dll"));

                    // is the add-in dll present in the SystemRoot directory already?
                    // if not, abort... the user should place the file there.
                    WIN32_FIND_DATA fd;
                    HANDLE hFile = FindFirstFile(szFile, &fd);
                    if( INVALID_HANDLE_VALUE != hFile )
                    {
                        // create the registry entries for MSDEV to find the add-in
                        key.CreateKey(HKEY_CURRENT_USER, "Software\\Microsoft\\DevStudio\\6.0\\AddIns\\SS_Log_AddIn.DSAddIn.1");
                        key.SetValue("", "1");
                        key.SetValue("Description", szDesc);
                        key.SetValue("DisplayName", "SS_Log_AddIn");
                        key.SetValue("Filename", szFile);

                        key.CreateKey(HKEY_CURRENT_USER, "Software\\Microsoft\\DevStudio\\6.0\\AddIns\\SS_Log_AddIn.DSAddIn.1\\Toolbar");
                        key.SetValue("SS_Log_AddInCommand", 9);

                        // register the add-in dll with regsvr32 (it is a com object).
                        TCHAR szCommand[MAX_PATH*2];
                        _tcscpy(szCommand, "regsvr32 ");
                        _tcscat(szCommand, szFile);
                        WinExec(szCommand, SW_SHOWNORMAL);

                        bShowMessage = FALSE;
                    }
                    else
                    {
                        MessageBox("Could not find the SS_Log_AddIn.dll file in your %SYSTEMROOT% directory.  Please copy SS_Log_AddIn.dll and SS_Log_Server.exe to your %SYSTEMROOT% (ex. \"C:\\Winnt\") and try this again.");
                        bShowMessage = FALSE;
                    }
                }
            }
		}
	}

    if( bShowMessage )
    {
        MessageBox("Installation failed.  See the notes in the SS_Log_AddIn.h file for instructions on how to install the plug-in manually.", "Information", MB_OK|MB_ICONINFORMATION);
        *pResult = 0;
        return;
    }

    // look to see if there is an open MSDEV window
    HWND hWnd = NULL;
    WNDENUMPROC lpEnumFunc = EnumWindowsProc;
    EnumWindows( lpEnumFunc, (LPARAM)&hWnd );

    // open a new instance of MSDEV if there is not already one running
    if( !hWnd )
    {
        WinExec("MSDEV", SW_SHOWNORMAL);
        while( !hWnd )
            EnumWindows( lpEnumFunc, (LPARAM)&hWnd );
    }

    // bring the MSDEV window to the top and send it the message
	BOOL bResult = FALSE;
    if( hWnd )
	{
        ::BringWindowToTop(hWnd);

		DWORD dwBytesRead = 0;
		bResult = WaitNamedPipe( "\\\\.\\pipe\\SS_Log_AddIn", 20000 );
		bResult = CallNamedPipe( "\\\\.\\pipe\\SS_Log_AddIn",
								 (LPVOID)szCommandLine,
								 _tcslen(szCommandLine)+1,
								 (LPVOID)NULL,
								 0, &dwBytesRead,
								 5000 );
	}

    *pResult = 0;
}

void SS_Log_ServerView::OnEditFilterSettings()
{
	// TODO: Add your command handler code here
	SS_String sKeyPath = GetDocument()->RegKey();
	sKeyPath ^= SSLOG_REG_FILTERS_SUBKEY;
	SSFilterSettings settings(sKeyPath, FALSE);
	SS_FilterSettingsDlg dlg(this, &settings);

	dlg.DoModal();
}

void SS_Log_ServerView::OnEditListviewFilter()
{
	// TODO: Add your command handler code here
	delete m_pFilterSettingsDlg;
	m_pFilterSettingsDlg = new SS_FilterSettingsDlg(this, m_pFilterSettings);
	m_pFilterSettingsDlg->Create(IDD_FILTERSETTINGS, this);
	m_pFilterSettingsDlg->ShowWindow(SW_NORMAL);
}

void SS_Log_ServerView::OnDraw(CDC* pDC)
{
	UNREFERENCED_PARAMETER(pDC);
	// TODO: Add your specialized code here and/or call the base class
}

void SS_Log_ServerView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
    // TODO: Add your specialized code here and/or call the base class
    CPage* pPage = new CPage( pInfo->m_rectDraw, pDC, MM_TEXT );

    //create and use a table
    TABLEHEADER* pTable = new TABLEHEADER;
    pTable->PointSize = 10;
    pTable->LineSize = 1;

    pTable->UseInches  = TRUE;
    pTable->AutoSize   = FALSE;
    pTable->Border     = FALSE;
    pTable->HLines     = FALSE;
    pTable->VLines     = FALSE;
    pTable->HeaderLines= TRUE;
    pTable->FillFlag   = FILL_NONE;
    pTable->NumColumns = 6;
    pTable->NumRows    = 30;
    pTable->StartRow   = 0.2;
    pTable->StartCol   = 0.2;
    pTable->EndCol     = 10.5;


    pTable->ColDesc[0].Init(0.5,"ID");
    pTable->ColDesc[1].Init(1.5,"Level");
    pTable->ColDesc[2].Init(4.5,"Message");
    pTable->ColDesc[3].Init(1.2,"Time, Date");
    pTable->ColDesc[4].Init(2.0,"File");
    pTable->ColDesc[5].Init(0.5,"Line #");

    pPage->Table(pTable);

    //place information in a table
    INT nRowCount = GetListCtrl().GetItemCount();
    SS_String sID, sMsg, sLvl, sLine, sFile, sTime;
    TCHAR szTextFinal[SSLOG_MAX_MESSAGE_LENGTH];
    INT nStartRow = 30*(pInfo->m_nCurPage-1);
    INT nEndRow = (nStartRow+30)>nRowCount ? nRowCount : (nStartRow+30);
    for( INT i=nStartRow; i<nEndRow; i++ )
    {
        RetreiveMessage(szTextFinal, i, SSLOG_MAX_MESSAGE_LENGTH);

        sID = i;
        sTime = _tcstok( szTextFinal, "\t" );
        sFile = _tcstok( NULL, "\t" );
        sLine = _tcstok( NULL, "\t" );
        sLvl = _tcstok( NULL, "\t" );
        sMsg = _tcstok( NULL, "\n" );

        INT nRow = i-nStartRow;
        pPage->Print(pTable,nRow,0,8,TEXT_CENTER,sID);
        pPage->Print(pTable,nRow,1,8,TEXT_LEFT,sLvl);
        pPage->Print(pTable,nRow,2,8,TEXT_LEFT,sMsg);
        pPage->Print(pTable,nRow,3,8,TEXT_CENTER,sTime);
        pPage->Print(pTable,nRow,4,8,TEXT_RIGHT,sFile);
        pPage->Print(pTable,nRow,5,8,TEXT_CENTER,sLine);
    }

    CListView::OnPrint(pDC, pInfo);
    delete pPage;
    delete pTable;
}

// ----------------------------------------------------------------------- //
//  Function:		SS_Log_ServerDoc::WriteLog()
//  Author:			Steve Schaneville
//  Parameters:		szLog [in] - a tab-delimited log entry string.
//  Return Values:	none
//  Notes:			The input string, presumably created by the SS_Log class,
//                  should be tab-delimited with these entries:
//                  Time/date -> Filename -> Line # -> Level -> Message Text.
// ----------------------------------------------------------------------- //
VOID SS_Log_ServerView::AddMessage(LOGMESSAGE* pMsg)
{
	if( !AfxGetMainWnd()->IsWindowVisible() )
		AfxGetMainWnd()->ShowWindow(SW_NORMAL);

    BOOL br = MessagePassesFilter( pMsg );
    if( br == TRUE )
    {
        char   chrTmp = '\0';

        LVITEM item;
        item.mask = LVIF_TEXT|LVIF_PARAM;
        item.lParam = GetListCtrl().GetItemCount();
        item.pszText = &chrTmp;
        item.iItem = 0;
        item.iSubItem = 0;
        GetListCtrl().InsertItem(&item);

        char szEntry[32];
        //itoa(item.lParam, szEntry, 10);
        itoa(pMsg->MessageID(), szEntry, 10);
        item.mask = LVIF_TEXT;
        item.pszText = szEntry;
        item.iSubItem = SSLW_COLUMN_ENTRY;
        GetListCtrl().SetItem(&item);

        item.pszText = pMsg->DateTime();
        item.iSubItem = SSLW_COLUMN_TIMEDATE;
        GetListCtrl().SetItem(&item);

        item.pszText = pMsg->Filename();
        item.iSubItem = SSLW_COLUMN_FILE;
        GetListCtrl().SetItem(&item);

        char szLine[32];
        itoa(pMsg->LineNumber(), szLine, 10);
        item.pszText = szLine;
        item.iSubItem = SSLW_COLUMN_LINE;
        GetListCtrl().SetItem(&item);

        char szFlagsText[256];
        GetFlagsText(pMsg->Flags(), szFlagsText, 256);
        item.pszText = szFlagsText;
        item.iSubItem = SSLW_COLUMN_LEVEL;
        GetListCtrl().SetItem(&item);

        item.pszText = pMsg->Message();
        item.iSubItem = SSLW_COLUMN_MESSAGE;
        GetListCtrl().SetItem(&item);
    }
}

// ----------------------------------------------------------------------- //
//  Function:		SS_Log_ServerDoc::RetreiveMessage()
//  Author:			Steve Schaneville
//  Parameters:		szText [out] - A textbuffer that will receive the log
//                                 entry information (tab delimited).
//                  nItem [in]   - The item in the ListCtrl that is to be read.
//                  nSize [in]   - The size of the szText buffer.
//  Return Values:	none
//  Notes:			This function produces a tab-delimited text string
//                  of the sub-entries of a given ListCtrl item.  The order
//                  of the sub-entries are NOT in the same order as they are
//                  in the ListCtrl, but rather, in the order that they are
//                  produced by the SS_Log class.
// ----------------------------------------------------------------------- //
VOID SS_Log_ServerView::RetreiveMessage(TCHAR *szText, int nItem, int nSize)
{
    TCHAR szTextIn[SSLOG_MAX_MESSAGE_LENGTH];

    LVITEM item;
    item.mask = LVIF_TEXT;
    item.pszText = szTextIn;
    item.cchTextMax = SSLOG_MAX_MESSAGE_LENGTH;
    item.iItem = nItem;
    item.iSubItem = SSLW_COLUMN_TIMEDATE;
    GetListCtrl().GetItem(&item);
    _tcsncpy( szText, szTextIn, nSize );
    nSize -= _tcslen(szTextIn);
    _tcsncat( szText, _T("\t"), nSize );
    nSize -= _tcslen(_T("\t"));

    item.iSubItem = SSLW_COLUMN_FILE;
    GetListCtrl().GetItem(&item);
    _tcsncat( szText, szTextIn, nSize );
    nSize -= _tcslen(szTextIn);
    _tcsncat( szText, _T("\t"), nSize );
    nSize -= _tcslen(_T("\t"));

    item.iSubItem = SSLW_COLUMN_LINE;
    GetListCtrl().GetItem(&item);
    _tcsncat( szText, szTextIn, nSize );
    nSize -= _tcslen(szTextIn);
    _tcsncat( szText, _T("\t"), nSize );
    nSize -= _tcslen(_T("\t"));

    item.iSubItem = SSLW_COLUMN_LEVEL;
    GetListCtrl().GetItem(&item);
    _tcsncat( szText, szTextIn, nSize );
    nSize -= _tcslen(szTextIn);
    _tcsncat( szText, _T("\t"), nSize );
    nSize -= _tcslen(_T("\t"));

    item.iSubItem = SSLW_COLUMN_MESSAGE;
    GetListCtrl().GetItem(&item);
    _tcsncat( szText, szTextIn, nSize );
    nSize -= _tcslen(szTextIn);
    _tcsncat( szText, _T("\n"), nSize );
    nSize -= _tcslen(_T("\t"));
}

VOID SS_Log_ServerView::EraseLog()
{
    GetListCtrl().DeleteAllItems();
}

VOID SS_Log_ServerView::RefreshView()
{
	EraseLog();
	BeginUpdate();

	MsgListIT it = GetDocument()->MessageList().Beginning();
	while( it.IsValid() )
	{
		if( it->MessageType() == MSGTYPE_LOG_MESSAGE )
		{
		    AddMessage( it );
		}
		++it;
	}

	FinishUpdate();
}

LRESULT SS_Log_ServerView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	switch( message )
	{
	case SSWM_BEGIN_UPDATE:
		BeginUpdate();
		break;
	case SSWM_ADD_MESSAGE:
		AddMessage((LOGMESSAGE*)lParam);
		break;
	case SSWM_FINISH_UPDATE:
		FinishUpdate();
		break;
	case SSWM_ERASE_LOG:
		EraseLog();
		break;
	case SSWM_REFRESH_VIEW:
		RefreshView();
		break;
    case SSWM_SET_REGISTRY_KEY:
        SetRegistryKey();
        break;

    default:
		break;
	}

	return CListView::WindowProc(message, wParam, lParam);
}

VOID SS_Log_ServerView::BeginUpdate()
{
	GetListCtrl().LockWindowUpdate();
}

VOID SS_Log_ServerView::FinishUpdate()
{
	GetListCtrl().UnlockWindowUpdate();
}

void SS_Log_ServerView::OnViewRefresh()
{
	// TODO: Add your command handler code here
	RefreshView();
}

VOID SS_Log_ServerView::SetRegistryKey()
{
    SS_String sKeyPath = GetDocument()->RegKey();
    SS_String sKeyCopyFrom = sKeyPath;
    sKeyPath ^= SSLOG_REG_LISTVIEW_SUBKEY;
    sKeyCopyFrom ^= SSLOG_REG_FILTERS_SUBKEY;
    delete m_pFilterSettings;
    m_pFilterSettings = new SSFilterSettings(sKeyPath, TRUE);
    m_pFilterSettings->InitFilters(sKeyCopyFrom);
}

BOOL SS_Log_ServerView::MessagePassesFilter(LOGMESSAGE* pMsg)
{
	BOOL br = TRUE;
	SS_String sTest;
	SS_String sMsg;

	if( m_pFilterSettings->UseTextInMessage() == TRUE )
	{
		sMsg = pMsg->Message();
		sTest = m_pFilterSettings->TextInMessage();
		if( sMsg.Find(sTest) == _T("") )
			br = FALSE;
	}

	if( m_pFilterSettings->UseFilename() == TRUE )
	{
		sMsg = pMsg->Filename();
		sTest = m_pFilterSettings->Filename();
		if( sMsg.Find(sTest) == _T("") )
			br = FALSE;
	}

	if( m_pFilterSettings->UseIDBetween() == TRUE )
	{
		SS_String sID1 = m_pFilterSettings->IDLower();
		SS_String sID2 = m_pFilterSettings->IDUpper();
		INT nID1 = sID1;
		INT nID2 = sID2;
		INT nMsgID = pMsg->MessageID();
		if( !(nID1 <= nMsgID && nID2 >= nMsgID) )
			br = FALSE;
	}

	// Checkbox filters
	DWORD dwMsg = pMsg->Flags();
	DWORD dwFlt = m_pFilterSettings->Filter();
	if( m_pFilterSettings->RequireAllFilters() )
	{
		if( dwMsg & (~(dwMsg & dwFlt)) ) // only if all flags checked will show
			br = FALSE;
	}
	else
	{
		if( !(dwMsg & dwFlt) ) // any 1 flag checked will show
			br = FALSE;
	}

	return br;
}

void SS_Log_ServerView::OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if( pNMListView->uNewState & LVIS_SELECTED )
	{
        if( !(pNMListView->uOldState & LVIS_SELECTED) )
        {
            FindMatchingInOutStruct* pInfo = new FindMatchingInOutStruct;
            pInfo->iItem = pNMListView->iItem;
            pInfo->pWnd = this;
            DWORD dwThreadID;
            HANDLE hThread = CreateThread(NULL, 0, FindMatchingInOutThread,
                (LPVOID)pInfo, 0, &dwThreadID);
            CloseHandle(hThread);
        }
	}

	*pResult = 0;
}

void SS_Log_ServerView::OnItemChanging(NMHDR* pNMHDR, LRESULT* pResult)
{
//	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
//    if( m_bProcessingInOutMatches == TRUE )
//    {
//        if( !(pNMListView->uNewState & LVIS_SELECTED) )
//        {
//            if( pNMListView->uOldState & LVIS_SELECTED )
//            {
//                UINT nState = GetListCtrl().GetItemState(pNMListView->iItem, LVIF_STATE);
//                nState |= LVIS_SELECTED;
//                GetListCtrl().SetItemState(pNMListView->iItem, nState, LVIF_STATE|LVIS_SELECTED);
//                m_bProcessingInOutMatches = FALSE;
//            }
//        }
//    }

	*pResult = 0;
}

BOOL SS_Log_ServerView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	return CListView::PreTranslateMessage(pMsg);
}
