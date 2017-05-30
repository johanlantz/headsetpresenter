// SS_FilterSettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SS_Log_Server.h"
#include "SS_FilterSettingsDlg.h"
#include "SS_Log_ServerDoc.h"
#include "SSFilterSettings.h"

#include "SS_RegistryKey.h"
#include "SS_String.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SS_FilterSettingsDlg dialog


#define IDD_DYNAMIC_BUTTONS (WM_USER + 1000)

SS_FilterSettingsDlg::SS_FilterSettingsDlg(CWnd* pParent, SSFilterSettings* pFilterSettings)
	: CDialog(SS_FilterSettingsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(SS_FilterSettingsDlg)
	m_bTextInMsg = FALSE;
	m_csTextInMsg = _T("");
	m_bFilename = FALSE;
	m_csFilename = _T("");
	m_bID = FALSE;
	m_csID1 = _T("");
	m_csID2 = _T("");
	m_bAutoApply = FALSE;
	//}}AFX_DATA_INIT
	m_bRequireAllFilters = FALSE;

	m_pFilterSettings = pFilterSettings;		
}

SS_FilterSettingsDlg::~SS_FilterSettingsDlg()
{
	INT nCount = m_pFilterSettings->FilterList().nCount;
	for( INT i=0; i<nCount; i++ )
	{
		delete m_pFilterSettings->FilterList().pFilterList[i]->pButton;
	}
}

void SS_FilterSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SS_FilterSettingsDlg)
	DDX_Control(pDX, IDC_REQ_ALL_FILTERS_RADIO, m_cRequireAllFiltersRadio);
    DDX_Control(pDX, IDC_REQ_ONE_FILTER_RADIO, m_cRequire1FilterRadio);
	DDX_Control(pDX, IDC_AUTO_APPLY_CHECK, m_cAutoApply);
	DDX_Control(pDX, IDC_SEARCH_FILTERS_GROUP, m_cSearchFiltersGroup);
	DDX_Control(pDX, IDC_FILTERS_GROUP, m_cFiltersGroup);
	DDX_Control(pDX, IDC_TEXTINMSG_CHECK, m_cTextInMsgCheck);
	DDX_Control(pDX, IDC_ID_CHECK, m_cIDCheck);
	DDX_Control(pDX, IDC_FILENAME_CHECK, m_cFilenameCheck);
	DDX_Control(pDX, IDC_ID_EDIT2, m_cID2);
	DDX_Control(pDX, IDC_ID_EDIT1, m_cID1);
	DDX_Control(pDX, IDC_FILENAME_EDIT, m_cFilename);
	DDX_Control(pDX, IDC_TEXTINMSG_EDIT, m_cTextInMsg);
	DDX_Check(pDX, IDC_TEXTINMSG_CHECK, m_bTextInMsg);
	DDX_Text(pDX, IDC_TEXTINMSG_EDIT, m_csTextInMsg);
	DDX_Check(pDX, IDC_FILENAME_CHECK, m_bFilename);
	DDX_Text(pDX, IDC_FILENAME_EDIT, m_csFilename);
	DDX_Check(pDX, IDC_ID_CHECK, m_bID);
	DDX_Text(pDX, IDC_ID_EDIT1, m_csID1);
	DDX_Text(pDX, IDC_ID_EDIT2, m_csID2);
	DDX_Check(pDX, IDC_AUTO_APPLY_CHECK, m_bAutoApply);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(SS_FilterSettingsDlg, CDialog)
	//{{AFX_MSG_MAP(SS_FilterSettingsDlg)
	ON_BN_CLICKED(IDC_FILENAME_CHECK, OnFilenameCheck)
	ON_BN_CLICKED(IDC_ID_CHECK, OnIdCheck)
	ON_BN_CLICKED(IDC_TEXTINMSG_CHECK, OnTextinmsgCheck)
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	ON_BN_CLICKED(IDC_REQ_ALL_FILTERS_RADIO, OnReqAllFiltersRadio)
	ON_BN_CLICKED(IDC_REQ_ONE_FILTER_RADIO, OnReqOneFilterRadio)
	ON_BN_CLICKED(IDC_FILTERS_GROUP, OnFiltersGroup)
	ON_EN_CHANGE(IDC_TEXTINMSG_EDIT, OnChangeTextinmsgEdit)
	ON_EN_CHANGE(IDC_FILENAME_EDIT, OnChangeFilenameEdit)
	ON_EN_CHANGE(IDC_ID_EDIT1, OnChangeIdEdit1)
	ON_EN_CHANGE(IDC_ID_EDIT2, OnChangeIdEdit2)
    ON_BN_CLICKED(IDD_DYNAMIC_BUTTONS, OnDynamicButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SS_FilterSettingsDlg message handlers

BOOL SS_FilterSettingsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	ReadFilters();
	CreateButtons();
	
	if( !m_pFilterSettings->UseExtendedOptions() )
	{
		m_cTextInMsgCheck.EnableWindow(FALSE);
		m_cFilenameCheck.EnableWindow(FALSE);
		m_cIDCheck.EnableWindow(FALSE);
        m_cAutoApply.EnableWindow(FALSE);
        m_cRequire1FilterRadio.EnableWindow(FALSE);
        m_cRequireAllFiltersRadio.EnableWindow(FALSE);
	}

	SetupWindow();

    return TRUE;
}


