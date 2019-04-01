#pragma once
#include "afxcmn.h"
#include "tinyxml.h"
#include <string>
#include<vector>

using namespace std;
// AddDetectClassHouse �Ի���

class AddDetectClassHouse : public CDialogEx
{
	DECLARE_DYNAMIC(AddDetectClassHouse)

public:
	AddDetectClassHouse(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~AddDetectClassHouse();

// �Ի�������
	enum { IDD = IDD_DIALOG_ADDDETECTCLASSHOUSE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	
	int writeXML(CString name);
	void setList();

	DECLARE_MESSAGE_MAP()

public:
	vector<CString> className;//�������
	vector<int> numOfClassName;//������������

	int loadXML();
public:
	// ���е�����б�
	CListCtrl m_havedClassList;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonAdddetectclass();
//	afx_msg void OnKillfocusEditAddclassname();
};
