#pragma once

// CFindReplaceDlg 对话框

class CFindReplaceDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFindReplaceDlg)

protected:
	CDuilibSkinEditorView* GetCurrentView();
	void SetSearchAnchor(CSciWnd& sci, int nPos);

public:
	CFindReplaceDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFindReplaceDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_FINDREPLACE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonFindnext();
	bool DoFind();
	void Show();

	static CStringA W2Utf8(LPCWSTR lpwsz);
	static CString Utf8ToW(LPCSTR lpsz);
	CString m_strFind;
	CString m_strReplace;
	BOOL m_bUp;
	BOOL m_bMatchWhole;
	BOOL m_bMatchCase;
	BOOL m_bLoop;
	int m_nMode;
	afx_msg void OnBnClickedButtonReplace();
	afx_msg void OnBnClickedRadioModeRegex();
	virtual void OnOK();
};