BOOL SS_FilterSettingsDlg::ReadFilters()
{
    BOOL br = TRUE;
	SS_String sID;

	m_pFilterSettings->ReadSettingsFromReg();

	m_bTextInMsg = m_pFilterSettings->UseTextInMessage();
	m_csTextInMsg = (LPCTSTR)m_pFilterSettings->TextInMessage();
	m_bFilename = m_pFilterSettings->UseFilename();
	m_csFilename = (LPCTSTR)m_pFilterSettings->Filename();
	m_bID = m_pFilterSettings->UseIDBetween();
	sID = m_pFilterSettings->IDLower();
	m_csID1 = (LPCTSTR)sID;
	sID = m_pFilterSettings->IDUpper();
	m_csID2 = (LPCTSTR)sID;
	m_bRequireAllFilters = m_pFilterSettings->RequireAllFilters();
    m_bAutoApply = m_pFilterSettings->AutoApply();
	
	UpdateData(FALSE);
	
    return br;
}

BOOL SS_FilterSettingsDlg::WriteFilters()
{
    BOOL br = TRUE;

	UpdateData(TRUE);
	INT nCount = m_pFilterSettings->FilterList().nCount;
    for( INT i=0; i < nCount; i++ )
    {
        m_pFilterSettings->FilterList().pFilterList[i]->bIsChecked =
            m_pFilterSettings->FilterList().pFilterList[i]->pButton->GetCheck();
    }

	m_pFilterSettings->UseTextInMessage(m_bTextInMsg);
	m_pFilterSettings->TextInMessage((LPCTSTR)m_csTextInMsg);
	m_pFilterSettings->UseFilename(m_bFilename);
	m_pFilterSettings->Filename((LPCTSTR)m_csFilename);
	m_pFilterSettings->UseIDBetween(m_bID);
	SS_String sNumber;
	sNumber = m_csID1;
	m_pFilterSettings->IDLower(sNumber);
	sNumber = m_csID2;
	m_pFilterSettings->IDUpper(sNumber);
	m_pFilterSettings->RequireAllFilters(m_bRequireAllFilters);
    m_pFilterSettings->AutoApply(m_bAutoApply);

	m_pFilterSettings->WriteSettingsToReg();

    return br;
}

BOOL SS_FilterSettingsDlg::CreateButtons()
{
    BOOL br = TRUE;

    // create a button for each filter in our m_FilterList
    CButton* pButton = NULL;
    CRect rectWnd;
    GetWindowRect(&rectWnd);
    INT nTop = 90;
    INT nRight = rectWnd.right-rectWnd.left - 300;
    CRect rect(24, nTop, nRight, nTop + 15);
    for( INT i=0; i < m_pFilterSettings->FilterList().nCount; i++ )
    {
        pButton = new CButton;
        rect.top = nTop;
        rect.bottom = nTop + 15;
        pButton->Create( m_pFilterSettings->FilterList().pFilterList[i]->sFilterName,
                         WS_CHILD|WS_TABSTOP|WS_VISIBLE|BS_AUTOCHECKBOX,
                         rect,
                         this,
                         IDD_DYNAMIC_BUTTONS );
        pButton->SetFont(GetFont());
        nTop += 20;
        pButton->SetCheck(m_pFilterSettings->FilterList().pFilterList[i]->bIsChecked);
        m_pFilterSettings->FilterList().pFilterList[i]->pButton = pButton;
    }

    // now resize the dialog to fit the buttons
    SetWindowPos(this, 0, 0, rectWnd.right-rectWnd.left, nTop+52, SWP_NOMOVE|SWP_NOOWNERZORDER|SWP_NOZORDER);

	CRect rectGrp;
	GetClientRect(rect);
	m_cFiltersGroup.GetWindowRect(rectGrp);
	ScreenToClient(rectGrp);
	rectGrp.bottom = rect.bottom - 10;
	m_cFiltersGroup.MoveWindow(rectGrp);
	
    CRect rectGrp2;
    GetClientRect(rect);
    m_cSearchFiltersGroup.GetWindowRect(rectGrp2);
    ScreenToClient(rectGrp2);
    rectGrp2.bottom = rect.bottom - 10;
    m_cSearchFiltersGroup.MoveWindow(rectGrp2);
    
    return br;
}

