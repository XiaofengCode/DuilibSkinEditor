
// DuilibSkinEditorView.cpp : CDuilibSkinEditorView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "DuilibSkinEditor.h"
#endif

#include "DuilibSkinEditorDoc.h"
#include "DuilibSkinEditorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDuilibSkinEditorView

IMPLEMENT_DYNCREATE(CDuilibSkinEditorView, CView)

BEGIN_MESSAGE_MAP(CDuilibSkinEditorView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CDuilibSkinEditorView::OnFilePrintPreview)
	ON_COMMAND(ID_EDIT_CUT, &CDuilibSkinEditorView::OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, &CDuilibSkinEditorView::OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_COPY, &CDuilibSkinEditorView::OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, &CDuilibSkinEditorView::OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_FINDREPLACE, &CDuilibSkinEditorView::OnEditFindRepalce)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FINDREPLACE, &CDuilibSkinEditorView::OnUpdateEditFindRepalce)
	ON_COMMAND(ID_EDIT_PASTE, &CDuilibSkinEditorView::OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, &CDuilibSkinEditorView::OnUpdateEditPaste)
	ON_COMMAND(ID_EDIT_UNDO, &CDuilibSkinEditorView::OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, &CDuilibSkinEditorView::OnUpdateEditUndo)
	ON_COMMAND(ID_EDIT_REDO, &CDuilibSkinEditorView::OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, &CDuilibSkinEditorView::OnUpdateEditRedo)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_MESSAGE(WM_ATTIBUTE_VALUE_AUTOSEL, &CDuilibSkinEditorView::OnAttibuteValueAutoSel)
	ON_WM_SETFOCUS()
	ON_MESSAGE(0x2222, OnSelectControl)
	ON_MESSAGE(0x2223, OnSetEditFocus)
END_MESSAGE_MAP()

// CDuilibSkinEditorView 构造/析构

CDuilibSkinEditorView::CDuilibSkinEditorView()
{
	ZeroMemory(&m_posTagLastSel, sizeof(m_posTagLastSel));
}

CDuilibSkinEditorView::~CDuilibSkinEditorView()
{

}

BOOL CDuilibSkinEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CDuilibSkinEditorView 绘制

void CDuilibSkinEditorView::OnDraw(CDC* /*pDC*/)
{
	CDuilibSkinEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CDuilibSkinEditorView 打印


void CDuilibSkinEditorView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CDuilibSkinEditorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CDuilibSkinEditorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CDuilibSkinEditorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CDuilibSkinEditorView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CDuilibSkinEditorView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CDuilibSkinEditorView 诊断

#ifdef _DEBUG
void CDuilibSkinEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CDuilibSkinEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDuilibSkinEditorDoc* CDuilibSkinEditorView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDuilibSkinEditorDoc)));
	return (CDuilibSkinEditorDoc*)m_pDocument;
}

#endif //_DEBUG


// CDuilibSkinEditorView 消息处理程序


void CDuilibSkinEditorView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	
	m_duiProp.LoadPropertyFile();
	CRect rectDummy;
	rectDummy.SetRect(0,0,100,100);
	if (!m_sci.Create(0, WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_CLIPCHILDREN, rectDummy, this, 0x1111))
	{
		return;      // 未能创建
	}
	m_sci.InitForXML();
	m_sci.sci_SetCodePage(0);
	m_sci.sci_SetCodePage(SC_CP_UTF8);

	// 	sci.sci_StyleSetFont(STYLE_DEFAULT, _T("Courier New"));
	// 	sci.sci_StyleSetSize(STYLE_DEFAULT, 10);

	m_sci.ClearTextAll();
	// 	xml_string_writer writer;
	// 	writer.pSciWnd = &sci;
	// 	m_pDoc->m_doc.child(_T("Window")).print(writer);
	
	if (m_pDocument->GetPathName().GetLength())
	{
		if (m_sci.LoadFile(m_pDocument->GetPathName()))
		{
			_tstat64(m_pDocument->GetPathName(), &m_statFile);
		}
	}
	else
	{
		m_sci.LoadFile(theApp.GetStartupDir() + _T("template.xml"));
	}
	m_sci.SetSavePoint();
	m_sci.sci_EmptyUndoBuffer();
	SetTimer(1, 100, NULL);
	SetTimer(2, 2000, NULL);
}

