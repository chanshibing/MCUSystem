
// MCU_systemView.cpp : CMCU_systemView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MCU_system.h"
#endif
#include "MCU_systemDoc.h"
#include "MCU_systemView.h"

#include "CvvImage.h"
#include "ProgStatusBar.h"
#include "Excel.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define IDC_D_BTN_PREVIOUS 10000
#define IDC_D_BTN_NEXT 10001
#define WM_USER_MSG 1300
#define WM_USER_STOP_MSG 1301
#define WM_USER_DETECT_MSG 1302
#define WM_USER_DETECT_STOP_MSG 1303
#define WM_USER_TXPJ_MSG 1304
#define IDC_EDIT_NEW 10//新的文本框
DWORD WINAPI imageStitching(LPVOID p);
DWORD WINAPI imageDetect(LPVOID p);
// CMCU_systemView

IMPLEMENT_DYNCREATE(CMCU_systemView, CView)

BEGIN_MESSAGE_MAP(CMCU_systemView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_FILE_OPEN, &CMCU_systemView::OnFileOpen)
	ON_COMMAND(ID_SETPARA, &CMCU_systemView::OnSetpara)
	ON_COMMAND(ID_BUTTON_OPENFILE, &CMCU_systemView::OnButtonOpenfile_toolBar)
	ON_COMMAND(ID_FILE_SAVE, &CMCU_systemView::OnFileSave)
	ON_COMMAND(ID_BUTTON_SAVE, &CMCU_systemView::OnButtonSave_toolBar)
	ON_COMMAND(ID_BUTTON_REVOKE, &CMCU_systemView::OnButtonRevoke)
	ON_COMMAND(ID_BUTTON_REDO, &CMCU_systemView::OnButtonRedo)
	ON_COMMAND(ID_BUTTON_START, &CMCU_systemView::OnButtonStart)
	ON_COMMAND(ID_BUTTON_PAUSE, &CMCU_systemView::OnButtonPause)
	ON_COMMAND(ID_BUTTON_STOP, &CMCU_systemView::OnButtonStop)
	ON_COMMAND(ID_BUTTON_RIGHTROATE, &CMCU_systemView::OnButtonRightroate)
	ON_COMMAND(ID_BUTTON_ENLARGE, &CMCU_systemView::OnButtonEnlarge)
	ON_COMMAND(ID_BUTTON_NARROW, &CMCU_systemView::OnButtonNarrow)
	ON_COMMAND(ID_BUTTON_CUTOUT, &CMCU_systemView::OnButtonCutout)
	ON_COMMAND(ID_BUTTON_ADDTEXT, &CMCU_systemView::OnButtonAddtext)
	ON_COMMAND(ID_BUTTON_LEFTROATE, &CMCU_systemView::OnButtonLeftroate)
	ON_BN_CLICKED(IDC_D_BTN_PREVIOUS, &CMCU_systemView::OnButtonPreviousClick)
	ON_BN_CLICKED(IDC_D_BTN_NEXT, &CMCU_systemView::OnButtonNextClick)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_ENLARGE, &CMCU_systemView::OnUpdateButtonEnlarge)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_NARROW, &CMCU_systemView::OnUpdateButtonNarrow)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_UPDATE_COMMAND_UI(ID_BUTTON_CUTOUT, &CMCU_systemView::OnUpdateButtonCutout)
	ON_COMMAND(ID_BUTTON_MAKESURE, &CMCU_systemView::OnButtonMakesure)
	ON_COMMAND(ID_BUTTON_CANCEL, &CMCU_systemView::OnButtonCancel)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_MAKESURE, &CMCU_systemView::OnUpdateButtonMakesure)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_CANCEL, &CMCU_systemView::OnUpdateButtonCancel)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_ADDTEXT, &CMCU_systemView::OnUpdateButtonAddtext)
	ON_WM_LBUTTONDBLCLK()
	ON_UPDATE_COMMAND_UI(ID_BUTTON_LEFTROATE, &CMCU_systemView::OnUpdateButtonLeftroate)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_RIGHTROATE, &CMCU_systemView::OnUpdateButtonRightroate)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_PAUSE, &CMCU_systemView::OnUpdateButtonPause)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_STOP, &CMCU_systemView::OnUpdateButtonStop)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_START, &CMCU_systemView::OnUpdateButtonStart)
	ON_COMMAND(ID_DETECT, &CMCU_systemView::OnDetect)
	ON_MESSAGE(WM_USER_MSG, &CMCU_systemView::imageStitchingOver)
	ON_MESSAGE(WM_USER_STOP_MSG, &CMCU_systemView::imageStitchingStop)
	ON_MESSAGE(WM_USER_DETECT_MSG, &CMCU_systemView::imageDetectOver)
	ON_MESSAGE(WM_USER_DETECT_STOP_MSG, &CMCU_systemView::imageDetectStopOver)
	ON_MESSAGE(WM_USER_TXPJ_MSG, &CMCU_systemView::showTXPJMessage)
	ON_COMMAND(ID_SAVEASBOM, &CMCU_systemView::OnSaveasbom)
	ON_COMMAND(ID_DETECTSTOP, &CMCU_systemView::OnDetectstop)
	ON_UPDATE_COMMAND_UI(ID_DETECTSTOP, &CMCU_systemView::OnUpdateDetectstop)
	ON_UPDATE_COMMAND_UI(ID_DETECT, &CMCU_systemView::OnUpdateDetect)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_REVOKE, &CMCU_systemView::OnUpdateButtonRevoke)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_REDO, &CMCU_systemView::OnUpdateButtonRedo)
	ON_COMMAND(ID_STATISTICSPIN, &CMCU_systemView::OnStatisticspin)
	ON_COMMAND(ID_SETBOARDSIZE, &CMCU_systemView::OnSetboardsize)
	ON_WM_CREATE()
	ON_COMMAND(ID_COMPARE_SELECTIMGS, &CMCU_systemView::OnCompareSelectimgs)
	ON_UPDATE_COMMAND_UI(ID_COMPARE_START, &CMCU_systemView::OnUpdateCompareStart)
	ON_UPDATE_COMMAND_UI(ID_COMPARE_PAUSE, &CMCU_systemView::OnUpdateComparePause)
	ON_UPDATE_COMMAND_UI(ID_COMPARE_STOP, &CMCU_systemView::OnUpdateCompareStop)
	ON_COMMAND(ID_COMPARE_START, &CMCU_systemView::OnCompareStart)
	ON_COMMAND(ID_COMPARE_PAUSE, &CMCU_systemView::OnComparePause)
	ON_COMMAND(ID_COMPARE_STOP, &CMCU_systemView::OnCompareStop)
	ON_NOTIFY(LVN_ITEMCHANGED, 55, &CMCU_systemView::OnItemchangedList)
	ON_NOTIFY(NM_CLICK, 55, &CMCU_systemView::OnNMClickList)
	ON_NOTIFY(NM_DBLCLK, 58, &CMCU_systemView::OnDblclkList)
	ON_EN_KILLFOCUS(59, &CMCU_systemView::OnKillfocusEditList)

	ON_WM_MOUSEWHEEL()
//	ON_WM_KEYDOWN()
//	ON_WM_KEYUP()
ON_COMMAND(ID_REFRESH, &CMCU_systemView::OnRefresh)
ON_UPDATE_COMMAND_UI(ID_BUTTON_ADD, &CMCU_systemView::OnUpdateButtonAdd)
ON_COMMAND(ID_BUTTON_ADD, &CMCU_systemView::OnButtonAdd)
ON_COMMAND(ID_ADDDETECTCLASSHOUSE, &CMCU_systemView::OnAdddetectclasshouse)
ON_COMMAND(ID_BUTTON_FILLRED, &CMCU_systemView::OnButtonFillred)
ON_UPDATE_COMMAND_UI(ID_BUTTON_FILLRED, &CMCU_systemView::OnUpdateButtonFillred)
END_MESSAGE_MAP()

// CMCU_systemView 构造/析构

CMCU_systemView::CMCU_systemView()
{
	// TODO:  在此处添加构造代码

}

CMCU_systemView::~CMCU_systemView()
{
}

BOOL CMCU_systemView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式


	return CView::PreCreateWindow(cs);
}

int CMCU_systemView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	

	m_staticMessage.Create("进程信息显示：", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, 56);
	m_editMessage.Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE, CRect(0, 0, 0, 0), this, 57);
	// TODO:  在此添加您专用的创建代码
	m_btn_previous.Create(_T("上一张"), WS_CHILD | WSF_VISIBLE | BS_PUSHBUTTON, CRect(0, 0, 0, 0), this, IDC_D_BTN_PREVIOUS);
	m_btn_next.Create(_T("下一张"), WS_CHILD | WSF_VISIBLE | BS_PUSHBUTTON, CRect(0, 0, 0, 0), this, IDC_D_BTN_NEXT);
	m_s_middle.Create(_T("/"), SS_CENTER | WSF_VISIBLE | SS_CENTERIMAGE, CRect(0, 0, 0, 0), this, 4);
	m_list.Create(LVS_ALIGNLEFT | WS_CHILD | WS_VISIBLE | LVS_SINGLESEL, CRect(0, 0, 0, 0), this, 55);
	LONG lStyle;
	lStyle = GetWindowLong(m_list.m_hWnd, GWL_STYLE);//获取当前窗口style
	lStyle &= ~LVS_TYPEMASK; //清除显示方式位
	lStyle |= LVS_REPORT; //设置style
	SetWindowLong(m_list.m_hWnd, GWL_STYLE, lStyle);//设置style

	DWORD dwStyle = m_list.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_CHECKBOXES;//item前生成checkbox控件
	m_list.SetExtendedStyle(dwStyle); //设置扩展风格
	m_list.InsertColumn(0, "编号", LVCFMT_LEFT, 80);//插入列
	m_list.InsertColumn(1, "类别", LVCFMT_LEFT, 50);//插入列
	m_list.InsertColumn(2, "类型", LVCFMT_LEFT, 70);//插入列

	m_messageList.Create(LVS_ALIGNLEFT | WS_CHILD | WS_VISIBLE | LVS_SINGLESEL, CRect(0, 0, 0, 0), this, 58);
	LONG lStyle1;
	lStyle1 = GetWindowLong(m_messageList.m_hWnd, GWL_STYLE);//获取当前窗口style
	lStyle1 &= ~LVS_TYPEMASK; //清除显示方式位
	lStyle1 |= LVS_REPORT; //设置style
	SetWindowLong(m_messageList.m_hWnd, GWL_STYLE, lStyle1);//设置style

	DWORD dwStyle1 = m_messageList.GetExtendedStyle();
	dwStyle1 |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle1 |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	m_messageList.SetExtendedStyle(dwStyle1); //设置扩展风格
	m_messageList.InsertColumn(0, "名称", LVCFMT_RIGHT, 100);//插入列
	m_messageList.InsertColumn(1, "数据", LVCFMT_CENTER, 100);//插入列
	m_messageList.InsertItem(0, "         编号：");
	m_messageList.InsertItem(1, "         类别：");
	m_messageList.InsertItem(2, "         类型：");
	m_messageList.InsertItem(3, "左上角坐标：");
	m_messageList.InsertItem(4, "右下角坐标：");
	m_messageList.InsertItem(5, "      引脚一：");
	m_messageList.InsertItem(6, "      引脚二：");
	m_messageList.InsertItem(7, "   字体大小：");
	m_messageList.SetItemText(7, 1, "50");
	m_messageList.InsertItem(8, "检测器件总数：");
	m_modifyMessageListEdit.Create(WS_CHILD | WS_VISIBLE | WS_BORDER, CRect(0, 0, 0, 0), this, 59);

	loadXML_Configure();//加载配置信息

	return 0;
}

