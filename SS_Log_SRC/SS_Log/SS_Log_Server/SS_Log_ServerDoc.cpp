// SS_Log_ServerDoc.cpp : implementation of the SS_Log_ServerDoc class
// Version 3.02

#include "stdafx.h"
#include "SS_Log_Server.h"

#include "SS_Log_ServerDoc.h"
#include "SSLogConnection.h"
#include "SSLogOutput.h"
#include "SSCommandLineInfo.h"
#include "SS_RegistryKey.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// SS_Log_ServerDoc

IMPLEMENT_DYNCREATE(SS_Log_ServerDoc, CDocument)

BEGIN_MESSAGE_MAP(SS_Log_ServerDoc, CDocument)
	//{{AFX_MSG_MAP(SS_Log_ServerDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////
// globals

SS_Log_ServerDoc* g_pDoc = NULL;
extern SS_Log_ServerApp theApp;

/////////////////////////////////////////////////////////////////////////////
// SS_Log_ServerDoc construction/destruction

SS_Log_ServerDoc::SS_Log_ServerDoc()
{
	// TODO: add one-time construction code here
    InitObject();
}

SS_Log_ServerDoc::~SS_Log_ServerDoc()
{
    delete Connection();
    AttachConnection(NULL);
    delete Output();
    AttachOutput(NULL);

	// delete our messages
	MsgListIT itMsg = MessageList().Beginning();
	while( itMsg.IsValid() )
	{
		delete itMsg.Current();
		++itMsg;
	}
	MessageList().RemoveAll();
}

VOID SS_Log_ServerDoc::InitObject()
{
	g_pDoc = this;
    AttachConnection(NULL);
    AttachOutput(NULL);

	// init the reg key
    SS_RegistryKey regKey(SSLOG_REG_DEFAULT_HIVE);
	SS_String sKey;
	sKey = SSLOG_REG_HIVE_SUBKEY;
	sKey ^= SSLOG_REG_DEFAULT_APP_KEY;
	sKey ^= SSLOG_REG_DEFAULT_APP_SUBKEY;
    LONG lr = regKey.OpenKey(sKey);
    if( ERROR_SUCCESS == lr )
    {
        TCHAR* szValue = NULL;
        lr = regKey.QueryValue(_T("MostRecentRegKey"), &szValue);
        if( ERROR_SUCCESS == lr )
        {
            sKey = SSLOG_REG_HIVE_SUBKEY;
            sKey ^= szValue;
        }
    }

	RegKey(sKey);
}

BOOL SS_Log_ServerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

    // create our connection
    // . we are responsible for deleting the new SSLogConnection in the
    //   destructor of this SS_Log_ServerApp instance
    // . the SSLogConnection class is responsible for deleting the
    //   new SSLogOutput instance whenever it sees fit (probably during
    //   destruction)
    SSLogOutput* pOutput = new SSLogOutput;
	AttachOutput( pOutput );
    if( Output() != NULL )
    {
        Output()->CreateDispatchTimer();
        SSLogConnection* pConnection = new SSLogConnection( Output() );
        if( pConnection )
        {
            AttachConnection( pConnection );
            Connection()->CreateListeningThread();
        }
        else
            ASSERT(FALSE);
    }
    else
        ASSERT(FALSE);

	Output()->SetMainWindow(AfxGetMainWnd());
	SSCommandLineInfo* pInfo = theApp.CommandLineInfo();
	Connection()->ConnectionName(pInfo->m_szServerName);

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// SS_Log_ServerDoc serialization

void SS_Log_ServerDoc::Serialize(CArchive& ar)
{
    if (ar.IsStoring())
    {
        // storing code
        MsgListIT it = m_MessageList.Beginning();
        SS_String sMsg;
        while( it.IsValid() )
        {
            sMsg += (LPCSTR)MakeTabDelimitedMessage(it);
            sMsg += "\n";
            ++it;
        }

        ar.WriteString(sMsg);
    }
    else
    {
        // loading code
        EraseLog();

        CString csText;
        ar.ReadString(csText);
        while( !csText.IsEmpty() )
        {
            LOGMESSAGE* pMsg = new LOGMESSAGE;
            ParseTabDelimitedMessage((LPCTSTR)csText, pMsg);
            MessageList().InsertEnd(pMsg);
            ar.ReadString(csText);
        }
        RefreshAllViews();
    }
}