CString CDuilibSkinEditorView::GetCurNodeName()
{
	CString strRet;
	int pos = m_sci.sci_GetCurrentPos();

	CStringA strText;
	m_sci.sci_GetTextRange(0, pos, strText);
	int nSpacePos = -1;
	for (int i=strText.GetLength()-1; i>=0; i--)
	{
		if(strText[i] == ' ' || strText[i] == '>')
		{
			nSpacePos = i;
		}
		else if(strText[i] == '<')
		{
			if(nSpacePos >= 0)
			{
				CStringA strRetA;
				m_sci.sci_GetTextRange(i + 1, nSpacePos, strRetA);
				return CString(strRetA);
			}
		}
	}
	return strRet;
}

void CDuilibSkinEditorView::AutoCompleteControl(CString objectName)
{
	CString strShow;

	for (xml_node node=m_duiProp.GetRoot().first_child(); node; node=node.next_sibling())
	{
		CString nodeName = node.name();
		nodeName.MakeUpper();
		objectName.MakeUpper();
		if(nodeName.Find(objectName) >= 0)
		{
			strShow += node.name();
			strShow += _T(" ");
		}
	}

	CStringA strShowA(strShow);
	if(!strShowA.IsEmpty())
	{
		m_bAutoCtrl = true;
		strShowA.TrimRight();
		m_sci.sci_AutocShow(0, strShowA);
	}
}

void CDuilibSkinEditorView::AutoCompleteAttibute(CString strAttrName)
{
	int nTagBegin;
	int nTagEnd;
	CStringA strCtrlText;
	GetCurrentTagPos(nTagBegin, nTagEnd);
	if (nTagBegin < 0)
	{
		//没有控件
		return;
	}
	if (nTagEnd > 0)
	{
		//控件已经结束，当前位置不是输入属性的地方
		return;
	}
	int startPos = m_sci.sci_WordStartPosition(nTagBegin + 1, TRUE);
	int endPos = m_sci.sci_WordEndPosition(nTagBegin + 1, TRUE);
	CStringA strCtrlNameA;
	m_sci.sci_GetTextRange(startPos, endPos, strCtrlNameA);
	if (strCtrlNameA.GetLength() == 0)
	{
		return;
	}
	int nMaxLen = m_sci.sci_GetTextLength();
	CStringA strTextA;
	m_sci.sci_GetTextRange(endPos, nMaxLen - 1, strTextA);
	strTextA.TrimLeft();
	CStringArray arAttrs;
	int nPos = endPos;
	bool bValue = false;
	CString strAttr;
	for (LPCSTR p = strTextA; *p; p++)
	{
		if (*p == '\"')
		{
			p++;
			while (*p != '\"')
			{
				if (!(*p))
				{
					break;
				}
				p++;
			}
			if (bValue)
			{
				bValue = false;
				strAttr.Empty();
			}
			continue;
		}
		if (*p == '\'')
		{
			p++;
			while (*p != '\'')
			{
				if (!(*p))
				{
					break;
				}
				p++;
			}
			if (bValue)
			{
				bValue = false;
				strAttr.Empty();
			}
			continue;
		}
		if (*p == '=')
		{
			if (!bValue && strAttr.GetLength())
			{
				arAttrs.Add(strAttr);
				strAttr.Empty();
			}
			bValue = true;
			continue;
		}
		if ((*p >= 'a' && *p <= 'z')
			|| (*p >= 'A' && *p <= 'Z')
			|| (*p >= '0' && *p <= '9')
			|| *p == '_')
		{
			strAttr += *p;
		}
		else if (*p == '\t' || *p == '\r' || *p == '\n' || *p == ' ')
		{
			continue;
		}
		else
		{
			if (!bValue && strAttr.GetLength())
			{
				arAttrs.Add(strAttr);
				strAttr.Empty();
			}
			break;
		}
	}
	CString strCtrlName(strCtrlNameA);

	CStringArray arCtrlAttrs;
	while (strCtrlName.GetLength())
	{
		xml_node node = m_duiProp.FindControl(strCtrlName);
		for (xml_node nodeAttr = node.first_child(); nodeAttr; nodeAttr=nodeAttr.next_sibling())
		{
			bool bExist = false;
			LPCTSTR className = nodeAttr.attribute(_T("name")).value();
			for (int i = 0; i < arCtrlAttrs.GetSize(); i++)
			{
				if (arCtrlAttrs[i].CompareNoCase(className) == 0)
				{
					bExist = true;
					break;
				}
			}
			if (bExist)
			{
				continue;
			}
			arCtrlAttrs.Add(className);
		}
		strCtrlName = node.attribute(_T("parent")).value();
	}

	CString strShow;
	while (arCtrlAttrs.GetSize())
	{
		int nMin = 0;
		for (int i = 1; i < arCtrlAttrs.GetSize(); i++)
		{
			if (arCtrlAttrs[i].CompareNoCase(arCtrlAttrs[nMin]) < 0)
			{
				nMin = i;
			}
		}

		CString className = arCtrlAttrs[nMin];
		arCtrlAttrs.RemoveAt(nMin);
		bool bExist = false;
		for (int i = 0; i < arAttrs.GetCount(); i++)
		{
			if (arAttrs[i].CompareNoCase(className) == 0)
			{
				bExist = true;
				break;
			}
		}
		if (bExist)
		{
			continue;
		}
		if(strAttrName.IsEmpty())
		{
			strShow += className;
			strShow += _T(" ");
		}
		else
		{
			CString strClass = className;
			strClass.MakeUpper();
			strAttrName.MakeUpper();
			if(strClass.Find(strAttrName) >= 0)
			{
				strShow += className;
				strShow += _T(" ");
			}
		}
	}
// 	for (int i = 0; i < arCtrlAttrs.GetSize(); i++)
// 	{
// 		LPCTSTR className = arCtrlAttrs[i];
// 		bool bExist = false;
// 		for (int i = 0; i < arAttrs.GetCount(); i++)
// 		{
// 			if (arAttrs[i].CompareNoCase(className) == 0)
// 			{
// 				bExist = true;
// 				break;
// 			}
// 		}
// 		if (bExist)
// 		{
// 			continue;
// 		}
// 		if(strAttrName.IsEmpty())
// 		{
// 			strShow += className;
// 			strShow += _T(" ");
// 		}
// 		else
// 		{
// 			CString strClass = className;
// 			strClass.MakeUpper();
// 			strAttrName.MakeUpper();
// 			if(strClass.Find(strAttrName) >= 0)
// 			{
// 				strShow += className;
// 				strShow += _T(" ");
// 			}
// 		}
// 	}

	CStringA strShowA(strShow);
	if(!strShowA.IsEmpty())
	{
		m_bAutoCtrl = false;
		m_strLastCtrlName = strCtrlNameA;
		strShowA.TrimRight();
		m_sci.sci_AutocShow(0, strShowA);
	}
}

