// SetupParaDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MCU_system.h"
#include "SetupParaDlg.h"
#include "afxdialogex.h"


// SetupParaDlg �Ի���

IMPLEMENT_DYNAMIC(SetupParaDlg, CDialogEx)

SetupParaDlg::SetupParaDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(SetupParaDlg::IDD, pParent)
{

}

SetupParaDlg::~SetupParaDlg()
{
}

void SetupParaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(SetupParaDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, &SetupParaDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// SetupParaDlg ��Ϣ�������

BOOL SetupParaDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	if(numOfRowImage == 0 || numOfColImage == 0)
	{
		loadXML();
	}
	else
	{
		CString Cstring;
		Cstring.Format("%d", numOfRowImage);
		GetDlgItem(IDC_EDIT1)->SetWindowTextA(Cstring);

		Cstring.Format("%d", numOfColImage);
		GetDlgItem(IDC_EDIT2)->SetWindowTextA(Cstring);

		Cstring.Format("%.2f", widthByImage);
		GetDlgItem(IDC_EDIT3)->SetWindowTextA(Cstring);

		Cstring.Format("%.2f", heightByImage);
		GetDlgItem(IDC_EDIT4)->SetWindowTextA(Cstring);

		Cstring.Format("%d", numOfImage);
		GetDlgItem(IDC_EDIT5)->SetWindowTextA(Cstring);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

int SetupParaDlg::loadXML()
{
	TiXmlDocument doc;
	if (!doc.LoadFile("configure.xml"))
	{
		return 0;
	}

	TiXmlElement* root = doc.FirstChildElement();
	if (root == NULL)
	{
		doc.Clear();
		return 0;
	}

	for (TiXmlElement* elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
	{
		string elemName = elem->Value();
		const char* attr;
		attr = elem->Attribute("priority");
		if (strcmp(attr, "1") == 0)
		{
			TiXmlElement* e1 = elem->FirstChildElement("numOfRowImage");
			TiXmlNode* e2 = e1->FirstChild();
			const char *numOfRowImage_char = e2->ToText()->Value();
			CString Cstring;
			Cstring.Format("%s", numOfRowImage_char);
			GetDlgItem(IDC_EDIT1)->SetWindowTextA(Cstring);

			e1 = elem->FirstChildElement("numOfColImage");
			e2 = e1->FirstChild();
			const char *numOfColImage_char = e2->ToText()->Value();
			Cstring.Format("%s", numOfColImage_char);
			GetDlgItem(IDC_EDIT2)->SetWindowTextA(Cstring);

			e1 = elem->FirstChildElement("widthByImage");
			e2 = e1->FirstChild();
			const char *widthByImage_char = e2->ToText()->Value();
			Cstring.Format("%s", widthByImage_char);
			GetDlgItem(IDC_EDIT3)->SetWindowTextA(Cstring);

			e1 = elem->FirstChildElement("heightByImage");
			e2 = e1->FirstChild();
			const char *heightByImage_char = e2->ToText()->Value();
			Cstring.Format("%s", heightByImage_char);
			GetDlgItem(IDC_EDIT4)->SetWindowTextA(Cstring);

			e1 = elem->FirstChildElement("numOfImage");
			e2 = e1->FirstChild();
			const char *numOfImage_char = e2->ToText()->Value();
			Cstring.Format("%s", numOfImage_char);
			GetDlgItem(IDC_EDIT5)->SetWindowTextA(Cstring);
		}
	}
	doc.Clear();
	return 1;

}


HBRUSH SetupParaDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if (IDC_STATIC_W1 == pWnd->GetDlgCtrlID())
	{
		pDC->SetTextColor(RGB(255, 0, 0));	//��ɫ
	}
	if (IDC_STATIC_W2 == pWnd->GetDlgCtrlID())
	{
		pDC->SetTextColor(RGB(255, 0, 0));	//��ɫ
	}
	if (IDC_STATIC_W3 == pWnd->GetDlgCtrlID())
	{
		pDC->SetTextColor(RGB(255, 0, 0));	//��ɫ
	}
	if (IDC_STATIC_W4 == pWnd->GetDlgCtrlID())
	{
		pDC->SetTextColor(RGB(255, 0, 0));	//��ɫ
	}
	if (IDC_STATIC_W5 == pWnd->GetDlgCtrlID())
	{
		pDC->SetTextColor(RGB(255, 0, 0));	//��ɫ
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


void SetupParaDlg::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString row_s, col_s, num_s, raito_row_s, raito_col_s;
	GetDlgItem(IDC_EDIT1)->GetWindowText(row_s);
	GetDlgItem(IDC_EDIT2)->GetWindowText(col_s);
	GetDlgItem(IDC_EDIT3)->GetWindowText(raito_row_s);
	GetDlgItem(IDC_EDIT4)->GetWindowText(raito_col_s);
	GetDlgItem(IDC_EDIT5)->GetWindowText(num_s);
	numOfRowImage = _ttoi(row_s);
	numOfColImage = _ttoi(col_s);
	numOfImage = _ttoi(num_s);
	widthByImage = atof(raito_row_s);
	heightByImage = atof(raito_col_s);
	if (!numOfRowImage)
	{
		GetDlgItem(IDC_STATIC_W2)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_W3)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_W4)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_W5)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_W1)->ShowWindow(TRUE);
		return;
	}
	else if (!numOfColImage)
	{
		GetDlgItem(IDC_STATIC_W1)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_W3)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_W4)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_W5)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_W2)->ShowWindow(TRUE);
		return;
	}
	else if (widthByImage <= 0 || widthByImage > 1)
	{
		GetDlgItem(IDC_STATIC_W2)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_W1)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_W4)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_W5)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_W3)->ShowWindow(TRUE);
		return;
	}
	else if (heightByImage <= 0 || heightByImage > 1)
	{
		GetDlgItem(IDC_STATIC_W2)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_W3)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_W1)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_W5)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_W4)->ShowWindow(TRUE);
		return;
	}
	else if (!numOfImage)
	{
		GetDlgItem(IDC_STATIC_W2)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_W3)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_W4)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_W1)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_W5)->ShowWindow(TRUE);
		return;
	}

	GetDlgItem(IDC_STATIC_W2)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_W3)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_W4)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_W5)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_W1)->ShowWindow(FALSE);
	CDialogEx::OnOK();
}
