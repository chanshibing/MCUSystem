// addDetect.cpp : 实现文件
//

#include "stdafx.h"
#include "MCU_system.h"
#include "addDetect.h"
#include "afxdialogex.h"

// addDetect 对话框

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


// addDetect 消息处理程序



BOOL addDetect::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	for (int i = 0; i < className.size(); i++)
	{
		m_class.InsertString(i, className[i]);
	}
	m_class.SetCurSel(0);   //默认选择第一个 
	// TODO:  在此添加额外的初始化
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void addDetect::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	m_class.GetWindowTextA(instanceClass);
	currentIndex = m_class.GetCurSel();
	GetDlgItem(IDC_EDIT_INSTANCEWORD)->GetWindowText(instanceWord);
	CDialogEx::OnOK();
}
