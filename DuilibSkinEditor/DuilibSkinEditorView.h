
// DuilibSkinEditorView.h : CDuilibSkinEditorView 类的接口
//

#pragma once

#include "SciWnd.h"
#include "DuiDesignerProperty.h"
#include "xmlMatchedTagsHighlighter.h"

enum AttrType
{
	AtBool,
	AtColor,
};

#define WM_ATTIBUTE_VALUE_AUTOSEL		(WM_USER + 0x1000)
class CDuilibSkinEditorView : public CView
{
protected: // 仅从序列化创建
	CDuilibSkinEditorView();
	DECLARE_DYNCREATE(CDuilibSkinEditorView)

	CString GetCurNodeName();
	void AutoCompleteControl(CString objectName);
	void AutoCompleteAttibute(CString AttrName);
	void AutoCompleteAttibuteValue();
	void AutoCompleteAttibuteValue(CString strCtrl, CString strAttr);
	void findMatchingBracePos(int & braceAtCaret, int & braceOpposite);
	bool braceMatch();
// 特性
public:
	CDuilibSkinEditorDoc* GetDocument() const;
	CSciWnd& GetSci();

// 操作
public:
	void OnFileSave(CArchive& ar);
	void OnPostFileSave();

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CDuilibSkinEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CSciWnd m_sci;
	CDuiDesignerProperty m_duiProp;
	CString m_strLastCtrlName;
	bool m_bAutoCtrl;
	struct _stat64 m_statFile;
	XmlMatchedTagsHighlighter::XmlMatchedTagsPos m_posTagLastSel;
	//HANDLE m_hFileChangeEvent;

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnEditCut();
	afx_msg void OnUpdateEditCut(CCmdUI *pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI *pCmdUI);
	afx_msg void OnEditFindRepalce();
	afx_msg void OnUpdateEditFindRepalce(CCmdUI *pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI *pCmdUI);
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI *pCmdUI);
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditRedo(CCmdUI *pCmdUI);
	afx_msg LRESULT OnSelectControl(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetEditFocus(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	void AutoCompleteTag();

	void GetCurrentTagPos(int &nTagBegin, int &nTagEnd);

	void OnAutoSelection();

	void AutoIndent();

	void OnSettingsChanged();

protected:
	afx_msg LRESULT OnAttibuteValueAutoSel(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnSetFocus(CWnd* pOldWnd);
};

BOOL CreateString_InitWithUTF8Text(CString& str, char* pUTF8Text);
char* CreateUTF8TextInitWithString(CString strValue);

#ifndef _DEBUG  // DuilibSkinEditorView.cpp 中的调试版本
inline CDuilibSkinEditorDoc* CDuilibSkinEditorView::GetDocument() const
   { return reinterpret_cast<CDuilibSkinEditorDoc*>(m_pDocument); }
#endif

