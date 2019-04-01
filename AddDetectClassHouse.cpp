// AddDetectClassHouse.cpp : 实现文件
//

#include "stdafx.h"
#include "MCU_system.h"
#include "AddDetectClassHouse.h"
#include "afxdialogex.h"
#include<sstream>

// AddDetectClassHouse 对话框

IMPLEMENT_DYNAMIC(AddDetectClassHouse, CDialogEx)

AddDetectClassHouse::AddDetectClassHouse(CWnd* pParent /*=NULL*/)
	: CDialogEx(AddDetectClassHouse::IDD, pParent)
{

}

AddDetectClassHouse::~AddDetectClassHouse()
{
}

void AddDetectClassHouse::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_havedClassList);
}


BEGIN_MESSAGE_MAP(AddDetectClassHouse, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_ADDDETECTCLASS, &AddDetectClassHouse::OnBnClickedButtonAdddetectclass)
//	ON_EN_KILLFOCUS(IDC_EDIT_ADDCLASSNAME, &AddDetectClassHouse::OnKillfocusEditAddclassname)
END_MESSAGE_MAP()


// AddDetectClassHouse 消息处理程序


BOOL AddDetectClassHouse::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	LONG lStyle;
	lStyle = GetWindowLong(m_havedClassList.m_hWnd, GWL_STYLE);//获取当前窗口style
	lStyle &= ~LVS_TYPEMASK; //清除显示方式位
	lStyle |= LVS_REPORT; //设置style
	SetWindowLong(m_havedClassList.m_hWnd, GWL_STYLE, lStyle);//设置style

	DWORD dwStyle = m_havedClassList.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	m_havedClassList.SetExtendedStyle(dwStyle); //设置扩展风格
	m_havedClassList.InsertColumn(0, "ID", LVCFMT_CENTER, 50);//插入列
	m_havedClassList.InsertColumn(1, "类别", LVCFMT_CENTER, 185);//插入列
	int code = loadXML();//加载类别库
	if (code == 0)
	{
		MessageBox("加载检测器件类别库失败!!!", "MCU系统");
		return 0;
	}
	else if (code == 1)
	{
		MessageBox("检测器件类别库为空!!!", "MCU系统");
		return 0;
	}
	setList();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}
void AddDetectClassHouse::setList()
{
	m_havedClassList.DeleteAllItems();

	for (int i = 0; i < className.size(); i++)
	{
		stringstream ss;
		ss << (i + 1);
		string s = ss.str();
		m_havedClassList.InsertItem(i, s.c_str());
		m_havedClassList.SetItemText(i, 1, className[i]);
	}
}

int AddDetectClassHouse::loadXML()
{
	className.clear();//清空类别容器
	numOfClassName.clear();

	TiXmlDocument doc;
	if (!doc.LoadFile("componentWarehouse.xml"))
	{
		return 0;
	}

	TiXmlElement* root = doc.FirstChildElement();
	if (root == NULL)
	{
		doc.Clear();
		return 1;
	}
	TiXmlElement* elem = root->FirstChildElement();
	if (elem == NULL)
	{
		doc.Clear();
		return 1;
	}

	for (TiXmlElement* e1 = elem->FirstChildElement(); e1 != NULL; e1 = e1->NextSiblingElement())
	{
		TiXmlNode* e2 = e1->FirstChild();
		const char *numOfRowImage_char = e2->ToText()->Value();
		CString Cstring;
		Cstring.Format("%s", numOfRowImage_char);
		className.push_back(Cstring);//加入到类别容器中
		numOfClassName.push_back(0);
	}

	if (className.size() == 0)
	{
		doc.Clear();
		return 1;
	}

	doc.Clear();
	return 2;

}
int AddDetectClassHouse::writeXML(CString name)
{
	TiXmlDocument doc;
	if (!doc.LoadFile("componentWarehouse.xml"))
	{
		return 0;
	}

	TiXmlElement* root = doc.FirstChildElement();
	if (root == NULL)
	{
		doc.Clear();
		return 1;
	}
	TiXmlElement* elem = root->FirstChildElement();
	if (elem == NULL)
	{
		doc.Clear();
		return 1;
	}

	TiXmlElement *nameElement = new TiXmlElement("className");
	elem->LinkEndChild(nameElement);

	TiXmlText *nameContent = new TiXmlText(name);
	nameElement->LinkEndChild(nameContent);

	doc.SaveFile("componentWarehouse.xml");

	doc.Clear();
	return 2;
}

/*
	#添加检测类别按钮#
*/
void AddDetectClassHouse::OnBnClickedButtonAdddetectclass()
{
	// TODO:  在此添加控件通知处理程序代码
	CString name;
	GetDlgItem(IDC_EDIT_ADDCLASSNAME)->GetWindowText(name);
	if (name == "")
	{
		return;
	}
	int code = writeXML(name);
	loadXML();
	if (code == 0)
	{
		MessageBox("加载检测器件类别库失败!!!", "MCU系统");
		return;
	}
	else if (code == 1)
	{
		MessageBox("检测器件类别库为空!!!", "MCU系统");
		return;
	}
	setList();
}


//void AddDetectClassHouse::OnKillfocusEditAddclassname()
//{
//	// TODO:  在此添加控件通知处理程序代码
//}
