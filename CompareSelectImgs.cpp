// CompareSelectImgs.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MCU_system.h"
#include "CompareSelectImgs.h"
#include "afxdialogex.h"


// CompareSelectImgs �Ի���

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


// CompareSelectImgs ��Ϣ�������

BOOL CompareSelectImgs::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	/*m_EditBrowse1.EnableFileBrowseButton(_T(""), _T("Text Files(*.jpg)|*.jpg||"));
	m_EditBrowse2.EnableFileBrowseButton(_T(""), _T("Text Files(*.jpg)|*.png|*.JPG|*.bmp"));
	m_EditBrowse3.EnableFileBrowseButton(_T(""),_T("Text Files(*.txt)|*.txt|All Files (*.*)|*.*||"));*/
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

void CompareSelectImgs::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	GetDlgItemText(IDC_MFCEDITBROWSE1, mFilePath1);
	GetDlgItemText(IDC_MFCEDITBROWSE2, mFilePath2);
	GetDlgItemText(IDC_MFCEDITBROWSE3, mFilePath3);
	if (((mFilePath1.Find(".jpg") == -1 && mFilePath1.Find(".png") == -1 && mFilePath1.Find(".bmp") == -1 && mFilePath1.Find(".JPG") == -1)
		|| (mFilePath2.Find(".jpg") == -1 && mFilePath2.Find(".png") == -1 && mFilePath2.Find(".bmp") == -1 && mFilePath2.Find(".JPG") == -1)
		|| (mFilePath3.Find(".xls") == -1 && mFilePath1.Find(".xlsx") == -1)))
	{
		MessageBox(_T("�ļ����Ͳ���ȷ,����ѡ��"), _T("MCUϵͳ"));
		return;
	}
	CDialogEx::OnOK();
}