/////////////////////////////////////////////////////////////////////////////
// SS_Log_ServerDoc diagnostics

#ifdef _DEBUG
void SS_Log_ServerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void SS_Log_ServerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// SS_Log_ServerDoc commands
VOID SS_Log_ServerDoc::SendMessageToAllViews(UpdateMessage nMessage, WPARAM wParam, LPARAM lParam)
{
    POSITION pos = GetFirstViewPosition();
    while (pos != NULL)
    {
        CView* pView = GetNextView(pos);
        pView->SendMessage((UINT)nMessage, wParam, lParam);
    }
}

VOID SS_Log_ServerDoc::BeginUpdate()
{
    SendMessageToAllViews(SSWM_BEGIN_UPDATE);
}

VOID SS_Log_ServerDoc::FinishUpdate()
{
    SendMessageToAllViews(SSWM_FINISH_UPDATE);
}

VOID SS_Log_ServerDoc::AddMessage(LOGMESSAGE* pMsg)
{
	ASSERT(pMsg);
	LOGMESSAGE* pMsgNew = new LOGMESSAGE(*pMsg);
    MessageList().InsertEnd(pMsgNew);

    SendMessageToAllViews(SSWM_ADD_MESSAGE, 0, (LPARAM)pMsgNew);
}

VOID SS_Log_ServerDoc::EraseLog()
{
	MessageList().RemoveAll();
    SendMessageToAllViews(SSWM_ERASE_LOG);
}

VOID SS_Log_ServerDoc::RefreshAllViews()
{
    SendMessageToAllViews(SSWM_REFRESH_VIEW);
}

VOID SS_Log_ServerDoc::SetRegistryKey(LOGMESSAGE* pMsg)
{
	SS_String sKey;
	sKey = SSLOG_REG_HIVE_SUBKEY;
	sKey ^= pMsg->Message();
	RegKey(sKey);

    SendMessageToAllViews(SSWM_SET_REGISTRY_KEY);
}

VOID SS_Log_ServerDoc::RegistryFiltersChanged(LOGMESSAGE* pMsg)
{
    SendMessageToAllViews(SSWM_REGISTRY_FILTERS_CHANGED);
}

SS_String SS_Log_ServerDoc::MakeTabDelimitedMessage(LOGMESSAGE* pMsg)
{
    SS_String sFlags;
    GetFlagsText(pMsg->Flags(), sFlags.GetBuffer(256), 256);
    sFlags.ReleaseBuffer();

    SS_String sMessage;
    sMessage += pMsg->MessageID();
    sMessage += "\t";
    sMessage += pMsg->DateTime();
    sMessage += "\t";
    sMessage += sFlags;
    sMessage += "\t";
    sMessage += pMsg->Filename();
    sMessage += "\t";
    sMessage += pMsg->LineNumber();
    sMessage += "\t";
    sMessage += pMsg->Message();

    return sMessage;
}

BOOL SS_Log_ServerDoc::ParseTabDelimitedMessage(SS_String sMsg, LOGMESSAGE* pMsg)
{
    ASSERT(pMsg);
    DWORD dwFlags;
    SS_String sTok;

    sTok = sMsg.FirstToken("\t");
    pMsg->MessageID( sTok );
    sTok = sMsg.NextToken();
    pMsg->DateTime( sTok );

    sTok = sMsg.NextToken();
    GetFlagsFromText( sTok, &dwFlags );
    pMsg->Flags( dwFlags );

    sTok = sMsg.NextToken();
    pMsg->Filename( sTok );
    sTok = sMsg.NextToken();
    pMsg->LineNumber( sTok );
    sTok = sMsg.NextToken();
    pMsg->Message( sTok );

    return TRUE;
}