void CDuilibSkinEditorView::AutoCompleteAttibuteValue()
{
	
}

void CDuilibSkinEditorView::AutoCompleteAttibuteValue(CString strCtrl, CString strAttr)
{
	CString strCtrlName (strCtrl);
	while (strCtrlName.GetLength())
	{
		xml_node node = m_duiProp.FindControl(strCtrlName);
		for (xml_node nodeAttr = node.first_child(); nodeAttr; nodeAttr=nodeAttr.next_sibling())
		{
			CString strAttrName(nodeAttr.attribute(_T("name")).value());
			if (strAttrName.CompareNoCase(strAttr) == 0)
			{
				CString strType(nodeAttr.attribute(_T("type")).value());
				if (strType.CompareNoCase(_T("BOOL")) == 0)
				{
					PostMessage(WM_ATTIBUTE_VALUE_AUTOSEL, AtBool);
				}
				else if (strType.CompareNoCase(_T("COLOR")) == 0)
				{
					PostMessage(WM_ATTIBUTE_VALUE_AUTOSEL, AtColor);
				}
				break;
			}
		}
		strCtrlName = node.attribute(_T("parent")).value();
	}
}

void CDuilibSkinEditorView::findMatchingBracePos(int & braceAtCaret, int & braceOpposite)
{
	int caretPos = m_sci.sci_GetCurrentPos();
	braceAtCaret = -1;
	braceOpposite = -1;
	TCHAR charBefore = '\0';

	int lengthDoc = m_sci.sci_GetLength();

	if ((lengthDoc > 0) && (caretPos > 0))
	{
		charBefore = m_sci.sci_GetCharAt(caretPos - 1);
	}
	// Priority goes to character before caret
	if (charBefore && _tcschr(TEXT("[](){}"), charBefore))
	{
		braceAtCaret = caretPos - 1;
	}

	if (lengthDoc > 0  && (braceAtCaret < 0))
	{
		// No brace found so check other side
		TCHAR charAfter = m_sci.sci_GetCharAt(caretPos);
		if (charAfter && _tcschr(TEXT("[](){}"), charAfter))
		{
			braceAtCaret = caretPos;
		}
	}
	if (braceAtCaret >= 0)
		braceOpposite = m_sci.sci_BraceMatch(braceAtCaret, 0);
}

