// SciWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "SciWnd.h"
#include "colors.h"

#define VIEW_MARGIN_LINENUMBER 0
#define VIEW_MARGIN_BREAK	1
//#define VIEW_MARGIN_MARKER	2
#define VIEW_MARGIN_FOLD	2

#define BREAK_TYPEN	0

// CSciWnd
IMPLEMENT_DYNAMIC(CSciWnd, CWnd)

CSciWnd::CSciWnd()
{
	m_hSciLexer = ::LoadLibrary(_T("SciLexer.dll"));
	memset(m_arrSpace,' ', 1024);
	memset(m_arrTab, '	', 1024);
}

CSciWnd::~CSciWnd()
{
	::FreeLibrary(m_hSciLexer);
}

BEGIN_MESSAGE_MAP(CSciWnd, CWnd)
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()


// CSciWnd 消息处理程序
BOOL CSciWnd::Create (DWORD dwExStyle, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	if(CreateEx(0, _T("Scintilla"), _T(""), dwStyle, rect, pParentWnd, nID, NULL))
	{	
		m_sendeditor = (int (__cdecl *)(void *,int,int,int))SendMessage(SCI_GETDIRECTFUNCTION,0,0);
		m_pSendEditor = (void *)SendMessage(SCI_GETDIRECTPOINTER,0,0);
		ModifyStyleEx(0, WS_EX_LAYERED);
		SetLayeredWindowAttributes(0xFF0000, 255, LWA_ALPHA);

		return TRUE;
	}
	return FALSE;
}

BOOL CSciWnd::LoadFile(LPCTSTR szPath)
{
	if (szPath == NULL || *szPath == '\0')
		return TRUE;

	CFile file;
	if(!file.Open(szPath, CFile::modeRead))	return FALSE;
	UINT buflen = (UINT)file.GetLength();

	CHAR *pBuffer = new CHAR[buflen + 1];
	file.Read((void *)pBuffer, buflen);
	pBuffer[buflen] = 0;
	sci_SetText(pBuffer);
	SetSavePoint();//这是未修改的文档

	delete []pBuffer;
	file.Close();

	return TRUE;
}

BOOL CSciWnd::SaveFile(LPCTSTR szPath)
{

	if (szPath == NULL || *szPath == '\0')
		return TRUE;

	CFile file;
	if(!file.Open(szPath, CFile::modeCreate|CFile::modeWrite))	return FALSE;

	int buflen = sci_GetLength();
	TCHAR *pBuffer = new TCHAR[buflen + 1];
	if (pBuffer != NULL)
	{
		execute(SCI_GETTEXT, buflen + 1,(long)pBuffer);
		file.Write((void *)pBuffer, buflen);
		delete [] pBuffer;
	}
	file.Close();
	return TRUE;
}