// CMCU_systemView 绘制

void CMCU_systemView::OnDraw(CDC* pDC)
{
	CMCU_systemDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	if (flag_compareBoard)//如果模块对比显示
	{
		showBoardPicture(boardImg1, boardImg2);
	}
	else
	{ 
		showPicture(show_img);
	}
	
	//设置控件位置
	CRect pictureControl;
	GetClientRect(&pictureControl);
	//大小位置
	int right = pictureControl.right;
	m_staticMessage.MoveWindow(CRect(0, pictureControl.bottom / 2, 200, pictureControl.bottom ));
	m_editMessage.MoveWindow(CRect(1, pictureControl.bottom / 2 + 20, 198, pictureControl.bottom - 1));
	m_list.MoveWindow(CRect(pictureControl.right - 200, pictureControl.top, pictureControl.right, pictureControl.bottom));
	m_messageList.MoveWindow(CRect(0, pictureControl.top, 200, pictureControl.bottom / 2));

	


	// TODO:  在此处为本机数据添加绘制代码
}



// CMCU_systemView 打印
BOOL CMCU_systemView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CMCU_systemView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  添加额外的打印前进行的初始化过程
}

void CMCU_systemView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  添加打印后进行的清理过程
}


// CMCU_systemView 诊断

#ifdef _DEBUG
void CMCU_systemView::AssertValid() const
{
	CView::AssertValid();
}

void CMCU_systemView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMCU_systemDoc* CMCU_systemView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMCU_systemDoc)));
	return (CMCU_systemDoc*)m_pDocument;
}
#endif //_DEBUG


// CMCU_systemView 消息处理程序


/*
*打开文件目录
*
*/
void CMCU_systemView::OnFileOpen()
{
	// TODO:  在此添加命令处理程序代码
	int returnStatus = openFilePath.openFile(m_hWnd, flag_setPara);//打开文件目录

	adjustShowPictureRegion(200, 200, 0, 40);//调整图片显示区域大小

	if (returnStatus == 1)
	{
		MessageBox(_T("打开失败"), _T("MCU系统"));
		return;
	}
	else if (returnStatus == 2)
	{
		detectAlgorithm.instanceRect_vector.clear();
		currentRowInList = -1;
		setListControlContent();
		flag_compareBoard = false;//模块对比
		flag_showComponent = false;//不显示翻页组件
		currentPage = 0;
		Mat img_;
		img_.copyTo(src_img);
		src_img.copyTo(show_img);
		CWnd::Invalidate(TRUE);//调用ondraw函数
		
		srcROI2d.x = 0;
		srcROI2d.y = 0;
		srcROI2d.width = show_img.cols;
		srcROI2d.height = show_img.rows;

		initView();//初始化界面
		//设置状态栏信息
		pFrame = (CMainFrame *)AfxGetMainWnd();
		pFrame->m_wndStatusBar.OnProgress(0);
		pFrame->m_wndStatusBar.SetPaneText(1, "就绪", TRUE);
		//初始化队列
		editAlgorithm.setQueue_NULL();

		setStartCompareEnable(true);//开始对比按钮
		setPauseCompareEnable(true);//暂停对比按钮
		setStopCompareEnable(true);//停止对比按钮
		setAddEnable(true);
		setFillRedEnable(true);
		flag_addDetect = false;

		MessageBox(_T("当前文件目录内没有图像，请确认目录是否正确"), _T("MCU系统"));
		return;
	}
	else//执行操作
	{
		detectAlgorithm.instanceRect_vector.clear();
		currentRowInList = -1;
		setListControlContent();
		currentPage = 0;
		src_img = imread(openFilePath.imageFilePathNames[currentPage]);
		src_img.copyTo(show_img);
		CWnd::Invalidate(TRUE);//调用ondraw函数

		flag_compareBoard = false;//模块对比
		flag_showComponent = true;//显示翻页组件

		srcROI2d.x = 0;
		srcROI2d.y = 0;
		srcROI2d.width = show_img.cols;
		srcROI2d.height = show_img.rows;

		havePictuireView();//有图像界面
		//设置状态栏信息
		pFrame = (CMainFrame *)AfxGetMainWnd();
		pFrame->m_wndStatusBar.OnProgress(0);
		pFrame->m_wndStatusBar.SetPaneText(1, "就绪", TRUE);
		//初始化队列
		editAlgorithm.setQueue_NULL();

		setStartCompareEnable(true);//开始对比按钮
		setPauseCompareEnable(true);//暂停对比按钮
		setStopCompareEnable(true);//停止对比按钮
		setAddEnable(true);
		setFillRedEnable(true);
		flag_addDetect = false;

	}
}

/*
*调整图片显示区域大小
*
*/
void CMCU_systemView::adjustShowPictureRegion(int left, int right, int top, int bottom){
	clientToLeft = left;//图片显示区域到客户端左边距离
	clientToRight = right;//图片显示区域到客户端右边距离
	clientToUp = top;//图片显示区域到客户端上边距离
	clientToBottom = bottom;//图片显示区域到客户端下边距离
}