bool CDuilibSkinEditorView::braceMatch()
{
	int braceAtCaret = -1;
	int braceOpposite = -1;
	findMatchingBracePos(braceAtCaret, braceOpposite);

	if ((braceAtCaret != -1) && (braceOpposite == -1))
	{
		m_sci.sci_BraceBadLight(braceAtCaret);
		m_sci.sci_SetHighlightGuide(0);
	}
	else
	{
		m_sci.sci_BraceHighlight(braceAtCaret, braceOpposite);

		if (m_sci.sci_GetIndentationGuides())
		{
			int columnAtCaret = m_sci.sci_GetColumn(braceAtCaret);
			int columnOpposite = m_sci.sci_GetColumn(braceOpposite);
			m_sci.sci_SetHighlightGuide((columnAtCaret < columnOpposite)?columnAtCaret:columnOpposite);
		}
	}

// 	const bool enable = (braceAtCaret != -1) && (braceOpposite != -1);
// 	enableCommand(IDM_SEARCH_GOTOMATCHINGBRACE, enable, MENU | TOOLBAR);
// 	enableCommand(IDM_SEARCH_SELECTMATCHINGBRACES, enable, MENU);
	return (braceAtCaret != -1);
}

void CDuilibSkinEditorView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if(!m_sci.GetSafeHwnd())	return;

	CRect rcClient;
	GetClientRect(rcClient);
	//rcClient.DeflateRect(1,1,1,1);
	m_sci.MoveWindow(rcClient);
}


void CDuilibSkinEditorView::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == 1)
	{
		// 		sci.sci_SetCurrentPos(0);
		// 		sci.sci_SetSel(0,0);
		m_sci.sci_GoToPos(0);
		KillTimer(nIDEvent);
		m_sci.SetFocus();
		return;
	}
	else if (nIDEvent == 2)
	{
		if (m_pDocument->GetPathName().GetLength() == 0)
		{
			return;
		}
		static long lProcessing = 0;
		static bool bProcessing = false;
		if (InterlockedExchange(&lProcessing, 1))
		{
			return;
		}
		struct _stat64 statFile;
		if (_tstat64(m_pDocument->GetPathName(), &statFile) == 0)
		{
			if (statFile.st_mtime != m_statFile.st_mtime)
			{
				if (AfxMessageBox(m_pDocument->GetPathName() + _T("已经发生改变，是否重新加载？"), MB_YESNO) != IDYES)
				{
					m_statFile = statFile;
					InterlockedExchange(&lProcessing, 0);
					return;
				}
				if (m_sci.LoadFile(m_pDocument->GetPathName()))
				{
					m_statFile = statFile;
				}
			}
		}
		else
		{
			if (AfxMessageBox(m_pDocument->GetPathName() + _T("已经不存在，是否关闭？"), MB_YESNO) != IDYES)
			{
				InterlockedExchange(&lProcessing, 0);
				this->CloseWindow();
				return;
			}
		}
		InterlockedExchange(&lProcessing, 0);
	}

	CView::OnTimer(nIDEvent);
}


void CDuilibSkinEditorView::OnEditCut()
{
	if (!IsWindow(m_sci))
	{
		return;
	}
	m_sci.sci_Cut();
}

void CDuilibSkinEditorView::OnUpdateEditCut(CCmdUI *pCmdUI)
{
	if (!IsWindow(m_sci))
	{
		return;
	}
	pCmdUI->Enable(m_sci.sci_CanCut());
}

void CDuilibSkinEditorView::OnEditCopy()
{
	if (!IsWindow(m_sci))
	{
		return;
	}
	m_sci.sci_Copy();
}

void CDuilibSkinEditorView::OnUpdateEditCopy(CCmdUI *pCmdUI)
{
	if (!IsWindow(m_sci))
	{
		return;
	}
	pCmdUI->Enable(m_sci.sci_CanCopy());
}

