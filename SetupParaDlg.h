#pragma once

#include "tinyxml.h"
#include <string>
#include "afxwin.h"

using namespace std;

// SetupParaDlg 对话框

class SetupParaDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SetupParaDlg)

public:
	SetupParaDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SetupParaDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_SETPARA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int numOfRowImage = 0;//每行的图片数量
	int numOfColImage = 0;//每列的图片数量
	int numOfImage = 0;//总的图片数量
	float widthByImage = 0.0;//图片行重叠部分的比例
	float heightByImage = 0.0;//图片列重叠部分的比例

	int loadXML();//读入XML
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedOk();
};
