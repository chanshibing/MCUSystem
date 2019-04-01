#pragma once
#include "afxeditbrowsectrl.h"


// CompareSelectImgs 对话框

class CompareSelectImgs : public CDialogEx
{
	DECLARE_DYNAMIC(CompareSelectImgs)

public:
	CompareSelectImgs(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CompareSelectImgs();

// 对话框数据
	enum { IDD = IDD_DIALOG_COMPARE_SELECTIMGS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString mFilePath1;//正面图路径
	CString mFilePath2;//反面图路径
	CString mFilePath3;//excel路径
public:
	afx_msg void OnBnClickedOk();
	// 选择正面图
	CMFCEditBrowseCtrl m_EditBrowse1;
	// 选择反面图
	CMFCEditBrowseCtrl m_EditBrowse2;
	// 选择excel
	CMFCEditBrowseCtrl m_EditBrowse3;
	virtual BOOL OnInitDialog();
};
