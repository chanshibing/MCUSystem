#pragma once
#include "afxeditbrowsectrl.h"


// CompareSelectImgs �Ի���

class CompareSelectImgs : public CDialogEx
{
	DECLARE_DYNAMIC(CompareSelectImgs)

public:
	CompareSelectImgs(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CompareSelectImgs();

// �Ի�������
	enum { IDD = IDD_DIALOG_COMPARE_SELECTIMGS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString mFilePath1;//����ͼ·��
	CString mFilePath2;//����ͼ·��
	CString mFilePath3;//excel·��
public:
	afx_msg void OnBnClickedOk();
	// ѡ������ͼ
	CMFCEditBrowseCtrl m_EditBrowse1;
	// ѡ����ͼ
	CMFCEditBrowseCtrl m_EditBrowse2;
	// ѡ��excel
	CMFCEditBrowseCtrl m_EditBrowse3;
	virtual BOOL OnInitDialog();
};
