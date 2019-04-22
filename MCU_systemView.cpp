
// MCU_systemView.cpp : CMCU_systemView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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
#define IDC_EDIT_NEW 10//�µ��ı���
DWORD WINAPI imageStitching(LPVOID p);
DWORD WINAPI imageDetect(LPVOID p);
// CMCU_systemView

IMPLEMENT_DYNCREATE(CMCU_systemView, CView)

BEGIN_MESSAGE_MAP(CMCU_systemView, CView)
	// ��׼��ӡ����
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

// CMCU_systemView ����/����

CMCU_systemView::CMCU_systemView()
{
	// TODO:  �ڴ˴���ӹ������

}

CMCU_systemView::~CMCU_systemView()
{
}

BOOL CMCU_systemView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ


	return CView::PreCreateWindow(cs);
}

int CMCU_systemView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	

	m_staticMessage.Create("������Ϣ��ʾ��", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, 56);
	m_editMessage.Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE, CRect(0, 0, 0, 0), this, 57);
	// TODO:  �ڴ������ר�õĴ�������
	m_btn_previous.Create(_T("��һ��"), WS_CHILD | WSF_VISIBLE | BS_PUSHBUTTON, CRect(0, 0, 0, 0), this, IDC_D_BTN_PREVIOUS);
	m_btn_next.Create(_T("��һ��"), WS_CHILD | WSF_VISIBLE | BS_PUSHBUTTON, CRect(0, 0, 0, 0), this, IDC_D_BTN_NEXT);
	m_s_middle.Create(_T("/"), SS_CENTER | WSF_VISIBLE | SS_CENTERIMAGE, CRect(0, 0, 0, 0), this, 4);
	m_list.Create(LVS_ALIGNLEFT | WS_CHILD | WS_VISIBLE | LVS_SINGLESEL, CRect(0, 0, 0, 0), this, 55);
	LONG lStyle;
	lStyle = GetWindowLong(m_list.m_hWnd, GWL_STYLE);//��ȡ��ǰ����style
	lStyle &= ~LVS_TYPEMASK; //�����ʾ��ʽλ
	lStyle |= LVS_REPORT; //����style
	SetWindowLong(m_list.m_hWnd, GWL_STYLE, lStyle);//����style

	DWORD dwStyle = m_list.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
	dwStyle |= LVS_EX_GRIDLINES;//�����ߣ�ֻ������report����listctrl��
	dwStyle |= LVS_EX_CHECKBOXES;//itemǰ����checkbox�ؼ�
	m_list.SetExtendedStyle(dwStyle); //������չ���
	m_list.InsertColumn(0, "���", LVCFMT_LEFT, 80);//������
	m_list.InsertColumn(1, "���", LVCFMT_LEFT, 50);//������
	m_list.InsertColumn(2, "����", LVCFMT_LEFT, 70);//������

	m_messageList.Create(LVS_ALIGNLEFT | WS_CHILD | WS_VISIBLE | LVS_SINGLESEL, CRect(0, 0, 0, 0), this, 58);
	LONG lStyle1;
	lStyle1 = GetWindowLong(m_messageList.m_hWnd, GWL_STYLE);//��ȡ��ǰ����style
	lStyle1 &= ~LVS_TYPEMASK; //�����ʾ��ʽλ
	lStyle1 |= LVS_REPORT; //����style
	SetWindowLong(m_messageList.m_hWnd, GWL_STYLE, lStyle1);//����style

	DWORD dwStyle1 = m_messageList.GetExtendedStyle();
	dwStyle1 |= LVS_EX_FULLROWSELECT;//ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
	dwStyle1 |= LVS_EX_GRIDLINES;//�����ߣ�ֻ������report����listctrl��
	m_messageList.SetExtendedStyle(dwStyle1); //������չ���
	m_messageList.InsertColumn(0, "����", LVCFMT_RIGHT, 100);//������
	m_messageList.InsertColumn(1, "����", LVCFMT_CENTER, 100);//������
	m_messageList.InsertItem(0, "         ��ţ�");
	m_messageList.InsertItem(1, "         ���");
	m_messageList.InsertItem(2, "         ���ͣ�");
	m_messageList.InsertItem(3, "���Ͻ����꣺");
	m_messageList.InsertItem(4, "���½����꣺");
	m_messageList.InsertItem(5, "      ����һ��");
	m_messageList.InsertItem(6, "      ���Ŷ���");
	m_messageList.InsertItem(7, "   �����С��");
	m_messageList.SetItemText(7, 1, "50");
	m_messageList.InsertItem(8, "�������������");
	m_modifyMessageListEdit.Create(WS_CHILD | WS_VISIBLE | WS_BORDER, CRect(0, 0, 0, 0), this, 59);

	loadXML_Configure();//����������Ϣ

	return 0;
}

// CMCU_systemView ����

void CMCU_systemView::OnDraw(CDC* pDC)
{
	CMCU_systemDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	if (flag_compareBoard)//���ģ��Ա���ʾ
	{
		showBoardPicture(boardImg1, boardImg2);
	}
	else
	{ 
		showPicture(show_img);
	}
	
	//���ÿؼ�λ��
	CRect pictureControl;
	GetClientRect(&pictureControl);
	//��Сλ��
	int right = pictureControl.right;
	m_staticMessage.MoveWindow(CRect(0, pictureControl.bottom / 2, 200, pictureControl.bottom ));
	m_editMessage.MoveWindow(CRect(1, pictureControl.bottom / 2 + 20, 198, pictureControl.bottom - 1));
	m_list.MoveWindow(CRect(pictureControl.right - 200, pictureControl.top, pictureControl.right, pictureControl.bottom));
	m_messageList.MoveWindow(CRect(0, pictureControl.top, 200, pictureControl.bottom / 2));

	


	// TODO:  �ڴ˴�Ϊ����������ӻ��ƴ���
}



// CMCU_systemView ��ӡ
BOOL CMCU_systemView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CMCU_systemView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CMCU_systemView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  ��Ӵ�ӡ����е��������
}


// CMCU_systemView ���

#ifdef _DEBUG
void CMCU_systemView::AssertValid() const
{
	CView::AssertValid();
}

void CMCU_systemView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMCU_systemDoc* CMCU_systemView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMCU_systemDoc)));
	return (CMCU_systemDoc*)m_pDocument;
}
#endif //_DEBUG


// CMCU_systemView ��Ϣ�������


/*
*���ļ�Ŀ¼
*
*/
void CMCU_systemView::OnFileOpen()
{
	// TODO:  �ڴ���������������
	int returnStatus = openFilePath.openFile(m_hWnd, flag_setPara);//���ļ�Ŀ¼

	adjustShowPictureRegion(200, 200, 0, 40);//����ͼƬ��ʾ�����С

	if (returnStatus == 1)
	{
		MessageBox(_T("��ʧ��"), _T("MCUϵͳ"));
		return;
	}
	else if (returnStatus == 2)
	{
		detectAlgorithm.instanceRect_vector.clear();
		currentRowInList = -1;
		setListControlContent();
		flag_compareBoard = false;//ģ��Ա�
		flag_showComponent = false;//����ʾ��ҳ���
		currentPage = 0;
		Mat img_;
		img_.copyTo(src_img);
		src_img.copyTo(show_img);
		CWnd::Invalidate(TRUE);//����ondraw����
		
		srcROI2d.x = 0;
		srcROI2d.y = 0;
		srcROI2d.width = show_img.cols;
		srcROI2d.height = show_img.rows;

		initView();//��ʼ������
		//����״̬����Ϣ
		pFrame = (CMainFrame *)AfxGetMainWnd();
		pFrame->m_wndStatusBar.OnProgress(0);
		pFrame->m_wndStatusBar.SetPaneText(1, "����", TRUE);
		//��ʼ������
		editAlgorithm.setQueue_NULL();

		setStartCompareEnable(true);//��ʼ�ԱȰ�ť
		setPauseCompareEnable(true);//��ͣ�ԱȰ�ť
		setStopCompareEnable(true);//ֹͣ�ԱȰ�ť
		setAddEnable(true);
		setFillRedEnable(true);
		flag_addDetect = false;

		MessageBox(_T("��ǰ�ļ�Ŀ¼��û��ͼ����ȷ��Ŀ¼�Ƿ���ȷ"), _T("MCUϵͳ"));
		return;
	}
	else//ִ�в���
	{
		detectAlgorithm.instanceRect_vector.clear();
		currentRowInList = -1;
		setListControlContent();
		currentPage = 0;
		src_img = imread(openFilePath.imageFilePathNames[currentPage]);
		src_img.copyTo(show_img);
		CWnd::Invalidate(TRUE);//����ondraw����

		flag_compareBoard = false;//ģ��Ա�
		flag_showComponent = true;//��ʾ��ҳ���

		srcROI2d.x = 0;
		srcROI2d.y = 0;
		srcROI2d.width = show_img.cols;
		srcROI2d.height = show_img.rows;

		havePictuireView();//��ͼ�����
		//����״̬����Ϣ
		pFrame = (CMainFrame *)AfxGetMainWnd();
		pFrame->m_wndStatusBar.OnProgress(0);
		pFrame->m_wndStatusBar.SetPaneText(1, "����", TRUE);
		//��ʼ������
		editAlgorithm.setQueue_NULL();

		setStartCompareEnable(true);//��ʼ�ԱȰ�ť
		setPauseCompareEnable(true);//��ͣ�ԱȰ�ť
		setStopCompareEnable(true);//ֹͣ�ԱȰ�ť
		setAddEnable(true);
		setFillRedEnable(true);
		flag_addDetect = false;

	}
}

