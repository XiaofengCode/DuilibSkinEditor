// SettingsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DuilibSkinEditor.h"
#include "SettingsDlg.h"
#include "afxdialogex.h"
#include "DuilibSkinEditorDoc.h"


// CSettingsDlg 对话框

IMPLEMENT_DYNAMIC(CSettingsDlg, CDialogEx)

CSettingsDlg::CSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSettingsDlg::IDD, pParent)
	, m_bAutoWrap(FALSE)
{
	m_bAutoWrap = theApp.GetInt(_T("AutoWrap"), 0);
}

CSettingsDlg::~CSettingsDlg()
{
}

void CSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CLRBTN_BG, m_clrBtnBG);
	DDX_Control(pDX, IDC_COMBO_FONT, m_cmbFont);
	DDX_Control(pDX, IDC_COMBO_FONTSIZE, m_cmbFontSize);
	DDX_Control(pDX, IDC_COMBO_LINESPACING, m_cmbLineSpacing);
	DDX_Check(pDX, IDC_CHK_AUTOWRAP, m_bAutoWrap);
}


BEGIN_MESSAGE_MAP(CSettingsDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &CSettingsDlg::OnApply)
END_MESSAGE_MAP()


// CSettingsDlg 消息处理程序
int CALLBACK EnumFontFamExProc(
	const LOGFONT *lpelfe,
	const TEXTMETRIC *lpntme,
	DWORD FontType,
	LPARAM lParam
	)
{
	((CSettingsDlg*)lParam)->EnumFontCallback(lpelfe);
	return 1;
}

void CSettingsDlg::EnumFontCallback(const LOGFONT *lpelfe)
{
	m_cmbFont.AddString(lpelfe->lfFaceName);
}

BOOL CSettingsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CString strFont = theApp.GetString(_T("FontName"), _T("Courier New"));
	int nFontSize = theApp.GetInt(_T("FontSize"), 12);
	int nLineSpacing = theApp.GetInt(_T("LineSpacing"), 3);
	int nBgColor = theApp.GetInt(_T("BgColor"), 0xBBFFBB);
	LOGFONT font = {0};
	EnumFontFamiliesEx(GetDC()->m_hDC, &font, EnumFontFamExProc, (LPARAM)this, 0);
	m_cmbFont.SetWindowText(strFont);
	CString strText;
	strText.Format(_T("%d"), nFontSize);
	m_cmbFontSize.SetWindowText(strText);
	strText.Format(_T("%d"), nLineSpacing);
	m_cmbLineSpacing.SetWindowText(strText);
	m_clrBtnBG.SetColor(nBgColor);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CSettingsDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	OnApply();
	CDialogEx::OnOK();
}


void CSettingsDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialogEx::OnCancel();
}


void CSettingsDlg::OnApply()
{
	UpdateData();
	CString strFont;
	CString strFontSize;
	CString strLineSpacing;
	int nBgColor;
	m_cmbFont.GetWindowText(strFont);
	m_cmbFontSize.GetWindowText(strFontSize);
	m_cmbLineSpacing.GetWindowText(strLineSpacing);
	nBgColor = m_clrBtnBG.GetColor();

	theApp.WriteString(_T("FontName"), strFont);
	theApp.WriteInt(_T("FontSize"), _ttoi(strFontSize));
	theApp.WriteInt(_T("LineSpacing"), _ttoi(strLineSpacing));
	theApp.WriteInt(_T("BgColor"), nBgColor);
	theApp.WriteInt(_T("AutoWrap"), !!m_bAutoWrap);

	POSITION pos = theApp.GetFirstDocTemplatePosition();
	while (pos)
	{
		CDocTemplate* pDocTempl = theApp.GetNextDocTemplate(pos);
		POSITION pos2 = pDocTempl->GetFirstDocPosition();
		while (pos2)
		{
			CDuilibSkinEditorDoc* pDoc = (CDuilibSkinEditorDoc*)pDocTempl->GetNextDoc(pos2);
			if (!pDoc)
			{
				continue;
			}
			pDoc->OnSettingsChanged();
		}
	}
}
