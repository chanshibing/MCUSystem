// AddDetectClassHouse.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MCU_system.h"
#include "AddDetectClassHouse.h"
#include "afxdialogex.h"
#include<sstream>

// AddDetectClassHouse �Ի���

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


// AddDetectClassHouse ��Ϣ�������


BOOL AddDetectClassHouse::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	LONG lStyle;
	lStyle = GetWindowLong(m_havedClassList.m_hWnd, GWL_STYLE);//��ȡ��ǰ����style
	lStyle &= ~LVS_TYPEMASK; //�����ʾ��ʽλ
	lStyle |= LVS_REPORT; //����style
	SetWindowLong(m_havedClassList.m_hWnd, GWL_STYLE, lStyle);//����style

	DWORD dwStyle = m_havedClassList.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
	dwStyle |= LVS_EX_GRIDLINES;//�����ߣ�ֻ������report����listctrl��
	m_havedClassList.SetExtendedStyle(dwStyle); //������չ���
	m_havedClassList.InsertColumn(0, "ID", LVCFMT_CENTER, 50);//������
	m_havedClassList.InsertColumn(1, "���", LVCFMT_CENTER, 185);//������
	int code = loadXML();//��������
	if (code == 0)
	{
		MessageBox("���ؼ����������ʧ��!!!", "MCUϵͳ");
		return 0;
	}
	else if (code == 1)
	{
		MessageBox("�����������Ϊ��!!!", "MCUϵͳ");
		return 0;
	}
	setList();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
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
	className.clear();//����������
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
		className.push_back(Cstring);//���뵽���������
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
	#��Ӽ�����ť#
*/
void AddDetectClassHouse::OnBnClickedButtonAdddetectclass()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
		MessageBox("���ؼ����������ʧ��!!!", "MCUϵͳ");
		return;
	}
	else if (code == 1)
	{
		MessageBox("�����������Ϊ��!!!", "MCUϵͳ");
		return;
	}
	setList();
}


//void AddDetectClassHouse::OnKillfocusEditAddclassname()
//{
//	// TODO:  �ڴ���ӿؼ�֪ͨ����������
//}
