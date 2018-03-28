#include "StdAfx.h"
#include "DuiDesignerProperty.h"


CDuiDesignerProperty::CDuiDesignerProperty(void)
{
}


CDuiDesignerProperty::~CDuiDesignerProperty(void)
{
}

BOOL CDuiDesignerProperty::LoadPropertyFile()
{
	if(!xml.load_file(theApp.GetStartupDir() + _T("DuiLib.xml")))
	{
		AfxMessageBox(_T("载入DuiLib属性文件失败!"));
		return FALSE;
	}
	return TRUE;
}

xml_node CDuiDesignerProperty::GetRoot()
{
	return xml.root().child(_T("Controls"));
}

BOOL CDuiDesignerProperty::IsBaseFromControlUI(LPCTSTR className)
{
	if(_tcsicmp(className, _T("Control")) == 0)
		return TRUE;

	xml_node nodeFind = FindControl(className);
	xml_attribute attr = nodeFind.attribute(_T("parent"));
	if(attr)
	{
		xml_node nodeFind = FindControl(attr.value());
		if(nodeFind)
		{
			return IsBaseFromControlUI(nodeFind.name());
		}
	}
	return FALSE;
}


BOOL CDuiDesignerProperty::IsBaseFromContainer(LPCTSTR className)
{
	if(_tcsicmp(className, _T("Container")) == 0)
		return TRUE;

	xml_node nodeFind = FindControl(className);
	xml_attribute attr = nodeFind.attribute(_T("parent"));
	if(attr)
	{
		return IsBaseFromContainer(attr.value());
	}
	return FALSE;
}


BOOL CDuiDesignerProperty::IsWindowForm(LPCTSTR className)
{
	if(_tcsicmp(className, _T("Window")) == 0)
		return TRUE;
	return FALSE;
}


BOOL CDuiDesignerProperty::GetDefaultFont(xml_node nodeDoc)
{
	xml_node nodeFont = FindControl(nodeDoc.name());
	if(!nodeFont)	return FALSE;

	for(xml_node nodeAttr=nodeFont.first_child(); nodeAttr; nodeAttr=nodeAttr.next_sibling())
	{
		LPCTSTR attrName = nodeAttr.attribute(_T("name")).value();
		LPCTSTR attrValue = nodeAttr.attribute(_T("default")).value();

		xml_attribute attr = nodeDoc.attribute(attrName);
		if(!attr) attr = nodeDoc.append_attribute(attrName);
		attr.set_value(attrValue);
	}
	return TRUE;
}

xml_node CDuiDesignerProperty::GetDefaultAttribute(xml_node nodeControl, LPCTSTR attrName)
{
	return nodeControl.find_child_by_attribute(_T("Attribute"), _T("name"), attrName);
}

xml_node CDuiDesignerProperty::FindControl(LPCTSTR className)
{
	xml_node retnode;

	pugi::xml_node root = GetRoot();
	for (pugi::xml_node node = root.first_child(); node; node = node.next_sibling())
	{
		if(0 == _tcsicmp(node.name(), className))
		{
			retnode = node;
			break;
		}
	}
	return retnode;
}

UINT CDuiDesignerProperty::GetControlType(LPCTSTR clasName)
{
	LPCTSTR nodeName = clasName;

	if(0 == _tcsicmp(nodeName, _T("Window")))
	{
		return TREENODETYPE_WINDOW;
	}
	else if(0 == _tcsicmp(nodeName, _T("ActiveX")))
	{
		return TREENODETYPE_ACTIVE;
	}
	else if(0 == _tcsicmp(nodeName, _T("Button")))
	{
		return TREENODETYPE_BUTTON;
	}
	else if(0 == _tcsicmp(nodeName, _T("Combo")))
	{
		return TREENODETYPE_COMBO;
	}
	else if(0 == _tcsicmp(nodeName, _T("Container")))
	{
		return TREENODETYPE_CONTAINER;
	}
	else if(0 == _tcsicmp(nodeName, _T("Control")))
	{
		return TREENODETYPE_CONTROL;
	}
	else if(0 == _tcsicmp(nodeName, _T("Default")))
	{
		return TREENODETYPE_DEFAULT;
	}
	else if(0 == _tcsicmp(nodeName, _T("Edit")))
	{
		return TREENODETYPE_EDIT;
	}
	else if(0 == _tcsicmp(nodeName, _T("Font")))
	{
		return TREENODETYPE_FONT;
	}
	else if(0 == _tcsicmp(nodeName, _T("HorizontalLayout")))
	{
		return TREENODETYPE_HORIZONTALLAYOUT;
	}
	else if(0 == _tcsicmp(nodeName, _T("Label")))
	{
		return TREENODETYPE_LABEL;
	}
	else if(0 == _tcsicmp(nodeName, _T("Option")))
	{
		return TREENODETYPE_OPTION;
	}
	else if(0 == _tcsicmp(nodeName, _T("Pointer")))
	{
		return TREENODETYPE_POINTER;
	}
	else if(0 == _tcsicmp(nodeName, _T("Progress")))
	{
		return TREENODETYPE_PROGRESS;
	}
	else if(0 == _tcsicmp(nodeName, _T("ScrollBar")))
	{
		return TREENODETYPE_SCROLLBAR;
	}
	else if(0 == _tcsicmp(nodeName, _T("Slider")))
	{
		return TREENODETYPE_SLIDER;
	}
	else if(0 == _tcsicmp(nodeName, _T("TabLayout")))
	{
		return TREENODETYPE_TABLAYOUT;
	}
	else if(0 == _tcsicmp(nodeName, _T("Text")))
	{
		return TREENODETYPE_TEXT;
	}
	else if(0 == _tcsicmp(nodeName, _T("TileLayout")))
	{
		return TREENODETYPE_TILELAYOUT;
	}
	else if(0 == _tcsicmp(nodeName, _T("VerticalLayout")))
	{
		return TREENODETYPE_VERTICALLAYOUT;
	}
	else if(0 == _tcsicmp(nodeName, _T("Window")))
	{
		return TREENODETYPE_WINDOW;
	}
	else
	{
		return TREENODETYPE_UNKNOW;
	}
}