/*
*����ͼƬ��ʾ�����С
*
*/
void CMCU_systemView::adjustShowPictureRegion(int left, int right, int top, int bottom){
	clientToLeft = left;//ͼƬ��ʾ���򵽿ͻ�����߾���
	clientToRight = right;//ͼƬ��ʾ���򵽿ͻ����ұ߾���
	clientToUp = top;//ͼƬ��ʾ���򵽿ͻ����ϱ߾���
	clientToBottom = bottom;//ͼƬ��ʾ���򵽿ͻ����±߾���
}

/*
*�����ļ�
*
*/
void CMCU_systemView::OnFileSave()
{
	// TODO:  �ڴ���������������
	CString FilePath;
	TCHAR szFilter[] = _T("ͼƬ(*.png;*.jpg;*.jpeg;*.bmp)|*.png;*.jpg;*.jpeg;*.bmp|�����ļ�(*.*)|*.*||");
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
				detectAlgorithm.drawRect(save_img, detectAlgorithm.instanceRect_vector[i].left_top, detectAlgorithm.instanceRect_vector[i].right_bottom, detectAlgorithm.instanceRect_vector[i].manaulAdd);//������
			}
		}
		
		FilePath = dlg.GetPathName();
		imwrite((LPCTSTR)FilePath, save_img);
	}
}