void CDuilibSkinEditorView::OnEditFindRepalce()
{
	theApp.GetMainWnd()->PostMessage(WM_FINDREPLACE);
}

void CDuilibSkinEditorView::OnUpdateEditFindRepalce(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
}

void CDuilibSkinEditorView::OnEditPaste()
{
	if (!IsWindow(m_sci))
	{
		return;
	}
	m_sci.sci_Paste();
}

void CDuilibSkinEditorView::OnUpdateEditPaste(CCmdUI *pCmdUI)
{
	if (!IsWindow(m_sci))
	{
		return;
	}
	pCmdUI->Enable(m_sci.sci_CanPaste());
}

void CDuilibSkinEditorView::OnEditUndo()
{
	if (!IsWindow(m_sci))
	{
		return;
	}
	m_sci.sci_Undo();
}

void CDuilibSkinEditorView::OnUpdateEditUndo(CCmdUI *pCmdUI)
{
	if (!IsWindow(m_sci))
	{
		return;
	}
	pCmdUI->Enable(m_sci.sci_CanUndo());
}

void CDuilibSkinEditorView::OnEditRedo()
{
	if (!IsWindow(m_sci))
	{
		return;
	}
	m_sci.sci_Redo();
}

void CDuilibSkinEditorView::OnUpdateEditRedo(CCmdUI *pCmdUI)
{
	if (!IsWindow(m_sci))
	{
		return;
	}
	pCmdUI->Enable(m_sci.sci_CanRedo());
}

LRESULT CDuilibSkinEditorView::OnSelectControl(WPARAM wParam, LPARAM lParam)
{
	LPCTSTR lpszFileName = (LPCTSTR)wParam;
	if (!lpszFileName || !lpszFileName[0])
	{
		return 0;
	}
	if (m_pDocument->GetPathName().Find(lpszFileName) < 0)
	{
		return 0;
	}
	CStringA strTextA;
	m_sci.sci_GetTextAll(strTextA);
	CString strTextT;
	CreateString_InitWithUTF8Text(strTextT, strTextA.GetBuffer(0));
	const char* pstr = CreateUTF8TextInitWithString(strTextT.Left(lParam));
	int nLen = strlen(pstr);
	delete[] pstr;
	//m_sci.sci_SetCurrentPos(strTextAPre.getp);
	int nTagBegin = nLen - 1;
	int nTagEnd = -1;
	int nStatus = 0;
	pstr = strTextA;
	for (LPCSTR p = pstr + nTagBegin; *p; p++)
	{
		switch (*p)
		{
		case '<':
			if (p[1] == '/')
			{
				nStatus--;
			}
			else
			{
				nStatus++;
			}
			break;
		case '/':
			break;
		case '>':
			if (p[- 1] == '/')
			{
				nStatus--;
			}
			if (nStatus == 0)
			{
				nTagEnd = p - pstr + 1;
			}
			break;
		}
		if (nTagEnd != -1)
		{
			break;
		}
	}

	m_sci.sci_SetSelectionStart(nTagBegin);
	m_sci.sci_SetSelectionEnd(nTagEnd);
	m_sci.sci_ScrollCaret();
	return 0;
}

LRESULT CDuilibSkinEditorView::OnSetEditFocus(WPARAM wParam, LPARAM lParam)
{
	//AfxMessageBox(_T("1"));
	Sleep(1);
	m_sci.SetFocus();
	//m_sci.BringWindowToTop();
	//m_sci.SetForegroundWindow();
	return 0;
}