/*
*保存文件
*
*/
void CMCU_systemView::OnFileSave()
{
	// TODO:  在此添加命令处理程序代码
	CString FilePath;
	TCHAR szFilter[] = _T("图片(*.png;*.jpg;*.jpeg;*.bmp)|*.png;*.jpg;*.jpeg;*.bmp|所有文件(*.*)|*.*||");
	CFileDialog dlg(FALSE, _T("png"), "", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
	if (dlg.DoModal() == IDOK)
	{
		src_img.copyTo(save_img);
		for (int i = 0; i < detectAlgorithm.instanceRect_vector.size(); i++)
		{
			int left = detectAlgorithm.instanceRect_vector[i].left_top.x;
			int right = detectAlgorithm.instanceRect_vector[i].right_bottom.x;
			int top = detectAlgorithm.instanceRect_vector[i].left_top.y;
			int bottom = detectAlgorithm.instanceRect_vector[i].right_bottom.y;
			if (detectAlgorithm.instanceRect_vector[i].flag_noMark == false
				&& left >= 0 && top >= 0 && right < srcROI2d.width && bottom < srcROI2d.height)
			{
				detectAlgorithm.drawRect(save_img, detectAlgorithm.instanceRect_vector[i].left_top, detectAlgorithm.instanceRect_vector[i].right_bottom, detectAlgorithm.instanceRect_vector[i].manaulAdd);//画矩形
			}
		}
		
		FilePath = dlg.GetPathName();
		imwrite((LPCTSTR)FilePath, save_img);
	}
}

/*
*显示图片
*
*/
void CMCU_systemView::showPicture(Mat mat)
{
	setPageTurningComponent();//设置翻页组件

	CDC* pDC = GetDC();
	HDC hDC = pDC->GetSafeHdc();
	IplImage *img = &IplImage(mat);
	CvvImage cimg;
	cimg.CopyOf(img);

	CRect rect;
	GetClientRect(&rect);
	//获取图片的宽 高  
	int width = mat.cols;
	int height = mat.rows;
	pictureRatio = 0.0;
	rect.left += clientToLeft;
	rect.bottom = rect.bottom - clientToBottom;
	rect.right -= clientToRight;
	int rectWidth = rect.right - rect.left;
	int rectHeight = rect.bottom - rect.top;
	if (rectWidth * 1.0 / rectHeight <= width * 1.0 / height)     //以长为主,调整大小
	{
		if (width != 0)
		{
			pictureRatio = (double)rectWidth / (double)width;//获得图像显示比例
			int realHeight = rectWidth * height / width;  //调整显示大小，不然显示超出边框
			rect.top = rect.top + rectHeight / 2 - realHeight / 2;
			rect.bottom = rect.top + realHeight;
		}
	}
	else
	{
		if (height != 0)
		{
			pictureRatio = (float)rectHeight / (float)height;//获得图像显示比例
			int realwidth = width * rectHeight / height;  //以宽为主，调整大小
			rect.left = rect.left + rectWidth / 2 - realwidth / 2;
			rect.right = rect.left + realwidth;
		}
	}

	//获取显示图片窗口的左上角坐标
	CRect pictureControl;
	GetClientRect(&pictureControl);
	//ScreenToClient(&pictureControl);
	//显示图片相对于客户端左上角坐标
	pictureInClient_ltPoint.y = pictureControl.top + rect.top;
	pictureInClient_ltPoint.x = pictureControl.left + rect.left;
	//显示图片相对于客户端右下角坐标
	pictureInClient_rbPoint.y = pictureControl.top + rect.bottom;
	pictureInClient_rbPoint.x = pictureControl.left + rect.right;

	

	cimg.DrawToHDC(hDC, &rect);
	cimg.Destroy();
	
	

	//画半透明矩形
	if (detectAlgorithm.instanceRect_vector.size() > 0 && flag_draw)
	{
		//画矩形框
		int sumNum = DrawRect();

		//显示检测器件总数
		CString sumNumString;
		sumNumString.Format("%d", sumNum);
		m_messageList.SetItemText(8, 1, sumNumString);

		if (fillRedEnable)
		{
			fillRed();//全部标记红
		}
		else if (flag_listFocus && !detectAlgorithm.instanceRect_vector[currentRowInList].flag_noMark)//如果当前焦点在listControl,并且当前元器件的不标记为假
		{
			setMessageListControlContent();
			DrawAlphaRect(pDC);
		}
	}
	
	ReleaseDC(pDC);
}
/*
	#画矩形框#
*/
int CMCU_systemView::DrawRect()
{
	//画矩形框
	CClientDC dc(this);
	dc.SelectStockObject(NULL_BRUSH); 
	CPen penRed(PS_SOLID, 3, RGB(255, 0, 0));//做一支红色粗细为1的笔
	CPen penBlue(PS_SOLID, 3, RGB(0, 0, 255));//做一支蓝色粗细为1的笔
	//dc.SelectObject(&penRed);//将pen放到dc上
	//dc.SetROP2(R2_NOT);
	CRect dcRect;
	int sumNum = 0;
	for (int i = 0; i < detectAlgorithm.instanceRect_vector.size(); i++)
	{
		if (!detectAlgorithm.instanceRect_vector[i].flag_noMark)//如果不标记元器件为假，则标记元器件
		{
			int left = detectAlgorithm.instanceRect_vector[i].left_top.x;
			int right = detectAlgorithm.instanceRect_vector[i].right_bottom.x;
			int top = detectAlgorithm.instanceRect_vector[i].left_top.y;
			int bottom = detectAlgorithm.instanceRect_vector[i].right_bottom.y;
			if (left >= 0 && top >= 0 && right < srcROI2d.width && bottom < srcROI2d.height)
			{
				sumNum++;
				dcRect.left = pictureInClient_ltPoint.x + left * pictureRatio;
				dcRect.top = pictureInClient_ltPoint.y + top * pictureRatio;
				dcRect.right = pictureInClient_ltPoint.x + right * pictureRatio;
				dcRect.bottom = pictureInClient_ltPoint.y + bottom * pictureRatio;
				if (detectAlgorithm.instanceRect_vector[i].manaulAdd)
				{
					dc.SelectObject(&penBlue);//蓝色笔绘画矩形
				}
				else
				{
					dc.SelectObject(&penRed);//红色笔绘画矩形
				}
				dc.Rectangle(dcRect);
			}
		}
	}
	return sumNum;
}
/*
	#画半透明矩形#
*/  
void CMCU_systemView::DrawAlphaRect(CDC *pDC)
{
	CRect dcRect;
	int left = detectAlgorithm.instanceRect_vector[currentRowInList].left_top.x;
	int right = detectAlgorithm.instanceRect_vector[currentRowInList].right_bottom.x;
	int top = detectAlgorithm.instanceRect_vector[currentRowInList].left_top.y;
	int bottom = detectAlgorithm.instanceRect_vector[currentRowInList].right_bottom.y;

	if (left >= 0 && top >= 0 && right < srcROI2d.width && bottom < srcROI2d.height)//约束在图片内
	{
		dcRect.left = pictureInClient_ltPoint.x + left * pictureRatio;
		dcRect.top = pictureInClient_ltPoint.y + top * pictureRatio;
		dcRect.right = pictureInClient_ltPoint.x + right * pictureRatio;
		dcRect.bottom = pictureInClient_ltPoint.y + bottom * pictureRatio;

		CDC cdc; cdc.CreateCompatibleDC(pDC);

		CBitmap bitmap, *pOldBitmap;
		bitmap.CreateCompatibleBitmap(pDC, dcRect.right, dcRect.bottom);
		CRect src(dcRect);
		src.OffsetRect(CSize(-dcRect.left, -dcRect.top));
		pOldBitmap = cdc.SelectObject(&bitmap);
		cdc.FillSolidRect(src, RGB(255, 0, 255));

		if (::AlphaBlend == 0)
		{
			pDC->BitBlt(dcRect.left, dcRect.top, dcRect.right - dcRect.left, dcRect.bottom - dcRect.top, &cdc, src.left, src.top, SRCINVERT);
		}
		else{

			BLENDFUNCTION bf;
			memset(&bf, 0, sizeof(bf));
			bf.SourceConstantAlpha = 0x7f; //半透明
			bf.BlendOp = AC_SRC_OVER;

			::AlphaBlend(pDC->GetSafeHdc(), dcRect.left, dcRect.top, dcRect.right - dcRect.left, dcRect.bottom - dcRect.top,
				cdc.GetSafeHdc(), src.left, src.top, src.right - src.left, src.bottom - src.top, bf);
		}

		cdc.SelectObject(pOldBitmap);
	}
}
/*
*显示模块对比图片
*
*/
void CMCU_systemView::showBoardPicture(Mat mat1, Mat mat2)
{
	setPageTurningComponent();//设置翻页组件

	CDC* pDC = GetDC();
	HDC hDC = pDC->GetSafeHdc();

	IplImage *img = &IplImage(mat1);
	CvvImage cimg;
	cimg.CopyOf(img);

	CRect rect;
	GetClientRect(&rect);
	//获取图片的宽 高  
	int width = mat1.cols;
	int height = mat1.rows;
	int width_s = rect.right / 2 - 5;
	if (width_s * 1.0 / rect.bottom <= width * 1.0 / height)     //以长为主,调整大小
	{
		if (width != 0)
		{
			int realHeight = width_s * height / width;  //调整显示大小，不然显示超出边框
			rect.top = rect.bottom / 2 - realHeight / 2;
			rect.bottom = rect.top + realHeight;
			rect.right = width_s;
		}
	}
	else
	{
		if (height != 0)
		{
			int realwidth = width * rect.bottom / height;  //以宽为主，调整大小
			rect.left = width_s / 2 - realwidth / 2;
			rect.right = rect.left + realwidth;
		}
	}

	cimg.DrawToHDC(hDC, &rect);
	cimg.Destroy();

	IplImage *img2 = &IplImage(mat2);
	CvvImage cimg2;
	cimg2.CopyOf(img2);

	CRect rect2;
	GetClientRect(&rect2);
	//获取图片的宽 高  
	width = mat2.cols;
	height = mat2.rows;
	if (width_s * 1.0 / rect2.bottom <= width * 1.0 / height)     //以长为主,调整大小
	{
		if (width != 0)
		{
			int realHeight = width_s * height / width;  //调整显示大小，不然显示超出边框
			rect2.top = rect2.bottom / 2 - realHeight / 2;
			rect2.bottom = rect2.top + realHeight;
			rect2.left = width_s + 10;
		}
	}
	else
	{
		if (height != 0)
		{
			int realwidth = width * rect2.bottom / height;  //以宽为主，调整大小
			rect2.left = width_s / 2 - realwidth / 2 + width_s + 10;
			rect2.right = rect2.left + realwidth;
		}
	}
	cimg2.DrawToHDC(hDC, &rect2);
	cimg2.Destroy();
	ReleaseDC(pDC);

}

/*
#设置翻页组件#
*/
void CMCU_systemView::setPageTurningComponent()
{
	if (flag_showComponent)//显示翻页组件
	{
		CRect pictureControl;
		GetClientRect(&pictureControl);
		//大小位置
		m_btn_previous.MoveWindow(CRect(pictureControl.right / 2 - 122, pictureControl.bottom - 35, pictureControl.right / 2 - 42, pictureControl.bottom - 5));
		m_btn_next.MoveWindow(CRect(pictureControl.right / 2 + 42, pictureControl.bottom - 35, pictureControl.right / 2 + 122, pictureControl.bottom - 5));
		m_s_middle.MoveWindow(CRect(pictureControl.right / 2 - 40, pictureControl.bottom - 35, pictureControl.right / 2 + 40, pictureControl.bottom - 5));
	
		//设置静态文本
		stringstream ss;
		ss << (currentPage + 1);
		string s1 = ss.str();
		ss.str("");
		ss << openFilePath.imageNum;
		s1 = s1 + " / " + ss.str();
		m_s_middle.SetWindowTextA(_T(s1.c_str()));

		m_btn_previous.ShowWindow(SW_SHOW);
		m_btn_next.ShowWindow(SW_SHOW);
		m_s_middle.ShowWindow(SW_SHOW);
	}
	else {//隐藏翻页组件
		m_btn_previous.ShowWindow(SW_HIDE);
		m_btn_next.ShowWindow(SW_HIDE);
		m_s_middle.ShowWindow(SW_HIDE);
	}
}
/*
	#隐藏翻页组件#
*/
void CMCU_systemView::hideButtonCompo(){
	flag_showComponent = false;
	setPageTurningComponent();
}
/*
#设置listControl#
参数：
*/
void CMCU_systemView::setListControl()
{
	CRect clientRect;
	GetClientRect(&clientRect);
	int right = clientRect.right;
	m_list.MoveWindow(CRect(clientRect.right - 200, clientRect.top, clientRect.right, clientRect.bottom));


	m_list.ShowWindow(SW_SHOW);
}
/*
#上一页#
*/
void CMCU_systemView::OnButtonPreviousClick()
{
	if (currentPage == 0)
	{
		MessageBox(_T("已经是第一页"), _T("MCU系统"));
		return;
	}
	currentPage--;
	src_img = imread(openFilePath.imageFilePathNames[currentPage]);//上一张图片
	src_img.copyTo(show_img);
	CWnd::Invalidate(TRUE);//调用ondraw函数

	flag_showComponent = true;//显示翻页组件

	srcROI2d.x = 0;
	srcROI2d.y = 0;
	srcROI2d.width = show_img.cols;
	srcROI2d.height = show_img.rows;

	havePictuireView();//有图像界面

	//初始化队列
	editAlgorithm.setQueue_NULL();
}

/*
#下一页#
*/
void CMCU_systemView::OnButtonNextClick()
{
	if (currentPage == openFilePath.imageNum - 1)
	{
		MessageBox(_T("已经是最后一页"), _T("MCU系统"));
		return;
	}
	currentPage++;
	src_img = imread(openFilePath.imageFilePathNames[currentPage]);//下一张图片
	src_img.copyTo(show_img);
	CWnd::Invalidate(TRUE);//调用ondraw函数

	flag_showComponent = true;//显示翻页组件

	srcROI2d.x = 0;
	srcROI2d.y = 0;
	srcROI2d.width = show_img.cols;
	srcROI2d.height = show_img.rows;

	havePictuireView();//有图像界面

	//初始化队列
	editAlgorithm.setQueue_NULL();
}

void CMCU_systemView::OnButtonOpenfile_toolBar()
{
	// TODO:  在此添加命令处理程序代码
	OnFileOpen();
}




void CMCU_systemView::OnButtonSave_toolBar()
{
	// TODO:  在此添加命令处理程序代码
	OnFileSave();
}

/*
*撤销
*
*/
void CMCU_systemView::OnButtonRevoke()
{
	// TODO:  在此添加命令处理程序代码
	if (!revokeEnable)
	{
		editAlgorithm.revoke(src_img, srcROI2d, detectAlgorithm);
		setListControlContent();
		setRedoEnable(false);
		if (editAlgorithm.view_index == 1)//如果当前队列位置为1，撤销按钮失效
		{
			setRevokeEnable(true);
		}
		else
		{
			setRevokeEnable(false);
		}
		if (editAlgorithm.enlargeNum >= 5){//设置放大按钮
			setEnlargeEnable(true);
		}
		else{
			setEnlargeEnable(false);
		}
		if (editAlgorithm.narrowNum > 0){//设置缩小按钮
			setNarrowEnable(false);
		}
		else{
			setNarrowEnable(true);
		}
		show_img = src_img(srcROI2d);
		Invalidate(TRUE);//调用ondraw函数
	}
}

/*
*恢复
*
*/
void CMCU_systemView::OnButtonRedo()
{
	// TODO:  在此添加命令处理程序代码
	if (!redoEnable)
	{
		editAlgorithm.redo(src_img, srcROI2d, detectAlgorithm);
		setListControlContent();
		setRevokeEnable(false);
		if (editAlgorithm.view_index == editAlgorithm.max_view_index)//如果当前位置为队列最大位置，恢复按钮失效
		{
			setRedoEnable(true);
		}
		else
		{
			setRedoEnable(false);
		}
		if (editAlgorithm.enlargeNum >= 5){//设置放大按钮
			setEnlargeEnable(true);
		}
		else{
			setEnlargeEnable(false);
		}
		if (editAlgorithm.narrowNum > 0){//设置缩小按钮
			setNarrowEnable(false);
		}
		else{
			setNarrowEnable(true);
		}
		show_img = src_img(srcROI2d);
		Invalidate(TRUE);//调用ondraw函数
	}
}

/*
*开始继续按钮
*
*/
void CMCU_systemView::OnButtonStart()
{
	// TODO:  在此添加命令处理程序代码
	if (!startEnable)
	{
		if (!goonStitching)//开始
		{
			if (!openFilePath.imageFilePathNames.size()){
				MessageBox(_T("请输入正确的图片路径！"), _T("MCU系统"));
				return;
			}
			if (!flag_setPara)
			{
				MessageBox(_T("请先配置参数！"), _T("MCU系统"));
				return;
			}
			setPauseEnable(false);
			setStopEnable(false);

			//设置状态栏信息
			pFrame->m_wndStatusBar.SetPaneText(1, "正在进行图像拼接，请稍等", TRUE);
			m_editMessage.SetWindowTextA("");

			DWORD threadId;
			Mat o;
			o.copyTo(outImage);

			
			sumProcessTimes = (numOfRowImage - 1) * numOfColImage + (numOfColImage - 1);//进度条长度
			currentProcessingTimes = 0;//进度条当前位置

			pFrame = (CMainFrame *)AfxGetMainWnd();
			try
			{
				imageStitchingThread = CreateThread(NULL, 0, imageStitching, this, 0, &threadId); // 创建线程
			}
			catch (const std::exception& ex)
			{
				MessageBox(_T(format("创建拼接线程失败！%s", ex.what()).c_str()), _T("MCU系统"));
			}
			setStartEnable(true);
			return;
		}
		else if (goonStitching){//继续
			if (ResumeThread(this->imageStitchingThread) != 0xFFFFFFFF)
			{
				setStartEnable(true);
				setPauseEnable(false);
				pFrame->m_wndStatusBar.SetPaneText(1, "正在进行图像拼接，请稍等", TRUE);
			}
			else
			{
				MessageBox(_T("继续失败！"), _T("MCU系统"));
				return;
			}
		}
	}
	
	
}

/*
*图像拼接线程
*
*/
DWORD WINAPI imageStitching(LPVOID p)
{
	CMCU_systemView *para = (CMCU_systemView *)p;
	
	//设置进度条长度
	para->pFrame->m_wndStatusBar.setRange(para->sumProcessTimes);
	para->flag_rowOrCol = true;//正在拼接行
	for (int j = 1, h = 0; h < para->numOfColImage; j += para->numOfRowImage, h++)//列
	{
		para->currentTXPJCol = h + 1;
		para->leftImage = imread(para->openFilePath.imageFilePathNames[j - 1]);

		//判断图片是否为空
		if (para->leftImage.cols == 0)
		{
			para->flag_TXPJSucess = 3;
			::SendMessage(para->GetSafeHwnd(), WM_USER_TXPJ_MSG, WPARAM(0), LPARAM(0));//调用显示信息函数
			para->MessageBox(_T("拼接失败！"), _T("MCU系统"));
			::PostMessage(para->GetSafeHwnd(), WM_USER_STOP_MSG, WPARAM(0), LPARAM(0));
			return 0;
		}

		for (int i = j; i < j + para->numOfRowImage - 1; i++)//行
		{
			if (para->flag_isStopStitching)//停止拼接线程
			{
				//调用拼接停止结束处理函数
				::PostMessage(para->GetSafeHwnd(), WM_USER_STOP_MSG, WPARAM(0), LPARAM(0));
				return 0;
			}

			para->currentTXPJRow = i - j + 1;
			para->rightImage = imread(para->openFilePath.imageFilePathNames[i]);

			//判断图片是否为空
			if (para->rightImage.cols == 0)
			{
				para->flag_TXPJSucess = 2;
				::SendMessage(para->GetSafeHwnd(), WM_USER_TXPJ_MSG, WPARAM(0), LPARAM(0));//调用显示信息函数
				para->MessageBox(_T("拼接失败！"), _T("MCU系统"));
				::PostMessage(para->GetSafeHwnd(), WM_USER_STOP_MSG, WPARAM(0), LPARAM(0));
				return 0;
			}
			
			para->flag_TXPJSucess = para->txpjAlgorithm.imageStitching_rows(para->leftImage, para->rightImage, para->outImage_row[h], para->widthByImage);//行拼接
			para->outImage_row[h].copyTo(para->leftImage);
			::SendMessage(para->GetSafeHwnd(), WM_USER_TXPJ_MSG, WPARAM(0), LPARAM(0));//调用显示信息函数
			
			if (para->flag_TXPJSucess == 0)//如果拼接失败
			{
				para->MessageBox(_T("拼接失败！"), _T("MCU系统"));
				::PostMessage(para->GetSafeHwnd(), WM_USER_STOP_MSG, WPARAM(0), LPARAM(0));
				return 0;
			}
			para->currentProcessingTimes++; //进度条当前位置+1
			para->pFrame->m_wndStatusBar.OnProgress(para->currentProcessingTimes);//设置进度条当前位置
		}

	}

	if (para->numOfColImage == 1)//如果只有一行
	{
		para->outImage_row[0].copyTo(para->outImage);
	}

	para->outImage_row[0].copyTo(para->leftImage);
	para->flag_rowOrCol = false;//正在拼接列
	for (int i = 1; i < para->numOfColImage; i++)
	{
		para->outImage_row[i].copyTo(para->rightImage);
		para->currentTXPJCol = i;
		if (para->flag_isStopStitching)//停止拼接线程
		{
			//调用拼接停止结束处理函数
			::PostMessage(para->GetSafeHwnd(), WM_USER_STOP_MSG, WPARAM(0), LPARAM(0));
			return 0;
		}
		para->flag_TXPJSucess = para->txpjAlgorithm.imageStitching_cols2(para->leftImage, para->rightImage, para->outImage, para->widthByImage, i);//列拼接
		para->outImage.copyTo(para->leftImage);
		::SendMessage(para->GetSafeHwnd(), WM_USER_TXPJ_MSG, WPARAM(0), LPARAM(0));//调用显示信息函数
		if (para->flag_TXPJSucess == 0)//如果拼接失败
		{
			para->MessageBox(_T("拼接失败！"), _T("MCU系统"));
			::PostMessage(para->GetSafeHwnd(), WM_USER_STOP_MSG, WPARAM(0), LPARAM(0));
			return 0;
		}
		para->currentProcessingTimes++;//进度条当前位置+1
		para->pFrame->m_wndStatusBar.OnProgress(para->currentProcessingTimes);//设置进度条当前位置
	}

	::PostMessage(para->GetSafeHwnd(), WM_USER_MSG, WPARAM(0), LPARAM(0));//调用拼接正常结束处理函数
	return 0;
}
/*
*显示图像拼接信息
*
*/
LRESULT CMCU_systemView::showTXPJMessage(WPARAM wp, LPARAM lp)
{
	CString message = "";
	string sucess = "";
	if (flag_TXPJSucess == 1)
	{
		sucess = "成功！";
	}
	else if (flag_TXPJSucess == 0)
	{
		sucess = "失败!";
	}
	else if (flag_TXPJSucess == 2)
	{
		message.Format("第 %d 行的第 %d 张图片为空！！！\r\n", currentTXPJCol, currentTXPJRow + 1);
		int length = m_editMessage.GetWindowTextLength();
		m_editMessage.SetSel(length, length);
		m_editMessage.ReplaceSel(message);
		return 0;
	}
	else if (flag_TXPJSucess == 3)
	{
		message.Format("第 %d 行的第 %d 张图片为空！！！\r\n", currentTXPJCol, 1);
		int length = m_editMessage.GetWindowTextLength();
		m_editMessage.SetSel(length, length);
		m_editMessage.ReplaceSel(message);
		return 0;
	}

	if (flag_rowOrCol)//如果正在拼接行
	{
		message.Format("第 %d 行的第 %d 张和第 %d 张图片拼接 %s\r\n", currentTXPJCol, currentTXPJRow, currentTXPJRow + 1, sucess.c_str());
		int length = m_editMessage.GetWindowTextLength();
		m_editMessage.SetSel(length, length);
		m_editMessage.ReplaceSel(message);
	}
	else
	{
		message.Format("第 %d 行和第 %d 行图片拼接 %s\r\n", currentTXPJCol, currentTXPJCol + 1, sucess.c_str());
		int length = m_editMessage.GetWindowTextLength();
		m_editMessage.SetSel(length, length);
		m_editMessage.ReplaceSel(message);
	}
	return 0;
}
/*
*图像拼接线程正常结束
*
*/
LRESULT CMCU_systemView::imageStitchingOver(WPARAM wp, LPARAM lp)
{
	WaitForSingleObject(imageStitchingThread, INFINITE);//等待程序结束
	CloseHandle(imageStitchingThread);

	flag_compareBoard = false;//模块对比
	flag_showComponent = false;//不显示翻页组件

	if (outImage.empty()){
		Mat img;
		img.copyTo(show_img);
		Invalidate(TRUE);//调用ondraw函数
		initView();//初始化界面
	}
	else
	{
		outImage.copyTo(src_img);
		src_img.copyTo(show_img);
		Invalidate(TRUE);//调用ondraw函数
		
		srcROI2d.x = 0;
		srcROI2d.y = 0;
		srcROI2d.width = show_img.cols;
		srcROI2d.height = show_img.rows;
		havePictuireView();//有图像界面
	}

	//初始化Queue
	editAlgorithm.setQueue_NULL();

	pFrame->m_wndStatusBar.OnProgress(0);
	pFrame->m_wndStatusBar.SetPaneText(1, "拼接完成", TRUE);

	flag_isStopStitching = false;//停止拼接线程标志

	return 1L;
}
/*
*图像拼接线程结束
*
*/
LRESULT CMCU_systemView::imageStitchingStop(WPARAM wp, LPARAM lp)
{
	WaitForSingleObject(imageStitchingThread, INFINITE);//等待程序结束
	CloseHandle(imageStitchingThread);

	flag_compareBoard = false;//模块对比
	flag_showComponent = false;//不显示翻页组件

	if (outImage.empty()){
		Mat img;
		img.copyTo(show_img);
		Invalidate(TRUE);//调用ondraw函数
		initView();//初始化界面
	}
	else
	{
		outImage.copyTo(src_img);
		src_img.copyTo(show_img);
		Invalidate(TRUE);//调用ondraw函数
		
		srcROI2d.x = 0;
		srcROI2d.y = 0;
		srcROI2d.width = show_img.cols;
		srcROI2d.height = show_img.rows;
		havePictuireView();//有图像界面
	}

	//初始化Queue
	editAlgorithm.setQueue_NULL();

	pFrame->m_wndStatusBar.OnProgress(0);
	pFrame->m_wndStatusBar.SetPaneText(1, "拼接停止", TRUE);

	flag_isStopStitching = false;//停止拼接线程标志

	return 1L;
}

/*
*初始化界面
*
*/
void CMCU_systemView::initView()
{
	setStartEnable(false);
	setPauseEnable(true);
	setStopEnable(true);
	setRevokeEnable(true);
	setRedoEnable(true);
	setLeftRoateEnable(true);
	setRightRoateEnable(true);
	setEnlargeEnable(true);
	setNarrowEnable(true);
	setCutOutEnable(true);
	setAddtextEnable(true);
	setMakeSureEnable(true);
	setCancelEnable(true);
	setDetectStartEnable(false);
	setDetectStopEnable(true);
	editAlgorithm.enlargeNum = 0;
	editAlgorithm.narrowNum = 0;
	flag_cutButtonDown = false;
	flag_addEditButtonDown = false;
	goonStitching = false;
	flag_detectOver = false;
	flag_listFocus = false;
	flag_draw = false;//不画
}
/*
*有图像界面
*
*/
void CMCU_systemView::havePictuireView()
{
	setStartEnable(false);
	setPauseEnable(true);
	setStopEnable(true);
	setRevokeEnable(true);
	setRedoEnable(true);
	setLeftRoateEnable(false);
	setRightRoateEnable(false);
	setEnlargeEnable(false);
	setNarrowEnable(true);
	setCutOutEnable(false);
	setAddtextEnable(false);
	setMakeSureEnable(true);
	setCancelEnable(true);
	editAlgorithm.enlargeNum = 0;
	editAlgorithm.narrowNum = 0;
	flag_cutButtonDown = false;
	flag_addEditButtonDown = false;
	goonStitching = false;
	flag_detectOver = false;
	flag_listFocus = false; 
}
/*
*暂停
*
*/
void CMCU_systemView::OnButtonPause()
{
	// TODO:  在此添加命令处理程序代码
	if (!pauseEnable)
	{
		if (SuspendThread(this->imageStitchingThread) != 0xFFFFFFFF)
		{
			setStartEnable(false);
			goonStitching = true;
			setPauseEnable(true);
			pFrame->m_wndStatusBar.SetPaneText(1, "暂停", TRUE);
		}
		else
		{
			MessageBox(_T("暂停失败！"), _T("MCU系统"));
			return;
		}
	}
}


/*
*停止
*
*/
void CMCU_systemView::OnButtonStop()
{
	// TODO:  在此添加命令处理程序代码
	if (!stopEnable)
	{
		if (pauseEnable)//如果线程暂停了，先唤醒线程，再停止
		{
			if (ResumeThread(this->imageStitchingThread) != 0xFFFFFFFF)
			{
				setStopEnable(true);
				setPauseEnable(true);
				flag_isStopStitching = true;//停止线程标志
				pFrame->m_wndStatusBar.SetPaneText(1, "正在停止拼接", TRUE);
			}
			else
			{
				MessageBox(_T("停止失败！"), _T("MCU系统"));
				return;
			}
		}
		else//否则直接停止线程
		{
			setStopEnable(true);
			setPauseEnable(true);
			flag_isStopStitching = true;//停止线程标志
			pFrame->m_wndStatusBar.SetPaneText(1, "正在停止拼接", TRUE);
		}
	}
}




/*
*左旋
*
*/
void CMCU_systemView::OnButtonLeftroate()
{
	// TODO:  在此添加命令处理程序代码
	//确定按钮no
	if (!leftRoateEnable)
	{
		setMakeSureEnable(true);
		setRevokeEnable(false);
		setRedoEnable(true);

		editAlgorithm.leftRotate(src_img, srcROI2d, detectAlgorithm);
		

		show_img = src_img(srcROI2d);
		Invalidate(TRUE);//调用ondraw函数
	}
	
}



/*
*右旋
*
*/
void CMCU_systemView::OnButtonRightroate()
{
	// TODO:  在此添加命令处理程序代码
	//确定按钮no
	if (!rightRoateEnable)
	{
		setMakeSureEnable(true);
		setRevokeEnable(false);
		setRedoEnable(true);

		editAlgorithm.rightRotate(src_img, srcROI2d, detectAlgorithm);

		show_img = src_img(srcROI2d);
		Invalidate(TRUE);//调用ondraw函数
	}
}


/*
*放大
*
*/
void CMCU_systemView::OnButtonEnlarge()
{
	// TODO:  在此添加命令处理程序代码
	if (!enlargeEnable)
	{
		if (editAlgorithm.enlargeNum < 5){//最多放大5次
			//确定按钮no
			setMakeSureEnable(true);

			//放大
			editAlgorithm.enlargeImg(src_img, ROI2d, srcROI2d, detectAlgorithm);
			show_img = src_img(ROI2d);

			showPicture(show_img);

			if (editAlgorithm.enlargeNum >= 5){
				setEnlargeEnable(true);
			}
			else{
				setEnlargeEnable(false);
			}
			setNarrowEnable(false);
			setRevokeEnable(false);
			setRedoEnable(true);


			//设置listControl列表
			setListControlCheckBox();
		}
	}
	
}


/*
*缩小
*
*/
void CMCU_systemView::OnButtonNarrow()
{
	// TODO:  在此添加命令处理程序代码
	if (!narrowEnable)
	{
		if (editAlgorithm.narrowNum > 0){
			//确定按钮no
			setMakeSureEnable(true);

			editAlgorithm.narrowImg(src_img, ROI2d, srcROI2d, detectAlgorithm);
			show_img = src_img(ROI2d);

			showPicture(show_img);

			if (editAlgorithm.narrowNum > 0){
				setNarrowEnable(false);
			}
			else{
				setNarrowEnable(true);
			}
			setEnlargeEnable(false);
			setRevokeEnable(false);
			setRedoEnable(true);


			//设置listControl列表
			setListControlCheckBox();
		}
	}
	
}


/*
*裁剪
*
*/
void CMCU_systemView::OnButtonCutout()
{
	// TODO:  在此添加命令处理程序代码
	if (!cutOutEnable)
	{
		flag_cutButtonDown = true;
		setCutOutEnable(true);
		setCancelEnable(false);
		//添加文本按钮
		setAddtextEnable(true);
	}
}


/*
*添加文本
*
*/
void CMCU_systemView::OnButtonAddtext()
{
	// TODO:  在此添加命令处理程序代码
	if (!addTextEnable)
	{
		flag_addEditButtonDown = true;
		//取消按钮yes
		setCancelEnable(false);
		setAddtextEnable(true);
		//裁剪按钮no
		setCutOutEnable(true);
	}
}

/*
*双击添加文本框
*
*/
void CMCU_systemView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (flag_addEditButtonDown
		&& point.x > pictureInClient_ltPoint.x && point.y > pictureInClient_ltPoint.y
		&& point.x < pictureInClient_rbPoint.x && point.y < pictureInClient_rbPoint.y){//添加文本框，约束在图片内
		flag_addEditButtonDown = false;
		p_newEdit = new CEdit();
		//创建文本框
		p_newEdit->Create(WS_CHILD | WS_VISIBLE | WS_BORDER | SS_LEFT | WS_EX_TOOLWINDOW,
			CRect(point.x, point.y, point.x + 80, point.y + 30), this, IDC_EDIT_NEW);
		//文本在图片中的位置
		point_textInPicture = Point((point.x - pictureInClient_ltPoint.x) / pictureRatio + srcROI2d.x, (point.y - pictureInClient_ltPoint.y) / pictureRatio + srcROI2d.y);
	}
	//双击矩形框
	else if (point.x > pictureInClient_ltPoint.x && point.y > pictureInClient_ltPoint.y
		&& point.x < pictureInClient_rbPoint.x && point.y < pictureInClient_rbPoint.y)
	{
		for (int i = 0; i < detectAlgorithm.instanceRect_vector.size(); i++)
		{
			int left = detectAlgorithm.instanceRect_vector[i].left_top.x * pictureRatio + pictureInClient_ltPoint.x;
			int right = detectAlgorithm.instanceRect_vector[i].right_bottom.x * pictureRatio + pictureInClient_ltPoint.x;
			int top = detectAlgorithm.instanceRect_vector[i].left_top.y * pictureRatio + pictureInClient_ltPoint.y;
			int bottom = detectAlgorithm.instanceRect_vector[i].right_bottom.y * pictureRatio + pictureInClient_ltPoint.y;
			//如果当前矩形框标记，并且鼠标坐标在矩形框内
			if (detectAlgorithm.instanceRect_vector[i].flag_noMark == false
				&& point.x > left
				&& point.x < right
				&& point.y > top
				&& point.y < bottom)
			{
				currentRowInList = i;//当前行
				flag_listFocus = true;
				UpdateWindow();
				showPicture(show_img);
				//设置高亮显示
				m_list.SetFocus();//设置焦点
				m_list.SetItemState(currentRowInList, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);//设置状态
				m_list.EnsureVisible(currentRowInList, FALSE);//设置当前视图可见
				break;
			}
		}
	}
	else if (point.x < pictureInClient_ltPoint.x || point.y < pictureInClient_ltPoint.y
		|| point.x > pictureInClient_rbPoint.x || point.y > pictureInClient_rbPoint.y)
	{
		flag_listFocus = false;
		UpdateWindow();
		showPicture(show_img);
		setListControlContent();
		currentRowInList = -1;// 当前行
		setFillRedEnable(false);
		//if (currentRowInList != -1)
		//{
		//	m_list.SetItemState(m_list.SetSelectionMark(currentRowInList), 0, LVIS_SELECTED);
		//	currentRowInList = -1;// 当前行
		//}
		
	}
	CView::OnLButtonDblClk(nFlags, point);
} 