void SS_FilterSettingsDlg::OnOK()
{
	// TODO: Add extra validation here
	OnApply();
	CDialog::OnOK();
}

BOOL SS_FilterSettingsDlg::SetupWindow()
{
	BOOL br = TRUE;

	UpdateData(TRUE);

	if( m_bTextInMsg )
		m_cTextInMsg.EnableWindow(TRUE);
	else
		m_cTextInMsg.EnableWindow(FALSE);

	if( m_bFilename )
		m_cFilename.EnableWindow(TRUE);
	else
		m_cFilename.EnableWindow(FALSE);

	if( m_bID )
	{
		m_cID1.EnableWindow(TRUE);
		m_cID2.EnableWindow(TRUE);
	}
	else
	{
		m_cID1.EnableWindow(FALSE);
		m_cID2.EnableWindow(FALSE);
	}

	if( m_bRequireAllFilters == TRUE )
		CheckRadioButton(IDC_REQ_ALL_FILTERS_RADIO, IDC_REQ_ONE_FILTER_RADIO, IDC_REQ_ALL_FILTERS_RADIO);
	else
		CheckRadioButton(IDC_REQ_ALL_FILTERS_RADIO, IDC_REQ_ONE_FILTER_RADIO, IDC_REQ_ONE_FILTER_RADIO);
	
	return br;
}

void SS_FilterSettingsDlg::OnFilenameCheck() 
{
	// TODO: Add your control notification handler code here
	SetupWindow();
    AutoApply();
}

void SS_FilterSettingsDlg::OnIdCheck() 
{
	// TODO: Add your control notification handler code here
	SetupWindow();
    AutoApply();
}

void SS_FilterSettingsDlg::OnTextinmsgCheck() 
{
	// TODO: Add your control notification handler code here
	SetupWindow();
    AutoApply();
}

void SS_FilterSettingsDlg::OnApply() 
{
	// TODO: Add your control notification handler code here
	WriteFilters();
	m_pParentWnd->PostMessage(SSWM_REFRESH_VIEW, 0, 0);	
}

void SS_FilterSettingsDlg::OnReqAllFiltersRadio() 
{
	// TODO: Add your control notification handler code here
	m_bRequireAllFilters = TRUE;
    AutoApply();
}

void SS_FilterSettingsDlg::OnReqOneFilterRadio() 
{
	// TODO: Add your control notification handler code here
	m_bRequireAllFilters = FALSE;	
    AutoApply();
}

void SS_FilterSettingsDlg::OnFiltersGroup() 
{
	// TODO: Add your control notification handler code here
	
}

BOOL SS_FilterSettingsDlg::AutoApply()
{
    UpdateData(TRUE);
    if( m_bAutoApply == TRUE )
    {
        OnApply();
        return TRUE;
    }
    else
        return FALSE;
}

void SS_FilterSettingsDlg::OnDynamicButton() 
{
    // TODO: Add your control notification handler code here
    AutoApply();
}

void SS_FilterSettingsDlg::OnChangeTextinmsgEdit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
    AutoApply();
}

void SS_FilterSettingsDlg::OnChangeFilenameEdit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
    AutoApply();
}

void SS_FilterSettingsDlg::OnChangeIdEdit1() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
    AutoApply();
}

void SS_FilterSettingsDlg::OnChangeIdEdit2() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
    AutoApply();	
}