BOOL CDuilibSkinEditorView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	NMHDR *phDR;
	phDR = (NMHDR*)lParam;

	if (phDR == NULL || phDR->hwndFrom != m_sci.m_hWnd)
	{
		return CView::OnNotify(wParam, lParam, pResult);
	}

	//输入"<",  列出Control
	//输入"<x",  列出包含'x'的Control
	//输入"</", 反括号, 回溯寻找未封闭的左括号, 自动列出未封闭的控件名
	//<Control a, 找到Control名 列出Control的属性列表


	SCNotification *pMsg = (SCNotification*)lParam;
	switch (pMsg->nmhdr.code)
	{
	case SCK_BACK:
		{
			//InsertMsg(_T("SCK_BACK"));
		}
		break;
	case SCN_CHARADDED:	//输入字符
		{
			if(pMsg->ch == '<' || pMsg->ch == '/')
			{
				AutoCompleteControl(_T(""));
			}
			else if(pMsg->ch == ' ')
			{
				AutoCompleteAttibute(_T(""));
			}
			else if(pMsg->ch == '>')
			{
				//自动完成标签
				AutoCompleteTag();
			}
			else if(pMsg->ch == '=')
			{
				
			}
			else if(pMsg->ch == '"')
			{

			}
			else if(pMsg->ch == '\n')
			{
				//自动缩进
				AutoIndent();

			}
			else
			{
				int pos = m_sci.sci_GetCurrentPos();
				int startPos = m_sci.sci_WordStartPosition(pos-1, TRUE);
				int endPos = m_sci.sci_WordEndPosition(pos-1, TRUE);

				CStringA objectNameA;
				m_sci.sci_GetTextRange(startPos, endPos, objectNameA);
				CString objectName(objectNameA);

				CStringA strLeftA;
				m_sci.sci_GetTextRange(startPos-1, startPos, strLeftA);
				if(strLeftA == '<' || strLeftA == '/')
				{
					AutoCompleteControl(objectName);
				}
				else
				{
					AutoCompleteAttibute(objectName);
				}
			}			
		}
		break;
	case SCN_MODIFIED:
		{
			if(pMsg->linesAdded!=0 &&
				m_sci.sci_GetMarginWidthN(0)!=0) //自动调整行号的宽度
			{
				int lineNumber = m_sci.sci_GetLineCount();
				int charWidth = m_sci.sci_TextWidth(STYLE_LINENUMBER, "9");
				CString tempLine;
				tempLine.Format(_T("%d"), lineNumber);
				m_sci.sci_SetMarginWidthN(0,tempLine.GetLength()*charWidth+4);
			}
		}
		break;
	case SCN_SAVEPOINTREACHED:	//文件被保存
		{
			theApp.RefreshPreview();
			m_pDocument->SetModifiedFlag(FALSE);
		}
		break;
	case SCN_SAVEPOINTLEFT: //文件被修改
		{
			m_pDocument->SetModifiedFlag(TRUE);
		}
		break;
	case SCN_AUTOCSELECTION:
		{
			OnAutoSelection();
		}
		break;
	case SCN_UPDATEUI:
		{
			braceMatch();
			XmlMatchedTagsHighlighter xmlTagMatchHiliter(&m_sci);
			XmlMatchedTagsHighlighter::XmlMatchedTagsPos posTag = xmlTagMatchHiliter.tagMatch(false);
			if (posTag.tagOpenStart && posTag.tagNameEnd &&
				m_posTagLastSel.tagOpenStart != posTag.tagOpenStart && m_posTagLastSel.tagNameEnd != posTag.tagNameEnd)
			{
				m_posTagLastSel = posTag;
				CStringA strName;
				m_sci.sci_GetTextRange(posTag.tagOpenStart + 1, posTag.tagNameEnd, strName);
				strName.TrimLeft();
				CStringA strAttrs;
				m_sci.sci_GetTextRange(posTag.tagNameEnd, posTag.tagOpenEnd - 1, strAttrs);
				theApp.GetMainWnd()->SendMessage(WM_REFRESH_PROPERTY, (WPARAM)(LPCTSTR)CString(strName), (LPARAM)(LPCTSTR)CString(strAttrs));			
			}
		}
		break;
	default:
		{
// 			CString sDbg;
// 			sDbg.Format(_T("SCN:%d\n"), pMsg->nmhdr.code);
// 			OutputDebugString(sDbg);
			m_sci.OnParentNotify(pMsg);
		}
		break;
	}


	return CView::OnNotify(wParam, lParam, pResult);
}

void CDuilibSkinEditorView::AutoCompleteTag()
{
	int nTagBegin = -1;
	int nTagEnd = -1;
	GetCurrentTagPos(nTagBegin, nTagEnd);

	if (nTagBegin < 0)
	{
		return;
	}
	CString strNode;
	if (nTagEnd >= 0)
	{
		CStringA strNode;
		m_sci.sci_GetTextRange(nTagBegin + 1, nTagEnd, strNode);
		strNode.TrimRight();
		if (strNode.Right(1) == "/")
		{
			return;
		}
		strNode.TrimLeft();
		if (strNode[0] == '/')
		{
			return;
		}
		for (int i = 0; i < strNode.GetLength(); i++)
		{
			if (strNode[i] == ' ' || strNode[i] == '\t' || strNode[i] == '\r' || strNode[i] == '\n')
			{
				strNode = strNode.Left(i);
				break;
			}
		}
		if (strNode.Right(1) == ">")
		{
			return;
		}
		strNode = "</" + strNode + ">";
		m_sci.sci_InsertText(m_sci.sci_GetCurrentPos(), (LPCSTR)strNode);
	}
}