/*
*文本框enter事件
*
*/
BOOL CMCU_systemView::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  在此添加专用代码和/或调用基类
	if (WM_KEYDOWN == pMsg->message && VK_RETURN == pMsg->wParam)
	{
		if (GetFocus() == GetDlgItem(IDC_EDIT_NEW)) //根据不同控件焦点判断是那个在执行 
		{
			flag_addEditButtonDown = true;
			// 获取文本内容
			CString csX;
			GetDlgItem(IDC_EDIT_NEW)->GetWindowText(csX);
			if (!csX.IsEmpty()){//输入文本内容不为空，向图片中输入文字
				USES_CONVERSION;
				char* p = T2A(csX.GetBuffer(0));
				csX.ReleaseBuffer();
				//向图中写入文字
				editAlgorithm.paDrawString(src_img, srcROI2d, p, point_textInPicture, Scalar(255, 0, 0), wordSize, true, true, detectAlgorithm);
			}
			p_newEdit->DestroyWindow();
			p_newEdit = NULL;
			delete p_newEdit;//删除新建的文本框

			setRevokeEnable(false);
			setRedoEnable(true);

			show_img = src_img(srcROI2d);
			Invalidate(TRUE);//调用ondraw函数
			return true;
		}
	}
	if (pMsg->message == WM_KEYDOWN && GetAsyncKeyState(VK_CONTROL))
	{

	}
	return CView::PreTranslateMessage(pMsg);
}

