#pragma once
#include "afxwin.h"
#include<vector>

// addDetect �Ի���

class addDetect : public CDialogEx
{
	DECLARE_DYNAMIC(addDetect)

public:
	addDetect(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~addDetect();

// �Ի�������
	enum { IDD = IDD_DIALOG_ADDDETECT };

public:
	CString instanceClass;
	CString instanceWord;
	int currentIndex;//��ǰλ������

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CComboBox m_class;
	virtual BOOL OnInitDialog();

	std::vector<CString> className;//�������1
	
};