void CSciWnd::InitForXML()
{
	//sci_UsePopup(FALSE);
	//sci_SetReadOnly(TRUE);

	//theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_WINDOWS_7);

	CStringA szFontName(theApp.GetString(_T("FontName"), _T("Courier New")));
	int nFontSize = theApp.GetInt(_T("FontSize"), 12);
	const int nLineSpacing = theApp.GetInt(_T("LineSpacing"), 3);
	const int bAutoWrap = theApp.GetInt(_T("AutoWrap"), 0);
	const COLORREF clrDefBack = theApp.GetInt(_T("BgColor"), 0xBBFFBB);	//背景色
	const int nUpperSpace = nLineSpacing;				//行上方间距
	const int nUnderSpace = nLineSpacing;				//行下方间距
	sci_StyleSetFont(STYLE_DEFAULT, szFontName);
	sci_StyleSetSize(STYLE_DEFAULT, nFontSize);
	sci_StyleSetFore(STYLE_DEFAULT, RGB(0,0,0));
	sci_StyleSetBack(STYLE_DEFAULT, clrDefBack);
	execute(SCI_SETEXTRAASCENT, nUpperSpace);
	execute(SCI_SETEXTRADESCENT, nUnderSpace);

// 
// 	sci_StyleSetBack(STYLE_BRACELIGHT,			clrDefBack);
// 	sci_StyleSetBack(STYLE_BRACEBAD,			clrDefBack);
// 	sci_StyleSetBack(STYLE_CONTROLCHAR,			clrDefBack);
// 	sci_StyleSetBack(STYLE_INDENTGUIDE,			clrDefBack);
// 	sci_StyleSetBack(STYLE_CALLTIP,				clrDefBack);

	sci_StyleSetFont(STYLE_LINENUMBER, szFontName);
	sci_StyleSetSize(STYLE_LINENUMBER, nFontSize);
	sci_StyleSetBack(STYLE_LINENUMBER, clrDefBack);
	sci_StyleSetFore(STYLE_LINENUMBER, 0x808080);

	sci_StyleSetBack(STYLE_INDENTGUIDE,	clrDefBack);
	sci_StyleSetFore(STYLE_INDENTGUIDE, 0xC0C0C0);

	sci_StyleSetCharacterSet(SCE_C_STRING, SC_CHARSET_DEFAULT);
	//sci_StyleSetWeight(STYLE_DEFAULT, 15);
	//sci_StyleClearAll();

	sci_SetSelBack(TRUE, RGB(192,192,192));
	//sci_SetSelFore(TRUE, RGB(255,255,255));

	//编码	
	sci_SetCodePage(SC_CP_UTF8);

	sci_SetLexer(SCLEX_XML);
	//XML
	COLORREF crfComment = RGB(0,150,0);
	for (int i = SCE_H_DEFAULT; i <= SCE_HPHP_OPERATOR; i++)
	{
		sci_StyleSetBack(i,			clrDefBack);
	}
	sci_StyleSetFore(SCE_H_COMMENT,				crfComment);
	sci_StyleSetFore(SCE_H_ATTRIBUTEUNKNOWN,	crfComment);
	//字符串双引号未封闭时
	sci_StyleSetFore(SCE_H_DOUBLESTRING,	RGB(128,0,255));
	sci_StyleSetFore(SCE_H_SINGLESTRING,	RGB(128,0,255));
	//标签未配对时颜色
	sci_StyleSetFore(SCE_H_TAGUNKNOWN,		RGB(0,0,255));
	sci_StyleSetFore(SCE_H_TAGEND,			RGB(0,0,255));
	//标签配对时颜色
	sci_StyleSetFore(SCE_H_TAG,				RGB(0,0,255));
	//属性颜色
	sci_StyleSetFore(SCE_H_ATTRIBUTE,		RGB(255,0,0));

	//行号
	sci_SetMarginTypeN(VIEW_MARGIN_LINENUMBER, SC_MARGIN_NUMBER);
	int lineNumber = sci_GetLineCount();
	int charWidth = sci_TextWidth(STYLE_LINENUMBER, "9");
	CString tempLine;
	tempLine.Format(_T("%d"), lineNumber);
	sci_SetMarginWidthN(VIEW_MARGIN_LINENUMBER,tempLine.GetLength()*charWidth+4);
	sci_SetMarginWidthN(SC_MARGIN_BACK, tempLine.GetLength()*charWidth+4);
	
	//代码折叠
	sci_SetMarginWidthN(VIEW_MARGIN_FOLD, SC_MARGIN_SYMBOL);
	sci_SetMarginWidthN(VIEW_MARGIN_FOLD,15);
	sci_SetMarginMaskN(VIEW_MARGIN_FOLD, SC_MASK_FOLDERS);
	sci_SetMarginSensitiveN(VIEW_MARGIN_FOLD, TRUE);
	sci_MarkerDefine(SC_MASK_FOLDERS, SC_MARK_BOXMINUSCONNECTED);

	sci_SetProperty("fold", "1");
	sci_SetProperty("fold.html", "1");
	sci_SetProperty("fold.compact", "0");
	sci_SetProperty("fold.hypertext.comment", "1");

	//如果折叠就在折叠行的下面画一条横线  }  
	sci_SetFoldFlags(SC_FOLDFLAG_LINEAFTER_CONTRACTED); 

	//////////////////////////////////////////////////////////////////////////
	sci_SetTabWidth(4);	//制表符宽度
	sci_SetIndentationGuides(SC_IV_LOOKBOTH);	//缩进提示线
	sci_SetIndent(4);	//缩进提示线的字符宽度
	sci_SetUseTabs(TRUE);

	//括号匹配的颜色
	sci_StyleSetBack(STYLE_BRACELIGHT, RGB(0,255,0));

	const int NB_FOLDER_STATE = 7;
	const int _markersArray[][7] = {
		{SC_MARKNUM_FOLDEROPEN, SC_MARKNUM_FOLDER, SC_MARKNUM_FOLDERSUB, SC_MARKNUM_FOLDERTAIL, SC_MARKNUM_FOLDEREND,        SC_MARKNUM_FOLDEROPENMID,     SC_MARKNUM_FOLDERMIDTAIL},
		{SC_MARK_MINUS,         SC_MARK_PLUS,      SC_MARK_EMPTY,        SC_MARK_EMPTY,         SC_MARK_EMPTY,               SC_MARK_EMPTY,                SC_MARK_EMPTY},
		{SC_MARK_ARROWDOWN,     SC_MARK_ARROW,     SC_MARK_EMPTY,        SC_MARK_EMPTY,         SC_MARK_EMPTY,               SC_MARK_EMPTY,                SC_MARK_EMPTY},
		{SC_MARK_CIRCLEMINUS,   SC_MARK_CIRCLEPLUS,SC_MARK_VLINE,        SC_MARK_LCORNERCURVE,  SC_MARK_CIRCLEPLUSCONNECTED, SC_MARK_CIRCLEMINUSCONNECTED, SC_MARK_TCORNERCURVE},
		{SC_MARK_BOXMINUS,      SC_MARK_BOXPLUS,   SC_MARK_VLINE,        SC_MARK_LCORNER,       SC_MARK_BOXPLUSCONNECTED,    SC_MARK_BOXMINUSCONNECTED,    SC_MARK_TCORNER}
	};
	for (int j = 0 ; j < NB_FOLDER_STATE ; ++j)
	{
		defineMarker(_markersArray[FOLDER_TYPE][j], _markersArray[4][j], 0xFFFFFF, 0x808080, 0xFF0000);
	}

	execute(SCI_MARKERENABLEHIGHLIGHT, true);
	execute(SCI_SETWHITESPACEFORE, true, 0);

// 	int mode = (meth == LINEWRAP_ALIGNED)?SC_WRAPINDENT_SAME:\
// 		(meth == LINEWRAP_INDENT)?SC_WRAPINDENT_INDENT:SC_WRAPINDENT_FIXED;
	execute(SCI_SETWRAPINDENTMODE, SC_WRAPINDENT_SAME);

	//当前行高亮显示
	sci_SetCaretLineVisible(TRUE);
	sci_SetCaretLineBack(extremeLitePurple);


	execute(SCI_SETWRAPMODE, (WPARAM)bAutoWrap);	//是否自动换行


	//execute(SCI_SETEDGECOLUMN, 80);				//当每行超过N个字符时特殊显示
	//execute(SCI_SETEDGEMODE, EDGE_BACKGROUND);	//超出部分显示方式

	execute(SCI_SETVIEWEOL, FALSE);				//是否显示行尾符号
	execute(SCI_SETVIEWWS, SCWS_INVISIBLE);		//显示TAB符号
// 	execute(SCI_SETWRAPVISUALFLAGSLOCATION, SC_WRAPVISUALFLAGLOC_DEFAULT);
// 	execute(SCI_SETWRAPVISUALFLAGS, SC_WRAPVISUALFLAG_END:SC_WRAPVISUALFLAG_NONE);
	execute(SCI_SETZOOM, 0);

	execute(SCI_SETMULTIPLESELECTION, true);
	// Make backspace or delete work with multiple selections
	execute(SCI_SETADDITIONALSELECTIONTYPING, true);
	// Turn virtual space on
	execute(SCI_SETVIRTUALSPACEOPTIONS, SCVS_RECTANGULARSELECTION);
	// Turn multi-paste on
	execute(SCI_SETMULTIPASTE, SC_MULTIPASTE_EACH);
	sci_SetScrollWidthTracking(TRUE);
	execute(SCI_SETSTYLEBITS, 8);

	//高亮格式
	sci_IndicSetFore(SCE_UNIVERSAL_FOUND_STYLE, red);
	sci_IndicSetFore(SCE_UNIVERSAL_FOUND_STYLE_SMART, liteGreen);
	sci_IndicSetFore(SCE_UNIVERSAL_FOUND_STYLE_INC, blue);
	sci_IndicSetFore(SCE_UNIVERSAL_TAGMATCH, purple);
	sci_IndicSetFore(SCE_UNIVERSAL_TAGATTR, yellow);
	sci_IndicSetFore(SCE_UNIVERSAL_FOUND_STYLE_EXT1, cyan);
	sci_IndicSetFore(SCE_UNIVERSAL_FOUND_STYLE_EXT2, orange);
	sci_IndicSetFore(SCE_UNIVERSAL_FOUND_STYLE_EXT3, yellow);
	sci_IndicSetFore(SCE_UNIVERSAL_FOUND_STYLE_EXT4, purple);
	sci_IndicSetFore(SCE_UNIVERSAL_FOUND_STYLE_EXT5, darkGreen);

	execute(SCI_INDICSETSTYLE, SCE_UNIVERSAL_FOUND_STYLE_SMART, INDIC_ROUNDBOX);
	execute(SCI_INDICSETSTYLE, SCE_UNIVERSAL_FOUND_STYLE, INDIC_ROUNDBOX);
	execute(SCI_INDICSETSTYLE, SCE_UNIVERSAL_FOUND_STYLE_INC, INDIC_ROUNDBOX);
	execute(SCI_INDICSETSTYLE, SCE_UNIVERSAL_TAGMATCH, INDIC_ROUNDBOX);
	execute(SCI_INDICSETSTYLE, SCE_UNIVERSAL_TAGATTR, INDIC_ROUNDBOX);
	execute(SCI_INDICSETSTYLE, SCE_UNIVERSAL_FOUND_STYLE_EXT1, INDIC_ROUNDBOX);
	execute(SCI_INDICSETSTYLE, SCE_UNIVERSAL_FOUND_STYLE_EXT2, INDIC_ROUNDBOX);
	execute(SCI_INDICSETSTYLE, SCE_UNIVERSAL_FOUND_STYLE_EXT3, INDIC_ROUNDBOX);
	execute(SCI_INDICSETSTYLE, SCE_UNIVERSAL_FOUND_STYLE_EXT4, INDIC_ROUNDBOX);
	execute(SCI_INDICSETSTYLE, SCE_UNIVERSAL_FOUND_STYLE_EXT5, INDIC_ROUNDBOX);	

	execute(SCI_INDICSETALPHA, SCE_UNIVERSAL_FOUND_STYLE_SMART, 100);
	execute(SCI_INDICSETALPHA, SCE_UNIVERSAL_FOUND_STYLE, 100);
	execute(SCI_INDICSETALPHA, SCE_UNIVERSAL_FOUND_STYLE_INC, 100);
	execute(SCI_INDICSETALPHA, SCE_UNIVERSAL_TAGMATCH, 100);
	execute(SCI_INDICSETALPHA, SCE_UNIVERSAL_TAGATTR, 100);
	execute(SCI_INDICSETALPHA, SCE_UNIVERSAL_FOUND_STYLE_EXT1, 100);
	execute(SCI_INDICSETALPHA, SCE_UNIVERSAL_FOUND_STYLE_EXT2, 100);
	execute(SCI_INDICSETALPHA, SCE_UNIVERSAL_FOUND_STYLE_EXT3, 100);
	execute(SCI_INDICSETALPHA, SCE_UNIVERSAL_FOUND_STYLE_EXT4, 100);
	execute(SCI_INDICSETALPHA, SCE_UNIVERSAL_FOUND_STYLE_EXT5, 100);	

	execute(SCI_INDICSETUNDER, SCE_UNIVERSAL_FOUND_STYLE_SMART, true);
	execute(SCI_INDICSETUNDER, SCE_UNIVERSAL_FOUND_STYLE, true);
	execute(SCI_INDICSETUNDER, SCE_UNIVERSAL_FOUND_STYLE_INC, true);
	execute(SCI_INDICSETUNDER, SCE_UNIVERSAL_TAGMATCH, true);
	execute(SCI_INDICSETUNDER, SCE_UNIVERSAL_TAGATTR, true);
	execute(SCI_INDICSETUNDER, SCE_UNIVERSAL_FOUND_STYLE_EXT1, true);
	execute(SCI_INDICSETUNDER, SCE_UNIVERSAL_FOUND_STYLE_EXT2, true);
	execute(SCI_INDICSETUNDER, SCE_UNIVERSAL_FOUND_STYLE_EXT3, true);
	execute(SCI_INDICSETUNDER, SCE_UNIVERSAL_FOUND_STYLE_EXT4, true);
	execute(SCI_INDICSETUNDER, SCE_UNIVERSAL_FOUND_STYLE_EXT5, true);

	//快捷键不再显示乱码
	execute(SCI_CLEARCMDKEY, (WPARAM)('F'+(SCMOD_CTRL<<16)), SCI_NULL);
	execute(SCI_CLEARCMDKEY, (WPARAM)('H'+(SCMOD_CTRL<<16)), SCI_NULL);
	execute(SCI_CLEARCMDKEY, (WPARAM)('Z'+((SCMOD_CTRL|SCMOD_SHIFT)<<16)), SCI_NULL);
	execute(SCI_CLEARCMDKEY, (WPARAM)('Y'+(SCMOD_CTRL<<16)), SCI_NULL);
	execute(SCI_CLEARCMDKEY, (WPARAM)SCK_ESCAPE, SCI_NULL);
	//execute(SCI_CLEARALLCMDKEYS);

}