/*
*鼠标左键按下
*
*/
void CMCU_systemView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (point.x > pictureInClient_ltPoint.x && point.y > pictureInClient_ltPoint.y
		&& point.x < pictureInClient_rbPoint.x && point.y < pictureInClient_rbPoint.y){//约束鼠标在图片内
		m_staticMessage.SetFocus();
		flag_LeftButtonDown = true;
		sourcePoint2d.x = srcROI2d.x;
		sourcePoint2d.y = srcROI2d.y;
		leftBtnDown_lt.clear();
		leftBtnDown_rb.clear();
		for (int i = 0; i < detectAlgorithm.instanceRect_vector.size(); i++)
		{
			leftBtnDown_lt.push_back(Point(detectAlgorithm.instanceRect_vector[i].left_top));
			leftBtnDown_rb.push_back(Point(detectAlgorithm.instanceRect_vector[i].right_bottom));
		}
		lButtonDown_point.x = point.x;
		lButtonDown_point.y = point.y;
	}
	CView::OnLButtonDown(nFlags, point);
}


/*
*鼠标左键抬起
*
*/
void CMCU_systemView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	flag_LeftButtonDown = false; 
	if (flag_addDetect)//如果添加元器件
	{
		INT_PTR nResponse = adddetect.DoModal();
		if (nResponse == IDOK)
		{
			// TODO:  在此放置处理何时用
			//  “确定”来关闭对话框的代码
			//添加手工识别器件
			InstanceRect instanceRect;
			instanceRect.left_top.x = (lButtonDown_point.x - pictureInClient_ltPoint.x) / pictureRatio;
			instanceRect.left_top.y = (lButtonDown_point.y - pictureInClient_ltPoint.y) / pictureRatio;
			instanceRect.right_bottom.x = (point.x - pictureInClient_ltPoint.x) / pictureRatio;
			instanceRect.right_bottom.y = (point.y - pictureInClient_ltPoint.y) / pictureRatio;
			instanceRect.manaulAdd = 1;//人工添加
			instanceRect.instanceClass = adddetect.currentIndex + 1;//类别名称对应的下标
			detectAlgorithm.numOfClassName[instanceRect.instanceClass - 1]++;//类别名称数量加一
			instanceRect.currentIndex = detectAlgorithm.numOfClassName[instanceRect.instanceClass - 1];//当前器件的下标
			CString instanceMark = "";
			instanceMark.Format("%s%d", adddetect.instanceClass, instanceRect.currentIndex);//序号
			instanceRect.instanceWord = adddetect.instanceWord;//类型
			detectAlgorithm.instanceRect_vector.push_back(instanceRect);
			detectAlgorithm.drawInstanceClassString(src_img, instanceMark, instanceRect.left_top, Scalar(255, 0, 0), wordSize, false, false);//调用画类别文字函数
			setListControlContent();//设置元器件列表
			show_img = src_img(srcROI2d);
			showPicture(show_img);
		}
		else
		{
			showPicture(show_img);
		}
	}

	CView::OnLButtonUp(nFlags, point);
}