/*
*��ʾͼƬ
*
*/
void CMCU_systemView::showPicture(Mat mat)
{
	setPageTurningComponent();//���÷�ҳ���

	CDC* pDC = GetDC();
	HDC hDC = pDC->GetSafeHdc();
	IplImage *img = &IplImage(mat);
	CvvImage cimg;
	cimg.CopyOf(img);

	CRect rect;
	GetClientRect(&rect);
	//��ȡͼƬ�Ŀ� ��  
	int width = mat.cols;
	int height = mat.rows;
	pictureRatio = 0.0;
	rect.left += clientToLeft;
	rect.bottom = rect.bottom - clientToBottom;
	rect.right -= clientToRight;
	int rectWidth = rect.right - rect.left;
	int rectHeight = rect.bottom - rect.top;
	if (rectWidth * 1.0 / rectHeight <= width * 1.0 / height)     //�Գ�Ϊ��,������С
	{
		if (width != 0)
		{
			pictureRatio = (double)rectWidth / (double)width;//���ͼ����ʾ����
			int realHeight = rectWidth * height / width;  //������ʾ��С����Ȼ��ʾ�����߿�
			rect.top = rect.top + rectHeight / 2 - realHeight / 2;
			rect.bottom = rect.top + realHeight;
		}
	}
	else
	{
		if (height != 0)
		{
			pictureRatio = (float)rectHeight / (float)height;//���ͼ����ʾ����
			int realwidth = width * rectHeight / height;  //�Կ�Ϊ����������С
			rect.left = rect.left + rectWidth / 2 - realwidth / 2;
			rect.right = rect.left + realwidth;
		}
	}

	//��ȡ��ʾͼƬ���ڵ����Ͻ�����
	CRect pictureControl;
	GetClientRect(&pictureControl);
	//ScreenToClient(&pictureControl);
	//��ʾͼƬ����ڿͻ������Ͻ�����
	pictureInClient_ltPoint.y = pictureControl.top + rect.top;
	pictureInClient_ltPoint.x = pictureControl.left + rect.left;
	//��ʾͼƬ����ڿͻ������½�����
	pictureInClient_rbPoint.y = pictureControl.top + rect.bottom;
	pictureInClient_rbPoint.x = pictureControl.left + rect.right;

	

	cimg.DrawToHDC(hDC, &rect);
	cimg.Destroy();
	
	

	//����͸������
	if (detectAlgorithm.instanceRect_vector.size() > 0 && flag_draw)
	{
		//�����ο�
		int sumNum = DrawRect();

		//��ʾ�����������
		CString sumNumString;
		sumNumString.Format("%d", sumNum);
		m_messageList.SetItemText(8, 1, sumNumString);

		if (fillRedEnable)
		{
			fillRed();//ȫ����Ǻ�
		}
		else if (flag_listFocus && !detectAlgorithm.instanceRect_vector[currentRowInList].flag_noMark)//�����ǰ������listControl,���ҵ�ǰԪ�����Ĳ����Ϊ��
		{
			setMessageListControlContent();
			DrawAlphaRect(pDC);
		}
	}
	
	ReleaseDC(pDC);
}
/*
	#�����ο�#
*/
int CMCU_systemView::DrawRect()
{
	//�����ο�
	CClientDC dc(this);
	dc.SelectStockObject(NULL_BRUSH); 
	CPen penRed(PS_SOLID, 3, RGB(255, 0, 0));//��һ֧��ɫ��ϸΪ1�ı�
	CPen penBlue(PS_SOLID, 3, RGB(0, 0, 255));//��һ֧��ɫ��ϸΪ1�ı�
	//dc.SelectObject(&penRed);//��pen�ŵ�dc��
	//dc.SetROP2(R2_NOT);
	CRect dcRect;
	int sumNum = 0;
	for (int i = 0; i < detectAlgorithm.instanceRect_vector.size(); i++)
	{
		if (!detectAlgorithm.instanceRect_vector[i].flag_noMark)//��������Ԫ����Ϊ�٣�����Ԫ����
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
					dc.SelectObject(&penBlue);//��ɫ�ʻ滭����
				}
				else
				{
					dc.SelectObject(&penRed);//��ɫ�ʻ滭����
				}
				dc.Rectangle(dcRect);
			}
		}
	}
	return sumNum;
}
/*
	#����͸������#
*/  
void CMCU_systemView::DrawAlphaRect(CDC *pDC)
{
	CRect dcRect;
	int left = detectAlgorithm.instanceRect_vector[currentRowInList].left_top.x;
	int right = detectAlgorithm.instanceRect_vector[currentRowInList].right_bottom.x;
	int top = detectAlgorithm.instanceRect_vector[currentRowInList].left_top.y;
	int bottom = detectAlgorithm.instanceRect_vector[currentRowInList].right_bottom.y;

	if (left >= 0 && top >= 0 && right < srcROI2d.width && bottom < srcROI2d.height)//Լ����ͼƬ��
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
			bf.SourceConstantAlpha = 0x7f; //��͸��
			bf.BlendOp = AC_SRC_OVER;

			::AlphaBlend(pDC->GetSafeHdc(), dcRect.left, dcRect.top, dcRect.right - dcRect.left, dcRect.bottom - dcRect.top,
				cdc.GetSafeHdc(), src.left, src.top, src.right - src.left, src.bottom - src.top, bf);
		}

		cdc.SelectObject(pOldBitmap);
	}
}
/*
*��ʾģ��Ա�ͼƬ
*
*/
void CMCU_systemView::showBoardPicture(Mat mat1, Mat mat2)
{
	setPageTurningComponent();//���÷�ҳ���

	CDC* pDC = GetDC();
	HDC hDC = pDC->GetSafeHdc();

	IplImage *img = &IplImage(mat1);
	CvvImage cimg;
	cimg.CopyOf(img);

	CRect rect;
	GetClientRect(&rect);
	//��ȡͼƬ�Ŀ� ��  
	int width = mat1.cols;
	int height = mat1.rows;
	int width_s = rect.right / 2 - 5;
	if (width_s * 1.0 / rect.bottom <= width * 1.0 / height)     //�Գ�Ϊ��,������С
	{
		if (width != 0)
		{
			int realHeight = width_s * height / width;  //������ʾ��С����Ȼ��ʾ�����߿�
			rect.top = rect.bottom / 2 - realHeight / 2;
			rect.bottom = rect.top + realHeight;
			rect.right = width_s;
		}
	}
	else
	{
		if (height != 0)
		{
			int realwidth = width * rect.bottom / height;  //�Կ�Ϊ����������С
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
	//��ȡͼƬ�Ŀ� ��  
	width = mat2.cols;
	height = mat2.rows;
	if (width_s * 1.0 / rect2.bottom <= width * 1.0 / height)     //�Գ�Ϊ��,������С
	{
		if (width != 0)
		{
			int realHeight = width_s * height / width;  //������ʾ��С����Ȼ��ʾ�����߿�
			rect2.top = rect2.bottom / 2 - realHeight / 2;
			rect2.bottom = rect2.top + realHeight;
			rect2.left = width_s + 10;
		}
	}
	else
	{
		if (height != 0)
		{
			int realwidth = width * rect2.bottom / height;  //�Կ�Ϊ����������С
			rect2.left = width_s / 2 - realwidth / 2 + width_s + 10;
			rect2.right = rect2.left + realwidth;
		}
	}
	cimg2.DrawToHDC(hDC, &rect2);
	cimg2.Destroy();
	ReleaseDC(pDC);

}

/*
#���÷�ҳ���#
*/
void CMCU_systemView::setPageTurningComponent()
{
	if (flag_showComponent)//��ʾ��ҳ���
	{
		CRect pictureControl;
		GetClientRect(&pictureControl);
		//��Сλ��
		m_btn_previous.MoveWindow(CRect(pictureControl.right / 2 - 122, pictureControl.bottom - 35, pictureControl.right / 2 - 42, pictureControl.bottom - 5));
		m_btn_next.MoveWindow(CRect(pictureControl.right / 2 + 42, pictureControl.bottom - 35, pictureControl.right / 2 + 122, pictureControl.bottom - 5));
		m_s_middle.MoveWindow(CRect(pictureControl.right / 2 - 40, pictureControl.bottom - 35, pictureControl.right / 2 + 40, pictureControl.bottom - 5));
	
		//���þ�̬�ı�
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
	else {//���ط�ҳ���
		m_btn_previous.ShowWindow(SW_HIDE);
		m_btn_next.ShowWindow(SW_HIDE);
		m_s_middle.ShowWindow(SW_HIDE);
	}
}
/*
	#���ط�ҳ���#
*/
void CMCU_systemView::hideButtonCompo(){
	flag_showComponent = false;
	setPageTurningComponent();
}
/*
#����listControl#
������
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
#��һҳ#
*/
void CMCU_systemView::OnButtonPreviousClick()
{
	if (currentPage == 0)
	{
		MessageBox(_T("�Ѿ��ǵ�һҳ"), _T("MCUϵͳ"));
		return;
	}
	currentPage--;
	src_img = imread(openFilePath.imageFilePathNames[currentPage]);//��һ��ͼƬ
	src_img.copyTo(show_img);
	CWnd::Invalidate(TRUE);//����ondraw����

	flag_showComponent = true;//��ʾ��ҳ���

	srcROI2d.x = 0;
	srcROI2d.y = 0;
	srcROI2d.width = show_img.cols;
	srcROI2d.height = show_img.rows;

	havePictuireView();//��ͼ�����

	//��ʼ������
	editAlgorithm.setQueue_NULL();
}

/*
#��һҳ#
*/
void CMCU_systemView::OnButtonNextClick()
{
	if (currentPage == openFilePath.imageNum - 1)
	{
		MessageBox(_T("�Ѿ������һҳ"), _T("MCUϵͳ"));
		return;
	}
	currentPage++;
	src_img = imread(openFilePath.imageFilePathNames[currentPage]);//��һ��ͼƬ
	src_img.copyTo(show_img);
	CWnd::Invalidate(TRUE);//����ondraw����

	flag_showComponent = true;//��ʾ��ҳ���

	srcROI2d.x = 0;
	srcROI2d.y = 0;
	srcROI2d.width = show_img.cols;
	srcROI2d.height = show_img.rows;

	havePictuireView();//��ͼ�����

	//��ʼ������
	editAlgorithm.setQueue_NULL();
}

void CMCU_systemView::OnButtonOpenfile_toolBar()
{
	// TODO:  �ڴ���������������
	OnFileOpen();
}




void CMCU_systemView::OnButtonSave_toolBar()
{
	// TODO:  �ڴ���������������
	OnFileSave();
}

/*
*����
*
*/
void CMCU_systemView::OnButtonRevoke()
{
	// TODO:  �ڴ���������������
	if (!revokeEnable)
	{
		editAlgorithm.revoke(src_img, srcROI2d, detectAlgorithm);
		setListControlContent();
		setRedoEnable(false);
		if (editAlgorithm.view_index == 1)//�����ǰ����λ��Ϊ1��������ťʧЧ
		{
			setRevokeEnable(true);
		}
		else
		{
			setRevokeEnable(false);
		}
		if (editAlgorithm.enlargeNum >= 5){//���÷Ŵ�ť
			setEnlargeEnable(true);
		}
		else{
			setEnlargeEnable(false);
		}
		if (editAlgorithm.narrowNum > 0){//������С��ť
			setNarrowEnable(false);
		}
		else{
			setNarrowEnable(true);
		}
		show_img = src_img(srcROI2d);
		Invalidate(TRUE);//����ondraw����
	}
}

/*
*�ָ�
*
*/
void CMCU_systemView::OnButtonRedo()
{
	// TODO:  �ڴ���������������
	if (!redoEnable)
	{
		editAlgorithm.redo(src_img, srcROI2d, detectAlgorithm);
		setListControlContent();
		setRevokeEnable(false);
		if (editAlgorithm.view_index == editAlgorithm.max_view_index)//�����ǰλ��Ϊ�������λ�ã��ָ���ťʧЧ
		{
			setRedoEnable(true);
		}
		else
		{
			setRedoEnable(false);
		}
		if (editAlgorithm.enlargeNum >= 5){//���÷Ŵ�ť
			setEnlargeEnable(true);
		}
		else{
			setEnlargeEnable(false);
		}
		if (editAlgorithm.narrowNum > 0){//������С��ť
			setNarrowEnable(false);
		}
		else{
			setNarrowEnable(true);
		}
		show_img = src_img(srcROI2d);
		Invalidate(TRUE);//����ondraw����
	}
}

/*
*��ʼ������ť
*
*/
void CMCU_systemView::OnButtonStart()
{
	// TODO:  �ڴ���������������
	if (!startEnable)
	{
		if (!goonStitching)//��ʼ
		{
			if (!openFilePath.imageFilePathNames.size()){
				MessageBox(_T("��������ȷ��ͼƬ·����"), _T("MCUϵͳ"));
				return;
			}
			if (!flag_setPara)
			{
				MessageBox(_T("�������ò�����"), _T("MCUϵͳ"));
				return;
			}
			setPauseEnable(false);
			setStopEnable(false);

			//����״̬����Ϣ
			pFrame->m_wndStatusBar.SetPaneText(1, "���ڽ���ͼ��ƴ�ӣ����Ե�", TRUE);
			m_editMessage.SetWindowTextA("");

			DWORD threadId;
			Mat o;
			o.copyTo(outImage);

			
			sumProcessTimes = (numOfRowImage - 1) * numOfColImage + (numOfColImage - 1);//����������
			currentProcessingTimes = 0;//��������ǰλ��

			pFrame = (CMainFrame *)AfxGetMainWnd();
			try
			{
				imageStitchingThread = CreateThread(NULL, 0, imageStitching, this, 0, &threadId); // �����߳�
			}
			catch (const std::exception& ex)
			{
				MessageBox(_T(format("����ƴ���߳�ʧ�ܣ�%s", ex.what()).c_str()), _T("MCUϵͳ"));
			}
			setStartEnable(true);
			return;
		}
		else if (goonStitching){//����
			if (ResumeThread(this->imageStitchingThread) != 0xFFFFFFFF)
			{
				setStartEnable(true);
				setPauseEnable(false);
				pFrame->m_wndStatusBar.SetPaneText(1, "���ڽ���ͼ��ƴ�ӣ����Ե�", TRUE);
			}
			else
			{
				MessageBox(_T("����ʧ�ܣ�"), _T("MCUϵͳ"));
				return;
			}
		}
	}
	
	
}

/*
*ͼ��ƴ���߳�
*
*/
DWORD WINAPI imageStitching(LPVOID p)
{
	CMCU_systemView *para = (CMCU_systemView *)p;
	
	//���ý���������
	para->pFrame->m_wndStatusBar.setRange(para->sumProcessTimes);
	para->flag_rowOrCol = true;//����ƴ����
	for (int j = 1, h = 0; h < para->numOfColImage; j += para->numOfRowImage, h++)//��
	{
		para->currentTXPJCol = h + 1;
		para->leftImage = imread(para->openFilePath.imageFilePathNames[j - 1]);

		//�ж�ͼƬ�Ƿ�Ϊ��
		if (para->leftImage.cols == 0)
		{
			para->flag_TXPJSucess = 3;
			::SendMessage(para->GetSafeHwnd(), WM_USER_TXPJ_MSG, WPARAM(0), LPARAM(0));//������ʾ��Ϣ����
			para->MessageBox(_T("ƴ��ʧ�ܣ�"), _T("MCUϵͳ"));
			::PostMessage(para->GetSafeHwnd(), WM_USER_STOP_MSG, WPARAM(0), LPARAM(0));
			return 0;
		}

		for (int i = j; i < j + para->numOfRowImage - 1; i++)//��
		{
			if (para->flag_isStopStitching)//ֹͣƴ���߳�
			{
				//����ƴ��ֹͣ����������
				::PostMessage(para->GetSafeHwnd(), WM_USER_STOP_MSG, WPARAM(0), LPARAM(0));
				return 0;
			}

			para->currentTXPJRow = i - j + 1;
			para->rightImage = imread(para->openFilePath.imageFilePathNames[i]);

			//�ж�ͼƬ�Ƿ�Ϊ��
			if (para->rightImage.cols == 0)
			{
				para->flag_TXPJSucess = 2;
				::SendMessage(para->GetSafeHwnd(), WM_USER_TXPJ_MSG, WPARAM(0), LPARAM(0));//������ʾ��Ϣ����
				para->MessageBox(_T("ƴ��ʧ�ܣ�"), _T("MCUϵͳ"));
				::PostMessage(para->GetSafeHwnd(), WM_USER_STOP_MSG, WPARAM(0), LPARAM(0));
				return 0;
			}
			
			para->flag_TXPJSucess = para->txpjAlgorithm.imageStitching_rows(para->leftImage, para->rightImage, para->outImage_row[h], para->widthByImage);//��ƴ��
			para->outImage_row[h].copyTo(para->leftImage);
			::SendMessage(para->GetSafeHwnd(), WM_USER_TXPJ_MSG, WPARAM(0), LPARAM(0));//������ʾ��Ϣ����
			
			if (para->flag_TXPJSucess == 0)//���ƴ��ʧ��
			{
				para->MessageBox(_T("ƴ��ʧ�ܣ�"), _T("MCUϵͳ"));
				::PostMessage(para->GetSafeHwnd(), WM_USER_STOP_MSG, WPARAM(0), LPARAM(0));
				return 0;
			}
			para->currentProcessingTimes++; //��������ǰλ��+1
			para->pFrame->m_wndStatusBar.OnProgress(para->currentProcessingTimes);//���ý�������ǰλ��
		}

	}

	if (para->numOfColImage == 1)//���ֻ��һ��
	{
		para->outImage_row[0].copyTo(para->outImage);
	}

	para->outImage_row[0].copyTo(para->leftImage);
	para->flag_rowOrCol = false;//����ƴ����
	for (int i = 1; i < para->numOfColImage; i++)
	{
		para->outImage_row[i].copyTo(para->rightImage);
		para->currentTXPJCol = i;
		if (para->flag_isStopStitching)//ֹͣƴ���߳�
		{
			//����ƴ��ֹͣ����������
			::PostMessage(para->GetSafeHwnd(), WM_USER_STOP_MSG, WPARAM(0), LPARAM(0));
			return 0;
		}
		para->flag_TXPJSucess = para->txpjAlgorithm.imageStitching_cols2(para->leftImage, para->rightImage, para->outImage, para->widthByImage, i);//��ƴ��
		para->outImage.copyTo(para->leftImage);
		::SendMessage(para->GetSafeHwnd(), WM_USER_TXPJ_MSG, WPARAM(0), LPARAM(0));//������ʾ��Ϣ����
		if (para->flag_TXPJSucess == 0)//���ƴ��ʧ��
		{
			para->MessageBox(_T("ƴ��ʧ�ܣ�"), _T("MCUϵͳ"));
			::PostMessage(para->GetSafeHwnd(), WM_USER_STOP_MSG, WPARAM(0), LPARAM(0));
			return 0;
		}
		para->currentProcessingTimes++;//��������ǰλ��+1
		para->pFrame->m_wndStatusBar.OnProgress(para->currentProcessingTimes);//���ý�������ǰλ��
	}

	::PostMessage(para->GetSafeHwnd(), WM_USER_MSG, WPARAM(0), LPARAM(0));//����ƴ����������������
	return 0;
}
/*
*��ʾͼ��ƴ����Ϣ
*
*/
LRESULT CMCU_systemView::showTXPJMessage(WPARAM wp, LPARAM lp)
{
	CString message = "";
	string sucess = "";
	if (flag_TXPJSucess == 1)
	{
		sucess = "�ɹ���";
	}
	else if (flag_TXPJSucess == 0)
	{
		sucess = "ʧ��!";
	}
	else if (flag_TXPJSucess == 2)
	{
		message.Format("�� %d �еĵ� %d ��ͼƬΪ�գ�����\r\n", currentTXPJCol, currentTXPJRow + 1);
		int length = m_editMessage.GetWindowTextLength();
		m_editMessage.SetSel(length, length);
		m_editMessage.ReplaceSel(message);
		return 0;
	}
	else if (flag_TXPJSucess == 3)
	{
		message.Format("�� %d �еĵ� %d ��ͼƬΪ�գ�����\r\n", currentTXPJCol, 1);
		int length = m_editMessage.GetWindowTextLength();
		m_editMessage.SetSel(length, length);
		m_editMessage.ReplaceSel(message);
		return 0;
	}

	if (flag_rowOrCol)//�������ƴ����
	{
		message.Format("�� %d �еĵ� %d �ź͵� %d ��ͼƬƴ�� %s\r\n", currentTXPJCol, currentTXPJRow, currentTXPJRow + 1, sucess.c_str());
		int length = m_editMessage.GetWindowTextLength();
		m_editMessage.SetSel(length, length);
		m_editMessage.ReplaceSel(message);
	}
	else
	{
		message.Format("�� %d �к͵� %d ��ͼƬƴ�� %s\r\n", currentTXPJCol, currentTXPJCol + 1, sucess.c_str());
		int length = m_editMessage.GetWindowTextLength();
		m_editMessage.SetSel(length, length);
		m_editMessage.ReplaceSel(message);
	}
	return 0;
}
/*
*ͼ��ƴ���߳���������
*
*/
LRESULT CMCU_systemView::imageStitchingOver(WPARAM wp, LPARAM lp)
{
	WaitForSingleObject(imageStitchingThread, INFINITE);//�ȴ��������
	CloseHandle(imageStitchingThread);

	flag_compareBoard = false;//ģ��Ա�
	flag_showComponent = false;//����ʾ��ҳ���

	if (outImage.empty()){
		Mat img;
		img.copyTo(show_img);
		Invalidate(TRUE);//����ondraw����
		initView();//��ʼ������
	}
	else
	{
		outImage.copyTo(src_img);
		src_img.copyTo(show_img);
		Invalidate(TRUE);//����ondraw����
		
		srcROI2d.x = 0;
		srcROI2d.y = 0;
		srcROI2d.width = show_img.cols;
		srcROI2d.height = show_img.rows;
		havePictuireView();//��ͼ�����
	}

	//��ʼ��Queue
	editAlgorithm.setQueue_NULL();

	pFrame->m_wndStatusBar.OnProgress(0);
	pFrame->m_wndStatusBar.SetPaneText(1, "ƴ�����", TRUE);

	flag_isStopStitching = false;//ֹͣƴ���̱߳�־

	return 1L;
}
/*
*ͼ��ƴ���߳̽���
*
*/
LRESULT CMCU_systemView::imageStitchingStop(WPARAM wp, LPARAM lp)
{
	WaitForSingleObject(imageStitchingThread, INFINITE);//�ȴ��������
	CloseHandle(imageStitchingThread);

	flag_compareBoard = false;//ģ��Ա�
	flag_showComponent = false;//����ʾ��ҳ���

	if (outImage.empty()){
		Mat img;
		img.copyTo(show_img);
		Invalidate(TRUE);//����ondraw����
		initView();//��ʼ������
	}
	else
	{
		outImage.copyTo(src_img);
		src_img.copyTo(show_img);
		Invalidate(TRUE);//����ondraw����
		
		srcROI2d.x = 0;
		srcROI2d.y = 0;
		srcROI2d.width = show_img.cols;
		srcROI2d.height = show_img.rows;
		havePictuireView();//��ͼ�����
	}

	//��ʼ��Queue
	editAlgorithm.setQueue_NULL();

	pFrame->m_wndStatusBar.OnProgress(0);
	pFrame->m_wndStatusBar.SetPaneText(1, "ƴ��ֹͣ", TRUE);

	flag_isStopStitching = false;//ֹͣƴ���̱߳�־

	return 1L;
}

/*
*��ʼ������
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
	flag_draw = false;//����
}
/*
*��ͼ�����
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
*��ͣ
*
*/
void CMCU_systemView::OnButtonPause()
{
	// TODO:  �ڴ���������������
	if (!pauseEnable)
	{
		if (SuspendThread(this->imageStitchingThread) != 0xFFFFFFFF)
		{
			setStartEnable(false);
			goonStitching = true;
			setPauseEnable(true);
			pFrame->m_wndStatusBar.SetPaneText(1, "��ͣ", TRUE);
		}
		else
		{
			MessageBox(_T("��ͣʧ�ܣ�"), _T("MCUϵͳ"));
			return;
		}
	}
}


/*
*ֹͣ
*
*/
void CMCU_systemView::OnButtonStop()
{
	// TODO:  �ڴ���������������
	if (!stopEnable)
	{
		if (pauseEnable)//����߳���ͣ�ˣ��Ȼ����̣߳���ֹͣ
		{
			if (ResumeThread(this->imageStitchingThread) != 0xFFFFFFFF)
			{
				setStopEnable(true);
				setPauseEnable(true);
				flag_isStopStitching = true;//ֹͣ�̱߳�־
				pFrame->m_wndStatusBar.SetPaneText(1, "����ֹͣƴ��", TRUE);
			}
			else
			{
				MessageBox(_T("ֹͣʧ�ܣ�"), _T("MCUϵͳ"));
				return;
			}
		}
		else//����ֱ��ֹͣ�߳�
		{
			setStopEnable(true);
			setPauseEnable(true);
			flag_isStopStitching = true;//ֹͣ�̱߳�־
			pFrame->m_wndStatusBar.SetPaneText(1, "����ֹͣƴ��", TRUE);
		}
	}
}




/*
*����
*
*/
void CMCU_systemView::OnButtonLeftroate()
{
	// TODO:  �ڴ���������������
	//ȷ����ťno
	if (!leftRoateEnable)
	{
		setMakeSureEnable(true);
		setRevokeEnable(false);
		setRedoEnable(true);

		editAlgorithm.leftRotate(src_img, srcROI2d, detectAlgorithm);
		

		show_img = src_img(srcROI2d);
		Invalidate(TRUE);//����ondraw����
	}
	
}



/*
*����
*
*/
void CMCU_systemView::OnButtonRightroate()
{
	// TODO:  �ڴ���������������
	//ȷ����ťno
	if (!rightRoateEnable)
	{
		setMakeSureEnable(true);
		setRevokeEnable(false);
		setRedoEnable(true);

		editAlgorithm.rightRotate(src_img, srcROI2d, detectAlgorithm);

		show_img = src_img(srcROI2d);
		Invalidate(TRUE);//����ondraw����
	}
}


/*
*�Ŵ�
*
*/
void CMCU_systemView::OnButtonEnlarge()
{
	// TODO:  �ڴ���������������
	if (!enlargeEnable)
	{
		if (editAlgorithm.enlargeNum < 5){//���Ŵ�5��
			//ȷ����ťno
			setMakeSureEnable(true);

			//�Ŵ�
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


			//����listControl�б�
			setListControlCheckBox();
		}
	}
	
}


/*
*��С
*
*/
void CMCU_systemView::OnButtonNarrow()
{
	// TODO:  �ڴ���������������
	if (!narrowEnable)
	{
		if (editAlgorithm.narrowNum > 0){
			//ȷ����ťno
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


			//����listControl�б�
			setListControlCheckBox();
		}
	}
	
}


/*
*�ü�
*
*/
void CMCU_systemView::OnButtonCutout()
{
	// TODO:  �ڴ���������������
	if (!cutOutEnable)
	{
		flag_cutButtonDown = true;
		setCutOutEnable(true);
		setCancelEnable(false);
		//����ı���ť
		setAddtextEnable(true);
	}
}


/*
*����ı�
*
*/
void CMCU_systemView::OnButtonAddtext()
{
	// TODO:  �ڴ���������������
	if (!addTextEnable)
	{
		flag_addEditButtonDown = true;
		//ȡ����ťyes
		setCancelEnable(false);
		setAddtextEnable(true);
		//�ü���ťno
		setCutOutEnable(true);
	}
}

/*
*˫������ı���
*
*/
void CMCU_systemView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (flag_addEditButtonDown
		&& point.x > pictureInClient_ltPoint.x && point.y > pictureInClient_ltPoint.y
		&& point.x < pictureInClient_rbPoint.x && point.y < pictureInClient_rbPoint.y){//����ı���Լ����ͼƬ��
		flag_addEditButtonDown = false;
		p_newEdit = new CEdit();
		//�����ı���
		p_newEdit->Create(WS_CHILD | WS_VISIBLE | WS_BORDER | SS_LEFT | WS_EX_TOOLWINDOW,
			CRect(point.x, point.y, point.x + 80, point.y + 30), this, IDC_EDIT_NEW);
		//�ı���ͼƬ�е�λ��
		point_textInPicture = Point((point.x - pictureInClient_ltPoint.x) / pictureRatio + srcROI2d.x, (point.y - pictureInClient_ltPoint.y) / pictureRatio + srcROI2d.y);
	}
	//˫�����ο�
	else if (point.x > pictureInClient_ltPoint.x && point.y > pictureInClient_ltPoint.y
		&& point.x < pictureInClient_rbPoint.x && point.y < pictureInClient_rbPoint.y)
	{
		for (int i = 0; i < detectAlgorithm.instanceRect_vector.size(); i++)
		{
			int left = detectAlgorithm.instanceRect_vector[i].left_top.x * pictureRatio + pictureInClient_ltPoint.x;
			int right = detectAlgorithm.instanceRect_vector[i].right_bottom.x * pictureRatio + pictureInClient_ltPoint.x;
			int top = detectAlgorithm.instanceRect_vector[i].left_top.y * pictureRatio + pictureInClient_ltPoint.y;
			int bottom = detectAlgorithm.instanceRect_vector[i].right_bottom.y * pictureRatio + pictureInClient_ltPoint.y;
			//�����ǰ���ο��ǣ�������������ھ��ο���
			if (detectAlgorithm.instanceRect_vector[i].flag_noMark == false
				&& point.x > left
				&& point.x < right
				&& point.y > top
				&& point.y < bottom)
			{
				currentRowInList = i;//��ǰ��
				flag_listFocus = true;
				UpdateWindow();
				showPicture(show_img);
				//���ø�����ʾ
				m_list.SetFocus();//���ý���
				m_list.SetItemState(currentRowInList, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);//����״̬
				m_list.EnsureVisible(currentRowInList, FALSE);//���õ�ǰ��ͼ�ɼ�
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
		currentRowInList = -1;// ��ǰ��
		setFillRedEnable(false);
		//if (currentRowInList != -1)
		//{
		//	m_list.SetItemState(m_list.SetSelectionMark(currentRowInList), 0, LVIS_SELECTED);
		//	currentRowInList = -1;// ��ǰ��
		//}
		
	}
	CView::OnLButtonDblClk(nFlags, point);
} 


/*
*�ı���enter�¼�
*
*/
BOOL CMCU_systemView::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  �ڴ����ר�ô����/����û���
	if (WM_KEYDOWN == pMsg->message && VK_RETURN == pMsg->wParam)
	{
		if (GetFocus() == GetDlgItem(IDC_EDIT_NEW)) //���ݲ�ͬ�ؼ������ж����Ǹ���ִ�� 
		{
			flag_addEditButtonDown = true;
			// ��ȡ�ı�����
			CString csX;
			GetDlgItem(IDC_EDIT_NEW)->GetWindowText(csX);
			if (!csX.IsEmpty()){//�����ı����ݲ�Ϊ�գ���ͼƬ����������
				USES_CONVERSION;
				char* p = T2A(csX.GetBuffer(0));
				csX.ReleaseBuffer();
				//��ͼ��д������
				editAlgorithm.paDrawString(src_img, srcROI2d, p, point_textInPicture, Scalar(255, 0, 0), wordSize, true, true, detectAlgorithm);
			}
			p_newEdit->DestroyWindow();
			p_newEdit = NULL;
			delete p_newEdit;//ɾ���½����ı���

			setRevokeEnable(false);
			setRedoEnable(true);

			show_img = src_img(srcROI2d);
			Invalidate(TRUE);//����ondraw����
			return true;
		}
	}
	if (pMsg->message == WM_KEYDOWN && GetAsyncKeyState(VK_CONTROL))
	{

	}
	return CView::PreTranslateMessage(pMsg);
}

/*
*����������
*
*/
void CMCU_systemView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (point.x > pictureInClient_ltPoint.x && point.y > pictureInClient_ltPoint.y
		&& point.x < pictureInClient_rbPoint.x && point.y < pictureInClient_rbPoint.y){//Լ�������ͼƬ��
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
*������̧��
*
*/
void CMCU_systemView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	flag_LeftButtonDown = false; 
	if (flag_addDetect)//������Ԫ����
	{
		INT_PTR nResponse = adddetect.DoModal();
		if (nResponse == IDOK)
		{
			// TODO:  �ڴ˷��ô����ʱ��
			//  ��ȷ�������رնԻ���Ĵ���
			//����ֹ�ʶ������
			InstanceRect instanceRect;
			instanceRect.left_top.x = (lButtonDown_point.x - pictureInClient_ltPoint.x) / pictureRatio;
			instanceRect.left_top.y = (lButtonDown_point.y - pictureInClient_ltPoint.y) / pictureRatio;
			instanceRect.right_bottom.x = (point.x - pictureInClient_ltPoint.x) / pictureRatio;
			instanceRect.right_bottom.y = (point.y - pictureInClient_ltPoint.y) / pictureRatio;
			instanceRect.manaulAdd = 1;//�˹����
			instanceRect.instanceClass = adddetect.currentIndex + 1;//������ƶ�Ӧ���±�
			detectAlgorithm.numOfClassName[instanceRect.instanceClass - 1]++;//�������������һ
			instanceRect.currentIndex = detectAlgorithm.numOfClassName[instanceRect.instanceClass - 1];//��ǰ�������±�
			CString instanceMark = "";
			instanceMark.Format("%s%d", adddetect.instanceClass, instanceRect.currentIndex);//���
			instanceRect.instanceWord = adddetect.instanceWord;//����
			detectAlgorithm.instanceRect_vector.push_back(instanceRect);
			detectAlgorithm.drawInstanceClassString(src_img, instanceMark, instanceRect.left_top, Scalar(255, 0, 0), wordSize, false, false);//���û�������ֺ���
			setListControlContent();//����Ԫ�����б�
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
*����ƶ�
*
*/
void CMCU_systemView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//���������ʽ
	if (flag_isStopStitching)
	{
		::SetCursor(wait_cursor);
	}
	else
	{
		::SetCursor(arrow_cursor);
	}
	if (flag_LeftButtonDown && !flag_cutButtonDown && !flag_addDetect&& editAlgorithm.enlargeNum > 0){//��ק
		editAlgorithm.draftingImg(src_img, srcROI2d, sourcePoint2d, lButtonDown_point, point, detectAlgorithm, leftBtnDown_lt, leftBtnDown_rb);//��קͼƬ
		show_img = src_img(srcROI2d);

		showPicture(show_img);

		//����listControl�б�
		setListControlCheckBox();
	}
	else if (flag_LeftButtonDown && flag_cutButtonDown && !flag_addDetect
		&& point.x > pictureInClient_ltPoint.x && point.y > pictureInClient_ltPoint.y
		&& point.x < pictureInClient_rbPoint.x && point.y < pictureInClient_rbPoint.y){//�ü�,Լ�������ͼƬ��
		
		Mat img;
		editAlgorithm.drawCutRect(src_img, img, srcROI2d, cutROI2d, lButtonDown_point, pictureInClient_ltPoint, point, lineThickness, pictureRatio);//���ü�����

		img = img(srcROI2d);
		showPicture(img);

		//ȷ����ťyes
		setMakeSureEnable(false);
	}
	else if (flag_LeftButtonDown && !flag_cutButtonDown && flag_addDetect
		&& point.x > pictureInClient_ltPoint.x && point.y > pictureInClient_ltPoint.y
		&& point.x < pictureInClient_rbPoint.x && point.y < pictureInClient_rbPoint.y)//���ʶ��Ԫ����
	{
		showPicture(show_img);
		detectAlgorithm.drawAddDetect(this, Point(pictureInClient_ltPoint.x, pictureInClient_ltPoint.y),
			Point(pictureInClient_rbPoint.x, pictureInClient_rbPoint.y),
			Point(lButtonDown_point.x, lButtonDown_point.y), point, pictureRatio);//���ʶ������
	
	}
	CView::OnMouseMove(nFlags, point);
}

/*
*ȷ����ť
*
*/
void CMCU_systemView::OnButtonMakesure()
{
	// TODO:  �ڴ���������������
	if (!makeSureEnable)
	{
		if (flag_cutButtonDown){//�ü�ͼƬ
			editAlgorithm.cutImg(src_img, show_img, srcROI2d, cutROI2d, detectAlgorithm);//�ü�ͼƬ
			
			//�Ŵ���С��ť��ԭ
			setEnlargeEnable(false);
			setNarrowEnable(true);
			setRevokeEnable(false);
			setRedoEnable(true);

			Invalidate(TRUE);//����ondraw����
			setListControlContent();
			//ȷ����ťno
			setMakeSureEnable(true);
		}
		if (flag_detectOver){//ͼ�������
			
			//�����λ���ͼƬ��
			for (int i = 0; i < detectAlgorithm.instanceRect_vector.size(); i++)
			{
				if (!detectAlgorithm.instanceRect_vector[i].flag_noMark)
				{
					detectAlgorithm.drawRect(src_img, detectAlgorithm.instanceRect_vector[i].left_top, detectAlgorithm.instanceRect_vector[i].right_bottom,detectAlgorithm.instanceRect_vector[i].manaulAdd);//������
				}
			}
			havePictuireView();//��ͼ�����
			flag_draw = false;//����
			m_list.DeleteAllItems();//����б�
			UpdateWindow();
			showPicture(src_img);
		}
	}
}


/*
*ȡ����ť
*
*/
void CMCU_systemView::OnButtonCancel()
{
	// TODO:  �ڴ���������������
	if (!cancelEnable)
	{
		if (flag_cutButtonDown){//ȡ���ü�
			flag_cutButtonDown = false;

			//�ü� ����ı� ȷ�� ȡ����ť��ԭ
			setCutOutEnable(false); 
			setMakeSureEnable(true);
			setCancelEnable(true);
			setAddtextEnable(false);

			show_img = src_img(srcROI2d);
			showPicture(show_img);
		}
		else if (flag_addEditButtonDown)//ȡ������ı�
		{
			flag_addEditButtonDown = false;
			//�ü� ����ı� ȡ����ť��ԭ
			setCutOutEnable(false);
			setCancelEnable(true);
			setAddtextEnable(false);

			if (p_newEdit){
				p_newEdit->DestroyWindow();
				p_newEdit = NULL;
				delete p_newEdit;//ɾ���½����ı���
				showPicture(src_img);
			}
		}
		else if (flag_addDetect)//ȡ�����ʶ������
		{
			flag_addDetect = false;

			//�ü� ����ı� ȷ�� ȡ����ť��ԭ
			setAddEnable(false);
			setMakeSureEnable(true);
			setCancelEnable(true);
			setAddtextEnable(false);
		}
	}
}

/*
*���Ŀ�갴ť
*
*/
void CMCU_systemView::OnDetect()
{
	// TODO:  �ڴ���������������
	if (!detectStartEnable)
	{
		//��������XML
		int code = addDetectClassHouse.loadXML();
		if (code == 0)
		{
			MessageBox("���ؼ����������ʧ��!!!", "MCUϵͳ");
			return;
		}
		else if (code == 1)
		{
			MessageBox("�����������Ϊ��!!!", "MCUϵͳ");
			return;
		}

		adddetect.className.clear();
		detectAlgorithm.numOfClassName.clear();
		adddetect.className = addDetectClassHouse.className;//��ֵ
		detectAlgorithm.numOfClassName = addDetectClassHouse.numOfClassName;
		

		pFrame = (CMainFrame *)AfxGetMainWnd();
		pFrame->m_wndStatusBar.SetPaneText(1, "���ڽ���ͼ���⣬���Ե�", TRUE);//����״̬��

		imwrite((LPCTSTR)"python//images//detectImg.png", src_img);

		imageDetectThread = CreateThread(NULL, 0, imageDetect, this, 0, NULL);

		setDetectStartEnable(true);
		setDetectStopEnable(false);

		//���ط�ҳ���
		hideButtonCompo();

		setStartCompareEnable(true);//��ʼ�ԱȰ�ť
		setPauseCompareEnable(true);//��ͣ�ԱȰ�ť
		setStopCompareEnable(true);//ֹͣ�ԱȰ�ť
	}
}
/*
*ͼ�����߳�
*
*/
DWORD WINAPI imageDetect(LPVOID p)
{
	CMCU_systemView *para = (CMCU_systemView *)p;

	//Ԫ�������
	para->detectAlgorithm.detect_bat();

	//����Ԫ������
	int returnCode = para->detectAlgorithm.drawDetectInstances(para->src_img, para->addDetectClassHouse.className, para->wordSize);
	if (returnCode == 1)
	{
		para->MessageBox("�������������һ�£�������ȷ�ϣ�����", "MCUϵͳ");
		//����״̬����Ϣ
		::PostMessage(para->GetSafeHwnd(), WM_USER_DETECT_STOP_MSG, WPARAM(0), LPARAM(0));
		return 0;
	}

	if (para->flag_isStopDetect)//ֹͣ���
	{
		//����״̬����Ϣ
		para->detectAlgorithm.instanceRect_vector.clear();//���Ԫ������Ϣ
		para->currentRowInList = -1;
		::PostMessage(para->GetSafeHwnd(), WM_USER_DETECT_STOP_MSG, WPARAM(0), LPARAM(0));
		return 0;
	}
	//����״̬����Ϣ
	::PostMessage(para->GetSafeHwnd(), WM_USER_DETECT_MSG, WPARAM(0), LPARAM(0));

	return 0;
}

/*
*ͼ�����߳̽����߳�
*
*/
LRESULT CMCU_systemView::imageDetectOver(WPARAM wp, LPARAM lp)
{
	WaitForSingleObject(imageDetectThread, INFINITE);//�ȴ��������
	CloseHandle(imageDetectThread);

	flag_compareBoard = false;//ģ��Ա�
	flag_showComponent = false;//����ʾ��ҳ���

	setDetectStartEnable(false);
	setDetectStopEnable(true);
	setAddEnable(false);//���Ԫ������ť�ɰ�
	setFillRedEnable(false);//һ����찴ť�ɰ�

	flag_draw = true;//��

	//��ʼ������
	editAlgorithm.setQueue_NULL();

	//��ʾ���ͼƬ
	src_img.copyTo(show_img);
	Invalidate(TRUE);//����ondraw����
	srcROI2d.x = 0;
	srcROI2d.y = 0;
	srcROI2d.width = show_img.cols;
	srcROI2d.height = show_img.rows;

	setListControlContent();//����Ԫ�����б�����

	pFrame->m_wndStatusBar.OnProgress(0);
	pFrame->m_wndStatusBar.SetPaneText(1, "������", TRUE);
	return 1L;
}
/*
*ͼ�����߳̽����߳�
*
*/
LRESULT CMCU_systemView::imageDetectStopOver(WPARAM wp, LPARAM lp)
{
	WaitForSingleObject(imageDetectThread, INFINITE);//�ȴ��������
	CloseHandle(imageDetectThread);

	flag_compareBoard = false;//ģ��Ա�
	flag_showComponent = false;//����ʾ��ҳ���

	setDetectStartEnable(false);
	setDetectStopEnable(true);
	detectAlgorithm.instanceRect_vector.clear();
	currentRowInList = -1;
	flag_draw = false;//����

	//��ʼ������
	editAlgorithm.setQueue_NULL();

	pFrame->m_wndStatusBar.OnProgress(0);
	pFrame->m_wndStatusBar.SetPaneText(1, "��ֹͣ", TRUE);

	flag_isStopDetect = false;

	return 1L;
}
/*
	ֹͣ���
*/
void CMCU_systemView::OnDetectstop()
{
	// TODO:  �ڴ���������������
	if (!detectStopEnable)
	{
		flag_isStopDetect = true;
		detectAlgorithm.kill_detect();
	}
	
}

/*
����bom
*/
void CMCU_systemView::OnSaveasbom()
{
	// TODO:  �ڴ���������������
	CString FilePath;
	TCHAR szFilter[] = _T("ͼƬ(*.xlsx;*.xls)|*.xlsx;*.xls;|�����ļ�(*.*)|*.*||");
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
	#����listControl������#

*/
void CMCU_systemView::setListControlContent()
{
	m_list.DeleteAllItems();//����б�
	int listLength = detectAlgorithm.instanceRect_vector.size();//�б�Ԫ����������
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
			m_list.SetCheck(i, 1);//����CheckBox
		}
		else
		{
			m_list.SetCheck(i, 0);//����CheckBox
		}
		
	}
}
/*
	#����listCtrol��CheckBox#
*/
void CMCU_systemView::setListControlCheckBox()
{
	int listLength = detectAlgorithm.instanceRect_vector.size();//�б�Ԫ����������
	for (int i = 0; i < listLength; i++)
	{
		int left = detectAlgorithm.instanceRect_vector[i].left_top.x;
		int right = detectAlgorithm.instanceRect_vector[i].right_bottom.x;
		int top = detectAlgorithm.instanceRect_vector[i].left_top.y;
		int bottom = detectAlgorithm.instanceRect_vector[i].right_bottom.y;
		if (detectAlgorithm.instanceRect_vector[i].flag_noMark == false && left >= 0 && top >= 0 && right < srcROI2d.width && bottom < srcROI2d.height)
		{
			m_list.SetCheck(i, 1);//����CheckBox
		}
		else
		{
			m_list.SetCheck(i, 0);//����CheckBox
		}

	}
}
/*
	#listControl��Ϣ��Ӧ�¼�#
	item�л�����ʱ(�����ü��̺�����л�itemʱ)��״̬��һЩ�仯˳��
	���listctrl�ؼ���LVN_ITEMCHANGED��Ϣ��Ӧ����
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
	#listControl��CheckBox��Ϣ#
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

		//ע�⣬bCheckStateΪTRUE��checkbox�ӹ�ѡ״̬��Ϊδ��ѡ״̬
		if (detectAlgorithm.instanceRect_vector.size() > 0)
		{
			if (bCheckState)
			{
				detectAlgorithm.instanceRect_vector[nItem].flag_noMark = true;//����ǵ�ǰԪ����
				UpdateWindow();
				showPicture(show_img);
				//...............
			}
			else
			{
				detectAlgorithm.instanceRect_vector[nItem].flag_noMark = false;//��ǵ�ǰԪ����
				UpdateWindow();
				showPicture(show_img);
				//...............
			}
		}
		
	}

	*pResult = 0;
}
/*
ͳ��Pin��
*/
void CMCU_systemView::OnStatisticspin()
{
	// TODO:  �ڴ���������������
	statisticsTip.statisticsPin(detectAlgorithm);
}


/*
*���ò���
*
*/
void CMCU_systemView::OnSetpara()
{
	// TODO:  �ڴ���������������
	INT_PTR nResponse = setupParaDlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO:  �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
		numOfRowImage = setupParaDlg.numOfRowImage;
		numOfColImage = setupParaDlg.numOfColImage;
		numOfImage = setupParaDlg.numOfImage;
		widthByImage = setupParaDlg.widthByImage;
		heightByImage = setupParaDlg.heightByImage;
		flag_setPara = true;//��������
	}
}

/*
	#����ģ��ߴ�#
*/
void CMCU_systemView::OnSetboardsize()
{
	// TODO:  �ڴ���������������
	INT_PTR nResponse = setBoardSize.DoModal();
	if (nResponse == IDOK)
	{
		// TODO:  �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
		boardWidth = setBoardSize.boardWidth;
		boardHeight = setBoardSize.boardHeight;
	}
}

/*
	#ģ��Ա�ѡ��ͼƬ#
*/
void CMCU_systemView::OnCompareSelectimgs()
{
	// TODO:  �ڴ���������������
	INT_PTR nResponse = compareSelectImgs.DoModal();
	if (nResponse == IDOK)
	{
		// TODO:  �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
		flag_compareBoard = true;//ģ��Ա�
		flag_showComponent = false;//���ط�ҳ���

		setStartCompareEnable(false);//��ʼ�ԱȰ�ť
		setPauseCompareEnable(true);//��ͣ�ԱȰ�ť
		setStopCompareEnable(true);//ֹͣ�ԱȰ�ť

		boardImg1 = imread(compareSelectImgs.mFilePath1.GetBuffer());//����ͼ
		boardImg2 = imread(compareSelectImgs.mFilePath2.GetBuffer());//����ͼ
		resize(boardImg2, boardImg2, Size(boardImg1.cols, boardImg1.rows));
		flip(boardImg2, boardImg2, 0);
		Invalidate(TRUE);//����ondraw����
		initView();//��ʼ������
	}
}

/*
	#��ʼ�ԱȰ�ť#
*/
void CMCU_systemView::OnCompareStart()
{
	// TODO:  �ڴ�������������
	string excelPath = compareSelectImgs.mFilePath3.GetBuffer();
	compareModel.startCompareModel(boardImg2, excelPath);
	Invalidate(TRUE);
}

/*
	#��ͣ�ԱȰ�ť#
*/
void CMCU_systemView::OnComparePause()
{
	// TODO:  �ڴ���������������
}

/*
	#ֹͣ�ԱȰ�ť#
*/
void CMCU_systemView::OnCompareStop()
{
	// TODO:  �ڴ���������������
}


/*
*���°�ť״̬
*
*/
void CMCU_systemView::OnUpdateButtonStart(CCmdUI *pCmdUI)
{
	// TODO:  �ڴ������������û����洦��������
	pCmdUI->SetCheck(startEnable);
}
void CMCU_systemView::setStartEnable(bool enable)
{
	startEnable = enable;
}

void CMCU_systemView::OnUpdateButtonPause(CCmdUI *pCmdUI)
{
	// TODO:  �ڴ������������û����洦��������
	pCmdUI->SetCheck(pauseEnable);
}

void CMCU_systemView::setPauseEnable(bool enable)
{
	pauseEnable = enable;
}


void CMCU_systemView::OnUpdateButtonStop(CCmdUI *pCmdUI)
{
	// TODO:  �ڴ������������û����洦��������
	pCmdUI->SetCheck(stopEnable);
}

void CMCU_systemView::setStopEnable(bool enable)
{
	stopEnable = enable;
}

void CMCU_systemView::OnUpdateButtonEnlarge(CCmdUI *pCmdUI)
{
	// TODO:  �ڴ������������û����洦��������
	pCmdUI->SetCheck(enlargeEnable);
}

void CMCU_systemView::setEnlargeEnable(bool enable)
{
	enlargeEnable = enable;
}

void CMCU_systemView::OnUpdateButtonNarrow(CCmdUI *pCmdUI)
{
	// TODO:  �ڴ������������û����洦��������
	pCmdUI->SetCheck(narrowEnable);
}

void CMCU_systemView::setNarrowEnable(bool enable)
{
	narrowEnable = enable;
}

void CMCU_systemView::OnUpdateButtonCutout(CCmdUI *pCmdUI)
{
	// TODO:  �ڴ������������û����洦��������
	pCmdUI->SetCheck(cutOutEnable);
}

void CMCU_systemView::setCutOutEnable(bool enable)
{
	cutOutEnable = enable;
}

void CMCU_systemView::OnUpdateButtonMakesure(CCmdUI *pCmdUI)
{
	// TODO:  �ڴ������������û����洦��������
	pCmdUI->SetCheck(makeSureEnable);
}
void CMCU_systemView::setMakeSureEnable(bool enable)
{
	makeSureEnable = enable;
}

void CMCU_systemView::OnUpdateButtonCancel(CCmdUI *pCmdUI)
{
	// TODO:  �ڴ������������û����洦��������
	pCmdUI->SetCheck(cancelEnable);
}
void CMCU_systemView::setCancelEnable(bool enable)
{
	cancelEnable = enable;
}

void CMCU_systemView::OnUpdateButtonAddtext(CCmdUI *pCmdUI)
{
	// TODO:  �ڴ������������û����洦��������
	pCmdUI->SetCheck(addTextEnable);
}
void CMCU_systemView::setAddtextEnable(bool enable)
{
	addTextEnable = enable;
}


void CMCU_systemView::OnUpdateButtonLeftroate(CCmdUI *pCmdUI)
{
	// TODO:  �ڴ������������û����洦��������
	pCmdUI->SetCheck(leftRoateEnable);
}
void CMCU_systemView::setLeftRoateEnable(bool enable)
{
	leftRoateEnable = enable;
}

void CMCU_systemView::OnUpdateButtonRightroate(CCmdUI *pCmdUI)
{
	// TODO:  �ڴ������������û����洦��������
	pCmdUI->SetCheck(rightRoateEnable);
}
void CMCU_systemView::setRightRoateEnable(bool enable)
{
	rightRoateEnable = enable;
}

void CMCU_systemView::OnUpdateDetect(CCmdUI *pCmdUI)
{
	// TODO:  �ڴ������������û����洦��������
	pCmdUI->SetCheck(detectStartEnable);
}
void CMCU_systemView::setDetectStartEnable(bool enable)
{
	detectStartEnable = enable;
}

void CMCU_systemView::OnUpdateDetectstop(CCmdUI *pCmdUI)
{
	// TODO:  �ڴ������������û����洦��������
	pCmdUI->SetCheck(detectStopEnable);
}
void CMCU_systemView::setDetectStopEnable(bool enable)
{
	detectStopEnable = enable;
}

void CMCU_systemView::OnUpdateButtonRevoke(CCmdUI *pCmdUI)
{
	// TODO:  �ڴ������������û����洦��������
	pCmdUI->SetCheck(revokeEnable);
}
void CMCU_systemView::setRevokeEnable(bool enable)
{
	revokeEnable = enable;
}

void CMCU_systemView::OnUpdateButtonRedo(CCmdUI *pCmdUI)
{
	// TODO:  �ڴ������������û����洦��������
	pCmdUI->SetCheck(redoEnable);
}
void CMCU_systemView::setRedoEnable(bool enable)
{
	redoEnable = enable;
}

void CMCU_systemView::OnUpdateCompareStart(CCmdUI *pCmdUI)
{
	// TODO:  �ڴ������������û����洦��������
	pCmdUI->SetCheck(startCompareEnable);
}
void CMCU_systemView::setStartCompareEnable(bool enable)
{
	startCompareEnable = enable;
}

void CMCU_systemView::OnUpdateComparePause(CCmdUI *pCmdUI)
{
	// TODO:  �ڴ������������û����洦��������
	pCmdUI->SetCheck(pauseCompareEnable);
}
void CMCU_systemView::setPauseCompareEnable(bool enable)
{
	pauseCompareEnable = enable;
}

void CMCU_systemView::OnUpdateCompareStop(CCmdUI *pCmdUI)
{
	// TODO:  �ڴ������������û����洦��������
	pCmdUI->SetCheck(stopCompareEnable);
}
void CMCU_systemView::setStopCompareEnable(bool enable)
{
	stopCompareEnable = enable;
}

void CMCU_systemView::OnUpdateButtonAdd(CCmdUI *pCmdUI)
{
	// TODO:  �ڴ������������û����洦�������롢
	pCmdUI->SetCheck(addEnable);
}
void CMCU_systemView::setAddEnable(bool enable)
{
	addEnable = enable;
}

void CMCU_systemView::OnUpdateButtonFillred(CCmdUI *pCmdUI)
{
	// TODO:  �ڴ������������û����洦��������
	pCmdUI->SetCheck(fillRedEnable);
}
void CMCU_systemView::setFillRedEnable(bool enable)
{
	fillRedEnable = enable;
}
/*
	#������#
*/
BOOL CMCU_systemView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	#ˢ��#
*/
void CMCU_systemView::OnRefresh()
{
	// TODO:  �ڴ���������������
	int returnStatus = openFilePath.reFreshImgFile();
	if (returnStatus == 2)
	{
		detectAlgorithm.instanceRect_vector.clear();
		currentRowInList = -1;
		setListControlContent();
		flag_compareBoard = false;//ģ��Ա�
		flag_showComponent = false;//����ʾ��ҳ���
		currentPage = 0;
		Mat img_;
		img_.copyTo(src_img);
		src_img.copyTo(show_img);
		CWnd::Invalidate(TRUE);//����ondraw����

		srcROI2d.x = 0;
		srcROI2d.y = 0;
		srcROI2d.width = show_img.cols;
		srcROI2d.height = show_img.rows;
		initView();//��ʼ������
		//����״̬����Ϣ
		pFrame = (CMainFrame *)AfxGetMainWnd();
		pFrame->m_wndStatusBar.OnProgress(0);
		pFrame->m_wndStatusBar.SetPaneText(1, "����", TRUE);
		//��ʼ������
		editAlgorithm.setQueue_NULL();

		setStartCompareEnable(true);//��ʼ�ԱȰ�ť
		setPauseCompareEnable(true);//��ͣ�ԱȰ�ť
		setStopCompareEnable(true);//ֹͣ�ԱȰ�ť

		MessageBox(_T("��ǰ�ļ�Ŀ¼��û��ͼ����ȷ��Ŀ¼�Ƿ���ȷ"), _T("MCUϵͳ"));
		return;
	}
	else if (returnStatus == 0)//ִ�в���
	{
		detectAlgorithm.instanceRect_vector.clear();
		currentRowInList = -1;
		setListControlContent();
		currentPage = 0;
		src_img = imread(openFilePath.imageFilePathNames[currentPage]);
		src_img.copyTo(show_img);
		CWnd::Invalidate(TRUE);//����ondraw����

		flag_compareBoard = false;//ģ��Ա�
		flag_showComponent = true;//��ʾ��ҳ���

		srcROI2d.x = 0;
		srcROI2d.y = 0;
		srcROI2d.width = show_img.cols;
		srcROI2d.height = show_img.rows;
		havePictuireView();//��ͼ�����
		//����״̬����Ϣ
		pFrame = (CMainFrame *)AfxGetMainWnd();
		pFrame->m_wndStatusBar.OnProgress(0);
		pFrame->m_wndStatusBar.SetPaneText(1, "����", TRUE);
		//��ʼ������
		editAlgorithm.setQueue_NULL();

		setStartCompareEnable(true);//��ʼ�ԱȰ�ť
		setPauseCompareEnable(true);//��ͣ�ԱȰ�ť
		setStopCompareEnable(true);//ֹͣ�ԱȰ�ť

	}
}
/*
	#���ʶ������#
*/
void CMCU_systemView::OnButtonAdd()
{
	// TODO:  �ڴ���������������
	if (!addEnable)
	{
		flag_addDetect = true;
		setAddEnable(true);
		setCancelEnable(false);
	}
}

/*
	#��Ӽ�����#
*/
void CMCU_systemView::OnAdddetectclasshouse()
{
	// TODO:  �ڴ���������������
	INT_PTR nResponse = addDetectClassHouse.DoModal();
	if (nResponse == IDOK)
	{
		// TODO:  �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
		//����ֹ�ʶ������
		
	}
}

/*
	#һ����찴ť#
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
	#һ�����#
*/
void CMCU_systemView::fillRed()
{
	// TODO:  �ڴ���������������
	CDC* pDC = GetDC();
	for (int i = 0; i < detectAlgorithm.instanceRect_vector.size(); i++)
	{
		CRect dcRect;
		int left = detectAlgorithm.instanceRect_vector[i].left_top.x;
		int right = detectAlgorithm.instanceRect_vector[i].right_bottom.x;
		int top = detectAlgorithm.instanceRect_vector[i].left_top.y;
		int bottom = detectAlgorithm.instanceRect_vector[i].right_bottom.y;

		if (left >= 0 && top >= 0 && right < srcROI2d.width && bottom < srcROI2d.height && detectAlgorithm.instanceRect_vector[i].flag_noMark == false)//Լ����ͼƬ��
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
				bf.SourceConstantAlpha = 0x7f; //��͸��
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
	#����messageListCtrol������#
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
	#messageList˫���¼�#
*/
void CMCU_systemView::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	*pResult = 0;
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	CRect rc;

	m_row = pNMListView->iItem;//ѡ�е��к�
	m_column = pNMListView->iSubItem;//ѡ�е��к�

	if (m_column == 1 && m_row >= 0 && m_row < 8)
	{
		m_messageList.GetSubItemRect(m_row, m_column, LVIR_LABEL, rc);
		m_modifyMessageListEdit.SetParent(&m_messageList);
		m_modifyMessageListEdit.MoveWindow(rc);
		m_modifyMessageListEdit.SetWindowText(m_messageList.GetItemText(m_row, m_column));
		m_modifyMessageListEdit.ShowWindow(SW_SHOW);
		m_modifyMessageListEdit.SetFocus();
		m_modifyMessageListEdit.ShowCaret();//��ʾ���
		m_modifyMessageListEdit.SetSel(-1);//������Ƶ����
	}
	*pResult = 0;
}
/*
#m_modifyMessageListEdit�����ʧ�¼�#
*/
void CMCU_systemView::OnKillfocusEditList()
{
	CString str;
	m_modifyMessageListEdit.GetWindowText(str);
	m_messageList.SetItemText(m_row, 1, str);
	wordSize = _ttoi(str);
	writeXML_Configure();//д������
	m_modifyMessageListEdit.ShowWindow(SW_HIDE);
}
/*
	#��������#
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
	#д������#
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
	#���������ʾԪ������Ϣ��list#
*/
void CMCU_systemView::hideMessageList(){
	m_messageList.ShowWindow(SW_HIDE);
}
/*
#��ʾ�����ʾԪ������Ϣ��list#
*/
void CMCU_systemView::showMessageList(){
	m_messageList.ShowWindow(SW_SHOW);
}
/*
#�����ұ�����Ԫ�����б�#
*/
void CMCU_systemView::hideAllDetectList(){
	m_list.ShowWindow(SW_HIDE);
}
/*
#��ʾ�ұ�����Ԫ�����б�#
*/
void CMCU_systemView::showAllDetectList(){
	m_list.ShowWindow(SW_SHOW);
}
/*
#�������ͼ��ƴ����Ϣ��ʾ��#
*/
void CMCU_systemView::hideTXPJMessageEdit(){
	m_editMessage.ShowWindow(SW_HIDE);
}
/*
#��ʾ���ͼ��ƴ����Ϣ��ʾ��#
*/
void CMCU_systemView::showTXPJMessageEdit(){
	m_editMessage.ShowWindow(SW_SHOW);
}