void CDuilibSkinEditorView::GetCurrentTagPos(int &nTagBegin, int &nTagEnd)
{
	CStringA strText;
	nTagBegin = nTagEnd = -1;
	int pos = m_sci.sci_GetCurrentPos();
	m_sci.sci_GetTextRange(0, pos, strText);
	enum 
	{
		StatusNormal,
		StatusDoubleString,
		StatusSingleString,
	}nStatus = StatusNormal;
	for (int i = 0; i < strText.GetLength(); i++)
	{
		switch (strText[i])
		{
		case '\"':
			if (nStatus == StatusDoubleString)
			{
				nStatus = StatusNormal;
			}
			else if (nStatus == StatusNormal)
			{
				nStatus = StatusDoubleString;
			}
			break;
		case '\'':
			if (nStatus == StatusSingleString)
			{
				nStatus = StatusNormal;
			}
			else if (nStatus == StatusNormal)
			{
				nStatus = StatusSingleString;
			}
			break;
		case '<':
			if (nStatus == StatusNormal)
			{
				nTagBegin = i;
				nTagEnd = -1;
			}
			break;
		case '>':
			if (nStatus == StatusNormal)
			{
				nTagEnd = i;
			}
			break;
		}
	}
}

char* CreateUTF8TextInitWithString(CString strValue)  
{  
	char *buffer = NULL;  
	int  length;  

#ifdef _UNICODE  
	length = WideCharToMultiByte(CP_UTF8, 0, strValue, -1, NULL, 0, NULL, NULL);  
#else  
	return NULL;  
#endif  
	if (length <= 0)  
	{  
		return NULL;  
	}  

	buffer = new char[length];  
	if (buffer == NULL)  
	{  
		return NULL;  
	}  

	ZeroMemory(buffer, length);  

#ifdef _UNICODE  
	WideCharToMultiByte(CP_UTF8, 0, strValue, -1, buffer, length, NULL, NULL);  
#else  
	strcpy_s(buffer, length, strValue);  
#endif  

	return buffer;  
}  


BOOL CreateString_InitWithUTF8Text(CString& str, char* pUTF8Text)  
{  
	if (NULL == pUTF8Text)  
	{  
		return FALSE;  
	}  

	int  unicodeLen = ::MultiByteToWideChar( CP_UTF8,  
		0,  
		pUTF8Text,  
		-1,  
		NULL,  
		0 );   

	wchar_t*  pUnicode = new  wchar_t[unicodeLen+1];  
	if (NULL == pUnicode)  
	{  
		return FALSE;  
	}  

	MultiByteToWideChar( CP_UTF8, 0, pUTF8Text, -1, (LPWSTR)pUnicode, unicodeLen );  

	str = pUnicode;  

	delete []pUnicode;  
	return TRUE;  
}  

void CDuilibSkinEditorView::OnAutoSelection()
{
	int pos = m_sci.sci_GetCurrentPos();
	int startPos = m_sci.sci_WordStartPosition(pos, TRUE);
	int endPos = m_sci.sci_WordEndPosition(pos, TRUE);
	CStringA strSelected;
	m_sci.sci_AutocGetCurrentText(strSelected);

	m_sci.sci_SetTargetStart(startPos);
	m_sci.sci_SetTargetEnd(endPos);
	m_sci.sci_ReplaceTarget(-1, strSelected);
	int nCurrentPos = startPos + strSelected.GetLength();
	m_sci.sci_GoToPos(nCurrentPos);
	m_sci.sci_AutocCancel(); //不使用默认的插人

	if (m_bAutoCtrl)
	{
		return;
	}
	CString strCtrlName (m_strLastCtrlName);
	while (strCtrlName.GetLength())
	{
		xml_node node = m_duiProp.FindControl(strCtrlName);
		for (xml_node nodeAttr = node.first_child(); nodeAttr; nodeAttr=nodeAttr.next_sibling())
		{
			bool bExist = false;
			CStringA strAttrName(nodeAttr.attribute(_T("name")).value());
			if (strAttrName.CompareNoCase(strSelected) == 0)
			{
				CStringA strDefault(nodeAttr.attribute(_T("default")).value());
				m_sci.sci_InsertText(nCurrentPos, "=\"" + strDefault + "\"");
				nCurrentPos += strlen("=\"");
				m_sci.sci_SetSel(nCurrentPos, nCurrentPos + strDefault.GetLength());
				CString strComment(nodeAttr.attribute(_T("comment")).value());
				//转换成UTF-8
				CHAR* pstrTips = CreateUTF8TextInitWithString(strComment);
				if (pstrTips)
				{
					m_sci.sci_CallTipShow(nCurrentPos, pstrTips);
				}
				delete[] pstrTips;
				AutoCompleteAttibuteValue();
				break;
			}
		}
		strCtrlName = node.attribute(_T("parent")).value();
	}
}

