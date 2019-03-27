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
	, m_bLoop(TRUE)
	, m_nMode(0)
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
	DDX_Check(pDX, IDC_CHK_LOOP, m_bLoop);
	DDX_Radio(pDX, IDC_RADIO_MODE_NORMAL, m_nMode);
}


BEGIN_MESSAGE_MAP(CFindReplaceDlg, CDialogEx)
	ON_BN_CLICKED(ID_BUTTON_FINDNEXT, &CFindReplaceDlg::OnBnClickedButtonFindnext)
	ON_BN_CLICKED(IDC_BUTTON_REPLACE, &CFindReplaceDlg::OnBnClickedButtonReplace)
	ON_BN_CLICKED(IDC_RADIO_MODE_REGEX, &CFindReplaceDlg::OnBnClickedRadioModeRegex)
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

void CFindReplaceDlg::SetSearchAnchor(CSciWnd& sci, int nPos)
{
	sci.sci_SetSelectionStart(nPos);
	sci.sci_SetSelectionEnd(nPos);
	sci.sci_SearchAnchor();
}

// CFindReplaceDlg 消息处理程序


void CFindReplaceDlg::OnBnClickedButtonFindnext()
{
	if (!DoFind())
	{
		AfxMessageBox(_T("未找到指定文本!"));
	}
}

bool CFindReplaceDlg::DoFind()
{
	UpdateData();
	CDuilibSkinEditorView* pView = GetCurrentView();
	if (!pView)
	{
		return false;
	}
	CString strText;
	GetDlgItemText(IDC_EDIT_FINDTEXT, strText);
	if (strText.GetLength() == 0)
	{
		Show();
		return false;
	}
	CSciWnd& sci = pView->GetSci();

	sci.sci_CallTipCancel();

	CStringA strText2Find = W2Utf8(strText);

	int docLength = sci.sci_GetLength();
	int startPosition = sci.sci_GetSelectionStart();
	int endPosition = sci.sci_GetSelectionEnd();

	if (m_bUp)
	{
		if (startPosition > endPosition)
		{
			startPosition = endPosition;
		}
		endPosition = 0;
	}
	else
	{
		if (startPosition < endPosition)
		{
			startPosition = endPosition;
		}
		endPosition = docLength;
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
	if (m_nMode == 1)
	{
		nFlags |= SCFIND_POSIX;
	}
	else if (m_nMode == 2)
	{
		nFlags |= SCFIND_REGEXP;
	}
	SetSearchAnchor(sci, startPosition);
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
		if (m_bLoop)
		{
			SetSearchAnchor(sci, m_bUp ? docLength : 0);
			if (m_bUp)
			{
				nPos = sci.sci_SearchPrev(nFlags, strText2Find);
			}
			else
			{
				nPos = sci.sci_SearchNext(nFlags, strText2Find);
			}
		}
		if (nPos == -1)
		{
			return false;
		}
	}
	int nLineEndPos = sci.sci_GetLineEndPosition(sci.sci_GetCurLine());
	startPosition = sci.sci_GetSelectionStart();
	endPosition = sci.sci_GetSelectionEnd();

	void displaySectionCentered(int posStart, int posEnd, CSciWnd * pEditView, bool isDownwards) ;
	displaySectionCentered(startPosition, endPosition, &sci, !m_bUp);
// 	if (endPosition + 10 > nLineEndPos)
// 	{
// 		sci.sci_SetCurrentPos(nLineEndPos);
// 	}
// 	else
// 	{
// 		sci.sci_SetCurrentPos(endPosition + 10);
// 	}
// 	sci.sci_ScrollCaret();
// 	sci.sci_SetCurrentPos(endPosition);
// 
// 	if (m_bUp)
// 	{
// 		sci.sci_SearchPrev(nFlags, strText2Find);
// 	}
// 	else
// 	{
// 		sci.sci_SearchNext(nFlags, strText2Find);
// 	}
	return true;
}

void CFindReplaceDlg::Show()
{
	CDuilibSkinEditorView* pView = GetCurrentView();
	if (!pView)
	{
		return;
	}
	CSciWnd& sci = pView->GetSci();
	CStringA strSelA;
	sci.sci_GetSelText(strSelA);
	CString strText = Utf8ToW(strSelA);
	ShowWindow(SW_SHOW);
	SetFocus();
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_FINDTEXT);
	pEdit->SetWindowText(strText);
	pEdit->SetFocus();
	pEdit->SetSel(0, -1);

}

CStringA CFindReplaceDlg::W2Utf8(LPCWSTR lpwsz)
{
	int nLen = (wcslen(lpwsz) + 1) * sizeof(WCHAR);
	char* pstrUtf8 = new char[nLen];
	::WideCharToMultiByte(CP_UTF8, 0, lpwsz, -1, pstrUtf8, nLen, NULL, NULL);
	CStringA sRes(pstrUtf8, nLen);
	delete[] pstrUtf8;
	return sRes;
}

CString CFindReplaceDlg::Utf8ToW(LPCSTR lpsz)
{
	int nLen = (strlen(lpsz) + 1);
	WCHAR* pstr = new WCHAR[nLen];
	::MultiByteToWideChar(CP_UTF8, 0, lpsz, -1, pstr, nLen);
	CString sRes(pstr, nLen);
	delete[] pstr;
	return sRes;
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


void CFindReplaceDlg::OnBnClickedButtonReplace()
{
	UpdateData();
	CDuilibSkinEditorView* pView = GetCurrentView();
	if (!pView)
	{
		return;
	}
	CSciWnd& sci = pView->GetSci();
	CStringA strSel;
	sci.sci_GetSelText(strSel);
	if (strSel.GetLength() == 0)
	{
		DoFind();
		return;
	}
	CString strText;
	GetDlgItemText(IDC_EDIT_REPLACETEXT, strText);
	
	CStringA strTextA = W2Utf8(strText);
	int startOld = sci.sci_GetSelectionStart();
	int endOld = sci.sci_GetSelectionEnd();
	if (m_bUp)
	{
		SetSearchAnchor(sci, max(startOld, endOld));
	}
	else
	{
		SetSearchAnchor(sci, min(startOld, endOld));
	}
	if (!DoFind())
	{
		AfxMessageBox(_T("未找到指定文本!"));
		return;
	}
	if (startOld == sci.sci_GetSelectionStart() && endOld == sci.sci_GetSelectionEnd())
	{
		sci.sci_ReplaceSel(strTextA);
		DoFind();
		return;
	}
}


void CFindReplaceDlg::OnBnClickedRadioModeRegex()
{
	//禁用全词匹配和向上
}


void CFindReplaceDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialogEx::OnOK();
}
