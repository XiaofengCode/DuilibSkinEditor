
#include "stdafx.h"

#include "PropertiesWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "DuilibSkinEditor.h"
#include <GdiPlus.h>
using namespace Gdiplus;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

CPropertiesWnd::CPropertiesWnd()
{
	m_pThreadShow = nullptr;
	m_duiProp.LoadPropertyFile();
}

CPropertiesWnd::~CPropertiesWnd()
{
	m_bQuit = true;
	m_evtChangedCtrl.SetEvent();
	WaitForSingleObject(m_pThreadShow, INFINITE);
}

BEGIN_MESSAGE_MAP(CPropertiesWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_EXPAND_ALL, OnExpandAllProperties)
	ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL, OnUpdateExpandAllProperties)
	ON_COMMAND(ID_SORTPROPERTIES, OnSortProperties)
	ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortProperties)
	ON_COMMAND(ID_PROPERTIES1, OnProperties1)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES1, OnUpdateProperties1)
	ON_COMMAND(ID_PROPERTIES2, OnProperties2)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES2, OnUpdateProperties2)
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar 消息处理程序

void CPropertiesWnd::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient,rectCombo;
	GetClientRect(rectClient);

	m_wndObjectCombo.GetWindowRect(&rectCombo);

	int cyCmb = rectCombo.Size().cy;
	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndObjectCombo.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), 200, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb + cyTlb, rectClient.Width(), rectClient.Height() -(cyCmb+cyTlb), SWP_NOACTIVATE | SWP_NOZORDER);
}

UINT AFX_CDECL ThreadShowProperty(LPVOID lpContext)
{
	CPropertiesWnd* pThis = (CPropertiesWnd*)lpContext;
	pThis->OnThreadShowProperty();
	return 0;
}

void CPropertiesWnd::ShowProperty(LPCTSTR lpszCtrlName, LPCTSTR lpszCurAttrs)
{
	SetPropListFont();
	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();
	m_strCurCtrl = lpszCtrlName;
	m_strCurAttrs = lpszCurAttrs;
	if (m_pThreadShow)
	{
		m_evtChangedCtrl.SetEvent();
	}
	else
	{
		m_bQuit = false;
		m_evtChangedCtrl.ResetEvent();
		m_pThreadShow = AfxBeginThread(ThreadShowProperty, (LPVOID)this);
	}
}

void CPropertiesWnd::OnThreadShowProperty()
{
	while (1)
	{
_restart:
		//解析属性
		CMapStringToString mapAttrs;
		if (m_bQuit)
		{
			return;
		}
		ParseCurAttrs(mapAttrs);
		m_wndPropList.RemoveAll();
		while (m_strCurCtrl.GetLength())
		{
			xml_node node = m_duiProp.FindControl(m_strCurCtrl);
			for (xml_node nodeAttr = node.first_child(); nodeAttr; nodeAttr=nodeAttr.next_sibling())
			{
				if (WaitForSingleObject(m_evtChangedCtrl, 0) == WAIT_OBJECT_0)
				{
					goto _restart;
				}
				bool bExist = false;
				LPCTSTR pstrName = nodeAttr.attribute(_T("name")).value();
				LPCTSTR pstrType = nodeAttr.attribute(_T("type")).value();
				LPCTSTR pstrComment = nodeAttr.attribute(_T("comment")).value();
				CString strValue;
				LPCTSTR pstrDefault = NULL;
				bool bModify = false;
				if (mapAttrs.Lookup(pstrName, strValue))
				{
					bModify = true;
					pstrDefault = strValue;
				}
				else
				{
					pstrDefault = nodeAttr.attribute(_T("default")).value();
				}
				CMFCPropertyGridProperty* pProp = nullptr;
				if (_tcsicmp(pstrType, _T("INT")) == 0)
				{
					pProp = new CMFCPropertyGridProperty(pstrName, (_variant_t)_ttoi(pstrDefault), pstrComment);
					pProp->EnableSpinControl(TRUE);
				}
				else if(_tcsicmp(pstrType, _T("STRING")) == 0)
				{
					pProp = new CMFCPropertyGridProperty(pstrName, (_variant_t)pstrDefault, pstrComment);
				}
				else if(_tcsicmp(pstrType, _T("BOOL")) == 0)
				{
					pProp = new CMFCPropertyGridProperty(pstrName, (_variant_t)(_tcsicmp(pstrDefault, _T("true")) == 0), pstrComment);
				}
				else if(_tcsicmp(pstrType, _T("RECT")) == 0)
				{
					pProp = new CMFCPropertyGridProperty(pstrName, 0, TRUE);
					pProp->SetDescription(pstrComment);

					LPTSTR pstr = NULL;
					long nValue = _tcstol(pstrDefault, &pstr, 10);
					CMFCPropertyGridProperty* pPropSub = new CMFCPropertyGridProperty(_T("left"), COleVariant(nValue), _T("left"));
					pProp->EnableSpinControl(TRUE);
					pProp->AddSubItem(pPropSub);


					nValue = _tcstol(pstr + 1, &pstr, 10);
					pPropSub = new CMFCPropertyGridProperty(_T("top"), COleVariant(nValue), _T("top"));
					pProp->EnableSpinControl(TRUE);
					pProp->AddSubItem(pPropSub);

					nValue = _tcstol(pstr + 1, &pstr, 10);
					pPropSub = new CMFCPropertyGridProperty(_T("right"), COleVariant(nValue), _T("right"));
					pProp->EnableSpinControl(TRUE);
					pProp->AddSubItem(pPropSub);

					nValue = _tcstol(pstr + 1, &pstr, 10);
					pPropSub = new CMFCPropertyGridProperty(_T("bottom"), COleVariant(nValue), _T("bottom"));
					pProp->EnableSpinControl(TRUE);
					pProp->AddSubItem(pPropSub);
				}
				else if(_tcsicmp(pstrType, _T("DWORD")) == 0)
				{
					PTSTR p;
					if (pstrDefault[0] == '#')
					{
						pstrDefault++;
					}
					DWORD dw = _tcstoul(pstrDefault, &p, 16);
					CMFCPropertyGridColor32Property* pColorProp = new CMFCPropertyGridColor32Property(pstrName, dw, NULL, pstrComment);
					pColorProp->EnableOtherButton(_T("其他..."));
					pColorProp->EnableAutomaticButton(_T("默认"), 0);
					pColorProp->AllowEdit();
					pProp = pColorProp;
				}
				else if(_tcsicmp(pstrType, _T("SIZE")) == 0)
				{
					CMFCPropertyGridProperty* pSize = new CMFCPropertyGridProperty(pstrName, 0, TRUE);

					LPTSTR pstr = NULL;
					long nValue = _tcstol(pstrDefault, &pstr, 10);
					pProp = new CMFCPropertyGridProperty(_T("高度"), COleVariant(), _T("指定高度"));
					pProp->SetValue(COleVariant(nValue));
					pProp->EnableSpinControl(TRUE);
					pSize->AddSubItem(pProp);

					nValue = _tcstol(pstr + 1, &pstr, 10);
					pProp = new CMFCPropertyGridProperty( _T("宽度"), COleVariant(), _T("指定宽度"));
					pProp->EnableSpinControl(TRUE);
					pProp->SetValue(COleVariant(nValue));
					pSize->AddSubItem(pProp);
					pProp = pSize;
				}
				if (pProp)
				{
					m_wndPropList.AddProperty(pProp);
					if (bModify)
					{
						if (pProp->IsModified())
							AfxMessageBox(_T("Text"));
					}
				}
			}
			m_strCurCtrl = node.attribute(_T("parent")).value();
		}
		WaitForSingleObject(m_evtChangedCtrl, INFINITE);
	}
}

int CPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 创建组合:
// 	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
// 
// 	if (!m_wndObjectCombo.Create(dwViewStyle, rectDummy, this, 1))
// 	{
// 		TRACE0("未能创建属性组合 \n");
// 		return -1;      // 未能创建
// 	}
// 
// 	m_wndObjectCombo.AddString(_T("应用程序"));
// 	m_wndObjectCombo.AddString(_T("属性窗口"));
// 	m_wndObjectCombo.SetCurSel(0);

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("未能创建属性网格\n");
		return -1;      // 未能创建
	}

	m_wndPropList.SetAlphabeticMode(TRUE);

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROPERTIES);
	m_wndToolBar.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE /* 已锁定*/);
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_PROPERTIES_HC : IDR_PROPERTIES, 0, 0, TRUE /* 锁定*/);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);

	// 所有命令将通过此控件路由，而不是通过主框架路由:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	AdjustLayout();
	return 0;
}

void CPropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CPropertiesWnd::OnExpandAllProperties()
{
	m_wndPropList.ExpandAll();
}

void CPropertiesWnd::OnUpdateExpandAllProperties(CCmdUI* /* pCmdUI */)
{
}

