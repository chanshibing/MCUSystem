#pragma once


// SetBoardSize 对话框

class SetBoardSize : public CDialogEx
{
	DECLARE_DYNAMIC(SetBoardSize)

public:
	SetBoardSize(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SetBoardSize();

	float boardWidth = 0;
	float boardHeight = 0;
// 对话框数据
	enum { IDD = IDD_DIALOG_SETBOARDSIZE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