void CDuilibSkinEditorView::AutoIndent()
{
	CStringA strText;
	int nPos = m_sci.sci_GetCurrentPos();
	if (nPos <= 0)
	{
		return;
	}
	m_sci.sci_GetText(nPos, strText);
	if (strText.GetLength() == 0)
	{
		return;
	}

	int nTagBegin = -1;
	int nTagEnd = -1;
	bool bEnd = false;
	while (nPos)
	{
		nPos--;
		char cLetter = strText[nPos];
		if (cLetter == '<')
		{
			nTagBegin = nPos;
			break;
		}
		if (cLetter == '>')
		{
			nTagEnd = nPos;
		}
		else if (cLetter == '/')
		{
			if (nTagEnd != -1)
			{
				bEnd = true;
			}
		}
		else if (cLetter == ' ')
		{
		}
		else if (cLetter == '\t')
		{
		}
	}
	//m_sci.sci_GetCurLine()
	m_sci.sci_GetTextLine(m_sci.sci_LineFromPosition(nTagBegin), strText);
	CStringA strPre = strText;
	strText.TrimLeft();//删除左边的缩进
	strPre = strPre.Left(strPre.GetLength() - strText.GetLength());
	strPre.Remove('\r');
	strPre.Remove('\n');
	CStringA strPost(strPre);
	if (!bEnd)
	{
		strPre += '\t';
	}
	m_sci.sci_ReplaceSel(strPre);
	m_sci.sci_GetCurLine(strText);
	strText.TrimLeft();
	if (strText.GetLength())
	{
		if (strText[0] == '<')
		{
			strText.Delete(0);
			strText.TrimLeft();
			if (strText.GetLength())
			{
				if (strText[0] == '/')
				{
					m_sci.sci_InsertText(m_sci.sci_GetCurrentPos(), "\r\n" + strPost);
				}
			}
		}
	}
}

void CDuilibSkinEditorView::OnFileSave(CArchive& ar)
{
	if (IsWindow(m_sci.m_hWnd))
	{
		KillTimer(2);
		CStringA strText;
		m_sci.sci_GetTextAll(strText);
		ar.Write(strText.GetBuffer(), strText.GetLength());
		//ar.GetFile()->Close();
		//ar.Close();
		m_sci.SetSavePoint();
	}
}

void CDuilibSkinEditorView::OnPostFileSave()
{
	struct _stat64 statFile;
	if (_tstat64(m_pDocument->GetPathName(), &statFile) == 0)
	{
		m_statFile = statFile;
		SetTimer(2, 2000, NULL);
	}
}

void CDuilibSkinEditorView::OnSettingsChanged()
{
	m_sci.InitForXML();
}


afx_msg LRESULT CDuilibSkinEditorView::OnAttibuteValueAutoSel(WPARAM wParam, LPARAM lParam)
{
	m_bAutoCtrl = false;
// 	CString strTextSel;
// 	m_sci.sci_GetSelText(strTextSel);
	switch(wParam)
	{
	case AtBool:
		m_sci.sci_AutocShow(0, "true false");
		break;
	case AtColor:
		m_sci.sci_AutocShow(0, "#FFFFFFFF #FF000000");
		break;
	}
	return 0;
}


void CDuilibSkinEditorView::OnSetFocus(CWnd* pOldWnd)
{
	CView::OnSetFocus(pOldWnd);

	m_sci.SetFocus();
}

CSciWnd& CDuilibSkinEditorView::GetSci()
{
	return m_sci;
}