/*
*鼠标移动
*
*/
void CMCU_systemView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	//设置鼠标样式
	if (flag_isStopStitching)
	{
		::SetCursor(wait_cursor);
	}
	else
	{
		::SetCursor(arrow_cursor);
	}
	if (flag_LeftButtonDown && !flag_cutButtonDown && !flag_addDetect&& editAlgorithm.enlargeNum > 0){//拖拽
		editAlgorithm.draftingImg(src_img, srcROI2d, sourcePoint2d, lButtonDown_point, point, detectAlgorithm, leftBtnDown_lt, leftBtnDown_rb);//拖拽图片
		show_img = src_img(srcROI2d);

		showPicture(show_img);

		//设置listControl列表
		setListControlCheckBox();
	}
	else if (flag_LeftButtonDown && flag_cutButtonDown && !flag_addDetect
		&& point.x > pictureInClient_ltPoint.x && point.y > pictureInClient_ltPoint.y
		&& point.x < pictureInClient_rbPoint.x && point.y < pictureInClient_rbPoint.y){//裁剪,约束鼠标在图片内
		
		Mat img;
		editAlgorithm.drawCutRect(src_img, img, srcROI2d, cutROI2d, lButtonDown_point, pictureInClient_ltPoint, point, lineThickness, pictureRatio);//画裁剪矩阵

		img = img(srcROI2d);
		showPicture(img);

		//确定按钮yes
		setMakeSureEnable(false);
	}
	else if (flag_LeftButtonDown && !flag_cutButtonDown && flag_addDetect
		&& point.x > pictureInClient_ltPoint.x && point.y > pictureInClient_ltPoint.y
		&& point.x < pictureInClient_rbPoint.x && point.y < pictureInClient_rbPoint.y)//添加识别元器件
	{
		showPicture(show_img);
		detectAlgorithm.drawAddDetect(this, Point(pictureInClient_ltPoint.x, pictureInClient_ltPoint.y),
			Point(pictureInClient_rbPoint.x, pictureInClient_rbPoint.y),
			Point(lButtonDown_point.x, lButtonDown_point.y), point, pictureRatio);//添加识别器件
	
	}
	CView::OnMouseMove(nFlags, point);
}

/*
*确定按钮
*
*/
void CMCU_systemView::OnButtonMakesure()
{
	// TODO:  在此添加命令处理程序代码
	if (!makeSureEnable)
	{
		if (flag_cutButtonDown){//裁剪图片
			editAlgorithm.cutImg(src_img, show_img, srcROI2d, cutROI2d, detectAlgorithm);//裁剪图片
			
			//放大缩小按钮还原
			setEnlargeEnable(false);
			setNarrowEnable(true);
			setRevokeEnable(false);
			setRedoEnable(true);

			Invalidate(TRUE);//调用ondraw函数
			setListControlContent();
			//确定按钮no
			setMakeSureEnable(true);
		}
		if (flag_detectOver){//图像检测结束
			
			//将矩形画到图片中
			for (int i = 0; i < detectAlgorithm.instanceRect_vector.size(); i++)
			{
				if (!detectAlgorithm.instanceRect_vector[i].flag_noMark)
				{
					detectAlgorithm.drawRect(src_img, detectAlgorithm.instanceRect_vector[i].left_top, detectAlgorithm.instanceRect_vector[i].right_bottom,detectAlgorithm.instanceRect_vector[i].manaulAdd);//画矩形
				}
			}
			havePictuireView();//有图像界面
			flag_draw = false;//不画
			m_list.DeleteAllItems();//清空列表
			UpdateWindow();
			showPicture(src_img);
		}
	}
}


/*
*取消按钮
*
*/
void CMCU_systemView::OnButtonCancel()
{
	// TODO:  在此添加命令处理程序代码
	if (!cancelEnable)
	{
		if (flag_cutButtonDown){//取消裁剪
			flag_cutButtonDown = false;

			//裁剪 添加文本 确定 取消按钮还原
			setCutOutEnable(false); 
			setMakeSureEnable(true);
			setCancelEnable(true);
			setAddtextEnable(false);

			show_img = src_img(srcROI2d);
			showPicture(show_img);
		}
		else if (flag_addEditButtonDown)//取消添加文本
		{
			flag_addEditButtonDown = false;
			//裁剪 添加文本 取消按钮还原
			setCutOutEnable(false);
			setCancelEnable(true);
			setAddtextEnable(false);

			if (p_newEdit){
				p_newEdit->DestroyWindow();
				p_newEdit = NULL;
				delete p_newEdit;//删除新建的文本框
				showPicture(src_img);
			}
		}
		else if (flag_addDetect)//取消添加识别器件
		{
			flag_addDetect = false;

			//裁剪 添加文本 确定 取消按钮还原
			setAddEnable(false);
			setMakeSureEnable(true);
			setCancelEnable(true);
			setAddtextEnable(false);
		}
	}
}

/*
*检测目标按钮
*
*/
void CMCU_systemView::OnDetect()
{
	// TODO:  在此添加命令处理程序代码
	if (!detectStartEnable)
	{
		//加载类别库XML
		int code = addDetectClassHouse.loadXML();
		if (code == 0)
		{
			MessageBox("加载检测器件类别库失败!!!", "MCU系统");
			return;
		}
		else if (code == 1)
		{
			MessageBox("检测器件类别库为空!!!", "MCU系统");
			return;
		}

		adddetect.className.clear();
		detectAlgorithm.numOfClassName.clear();
		adddetect.className = addDetectClassHouse.className;//赋值
		detectAlgorithm.numOfClassName = addDetectClassHouse.numOfClassName;
		

		pFrame = (CMainFrame *)AfxGetMainWnd();
		pFrame->m_wndStatusBar.SetPaneText(1, "正在进行图像检测，请稍等", TRUE);//设置状态栏

		imwrite((LPCTSTR)"python//images//detectImg.png", src_img);

		imageDetectThread = CreateThread(NULL, 0, imageDetect, this, 0, NULL);

		setDetectStartEnable(true);
		setDetectStopEnable(false);

		//隐藏翻页组件
		hideButtonCompo();

		setStartCompareEnable(true);//开始对比按钮
		setPauseCompareEnable(true);//暂停对比按钮
		setStopCompareEnable(true);//停止对比按钮
	}
}
/*
*图像检测线程
*
*/
DWORD WINAPI imageDetect(LPVOID p)
{
	CMCU_systemView *para = (CMCU_systemView *)p;

	//元器件检测
	para->detectAlgorithm.detect_bat();

	//画出元器件框
	int returnCode = para->detectAlgorithm.drawDetectInstances(para->src_img, para->addDetectClassHouse.className, para->wordSize);
	if (returnCode == 1)
	{
		para->MessageBox("检测类别库数量不一致，请重新确认！！！", "MCU系统");
		//设置状态栏信息
		::PostMessage(para->GetSafeHwnd(), WM_USER_DETECT_STOP_MSG, WPARAM(0), LPARAM(0));
		return 0;
	}

	if (para->flag_isStopDetect)//停止检测
	{
		//设置状态栏信息
		para->detectAlgorithm.instanceRect_vector.clear();//清空元器件信息
		para->currentRowInList = -1;
		::PostMessage(para->GetSafeHwnd(), WM_USER_DETECT_STOP_MSG, WPARAM(0), LPARAM(0));
		return 0;
	}
	//设置状态栏信息
	::PostMessage(para->GetSafeHwnd(), WM_USER_DETECT_MSG, WPARAM(0), LPARAM(0));

	return 0;
}

/*
*图像检测线程结束线程
*
*/
LRESULT CMCU_systemView::imageDetectOver(WPARAM wp, LPARAM lp)
{
	WaitForSingleObject(imageDetectThread, INFINITE);//等待程序结束
	CloseHandle(imageDetectThread);

	flag_compareBoard = false;//模块对比
	flag_showComponent = false;//不显示翻页组件

	setDetectStartEnable(false);
	setDetectStopEnable(true);
	setAddEnable(false);//添加元器件按钮可按
	setFillRedEnable(false);//一键标红按钮可按

	flag_draw = true;//画

	//初始化队列
	editAlgorithm.setQueue_NULL();

	//显示检测图片
	src_img.copyTo(show_img);
	Invalidate(TRUE);//调用ondraw函数
	srcROI2d.x = 0;
	srcROI2d.y = 0;
	srcROI2d.width = show_img.cols;
	srcROI2d.height = show_img.rows;

	setListControlContent();//设置元器件列表内容

	pFrame->m_wndStatusBar.OnProgress(0);
	pFrame->m_wndStatusBar.SetPaneText(1, "检测完成", TRUE);
	return 1L;
}
/*
*图像检测线程结束线程
*
*/
LRESULT CMCU_systemView::imageDetectStopOver(WPARAM wp, LPARAM lp)
{
	WaitForSingleObject(imageDetectThread, INFINITE);//等待程序结束
	CloseHandle(imageDetectThread);

	flag_compareBoard = false;//模块对比
	flag_showComponent = false;//不显示翻页组件

	setDetectStartEnable(false);
	setDetectStopEnable(true);
	detectAlgorithm.instanceRect_vector.clear();
	currentRowInList = -1;
	flag_draw = false;//不画

	//初始化队列
	editAlgorithm.setQueue_NULL();

	pFrame->m_wndStatusBar.OnProgress(0);
	pFrame->m_wndStatusBar.SetPaneText(1, "已停止", TRUE);

	flag_isStopDetect = false;

	return 1L;
}
/*
	停止检测
*/
void CMCU_systemView::OnDetectstop()
{
	// TODO:  在此添加命令处理程序代码
	if (!detectStopEnable)
	{
		flag_isStopDetect = true;
		detectAlgorithm.kill_detect();
	}
	
}

