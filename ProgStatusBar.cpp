#include "stdafx.h"
#include "ProgStatusBar.h"
#include "StdAfx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
IMPLEMENT_DYNAMIC(CProgStatusBar, CStatusBar)
BEGIN_MESSAGE_MAP(CProgStatusBar, CStatusBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()
CProgStatusBar::CProgStatusBar()
{
}


CProgStatusBar::~CProgStatusBar()
{
}

int CProgStatusBar::OnCreate(LPCREATESTRUCT lpcs)
{
	lpcs->style |= WS_CLIPCHILDREN;
	VERIFY(CStatusBar::OnCreate(lpcs) == 0);
	VERIFY(m_wndProgBar.Create(WS_CHILD, CRect(), this, 1));
	m_wndProgBar.SetRange(0, 100);
	return 0;
}

void CProgStatusBar::OnSize(UINT nType, int cx, int cy)
{
	CStatusBar::OnSize(nType, cx, cy);
	CRect rc;
	GetItemRect(0, &rc);//��ȡ״̬���ĵ�һ�����ڵĳߴ磻
	m_wndProgBar.MoveWindow(&rc, FALSE);//�ƶ���������״̬���ĵ�һ�����ڣ�
	// TODO:  �ڴ˴������Ϣ����������
}

/*
���ý������ĵ�ǰ״̬
*/
void CProgStatusBar::OnProgress(UINT pct)
{
	CProgressCtrl& pc = m_wndProgBar;
	DWORD dwOldStyle = pc.GetStyle();
	DWORD dwNewStyle = dwOldStyle;
	if (pct>0) //���pct>0������ʾ������
		dwNewStyle |= WS_VISIBLE;
	else //�������ؽ�������
		dwNewStyle &= ~WS_VISIBLE;
	if (dwNewStyle != dwOldStyle) {
		SetWindowText(NULL); //��ʾ������ǰ���״̬����
		SetWindowLong(pc.m_hWnd, GWL_STYLE, dwNewStyle);
		//���ý�����������ʾ״̬��
	}
	// ���ý������ĵ�ǰλ�ã�
	pc.SetPos(pct);
	if (pct == 0)
		// ���pct����0��֪ͨ����ܴ�����ʾ������Ϣ��
		GetParent()->PostMessage(WM_SETMESSAGESTRING, AFX_IDS_IDLEMESSAGE);
}

/*
���ý������Ŀ��
*/
void CProgStatusBar::setRange(UINT range)
{
	m_wndProgBar.SetRange(0, range);
}

/////////////////////////////////////////////////////////////////////////�ĵ�װ�ش�������
//void CPrgsbarDoc::Serialize(CArchive& ar)
//{
//	CWnd* pFrame = AfxGetMainWnd();
//	if (!ar.IsStoring()) {
//		for (int pct = 10; pct <= 100; pct += 10) {//���ĵ�װ�ؽ��з��洦��
//			Sleep(150);
//			if (pFrame)
//				pFrame->SendMessage(MYWM_PROGRESS, pct);
//		}
//	}
//	if (pFrame)
//		pFrame->SendMessage(MYWM_PROGRESS, 0);
//	((CEditView*)m_viewList.GetHead())->SerializeRaw(ar);//��ʾ�ı��ļ������ݣ�
//}