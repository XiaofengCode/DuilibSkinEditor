
// DuilibSkinEditor.h : DuilibSkinEditor Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CDuilibSkinEditorApp:
// �йش����ʵ�֣������ DuilibSkinEditor.cpp
//

class CDuilibSkinEditorApp : public CWinAppEx
{
public:
	CDuilibSkinEditorApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	CString GetStartupDir();
	void RefreshPreview();

// ʵ��
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CDuilibSkinEditorApp theApp;
