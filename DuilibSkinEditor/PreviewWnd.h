#pragma once

typedef HWND (*PFN_Preview)(LPCWSTR lpwszSkinFile, HWND hParent);
class CDuiPreview
{
public:
	CDuiPreview()
	{
		m_hDuiPreview = LoadLibrary(_T("DuiPreview.dll"));
		PreviewSkin = (PFN_Preview)GetProcAddress(m_hDuiPreview, "Preview");
		if (!PreviewSkin)
		{
			FreeLibrary(m_hDuiPreview);
			m_hDuiPreview = NULL;
		}
	}
	~CDuiPreview()
	{
		if (m_hDuiPreview)
		{
			FreeLibrary(m_hDuiPreview);
		}
	}
	HWND CreateWnd(LPCWSTR lpwszSkinFile, HWND hParent)
	{
		if (!PreviewSkin)
		{
			return NULL;
		}
		return PreviewSkin(lpwszSkinFile, hParent);
	}
protected:
	HMODULE m_hDuiPreview;
	PFN_Preview PreviewSkin;
};

extern CDuiPreview g_duiPreview;

class CPreviewWnd : public CDockablePane
{
public:
	CPreviewWnd(void);
	~CPreviewWnd(void);
	void Refresh();
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnSelectControl(WPARAM wParam, LPARAM lParam);
};

