#include "StdAfx.h"
#include "PreviewWnd.h"


CDuiPreview g_duiPreview;
CPreviewWnd::CPreviewWnd(void)
{
}


CPreviewWnd::~CPreviewWnd(void)
{
}
BEGIN_MESSAGE_MAP(CPreviewWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(0x2222, OnSelectControl)
END_MESSAGE_MAP()


int CPreviewWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CPreviewWnd::Refresh()
{
	CMDIFrameWnd  *pFrame = (CMDIFrameWnd*)theApp.GetMainWnd();
	if (!pFrame)
	{
		return;
	}
	CMDIChildWnd  *pChild = (CMDIChildWnd   *)pFrame->GetActiveFrame();   
	if (!pChild)
	{
		return;
	}
	CView   *pV = (CView*)pChild->GetActiveView();
	if (!pV)
	{
		return;
	}
	CString strPathName = pV->GetDocument()->GetPathName();
	HWND hWnd = g_duiPreview.CreateWnd(strPathName, m_hWnd);
	if (!hWnd)
	{
		CRect   rect;
		GetClientRect(rect);
		CDC* pDc = GetDC();
		pDc->DrawText(_T("Load skin failed!"), rect, 0);
	}
	else
	{
		OnEraseBkgnd(GetDC());
	}
	pV->PostMessage(0x2223);
}

BOOL CPreviewWnd::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//设置brush为希望的背景颜色
	CBrush backBrush(0xFFFFFF);
	//保存旧的brush
	CBrush* pOldBrush = pDC->SelectObject(&backBrush);
	CRect rect;
	pDC->GetClipBox(&rect);
	//画需要的区域
	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
	pDC->SelectObject(pOldBrush);
	return TRUE;
	return CDockablePane::OnEraseBkgnd(pDC);
}

LRESULT CPreviewWnd::OnSelectControl(WPARAM wParam, LPARAM lParam)
{
	CMDIFrameWnd  *pFrame = (CMDIFrameWnd*)theApp.GetMainWnd();
	if (!pFrame)
	{
		return 0;
	}
	CMDIChildWnd  *pChild = (CMDIChildWnd   *)pFrame->GetActiveFrame();   
	if (!pChild)
	{
		return 0;
	}
	CView   *pV = (CView*)pChild->GetActiveView();
	if (!pV)
	{
		return 0;
	}
	return pV->SendMessage(0x2222, wParam, lParam);
}