/*
生成bom
*/
void CMCU_systemView::OnSaveasbom()
{
	// TODO:  在此添加命令处理程序代码
	CString FilePath;
	TCHAR szFilter[] = _T("图片(*.xlsx;*.xls)|*.xlsx;*.xls;|所有文件(*.*)|*.*||");
	CFileDialog dlg(FALSE, _T("xlsx"), "", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
	if (dlg.DoModal() == IDOK)
	{
		FilePath = dlg.GetPathName();
		Excel excel;
		bool flag = false;
		excel.write_to_excel((LPCTSTR)FilePath, adddetect.className, detectAlgorithm, boardWidth, boardHeight);
	}
}

/*
	#设置listControl的内容#

*/
void CMCU_systemView::setListControlContent()
{
	m_list.DeleteAllItems();//清空列表
	int listLength = detectAlgorithm.instanceRect_vector.size();//列表元器件的数量
	CString instanceString = "";
	for (int i = 0; i < listLength; i++)
	{
		instanceString.Format("%s%d", adddetect.className[detectAlgorithm.instanceRect_vector[i].instanceClass - 1], detectAlgorithm.instanceRect_vector[i].currentIndex);
		m_list.InsertItem(i, instanceString);
		instanceString = adddetect.className[detectAlgorithm.instanceRect_vector[i].instanceClass - 1];
		m_list.SetItemText(i, 1, instanceString);
		instanceString = detectAlgorithm.instanceRect_vector[i].instanceWord.c_str();
		m_list.SetItemText(i, 2, instanceString);

		int left = detectAlgorithm.instanceRect_vector[i].left_top.x;
		int right = detectAlgorithm.instanceRect_vector[i].right_bottom.x;
		int top = detectAlgorithm.instanceRect_vector[i].left_top.y;
		int bottom = detectAlgorithm.instanceRect_vector[i].right_bottom.y;
		if (detectAlgorithm.instanceRect_vector[i].flag_noMark == false && left >= 0 && top >= 0 && right < srcROI2d.width && bottom < srcROI2d.height)
		{
			m_list.SetCheck(i, 1);//设置CheckBox
		}
		else
		{
			m_list.SetCheck(i, 0);//设置CheckBox
		}
		
	}
}
/*
	#设置listCtrol的CheckBox#
*/
void CMCU_systemView::setListControlCheckBox()
{
	int listLength = detectAlgorithm.instanceRect_vector.size();//列表元器件的数量
	for (int i = 0; i < listLength; i++)
	{
		int left = detectAlgorithm.instanceRect_vector[i].left_top.x;
		int right = detectAlgorithm.instanceRect_vector[i].right_bottom.x;
		int top = detectAlgorithm.instanceRect_vector[i].left_top.y;
		int bottom = detectAlgorithm.instanceRect_vector[i].right_bottom.y;
		if (detectAlgorithm.instanceRect_vector[i].flag_noMark == false && left >= 0 && top >= 0 && right < srcROI2d.width && bottom < srcROI2d.height)
		{
			m_list.SetCheck(i, 1);//设置CheckBox
		}
		else
		{
			m_list.SetCheck(i, 0);//设置CheckBox
		}

	}
}
/*
	#listControl消息响应事件#
	item切换焦点时(包括用键盘和鼠标切换item时)，状态的一些变化顺序
	添加listctrl控件的LVN_ITEMCHANGED消息相应函数
*/
void CMCU_systemView::OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	CDC* pDC = GetDC();

	if ((pNMListView->uOldState & LVIS_FOCUSED) == LVIS_FOCUSED &&
		(pNMListView->uNewState & LVIS_FOCUSED) == 0)
	{
		
	}
	else if ((pNMListView->uOldState & LVIS_FOCUSED) == 0 &&
		(pNMListView->uNewState & LVIS_FOCUSED) == LVIS_FOCUSED)
	{
		flag_listFocus = true;
		currentRowInList = pNMListView->iItem;
		setFillRedEnable(false);
		UpdateWindow();
		showPicture(show_img);
	}

	if ((pNMListView->uOldState & LVIS_SELECTED) == LVIS_SELECTED &&
		(pNMListView->uNewState & LVIS_SELECTED) == 0)
	{
		
	}
	else if ((pNMListView->uOldState & LVIS_SELECTED) == 0 &&
		(pNMListView->uNewState & LVIS_SELECTED) == LVIS_SELECTED)
	{
		/*flag_listFocus = true;
		currentRowInList = pNMListView->iItem;
		setFillRedEnable(false);
		setMessageListControlContent();
		UpdateWindow();
		showPicture(show_img);*/
	}

	*pResult = 0;
	ReleaseDC(pDC);
}
/*
	#listControl中CheckBox消息#
*/
void CMCU_systemView::OnNMClickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	DWORD dwPos = GetMessagePos();
	CPoint pt(dwPos);
	m_list.ScreenToClient(&pt);

	UINT nFlag;
	int nItem = m_list.HitTest(pt, &nFlag);
	if (nFlag == LVHT_ONITEMSTATEICON)
	{
		BOOL bCheckState = m_list.GetCheck(nItem);

		//注意，bCheckState为TRUE，checkbox从勾选状态变为未勾选状态
		if (detectAlgorithm.instanceRect_vector.size() > 0)
		{
			if (bCheckState)
			{
				detectAlgorithm.instanceRect_vector[nItem].flag_noMark = true;//不标记当前元器件
				UpdateWindow();
				showPicture(show_img);
				//...............
			}
			else
			{
				detectAlgorithm.instanceRect_vector[nItem].flag_noMark = false;//标记当前元器件
				UpdateWindow();
				showPicture(show_img);
				//...............
			}
		}
		
	}

	*pResult = 0;
}
/*
统计Pin数
*/
void CMCU_systemView::OnStatisticspin()
{
	// TODO:  在此添加命令处理程序代码
	statisticsTip.statisticsPin(detectAlgorithm);
}


/*
*设置参数
*
*/
void CMCU_systemView::OnSetpara()
{
	// TODO:  在此添加命令处理程序代码
	INT_PTR nResponse = setupParaDlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO:  在此放置处理何时用
		//  “确定”来关闭对话框的代码
		numOfRowImage = setupParaDlg.numOfRowImage;
		numOfColImage = setupParaDlg.numOfColImage;
		numOfImage = setupParaDlg.numOfImage;
		widthByImage = setupParaDlg.widthByImage;
		heightByImage = setupParaDlg.heightByImage;
		flag_setPara = true;//参数配置
	}
}

/*
	#设置模块尺寸#
*/
void CMCU_systemView::OnSetboardsize()
{
	// TODO:  在此添加命令处理程序代码
	INT_PTR nResponse = setBoardSize.DoModal();
	if (nResponse == IDOK)
	{
		// TODO:  在此放置处理何时用
		//  “确定”来关闭对话框的代码
		boardWidth = setBoardSize.boardWidth;
		boardHeight = setBoardSize.boardHeight;
	}
}

/*
	#模块对比选择图片#
*/
void CMCU_systemView::OnCompareSelectimgs()
{
	// TODO:  在此添加命令处理程序代码
	INT_PTR nResponse = compareSelectImgs.DoModal();
	if (nResponse == IDOK)
	{
		// TODO:  在此放置处理何时用
		//  “确定”来关闭对话框的代码
		flag_compareBoard = true;//模块对比
		flag_showComponent = false;//隐藏翻页组件

		setStartCompareEnable(false);//开始对比按钮
		setPauseCompareEnable(true);//暂停对比按钮
		setStopCompareEnable(true);//停止对比按钮

		boardImg1 = imread(compareSelectImgs.mFilePath1.GetBuffer());//正面图
		boardImg2 = imread(compareSelectImgs.mFilePath2.GetBuffer());//反面图
		resize(boardImg2, boardImg2, Size(boardImg1.cols, boardImg1.rows));
		flip(boardImg2, boardImg2, 0);
		Invalidate(TRUE);//调用ondraw函数
		initView();//初始化界面
	}
}

/*
	#开始对比按钮#
*/
void CMCU_systemView::OnCompareStart()
{
	// TODO:  在此添加命令处理程序代
	string excelPath = compareSelectImgs.mFilePath3.GetBuffer();
	compareModel.startCompareModel(boardImg2, excelPath);
	Invalidate(TRUE);
}

/*
	#暂停对比按钮#
*/
void CMCU_systemView::OnComparePause()
{
	// TODO:  在此添加命令处理程序代码
}

/*
	#停止对比按钮#
*/
void CMCU_systemView::OnCompareStop()
{
	// TODO:  在此添加命令处理程序代码
}


/*
*更新按钮状态
*
*/
void CMCU_systemView::OnUpdateButtonStart(CCmdUI *pCmdUI)
{
	// TODO:  在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(startEnable);
}
void CMCU_systemView::setStartEnable(bool enable)
{
	startEnable = enable;
}

void CMCU_systemView::OnUpdateButtonPause(CCmdUI *pCmdUI)
{
	// TODO:  在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(pauseEnable);
}

void CMCU_systemView::setPauseEnable(bool enable)
{
	pauseEnable = enable;
}


void CMCU_systemView::OnUpdateButtonStop(CCmdUI *pCmdUI)
{
	// TODO:  在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(stopEnable);
}

void CMCU_systemView::setStopEnable(bool enable)
{
	stopEnable = enable;
}

void CMCU_systemView::OnUpdateButtonEnlarge(CCmdUI *pCmdUI)
{
	// TODO:  在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(enlargeEnable);
}

void CMCU_systemView::setEnlargeEnable(bool enable)
{
	enlargeEnable = enable;
}

void CMCU_systemView::OnUpdateButtonNarrow(CCmdUI *pCmdUI)
{
	// TODO:  在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(narrowEnable);
}

void CMCU_systemView::setNarrowEnable(bool enable)
{
	narrowEnable = enable;
}

void CMCU_systemView::OnUpdateButtonCutout(CCmdUI *pCmdUI)
{
	// TODO:  在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(cutOutEnable);
}

void CMCU_systemView::setCutOutEnable(bool enable)
{
	cutOutEnable = enable;
}

void CMCU_systemView::OnUpdateButtonMakesure(CCmdUI *pCmdUI)
{
	// TODO:  在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(makeSureEnable);
}
void CMCU_systemView::setMakeSureEnable(bool enable)
{
	makeSureEnable = enable;
}

void CMCU_systemView::OnUpdateButtonCancel(CCmdUI *pCmdUI)
{
	// TODO:  在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(cancelEnable);
}
void CMCU_systemView::setCancelEnable(bool enable)
{
	cancelEnable = enable;
}

void CMCU_systemView::OnUpdateButtonAddtext(CCmdUI *pCmdUI)
{
	// TODO:  在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(addTextEnable);
}
void CMCU_systemView::setAddtextEnable(bool enable)
{
	addTextEnable = enable;
}


void CMCU_systemView::OnUpdateButtonLeftroate(CCmdUI *pCmdUI)
{
	// TODO:  在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(leftRoateEnable);
}
void CMCU_systemView::setLeftRoateEnable(bool enable)
{
	leftRoateEnable = enable;
}

void CMCU_systemView::OnUpdateButtonRightroate(CCmdUI *pCmdUI)
{
	// TODO:  在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(rightRoateEnable);
}
void CMCU_systemView::setRightRoateEnable(bool enable)
{
	rightRoateEnable = enable;
}

void CMCU_systemView::OnUpdateDetect(CCmdUI *pCmdUI)
{
	// TODO:  在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(detectStartEnable);
}
void CMCU_systemView::setDetectStartEnable(bool enable)
{
	detectStartEnable = enable;
}

void CMCU_systemView::OnUpdateDetectstop(CCmdUI *pCmdUI)
{
	// TODO:  在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(detectStopEnable);
}
void CMCU_systemView::setDetectStopEnable(bool enable)
{
	detectStopEnable = enable;
}

void CMCU_systemView::OnUpdateButtonRevoke(CCmdUI *pCmdUI)
{
	// TODO:  在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(revokeEnable);
}
void CMCU_systemView::setRevokeEnable(bool enable)
{
	revokeEnable = enable;
}

void CMCU_systemView::OnUpdateButtonRedo(CCmdUI *pCmdUI)
{
	// TODO:  在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(redoEnable);
}
void CMCU_systemView::setRedoEnable(bool enable)
{
	redoEnable = enable;
}

void CMCU_systemView::OnUpdateCompareStart(CCmdUI *pCmdUI)
{
	// TODO:  在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(startCompareEnable);
}
void CMCU_systemView::setStartCompareEnable(bool enable)
{
	startCompareEnable = enable;
}

void CMCU_systemView::OnUpdateComparePause(CCmdUI *pCmdUI)
{
	// TODO:  在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(pauseCompareEnable);
}
void CMCU_systemView::setPauseCompareEnable(bool enable)
{
	pauseCompareEnable = enable;
}

void CMCU_systemView::OnUpdateCompareStop(CCmdUI *pCmdUI)
{
	// TODO:  在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(stopCompareEnable);
}
void CMCU_systemView::setStopCompareEnable(bool enable)
{
	stopCompareEnable = enable;
}

void CMCU_systemView::OnUpdateButtonAdd(CCmdUI *pCmdUI)
{
	// TODO:  在此添加命令更新用户界面处理程序代码、
	pCmdUI->SetCheck(addEnable);
}
void CMCU_systemView::setAddEnable(bool enable)
{
	addEnable = enable;
}