void CSciWnd::OnRButtonUp(UINT nFlags, CPoint point) 
{
	CWnd *pOwner = GetOwner();
	if (pOwner && IsWindow(pOwner->m_hWnd))
		pOwner->SendMessage(WM_SCIWND_RBUTTONUP, (WPARAM)&point, 0);
	CWnd::OnRButtonUp(nFlags, point);
}

BOOL CSciWnd::OnParentNotify(SCNotification *pMsg)
{
	switch (pMsg->nmhdr.code)
	{
	case SCN_STYLENEEDED:
		break;
	case SCN_CHARADDED:
		break;
	case SCN_SAVEPOINTREACHED:
		break;
	case SCN_SAVEPOINTLEFT: //文件被修改
		break;
	case SCN_MODIFYATTEMPTRO:
		break;
	case SCN_KEY:
		break;
	case SCN_DOUBLECLICK:
		break;
	case SCN_UPDATEUI:
		//m_SciWnd.BraceHighlight();
		break;
	case SCN_MODIFIED:
		{
			if(pMsg->linesAdded!=0 &&
				sci_GetMarginWidthN(VIEW_MARGIN_LINENUMBER)!=0) //自动调整行号的宽度
			{
				int lineNumber = sci_GetLineCount();
				int charWidth = sci_TextWidth(STYLE_LINENUMBER, "9");
				CString tempLine;
				tempLine.Format(_T("%d"), lineNumber);
				sci_SetMarginWidthN(VIEW_MARGIN_LINENUMBER,tempLine.GetLength()*charWidth+4);
			}
		}
		break;
	case SCN_MACRORECORD:
		break;
	case SCN_MARGINCLICK:
		{
			int line = sci_LineFromPosition(pMsg->position);
			sci_ToggleFold(line);
		}
		break;
	case SCN_NEEDSHOWN:
		break;
	case SCN_PAINTED:
		break;
	case SCN_USERLISTSELECTION:
		break;
	case SCN_URIDROPPED:
		break;
	case SCN_DWELLSTART:
		break;
	case SCN_DWELLEND:
		break;
	case SCN_ZOOM:
		break;
	case SCN_HOTSPOTCLICK:
		break;
	case SCN_HOTSPOTDOUBLECLICK:
		break;
	case SCN_HOTSPOTRELEASECLICK:
		break;
	case SCN_INDICATORCLICK:
		break;
	case SCN_INDICATORRELEASE:
		break;
	case SCN_CALLTIPCLICK:
		break;
	case SCN_AUTOCSELECTION:
		{
			int pos = sci_GetCurrentPos();
			int startPos = sci_WordStartPosition(pos-1, TRUE);
			int endPos = sci_WordEndPosition(pos-1, TRUE);
			CStringA strSelected;
			sci_AutocGetCurrentText(strSelected);

			sci_SetTargetStart(startPos);
			sci_SetTargetEnd(endPos);
			sci_ReplaceTarget(-1, strSelected);
			sci_GoToPos(startPos + strSelected.GetLength());

			sci_AutocCancel(); //不使用默认的插人
			return TRUE;
		}
		break;
	case SCN_AUTOCCANCELLED:
		break;
	case SCN_AUTOCCHARDELETED:
		break;
	case SCI_SETMODEVENTMASK:
		break;
	case SCI_GETMODEVENTMASK:
		break;
	case SCI_SETMOUSEDWELLTIME:
		break;
	case SCI_GETMOUSEDWELLTIME:
		break;
	case SCI_SETIDENTIFIER:
		break;
	case SCI_GETIDENTIFIER:
		break;
	case SCEN_CHANGE:
		break;
	case SCEN_SETFOCUS:
		break;
	case SCEN_KILLFOCUS:
		break;
	}

	return FALSE;
}