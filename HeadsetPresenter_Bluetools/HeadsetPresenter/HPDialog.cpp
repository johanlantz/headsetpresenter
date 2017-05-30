//////////////////////////////////////////////////////////////////////
//Headset Presenter by w2solutions (Johan Rosenquist)
//(C) All rights reserved 2002-2007
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HeadsetPresenter.h"

#include ".\hpdialog.h"


// HPDialog dialog

IMPLEMENT_DYNAMIC(HPDialog, CDialog)
//HPDialog::HPDialog(CWnd* pParent /*=NULL*/)
//	: CDialog(HPDialog::IDD, pParent)
//{	
	//this constructor is not used, use the one below	
//}

//This constructor passes the template id from the subclass to to CDialog
//It would probably work to use the old constructor above and pass pParent along
//However then : CDialog(HPDialog::IDD, pParent) would need to be changed 
HPDialog::HPDialog(UINT id /*=NULL*/)
	: CDialog(id)
{
	imageToDraw = NO_IMAGE_TO_DRAW;
	hasUpperLine = false;
	HeadlineFont.CreatePointFont(160,"Verdana");
	NormalTextFont.CreatePointFont(90,"Verdana");
	SmallTextFont.CreatePointFont(75,"Verdana");
	SmallHeadlineFont.CreatePointFont(120,"Verdana");
	LOGFONT lf;
	memset(&lf,0,sizeof(LOGFONT));
	lf.lfHeight = 14;
	lf.lfWeight = FW_BOLD;
	strcpy(lf.lfFaceName,"Arial");
	BoldStandardFont.CreateFontIndirect(&lf);

	LOGFONT lf2;
	memset(&lf2,0,sizeof(LOGFONT));
	lf2.lfHeight = 14;
	strcpy(lf2.lfFaceName,"Arial");
	StandardFont.CreateFontIndirect(&lf2);
}

HPDialog::~HPDialog()
{
}

void HPDialog::PrintStaticMessage(int controlId, CString text, int fontMode)
{
	CStatic* myS = (CStatic*)GetDlgItem(controlId);
	
	switch(fontMode)
	{
		case HEADLINE: 
			myS->SetFont(&HeadlineFont);
			break;
		case NORMAL_TEXT:
			myS->SetFont(&NormalTextFont);
			break;
		case SMALL_TEXT:
			myS->SetFont(&SmallTextFont);
			break;
		case SMALL_HEADLINE:
			myS->SetFont(&SmallHeadlineFont);
			break;
		case BOLD_STANDARD_FONT :
			myS->SetFont(&BoldStandardFont);
			break;
		case STANDARD_FONT:
			break;
		default:
			MessageBox("Invalid font selected");
			myS->SetFont(&NormalTextFont);
	}
	myS->ShowWindow(SW_HIDE);
	SetDlgItemText(controlId, text);
	myS->ShowWindow(SW_SHOW);
	//delete myFont;
}

void HPDialog::PrintStaticHeadlines(CString mainHeadlineText, CString subHeadlineText)
{
	mainHeadline.Create("", WS_CHILD|WS_VISIBLE|SS_LEFT, CRect(MAIN_HEADLINE_LEFT,MAIN_HEADLINE_TOP,MAIN_HEADLINE_RIGHT,MAIN_HEADLINE_BOTTOM), this);
	mainHeadline.SetFont(&BoldStandardFont);
	mainHeadline.SetWindowText(mainHeadlineText);
	if(subHeadlineText != "")
	{
		subHeadline.Create("", WS_CHILD|WS_VISIBLE|SS_LEFT, CRect(SUB_HEADLINE_LEFT,SUB_HEADLINE_TOP,SUB_HEADLINE_RIGHT,SUB_HEADLINE_BOTTOM), this);
		subHeadline.SetFont(&StandardFont);
		subHeadline.SetWindowText(subHeadlineText);
	}
}

void HPDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(HPDialog, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// HPDialog message handlers

BOOL HPDialog::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
//pDC->SetBkColor(RGB(255, 255, 255));
	// Set brush to desired background color
    CBrush backBrush(RGB(255, 255, 255));

    // Save old brush
    CBrush* pOldBrush = pDC->SelectObject(&backBrush);

    CRect rect;
    pDC->GetClipBox(&rect);     // Erase the area needed

    pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(),PATCOPY);
    pDC->SelectObject(pOldBrush);

	
	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}

HBRUSH HPDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if(nCtlColor == CTLCOLOR_STATIC)
	{
		hbr = (HBRUSH)::GetStockObject(HOLLOW_BRUSH);
		pDC->SetBkMode(TRANSPARENT);
	}
	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

void HPDialog::OnPaint()
{
	CPaintDC dc(this);
	CRect rect;
	GetClientRect(&rect);
	int width = rect.Width();
	int height = rect.Height();

	//If drawing is done here, then OnPaint in subclass can never call the OnPaint in this class since that will overdraw the things painted in OnEraseBkgnd
	dc.MoveTo(0,height-55);
	dc.LineTo(width,height-55);

	if(hasUpperLine)
	{
		dc.MoveTo(0,40);
		dc.LineTo(width,40);
	}
		
	if(imageToDraw != NO_IMAGE_TO_DRAW)
	{
		CBitmap bmp, *poldbmp; 
		CDC memdc; 


		// Load the bitmap resource 
		//bmp.LoadBitmap( IDB_INVISIO );  //JCHANGE1
		bmp.LoadBitmap( imageToDraw );  //This comes from the subclasses

		// Create a compatible memory DC 
		memdc.CreateCompatibleDC( &dc ); 


		// Select the bitmap into the DC 
		poldbmp = memdc.SelectObject( &bmp ); 


		// Copy (BitBlt) bitmap from memory DC to screen DC 
		if(imageToDraw == IDB_JABRA250)
		{
			int res = dc.BitBlt( 40, 10, 212, 180, &memdc, 0, 0, SRCCOPY ); 
		}
		else if(imageToDraw == IDB_NEWLOGO)
		{
			int res = dc.BitBlt( 20, 60, 91, 75, &memdc, 0, 0, SRCCOPY ); 
		}
		else if(imageToDraw == IDB_LOGO_50x50)
		{
			int res = dc.BitBlt( width-60, 5, 50, 50, &memdc, 0, 0, SRCCOPY ); 
		}
		


		memdc.SelectObject( poldbmp ); 
	}
	// Do not call CDialog::OnPaint() for painting messages
}