void CPropertiesWnd::OnSortProperties()
{
	m_wndPropList.SetAlphabeticMode(!m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnUpdateSortProperties(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnProperties1()
{
	// TODO: 在此处添加命令处理程序代码
}

void CPropertiesWnd::OnUpdateProperties1(CCmdUI* /*pCmdUI*/)
{
	// TODO: 在此处添加命令更新 UI 处理程序代码
}

void CPropertiesWnd::OnProperties2()
{
	// TODO: 在此处添加命令处理程序代码
}

void CPropertiesWnd::OnUpdateProperties2(CCmdUI* /*pCmdUI*/)
{
	// TODO: 在此处添加命令更新 UI 处理程序代码
}

void CPropertiesWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndPropList.SetFocus();
}

void CPropertiesWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	SetPropListFont();
}

void CPropertiesWnd::SetPropListFont()
{
	::DeleteObject(m_fntPropList.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

	m_wndPropList.SetFont(&m_fntPropList);
	m_wndObjectCombo.SetFont(&m_fntPropList);
}


bool IsSpace(TCHAR c)
{
	if (c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == '\t')
	{
		return true;
	}
	return false;
}

LPCTSTR SkipSpace(LPCTSTR lpsz)
{
	while (*lpsz && IsSpace(*lpsz))
	{
		lpsz = CharNext(lpsz);
	}
	return lpsz;
}

bool IsValueTokenChar(TCHAR c)
{
	if (c >= '0' && c <= '9')
	{
		return true;
	}
	if (c >= 'A' && c <= 'Z')
	{
		return true;
	}
	if (c >= 'a' && c <= 'z')
	{
		return true;
	}
	if (c == '_' || c == '-')
	{
		return true;
	}
	return false;
}

void CPropertiesWnd::ParseCurAttrs(CMapStringToString& mapAttrs)
{
	mapAttrs.RemoveAll();
	CString str(m_strCurAttrs);//复制一份，防止m_strCurAttrs被修改
	LPCTSTR p = (LPCTSTR)str;
	while (*p)
	{
		p = SkipSpace(p);
		bool bKey = true;
		CString strKey;
		CString strValue;

		while (IsValueTokenChar(*p))
		{
			strKey += *p;
			p++;
		}
		p = SkipSpace(p);
		if (*p != '=')
		{
			return;//不合法
		}
		p++;
		p = SkipSpace(p);
		if (*p != '\"')
		{
			return;//不合法
		}
		p++;
		while (*p != '\"')
		{
			strValue += *p;
			p++;
		}
		p++;
		strKey.MakeLower();
		mapAttrs[strKey] = strValue;
	}
}

CMFCPropertyGridColor32Property::CMFCPropertyGridColor32Property(const CString& strName, const COLORREF& color,CPalette* pPalette/*=NULL*/,LPCTSTR lpszDescr/*=NULL*/,DWORD_PTR dwData/*=0*/)
	:CMFCPropertyGridColorProperty(strName,color,pPalette,lpszDescr,dwData)
{
}

BOOL CMFCPropertyGridColor32Property::OnUpdateValue()
{
	ASSERT_VALID(this);

	if (m_pWndInPlace == NULL)
	{
		return FALSE;
	}

	ASSERT_VALID(m_pWndInPlace);
	ASSERT(::IsWindow(m_pWndInPlace->GetSafeHwnd()));

	CString strText;
	m_pWndInPlace->GetWindowText(strText);

	COLORREF colorCurr = m_Color;
	int nA = 0,nR = 0, nG = 0, nB = 0;
	_stscanf_s(strText, _T("%2X%2X%2X%2X"), &nA, &nR, &nG, &nB);
	if(nA == 0 && (nR !=0 || nG != 0 || nB !=  0))
		nA = 0xFF;
	m_Color = ARGB(nA, nR, nG, nB);

	if (colorCurr != m_Color)
	{
		m_pWndList->OnPropertyChanged(this);
	}

	return TRUE;
}

void CMFCPropertyGridColor32Property::OnDrawValue(CDC* pDC, CRect rect)
{
	CRect rectColor(rect);
	Gdiplus::Graphics g(pDC->GetSafeHdc());
	rect.left += rect.Height();
	CMFCPropertyGridProperty::OnDrawValue(pDC, rect);

	Gdiplus::Rect rc(rectColor.left, rectColor.top, rectColor.Height(), rectColor.Height());
	Gdiplus::SolidBrush brush(Gdiplus::Color((LOBYTE((m_Color)>>24)), GetBValue(m_Color), GetGValue(m_Color), GetRValue(m_Color)));
	g.FillRectangle(&brush, rc);
}

CString CMFCPropertyGridColor32Property::FormatProperty()
{
	ASSERT_VALID(this);

	CString str;
	str.Format(_T("%02X%02X%02X%02X"),GetAValue(m_Color),GetRValue(m_Color),GetGValue(m_Color),GetBValue(m_Color));

	return str;
}