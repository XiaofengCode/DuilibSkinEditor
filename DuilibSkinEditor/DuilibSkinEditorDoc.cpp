
// DuilibSkinEditorDoc.cpp : CDuilibSkinEditorDoc ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "DuilibSkinEditor.h"
#endif

#include "DuilibSkinEditorDoc.h"
#include "DuilibSkinEditorView.h"
#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CDuilibSkinEditorDoc

IMPLEMENT_DYNCREATE(CDuilibSkinEditorDoc, CDocument)

BEGIN_MESSAGE_MAP(CDuilibSkinEditorDoc, CDocument)
END_MESSAGE_MAP()


// CDuilibSkinEditorDoc ����/����

CDuilibSkinEditorDoc::CDuilibSkinEditorDoc()
{
	// TODO: �ڴ����һ���Թ������

}

void CDuilibSkinEditorDoc::SetModifiedFlag(BOOL bModified /*= TRUE*/)
{
	if (bModified)
	{
		if (!IsModified())
		{
			SetTitle(GetTitle() + '*');
		}
	}
	else
	{
		if (IsModified())
		{
			CString strTitle = GetTitle();
			strTitle.Delete(strTitle.GetLength()-1);
			SetTitle(strTitle);
		}
	}
	CDocument::SetModifiedFlag(bModified);
}

void CDuilibSkinEditorDoc::OnSettingsChanged()
{
	POSITION posView = GetFirstViewPosition();
	if (posView)
	{
		CDuilibSkinEditorView* pView = (CDuilibSkinEditorView*)GetNextView(posView);
		if (!pView)
		{
			return;
		}
		pView->OnSettingsChanged();
	}
}

CDuilibSkinEditorDoc::~CDuilibSkinEditorDoc()
{
}

BOOL CDuilibSkinEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	
	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CDuilibSkinEditorDoc ���л�

void CDuilibSkinEditorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
		POSITION posView = GetFirstViewPosition();
		if (posView)
		{
			CDuilibSkinEditorView* pView = (CDuilibSkinEditorView*)GetNextView(posView);
			if (!pView)
			{
				return;
			}
			pView->OnFileSave(ar);
		}
// 		CDuilibSkinEditorView* pView = (CDuilibSkinEditorView*)GetRoutingView();
// 		if (pView)
// 		{
// 			pView->OnFileSave(ar);
// 		}
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}

#ifdef SHARED_HANDLERS

// ����ͼ��֧��
void CDuilibSkinEditorDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// �޸Ĵ˴����Ի����ĵ�����
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// ������������֧��
void CDuilibSkinEditorDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ���ĵ����������������ݡ�
	// ���ݲ���Ӧ�ɡ�;���ָ�

	// ����:  strSearchContent = _T("point;rectangle;circle;ole object;")��
	SetSearchContent(strSearchContent);
}

void CDuilibSkinEditorDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CDuilibSkinEditorDoc ���

#ifdef _DEBUG
void CDuilibSkinEditorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDuilibSkinEditorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CDuilibSkinEditorDoc ����
