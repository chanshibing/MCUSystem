// CompareSelectImgs.cpp : 实现文件
//

#include "stdafx.h"
#include "MCU_system.h"
#include "CompareSelectImgs.h"
#include "afxdialogex.h"


// CompareSelectImgs 对话框

IMPLEMENT_DYNAMIC(CompareSelectImgs, CDialogEx)

CompareSelectImgs::CompareSelectImgs(CWnd* pParent /*=NULL*/)
	: CDialogEx(CompareSelectImgs::IDD, pParent)
{

}

CompareSelectImgs::~CompareSelectImgs()
{
}

void CompareSelectImgs::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCEDITBROWSE1, m_EditBrowse1);
	DDX_Control(pDX, IDC_MFCEDITBROWSE2, m_EditBrowse2);
	DDX_Control(pDX, IDC_MFCEDITBROWSE3, m_EditBrowse3);
}


BEGIN_MESSAGE_MAP(CompareSelectImgs, CDialogEx)
	ON_BN_CLICKED(IDOK, &CompareSelectImgs::OnBnClickedOk)
END_MESSAGE_MAP()


// CompareSelectImgs 消息处理程序

BOOL CompareSelectImgs::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	/*m_EditBrowse1.EnableFileBrowseButton(_T(""), _T("Text Files(*.jpg)|*.jpg||"));
	m_EditBrowse2.EnableFileBrowseButton(_T(""), _T("Text Files(*.jpg)|*.png|*.JPG|*.bmp"));
	m_EditBrowse3.EnableFileBrowseButton(_T(""),_T("Text Files(*.txt)|*.txt|All Files (*.*)|*.*||"));*/
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void CompareSelectImgs::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	GetDlgItemText(IDC_MFCEDITBROWSE1, mFilePath1);
	GetDlgItemText(IDC_MFCEDITBROWSE2, mFilePath2);
	GetDlgItemText(IDC_MFCEDITBROWSE3, mFilePath3);
	if (((mFilePath1.Find(".jpg") == -1 && mFilePath1.Find(".png") == -1 && mFilePath1.Find(".bmp") == -1 && mFilePath1.Find(".JPG") == -1)
		|| (mFilePath2.Find(".jpg") == -1 && mFilePath2.Find(".png") == -1 && mFilePath2.Find(".bmp") == -1 && mFilePath2.Find(".JPG") == -1)
		|| (mFilePath3.Find(".xls") == -1 && mFilePath1.Find(".xlsx") == -1)))
	{
		MessageBox(_T("文件类型不正确,重新选择！"), _T("MCU系统"));
		return;
	}
	CDialogEx::OnOK();
}


