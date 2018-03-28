#pragma once


// CSettingsDlg 对话框

class CSettingsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSettingsDlg)

public:
	CSettingsDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSettingsDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_SETTINGS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void EnumFontCallback(const LOGFONT *lpelfe);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnApply();
	CMFCColorButton m_clrBtnBG;
	CComboBox m_cmbFont;
	CComboBox m_cmbFontSize;
	CComboBox m_cmbLineSpacing;
	// 自动换行
	BOOL m_bAutoWrap;
};