void CMCU_systemView::OnUpdateButtonFillred(CCmdUI *pCmdUI)
{
	// TODO:  在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(fillRedEnable);
}
void CMCU_systemView::setFillRedEnable(bool enable)
{
	fillRedEnable = enable;
}
/*
	#鼠标滚轮#
*/
BOOL CMCU_systemView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	switch (zDelta)
	{
	case -120:
		if (GetKeyState(VK_CONTROL) < 0)
			OnButtonNarrow();
		break;
	case 120:
		if (GetKeyState(VK_CONTROL) < 0)
			OnButtonEnlarge();
		break;
	default:
		break;
	}
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

/*
	#刷新#
*/
void CMCU_systemView::OnRefresh()
{
	// TODO:  在此添加命令处理程序代码
	int returnStatus = openFilePath.reFreshImgFile();
	if (returnStatus == 2)
	{
		detectAlgorithm.instanceRect_vector.clear();
		currentRowInList = -1;
		setListControlContent();
		flag_compareBoard = false;//模块对比
		flag_showComponent = false;//不显示翻页组件
		currentPage = 0;
		Mat img_;
		img_.copyTo(src_img);
		src_img.copyTo(show_img);
		CWnd::Invalidate(TRUE);//调用ondraw函数

		srcROI2d.x = 0;
		srcROI2d.y = 0;
		srcROI2d.width = show_img.cols;
		srcROI2d.height = show_img.rows;
		initView();//初始化界面
		//设置状态栏信息
		pFrame = (CMainFrame *)AfxGetMainWnd();
		pFrame->m_wndStatusBar.OnProgress(0);
		pFrame->m_wndStatusBar.SetPaneText(1, "就绪", TRUE);
		//初始化队列
		editAlgorithm.setQueue_NULL();

		setStartCompareEnable(true);//开始对比按钮
		setPauseCompareEnable(true);//暂停对比按钮
		setStopCompareEnable(true);//停止对比按钮

		MessageBox(_T("当前文件目录内没有图像，请确认目录是否正确"), _T("MCU系统"));
		return;
	}
	else if (returnStatus == 0)//执行操作
	{
		detectAlgorithm.instanceRect_vector.clear();
		currentRowInList = -1;
		setListControlContent();
		currentPage = 0;
		src_img = imread(openFilePath.imageFilePathNames[currentPage]);
		src_img.copyTo(show_img);
		CWnd::Invalidate(TRUE);//调用ondraw函数

		flag_compareBoard = false;//模块对比
		flag_showComponent = true;//显示翻页组件

		srcROI2d.x = 0;
		srcROI2d.y = 0;
		srcROI2d.width = show_img.cols;
		srcROI2d.height = show_img.rows;
		havePictuireView();//有图像界面
		//设置状态栏信息
		pFrame = (CMainFrame *)AfxGetMainWnd();
		pFrame->m_wndStatusBar.OnProgress(0);
		pFrame->m_wndStatusBar.SetPaneText(1, "就绪", TRUE);
		//初始化队列
		editAlgorithm.setQueue_NULL();

		setStartCompareEnable(true);//开始对比按钮
		setPauseCompareEnable(true);//暂停对比按钮
		setStopCompareEnable(true);//停止对比按钮

	}
}
/*
	#添加识别器件#
*/
void CMCU_systemView::OnButtonAdd()
{
	// TODO:  在此添加命令处理程序代码
	if (!addEnable)
	{
		flag_addDetect = true;
		setAddEnable(true);
		setCancelEnable(false);
	}
}

/*
	#添加检测类别#
*/
void CMCU_systemView::OnAdddetectclasshouse()
{
	// TODO:  在此添加命令处理程序代码
	INT_PTR nResponse = addDetectClassHouse.DoModal();
	if (nResponse == IDOK)
	{
		// TODO:  在此放置处理何时用
		//  “确定”来关闭对话框的代码
		//添加手工识别器件
		
	}
}

/*
	#一键标红按钮#
*/
void CMCU_systemView::OnButtonFillred()
{
	if (!fillRedEnable)
	{
		setFillRedEnable(true);
		fillRed();
	}
	
}
/*
	#一键标红#
*/
void CMCU_systemView::fillRed()
{
	// TODO:  在此添加命令处理程序代码
	CDC* pDC = GetDC();
	for (int i = 0; i < detectAlgorithm.instanceRect_vector.size(); i++)
	{
		CRect dcRect;
		int left = detectAlgorithm.instanceRect_vector[i].left_top.x;
		int right = detectAlgorithm.instanceRect_vector[i].right_bottom.x;
		int top = detectAlgorithm.instanceRect_vector[i].left_top.y;
		int bottom = detectAlgorithm.instanceRect_vector[i].right_bottom.y;

		if (left >= 0 && top >= 0 && right < srcROI2d.width && bottom < srcROI2d.height && detectAlgorithm.instanceRect_vector[i].flag_noMark == false)//约束在图片内
		{
			dcRect.left = pictureInClient_ltPoint.x + left * pictureRatio;
			dcRect.top = pictureInClient_ltPoint.y + top * pictureRatio;
			dcRect.right = pictureInClient_ltPoint.x + right * pictureRatio;
			dcRect.bottom = pictureInClient_ltPoint.y + bottom * pictureRatio;

			CDC cdc; cdc.CreateCompatibleDC(pDC);

			CBitmap bitmap, *pOldBitmap;
			bitmap.CreateCompatibleBitmap(pDC, dcRect.right, dcRect.bottom);
			CRect src(dcRect);
			src.OffsetRect(CSize(-dcRect.left, -dcRect.top));
			pOldBitmap = cdc.SelectObject(&bitmap);
			cdc.FillSolidRect(src, RGB(255, 0, 255));

			if (::AlphaBlend == 0)
			{
				pDC->BitBlt(dcRect.left, dcRect.top, dcRect.right - dcRect.left, dcRect.bottom - dcRect.top, &cdc, src.left, src.top, SRCINVERT);
			}
			else{

				BLENDFUNCTION bf;
				memset(&bf, 0, sizeof(bf));
				bf.SourceConstantAlpha = 0x7f; //半透明
				bf.BlendOp = AC_SRC_OVER;

				::AlphaBlend(pDC->GetSafeHdc(), dcRect.left, dcRect.top, dcRect.right - dcRect.left, dcRect.bottom - dcRect.top,
					cdc.GetSafeHdc(), src.left, src.top, src.right - src.left, src.bottom - src.top, bf);
			}

			cdc.SelectObject(pOldBitmap);
		}
	}
	ReleaseDC(pDC);
}
/*
	#设置messageListCtrol的数据#
*/
void CMCU_systemView::setMessageListControlContent()
{
	if (currentRowInList != -1)
	{
		CString instanceString = "";
		instanceString.Format("%s%d", adddetect.className[detectAlgorithm.instanceRect_vector[currentRowInList].instanceClass - 1], detectAlgorithm.instanceRect_vector[currentRowInList].currentIndex);
		CString leftTopPoint = "";
		leftTopPoint.Format("( %d, %d )", (int)detectAlgorithm.instanceRect_vector[currentRowInList].left_top.x, (int)detectAlgorithm.instanceRect_vector[currentRowInList].left_top.y);
		CString rightBottomPoint = "";
		rightBottomPoint.Format("( %d, %d )", (int)detectAlgorithm.instanceRect_vector[currentRowInList].right_bottom.x, (int)detectAlgorithm.instanceRect_vector[currentRowInList].right_bottom.y);
		CString pin1 = "";
		pin1.Format("%d", detectAlgorithm.instanceRect_vector[currentRowInList].numOfPin1);
		CString pin2 = "";
		pin2.Format("%d", detectAlgorithm.instanceRect_vector[currentRowInList].numOfPin2);
		m_messageList.SetItemText(0, 1, instanceString);
		m_messageList.SetItemText(1, 1, adddetect.className[detectAlgorithm.instanceRect_vector[currentRowInList].instanceClass - 1]);
		m_messageList.SetItemText(2, 1, detectAlgorithm.instanceRect_vector[currentRowInList].instanceWord.c_str());
		m_messageList.SetItemText(3, 1, leftTopPoint);
		m_messageList.SetItemText(4, 1, rightBottomPoint);
		m_messageList.SetItemText(5, 1, pin1);
		m_messageList.SetItemText(6, 1, pin2);
	}
}
/*
	#messageList双击事件#
*/
void CMCU_systemView::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	*pResult = 0;
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	CRect rc;

	m_row = pNMListView->iItem;//选中的行号
	m_column = pNMListView->iSubItem;//选中的列号

	if (m_column == 1 && m_row >= 0 && m_row < 8)
	{
		m_messageList.GetSubItemRect(m_row, m_column, LVIR_LABEL, rc);
		m_modifyMessageListEdit.SetParent(&m_messageList);
		m_modifyMessageListEdit.MoveWindow(rc);
		m_modifyMessageListEdit.SetWindowText(m_messageList.GetItemText(m_row, m_column));
		m_modifyMessageListEdit.ShowWindow(SW_SHOW);
		m_modifyMessageListEdit.SetFocus();
		m_modifyMessageListEdit.ShowCaret();//显示光标
		m_modifyMessageListEdit.SetSel(-1);//将光标移到最后
	}
	*pResult = 0;
}
/*
#m_modifyMessageListEdit光标消失事件#
*/
void CMCU_systemView::OnKillfocusEditList()
{
	CString str;
	m_modifyMessageListEdit.GetWindowText(str);
	m_messageList.SetItemText(m_row, 1, str);
	wordSize = _ttoi(str);
	writeXML_Configure();//写入配置
	m_modifyMessageListEdit.ShowWindow(SW_HIDE);
}
/*
	#加载配置#
*/
int CMCU_systemView::loadXML_Configure()
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
			TiXmlElement* e1 = elem->FirstChildElement("wordSize");
			TiXmlNode* e2 = e1->FirstChild();
			const char *numOfRowImage_char = e2->ToText()->Value();
			CString Cstring;
			Cstring.Format("%s", numOfRowImage_char);
			m_messageList.SetItemText(7, 1, Cstring);
			wordSize = _ttoi(Cstring);
		}
	}
	doc.Clear();
	return 1;
}
/*
	#写入配置#
*/

int CMCU_systemView::writeXML_Configure()
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
			TiXmlElement* e1 = elem->FirstChildElement("wordSize");
			TiXmlNode* e2 = e1->FirstChild();
			CString Cstring;
			Cstring.Format("%d", wordSize);
			e2->SetValue(Cstring);
		}
	}
	doc.SaveFile("configure.xml");
	doc.Clear();
	return 1;
}
/*
	#隐藏左边显示元器件信息的list#
*/
void CMCU_systemView::hideMessageList(){
	m_messageList.ShowWindow(SW_HIDE);
}
/*
#显示左边显示元器件信息的list#
*/
void CMCU_systemView::showMessageList(){
	m_messageList.ShowWindow(SW_SHOW);
}
/*
#隐藏右边所有元器件列表#
*/
void CMCU_systemView::hideAllDetectList(){
	m_list.ShowWindow(SW_HIDE);
}
/*
#显示右边所有元器件列表#
*/
void CMCU_systemView::showAllDetectList(){
	m_list.ShowWindow(SW_SHOW);
}
/*
#隐藏左边图像拼接信息显示框#
*/
void CMCU_systemView::hideTXPJMessageEdit(){
	m_editMessage.ShowWindow(SW_HIDE);
}
/*
#显示左边图像拼接信息显示框#
*/
void CMCU_systemView::showTXPJMessageEdit(){
	m_editMessage.ShowWindow(SW_SHOW);
}