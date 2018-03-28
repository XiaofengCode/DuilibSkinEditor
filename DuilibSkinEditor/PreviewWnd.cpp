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
}

BOOL CPreviewWnd::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ�������Ϣ������������/�����Ĭ��ֵ

	//����brushΪϣ���ı�����ɫ
	CBrush backBrush(0xFFFFFF);
	//����ɵ�brush
	CBrush* pOldBrush = pDC->SelectObject(&backBrush);
	CRect rect;
	pDC->GetClipBox(&rect);
	//����Ҫ������
	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
	pDC->SelectObject(pOldBrush);
	return TRUE;
	return CDockablePane::OnEraseBkgnd(pDC);
}