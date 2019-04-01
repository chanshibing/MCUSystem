#pragma once

#include "tinyxml.h"
#include <string>
#include "afxwin.h"

using namespace std;

// SetupParaDlg �Ի���

class SetupParaDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SetupParaDlg)

public:
	SetupParaDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~SetupParaDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_SETPARA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int numOfRowImage = 0;//ÿ�е�ͼƬ����
	int numOfColImage = 0;//ÿ�е�ͼƬ����
	int numOfImage = 0;//�ܵ�ͼƬ����
	float widthByImage = 0.0;//ͼƬ���ص����ֵı���
	float heightByImage = 0.0;//ͼƬ���ص����ֵı���

	int loadXML();//����XML
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedOk();
};
