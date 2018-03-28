#pragma once

//文件定义的DuiLib属性列表
#define	TREENODETYPE_ACTIVE				0
#define	TREENODETYPE_BUTTON				1
#define	TREENODETYPE_COMBO				2
#define	TREENODETYPE_CONTAINER			3
#define	TREENODETYPE_CONTROL			4
#define	TREENODETYPE_DEFAULT			5
#define	TREENODETYPE_EDIT				6
#define	TREENODETYPE_FONT				7
#define	TREENODETYPE_HORIZONTALLAYOUT	8
#define	TREENODETYPE_LABEL				9
#define	TREENODETYPE_OPTION				10
#define	TREENODETYPE_POINTER			11
#define	TREENODETYPE_PROGRESS			12
#define	TREENODETYPE_SCROLLBAR			13
#define	TREENODETYPE_SLIDER				14
#define	TREENODETYPE_TABLAYOUT			15
#define	TREENODETYPE_TEXT				16
#define	TREENODETYPE_TILELAYOUT			17
#define	TREENODETYPE_UNKNOW				18
#define	TREENODETYPE_VERTICALLAYOUT		19
#define	TREENODETYPE_WINDOW				20

class CDuiDesignerProperty
{
public:
	CDuiDesignerProperty(void);
	virtual ~CDuiDesignerProperty(void);

	BOOL LoadPropertyFile();
	xml_node GetRoot();

	BOOL IsBaseFromControlUI(LPCTSTR className);
	BOOL IsBaseFromContainer(LPCTSTR className);
	BOOL IsWindowForm(LPCTSTR className);

	BOOL GetDefaultFont(xml_node nodeDoc);

	//获取控件默认参数, 传入参数nodeControl属于属性文档的节点
	xml_node GetDefaultAttribute(xml_node nodeControl, LPCTSTR attrName);

	xml_node FindControl(LPCTSTR className);
	static UINT GetControlType(LPCTSTR clasName);
public:
	xml_document xml;
};
