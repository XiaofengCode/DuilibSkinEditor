// FindReplaceDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DuilibSkinEditor.h"
#include "FindReplaceDlg.h"
#include "afxdialogex.h"


// CFindReplaceDlg 对话框

IMPLEMENT_DYNAMIC(CFindReplaceDlg, CDialogEx)

CFindReplaceDlg::CFindReplaceDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFindReplaceDlg::IDD, pParent)
	, m_strFind(_T(""))
	, m_strReplace(_T(""))
	, m_bUp(FALSE)
	, m_bMatchWhole(FALSE)
	, m_bMatchCase(FALSE)
{

}

CFindReplaceDlg::~CFindReplaceDlg()
{
}

void CFindReplaceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FINDTEXT, m_strFind);
	DDX_Text(pDX, IDC_EDIT_REPLACETEXT, m_strReplace);
	DDX_Check(pDX, IDC_CHK_UP, m_bUp);
	DDX_Check(pDX, IDC_CHK_MATCHWHOLE, m_bMatchWhole);
	DDX_Check(pDX, IDC_CHK_CASESENSITIVE, m_bMatchCase);
}


BEGIN_MESSAGE_MAP(CFindReplaceDlg, CDialogEx)
	ON_BN_CLICKED(ID_BUTTON_FINDNEXT, &CFindReplaceDlg::OnBnClickedButtonFindnext)
END_MESSAGE_MAP()


CDuilibSkinEditorView* CFindReplaceDlg::GetCurrentView()
{
	CMDIFrameWnd  *pFrame = (CMDIFrameWnd*)theApp.GetMainWnd();
	if (!pFrame)
	{
		return NULL;
	}
	CMDIChildWnd  *pChild = (CMDIChildWnd   *)pFrame->GetActiveFrame();   
	if (!pChild)
	{
		return NULL;
	}
	CView   *pV = (CView*)pChild->GetActiveView();
	if (!pV)
	{
		return NULL;
	}
	return static_cast<CDuilibSkinEditorView*>(pV);
}

// CFindReplaceDlg 消息处理程序


void CFindReplaceDlg::OnBnClickedButtonFindnext()
{
	UpdateData();
	CDuilibSkinEditorView* pView = GetCurrentView();
	if (!pView)
	{
		return;
	}
	CString strText;
	GetDlgItemText(IDC_EDIT_FINDTEXT, strText);
	CSciWnd& sci = pView->GetSci();

	sci.sci_CallTipCancel();
	CStringA strText2Find(strText);

	int docLength = sci.sci_GetLength();
	int startPosition = sci.sci_GetSelectionEnd();
	int endPosition = docLength;
	if (m_bUp)
	{
		startPosition--;
		endPosition = 0;
	}

	int nFlags = 0;
	if (m_bMatchWhole)
	{
		nFlags |= SCFIND_WHOLEWORD;
	}
	if (m_bMatchCase)
	{
		nFlags |= SCFIND_MATCHCASE;
	}
	sci.sci_SetCurrentPos(startPosition);
	sci.sci_SearchAnchor();
	int nPos = -1;
	if (m_bUp)
	{
		nPos = sci.sci_SearchPrev(nFlags, strText2Find);
	}
	else
	{
		nPos = sci.sci_SearchNext(nFlags, strText2Find);
	}
	if (nPos == -1)
	{
		return;
	}
	int nLineEndPos = sci.sci_GetLineEndPosition(sci.sci_GetCurLine());
	startPosition = sci.sci_GetSelectionStart();
	endPosition = sci.sci_GetSelectionEnd();
	if (endPosition + 10 > nLineEndPos)
	{
		sci.sci_SetCurrentPos(nLineEndPos);
	}
	else
	{
		sci.sci_SetCurrentPos(endPosition + 10);
	}
	sci.sci_ScrollCaret();
	sci.sci_SetCurrentPos(endPosition);
	if (m_bUp)
	{
		sci.sci_SearchPrev(nFlags, strText2Find);
	}
	else
	{
		sci.sci_SearchNext(nFlags, strText2Find);
	}
	//sci.sci_SetSelectionStart(startPosition);
	//sci.sci_SetSelectionEnd(endPosition);
}
void displaySectionCentered(int posStart, int posEnd, CSciWnd * pEditView, bool isDownwards) 
{
	// to make sure the found result is visible
	//When searching up, the beginning of the (possible multiline) result is important, when scrolling down the end
	int testPos = (isDownwards)?posEnd:posStart;
	pEditView->execute(SCI_SETCURRENTPOS, testPos);
	int currentlineNumberDoc = (int)pEditView->execute(SCI_LINEFROMPOSITION, testPos);
	int currentlineNumberVis = (int)pEditView->execute(SCI_VISIBLEFROMDOCLINE, currentlineNumberDoc);
	pEditView->execute(SCI_ENSUREVISIBLE, currentlineNumberDoc);	// make sure target line is unfolded

	int firstVisibleLineVis =	(int)pEditView->execute(SCI_GETFIRSTVISIBLELINE);
	int linesVisible =			(int)pEditView->execute(SCI_LINESONSCREEN) - 1;	//-1 for the scrollbar
	int lastVisibleLineVis =	(int)linesVisible + firstVisibleLineVis;

	//if out of view vertically, scroll line into (center of) view
	int linesToScroll = 0;
	if (currentlineNumberVis < firstVisibleLineVis)
	{
		linesToScroll = currentlineNumberVis - firstVisibleLineVis;
		//use center
		linesToScroll -= linesVisible/2;		
	}
	else if (currentlineNumberVis > lastVisibleLineVis)
	{
		linesToScroll = currentlineNumberVis - lastVisibleLineVis;
		//use center
		linesToScroll += linesVisible/2;
	}
	pEditView->sci_LineScroll(0, linesToScroll);

	//Make sure the caret is visible, scroll horizontally (this will also fix wrapping problems)
	pEditView->execute(SCI_GOTOPOS, posStart);
	pEditView->execute(SCI_GOTOPOS, posEnd);

	pEditView->execute(SCI_SETANCHOR, posStart);
}
