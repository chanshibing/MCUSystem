#pragma once
#include "afxcmn.h"
#include "tinyxml.h"
#include <string>
#include<vector>

using namespace std;
// AddDetectClassHouse 对话框

class AddDetectClassHouse : public CDialogEx
{
	DECLARE_DYNAMIC(AddDetectClassHouse)

public:
	AddDetectClassHouse(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~AddDetectClassHouse();

// 对话框数据
	enum { IDD = IDD_DIALOG_ADDDETECTCLASSHOUSE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	
	int writeXML(CString name);
	void setList();

	DECLARE_MESSAGE_MAP()

public:
	vector<CString> className;//类别名称
	vector<int> numOfClassName;//各个类别的数量

	int loadXML();
public:
	// 已有的类别列表
	CListCtrl m_havedClassList;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonAdddetectclass();
//	afx_msg void OnKillfocusEditAddclassname();
};
