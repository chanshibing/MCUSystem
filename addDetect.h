#pragma once
#include "afxwin.h"
#include<vector>

// addDetect 对话框

class addDetect : public CDialogEx
{
	DECLARE_DYNAMIC(addDetect)

public:
	addDetect(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~addDetect();

// 对话框数据
	enum { IDD = IDD_DIALOG_ADDDETECT };

public:
	CString instanceClass;
	CString instanceWord;
	int currentIndex;//当前位置索引

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CComboBox m_class;
	virtual BOOL OnInitDialog();

	std::vector<CString> className;//类别名称1
	
};
