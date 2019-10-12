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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

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

LRESULT CPreviewWnd::OnSelectControl(WPARAM wParam, LPARAM lParam)
{
	CString strFile((LPCTSTR)wParam);
	if (strFile.GetLength() == 0)
	{
		return 0;
	}
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
	CString strCurDoc = pV->GetDocument()->GetPathName();
	if (strCurDoc.Right(strFile.GetLength()).CompareNoCase(strFile) != 0)
	{
		pV = NULL;
		POSITION PosDocTemplate = theApp.GetFirstDocTemplatePosition();
		if (PosDocTemplate)
		{
			CDocTemplate* pDocTemplate = theApp.GetNextDocTemplate( PosDocTemplate );
			POSITION PosDoc = pDocTemplate->GetFirstDocPosition();
			while(PosDoc)
			{
				CDocument* pDoc = pDocTemplate->GetNextDoc( PosDoc );
				if (pDoc->GetPathName().Right(strFile.GetLength()).CompareNoCase(strFile) == 0)
				{
					POSITION PosView = pDoc->GetFirstViewPosition();
					CView *pView = (CView *)pDoc->GetNextView(PosView);
					CMDIChildWnd *pFram = (CMDIChildWnd *)pView->GetParentFrame();
					pFram->MDIActivate();
					pV = pView;
				}
			}
		}
		if (!pV)
		{
			int nPos = strCurDoc.ReverseFind('\\');
			if (nPos > 0)
			{
				strFile = strCurDoc.Left(nPos + 1) + strFile;
			}

			CDocument* pDoc = theApp.OpenDocumentFile(strFile);
			if (!pDoc)
			{
				return 0;
			}
			POSITION PosView = pDoc->GetFirstViewPosition();
			CView *pView = (CView *)pDoc->GetNextView(PosView);
			CMDIChildWnd *pFram = (CMDIChildWnd *)pView->GetParentFrame();
			pFram->MDIActivate();
			pV = pView;
		}
	}
	return pV->SendMessage(0x2222, wParam, lParam);
}
