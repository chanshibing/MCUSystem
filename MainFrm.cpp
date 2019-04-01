
// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "MCU_system.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MYWM_PROGRESS 3001
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_MESSAGE(MYWM_PROGRESS, OnProgress)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ״̬��ָʾ��
	/*ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,*/
	ID_INDICATOR_PROGRESS,
};

// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO:  �ڴ���ӳ�Ա��ʼ������
}

CMainFrame::~CMainFrame()
{
}
LRESULT CMainFrame::OnProgress(WPARAM wp, LPARAM lp)
{
	LRESULT a = 0;
	return a;
}
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	SetToolBar();
	if (!m_wndStatusBar.Create(this))
	{ 
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT));
	m_wndStatusBar.SetPaneInfo(1, ID_INDICATOR_PROGRESS, SBPS_NORMAL, 180);//����״̬������������Ŀ��
	return 0;
}

int CMainFrame::SetToolBar()
{
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_TOOLBAR1))
	{
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}

	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0); //ǧ���������䣬�ö�̶̳���������˵�����󹤾�������ʾ
	m_imgList.Create(25, 25, ILC_COLOR32 | ILC_MASK, 0, 3);

	CBitmap bitmap;
	for (int i = 0; i < 17; i++){
		bitmap.LoadBitmapA(IDB_BITMAP_OPENFILE + i);
		m_imgList.Add(HBITMAPTOHICON(bitmap));
		bitmap.DeleteObject();
	}
	m_wndToolBar.GetToolBarCtrl().SetImageList(&m_imgList);

	m_wndToolBar.EnableToolTips(TRUE);

	return 0;
}

HICON CMainFrame::HBITMAPTOHICON(CBitmap& bitmap)
{
	BITMAP bmp;
	bitmap.GetBitmap(&bmp);

	HDC hDC = ::GetDC(NULL);
	HBITMAP hbmMask = ::CreateCompatibleBitmap(hDC,
		bmp.bmWidth, bmp.bmHeight);

	ICONINFO ii = { 0 };
	ii.fIcon = TRUE;
	ii.hbmColor = bitmap;
	ii.hbmMask = hbmMask;

	HICON hIcon = ::CreateIconIndirect(&ii);  // һ��������Ҫ��ע����DestroyIcon�����ͷ�ռ�õ��ڴ漰��Դ������ᵼ���ڴ��GDI��Դ��й©
	::DeleteObject(hbmMask);

	::ReleaseDC(NULL, hDC);
	return hIcon;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO:  �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return TRUE;
}

// CMainFrame ���

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame ��Ϣ�������
