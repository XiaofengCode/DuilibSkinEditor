#pragma once

// CFindReplaceDlg �Ի���

class CFindReplaceDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFindReplaceDlg)

protected:
	CDuilibSkinEditorView* GetCurrentView();

public:
	CFindReplaceDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFindReplaceDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_FINDREPLACE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonFindnext();
	CString m_strFind;
	CString m_strReplace;
	BOOL m_bUp;
	BOOL m_bMatchWhole;
	BOOL m_bMatchCase;
};
