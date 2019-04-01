// addDetect.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MCU_system.h"
#include "addDetect.h"
#include "afxdialogex.h"

// addDetect �Ի���

IMPLEMENT_DYNAMIC(addDetect, CDialogEx)

addDetect::addDetect(CWnd* pParent /*=NULL*/)
	: CDialogEx(addDetect::IDD, pParent)
{

}

addDetect::~addDetect()
{
}

void addDetect::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CLASS, m_class);
}


BEGIN_MESSAGE_MAP(addDetect, CDialogEx)
	ON_BN_CLICKED(IDOK, &addDetect::OnBnClickedOk)
END_MESSAGE_MAP()


// addDetect ��Ϣ�������



BOOL addDetect::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	for (int i = 0; i < className.size(); i++)
	{
		m_class.InsertString(i, className[i]);
	}
	m_class.SetCurSel(0);   //Ĭ��ѡ���һ�� 
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

void addDetect::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_class.GetWindowTextA(instanceClass);
	currentIndex = m_class.GetCurSel();
	GetDlgItem(IDC_EDIT_INSTANCEWORD)->GetWindowText(instanceWord);
	CDialogEx::OnOK();
}
