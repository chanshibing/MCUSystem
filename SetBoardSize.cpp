// SetBoardSize.cpp : 实现文件
//

#include "stdafx.h"
#include "MCU_system.h"
#include "SetBoardSize.h"
#include "afxdialogex.h"


// SetBoardSize 对话框

IMPLEMENT_DYNAMIC(SetBoardSize, CDialogEx)

SetBoardSize::SetBoardSize(CWnd* pParent /*=NULL*/)
	: CDialogEx(SetBoardSize::IDD, pParent)
{

}

SetBoardSize::~SetBoardSize()
{
}

void SetBoardSize::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(SetBoardSize, CDialogEx)
	ON_BN_CLICKED(IDOK, &SetBoardSize::OnBnClickedOk)
END_MESSAGE_MAP()


// SetBoardSize 消息处理程序

BOOL SetBoardSize::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString Cstring;
	Cstring.Format("%.2f", boardWidth);
	GetDlgItem(IDC_EDIT_SIZEWIDTH)->SetWindowText(Cstring);
	Cstring.Format("%.2f", boardHeight);
	GetDlgItem(IDC_EDIT_SIZEHEIGHT)->SetWindowText(Cstring);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void SetBoardSize::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	CString width_s, height_s;
	GetDlgItem(IDC_EDIT_SIZEWIDTH)->GetWindowText(width_s);
	GetDlgItem(IDC_EDIT_SIZEHEIGHT)->GetWindowText(height_s);
	if (width_s != "")
	{
		boardWidth = atof(width_s);
	}
	else
	{
		boardWidth = 0;
	}
	if (height_s != "")
	{
		boardHeight = atof(height_s);
	}
	else
	{
		boardHeight = 0;
	}
	CDialogEx::OnOK();
}

