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
	GetItemRect(0, &rc);//获取状态条的第一个窗口的尺寸；
	m_wndProgBar.MoveWindow(&rc, FALSE);//移动进度条到状态条的第一个窗口；
	// TODO:  在此处添加消息处理程序代码
}

/*
设置进度条的当前状态
*/
void CProgStatusBar::OnProgress(UINT pct)
{
	CProgressCtrl& pc = m_wndProgBar;
	DWORD dwOldStyle = pc.GetStyle();
	DWORD dwNewStyle = dwOldStyle;
	if (pct>0) //如果pct>0，将显示进度条
		dwNewStyle |= WS_VISIBLE;
	else //否则隐藏进度条；
		dwNewStyle &= ~WS_VISIBLE;
	if (dwNewStyle != dwOldStyle) {
		SetWindowText(NULL); //显示进度条前清空状态条；
		SetWindowLong(pc.m_hWnd, GWL_STYLE, dwNewStyle);
		//设置进度条处于显示状态；
	}
	// 设置进度条的当前位置；
	pc.SetPos(pct);
	if (pct == 0)
		// 如果pct等于0，通知主框架窗口显示空闲信息；
		GetParent()->PostMessage(WM_SETMESSAGESTRING, AFX_IDS_IDLEMESSAGE);
}

/*
设置进度条的宽度
*/
void CProgStatusBar::setRange(UINT range)
{
	m_wndProgBar.SetRange(0, range);
}

/////////////////////////////////////////////////////////////////////////文档装载处理函数；
//void CPrgsbarDoc::Serialize(CArchive& ar)
//{
//	CWnd* pFrame = AfxGetMainWnd();
//	if (!ar.IsStoring()) {
//		for (int pct = 10; pct <= 100; pct += 10) {//对文档装载进行仿真处理；
//			Sleep(150);
//			if (pFrame)
//				pFrame->SendMessage(MYWM_PROGRESS, pct);
//		}
//	}
//	if (pFrame)
//		pFrame->SendMessage(MYWM_PROGRESS, 0);
//	((CEditView*)m_viewList.GetHead())->SerializeRaw(ar);//显示文本文件的内容；
//}