#pragma once


// SetBoardSize �Ի���

class SetBoardSize : public CDialogEx
{
	DECLARE_DYNAMIC(SetBoardSize)

public:
	SetBoardSize(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~SetBoardSize();

	float boardWidth = 0;
	float boardHeight = 0;
// �Ի�������
	enum { IDD = IDD_DIALOG_SETBOARDSIZE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
