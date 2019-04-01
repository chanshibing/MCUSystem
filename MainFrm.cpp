
// MainFrm.cpp : CMainFrame 类的实现
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
	ID_SEPARATOR,           // 状态行指示器
	/*ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,*/
	ID_INDICATOR_PROGRESS,
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO:  在此添加成员初始化代码
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
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT));
	m_wndStatusBar.SetPaneInfo(1, ID_INDICATOR_PROGRESS, SBPS_NORMAL, 180);//设置状态栏进度条窗格的宽度
	return 0;
}

int CMainFrame::SetToolBar()
{
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_TOOLBAR1))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}

	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0); //千万别忘了这句，好多教程都把这个忘了导致最后工具栏不显示
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

	HICON hIcon = ::CreateIconIndirect(&ii);  // 一旦不再需要，注意用DestroyIcon函数释放占用的内存及资源，否则会导致内存和GDI资源的泄漏
	::DeleteObject(hbmMask);

	::ReleaseDC(NULL, hDC);
	return hIcon;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO:  在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return TRUE;
}

// CMainFrame 诊断

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


// CMainFrame 消息处理程序
