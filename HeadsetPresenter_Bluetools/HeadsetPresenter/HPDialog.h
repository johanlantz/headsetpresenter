//////////////////////////////////////////////////////////////////////
//Headset Presenter by w2solutions (Johan Rosenquist)
//(C) All rights reserved 2002-2007
//////////////////////////////////////////////////////////////////////
#pragma once
#include "defines.h"

// HPDialog dialog

#define MAIN_HEADLINE_LEFT		10
#define MAIN_HEADLINE_TOP		5
#define MAIN_HEADLINE_RIGHT		MAIN_HEADLINE_LEFT + 400
#define MAIN_HEADLINE_BOTTOM	MAIN_HEADLINE_TOP + 15  //+ FontHeight

#define SUB_HEADLINE_LEFT		MAIN_HEADLINE_LEFT + 10  //indent 10
#define SUB_HEADLINE_TOP		MAIN_HEADLINE_BOTTOM + 2 //some space from mainHeadline to subHeadline
#define SUB_HEADLINE_RIGHT		MAIN_HEADLINE_RIGHT
#define SUB_HEADLINE_BOTTOM		SUB_HEADLINE_TOP + 15	//+FontHeight must increase if increasing size of fonts

class HPDialog : public CDialog
{
	DECLARE_DYNAMIC(HPDialog)

public:
	HPDialog(CWnd* pParent = NULL);   // standard constructor
	HPDialog(UINT id); //Used by subclass dialogs
	virtual ~HPDialog();

// Dialog Data
	//enum { IDD = IDD_HPDIALOG };  //removed this since it is a superclass, the contrsuctor does not use the id for this class

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void PrintStaticMessage(int controlId, CString text, int fontMode);
	void PrintStaticHeadlines(CString mainHeadlineText, CString subHeadlineText = "");
	int imageToDraw;
	bool hasUpperLine;
	CFont HeadlineFont;
	CFont NormalTextFont;
	CFont SmallTextFont;
	CFont SmallHeadlineFont;
	CFont BoldStandardFont;
	CFont StandardFont;
	CStatic mainHeadline;
	CStatic subHeadline;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
};
