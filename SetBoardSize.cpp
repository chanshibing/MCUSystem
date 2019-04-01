// SetBoardSize.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MCU_system.h"
#include "SetBoardSize.h"
#include "afxdialogex.h"


// SetBoardSize �Ի���

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


// SetBoardSize ��Ϣ�������

BOOL SetBoardSize::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString Cstring;
	Cstring.Format("%.2f", boardWidth);
	GetDlgItem(IDC_EDIT_SIZEWIDTH)->SetWindowText(Cstring);
	Cstring.Format("%.2f", boardHeight);
	GetDlgItem(IDC_EDIT_SIZEHEIGHT)->SetWindowText(Cstring);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void SetBoardSize::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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

