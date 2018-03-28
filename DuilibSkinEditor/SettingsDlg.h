#pragma once


// CSettingsDlg �Ի���

class CSettingsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSettingsDlg)

public:
	CSettingsDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSettingsDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_SETTINGS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
	// �Զ�����
	BOOL m_bAutoWrap;
};
