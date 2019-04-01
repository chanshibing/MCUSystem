
// MCU_systemView.h : CMCU_systemView 类的接口
//
#include <atlimage.h>


#include "MainFrm.h"

#include "opencv2/highgui/highgui.hpp"
#include <opencv2/opencv.hpp>    
#include <opencv2/imgproc/imgproc.hpp> 
#include "ReadImg_algorithm.h"
#include "SetupParaDlg.h"
#include "SetBoardSize.h"
#include "CompareSelectImgs.h"
#include "Edit_algorithm.h"
#include "TXPJ_algorithm.h"
#include "Detect_algorithm.h"
#include "StatisticsTip.h"
#include "CompareModel.h"
#include "addDetect.h"
#include "AddDetectClassHouse.h"
#include <mutex>
#include "tinyxml.h"
#pragma once

using namespace std;
using namespace cv;

class CMCU_systemView : public CView
{
protected: // 仅从序列化创建
	CMCU_systemView();
	DECLARE_DYNCREATE(CMCU_systemView)

// 特性
public:
	CMCU_systemDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CMCU_systemView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	//鼠标样式
	HCURSOR wait_cursor = LoadCursor(NULL, IDC_WAIT);
	HCURSOR arrow_cursor = LoadCursor(NULL, IDC_ARROW);
// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	CMainFrame *pFrame;
	Mat show_img;//显示的图片
	Mat src_img;//原始图像
	Mat save_img;
	Rect2d srcROI2d;//原始图像大小
	Rect2d ROI2d;//临时图像大小

	Rect ROI_cut;//裁剪的图像大小
	Rect2d cutROI2d;//裁剪的图像大小
	bool flag_LeftButtonDown = false;//鼠标左键按下标志
	Rect source_point;//原左上角坐标
	Rect2d sourcePoint2d;//原左上角坐标
	Rect lButtonDown_point;//鼠标左键按下时的坐标
	Rect pictureInClient_ltPoint;//显示图片相对于客户端 左上角坐标
	Rect pictureInClient_rbPoint;//显示图片相对于客户端 右下角坐标
	bool flag_cutButtonDown = false;//裁剪按钮按下标志
	double pictureRatio = 0;//原图和显示图像比例
	int lineThickness = 4;//线条粗细
	bool flag_addEditButtonDown = false;//添加文本按钮按下标志
	cv::Point point_textInPicture;//文本在原图片中的左上角坐标
	CEdit *p_newEdit;//新添加文本框

	Mat leftImage;
	Mat rightImage;
	Mat outImage_row[10];//临时存放每行的拼接图片
	Mat outImage;//用于返回的输出图片
	int sumProcessTimes = 0;//总的处理次数
	int currentProcessingTimes = 0;//当前处理次数
	HANDLE imageStitchingThread;//图像拼接线程

	bool goonStitching = false;//继续按钮
	bool flag_isStopStitching = false;//是否停止拼接
	bool flag_isStopDetect = false;//是否停止检测

	ReadImgAlgorithm openFilePath;//打开文件目录对象
	SetupParaDlg setupParaDlg;//设置参数框架对象
	SetBoardSize setBoardSize;//设置模块尺寸对象
	EditAlgorithm editAlgorithm;//编辑图像对象
	TXPJAlgorithm txpjAlgorithm;//图像拼接对象
	DetectAlgorithm detectAlgorithm;//检测对象
	StatisticsTip statisticsTip;//统计Pin数对象

	int numOfRowImage = 0;//每行的图片数量
	int numOfColImage = 0;//每列的图片数量
	int numOfImage = 0;//总的图片数量
	float widthByImage = 0.0;//图片重叠部分的比例
	float heightByImage = 0.0;//图片重叠部分的比例
	bool flag_setPara = false;//是否配置参数

	float boardWidth = 0.0;//模块的宽度
	float boardHeight = 0.0;//模块的高度

	bool startEnable = false;//开始按钮
	void setStartEnable(bool enable);//开始按钮
	bool pauseEnable = true;//暂停按钮
	void setPauseEnable(bool enable);//暂停按钮
	bool stopEnable = true;//停止按钮
	void setStopEnable(bool enable);//停止按钮

	bool revokeEnable = true;//撤销按钮
	void setRevokeEnable(bool enable);//撤掉按钮
	bool redoEnable = true;//恢复按钮
	void setRedoEnable(bool enable);//恢复按钮

	bool leftRoateEnable = true;//左旋按钮
	void setLeftRoateEnable(bool enable);//左旋按钮
	bool rightRoateEnable = true;//右旋按钮
	void setRightRoateEnable(bool enable);//右旋按钮
	bool enlargeEnable = true;//放大按钮
	void setEnlargeEnable(bool enable);//放大按钮
	bool narrowEnable = true;//缩小按钮
	void setNarrowEnable(bool enable);//缩小按钮
	bool cutOutEnable = true;//裁剪按钮
	void setCutOutEnable(bool enable);//裁剪按钮
	bool makeSureEnable = true;//确定按钮
	void setMakeSureEnable(bool enable);//确定按钮
	bool cancelEnable = true;//取消按钮
	void setCancelEnable(bool enable);//取消按钮
	bool addTextEnable = true;//添加文本按钮
	void setAddtextEnable(bool enable);//添加文本按钮
	bool addEnable = true;//添加识别器件按钮
	void setAddEnable(bool enable);//添加识别器件按钮
	bool fillRedEnable = true;//一键标红按钮
	void setFillRedEnable(bool enable);//一键标红按钮


	bool detectStartEnable = false;//检测开始按钮
	void setDetectStartEnable(bool enable);//检测开始按钮
	bool detectStopEnable = true;//检测停止按钮
	void setDetectStopEnable(bool enable);//检测停止按钮

	
	HANDLE imageDetectThread;//图像检测线程

	void MatToCImage(Mat mat, CImage& cimage);//Mat to CImage
	void showPicture(Mat mat);//显示图片
	void initView();//初始化界面
	void havePictuireView();//有图像界面

	int currentPage = 0;//当前页
	bool flag_showComponent = false;//是否显示翻页组件
	void setPageTurningComponent();//设置翻页组件

	LRESULT imageStitchingOver(WPARAM wp, LPARAM lp);//图像拼接线程结束
	LRESULT imageStitchingStop(WPARAM wp, LPARAM lp);//图像拼接线程结束
	LRESULT imageDetectOver(WPARAM wp, LPARAM lp);//图像检测线程结束
	LRESULT imageDetectStopOver(WPARAM wp, LPARAM lp);//图像检测线程结束
	LRESULT showTXPJMessage(WPARAM wp, LPARAM lp);//显示图像拼接信息

	CompareSelectImgs compareSelectImgs;//模块对比选择图片对象
	CompareModel compareModel;//模块对比
	bool flag_compareBoard = false;//模块对比
	void showBoardPicture(Mat mat1, Mat mat2);//显示模块对比图片
	Mat boardImg1;//正面图片
	Mat boardImg2;//反面图片

	bool startCompareEnable = true;//开始对比按钮
	void setStartCompareEnable(bool enable);//开始对比按钮
	bool pauseCompareEnable = true;//暂停对比按钮
	void setPauseCompareEnable(bool enable);//暂停对比按钮
	bool stopCompareEnable = true;//停止对比按钮
	void setStopCompareEnable(bool enable);//停止对比按钮

	void setListControl();//设置listControl的位置
	void setListControlContent();//设置listControl的内容
	void setListControlCheckBox();//设置listCtrol的CheckBox
	void OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult);
	int DrawRect();//画矩形框
	void DrawAlphaRect(CDC *pDC);//画半透明矩形
	void OnNMClickList(NMHDR *pNMHDR, LRESULT *pResult);//CheckBox消息

	bool flag_draw = false;//是否画矩形
	bool flag_listFocus = false;//是否画半透明矩形
	int currentRowInList = -1;//当前激活的列表行
	bool flag_detectOver = false;//检测结束
	vector<cv::Point> leftBtnDown_lt;//鼠标左键按下时，矩形框的坐标
	vector<cv::Point> leftBtnDown_rb;//鼠标左键按下时,矩形框的坐标

	bool flag_ctrl = false;//ctrl建是否被按下

	int currentTXPJRow = 0;//当前拼接行
	int currentTXPJCol = 0;//当前拼接列
	bool flag_rowOrCol = false;//当前正在拼接行还是列
	int flag_TXPJSucess = 0;//是否拼接成功

	bool flag_addDetect = false;//添加识别器件
	addDetect adddetect;//添加元器件界面对象

	AddDetectClassHouse addDetectClassHouse;//添加检测类别库

	void setMessageListControlContent();//设置MessagelistControl的内容
	void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);//m_messageList双击事件
	void OnKillfocusEditList();//m_modifyMessageListEdit光标消失事件

	void fillRed();//一键标红
	int wordSize = 0;//字体大小
	int loadXML_Configure();//加载配置
	int writeXML_Configure();//写入配置

	int m_row;//选中的行号
	int m_column;//选中的列号

	std::mutex _lock;//主线程锁
private:

public:
	afx_msg void OnFileOpen();
	afx_msg void OnSetpara();
	afx_msg void OnButtonOpenfile_toolBar();
	afx_msg void OnFileSave();
	afx_msg void OnButtonSave_toolBar();
	afx_msg void OnButtonRevoke();
	afx_msg void OnButtonRedo();
	afx_msg void OnButtonStart();
	afx_msg void OnButtonPause();
	afx_msg void OnButtonStop();
	afx_msg void OnButtonRightroate();
	afx_msg void OnButtonEnlarge();
	afx_msg void OnButtonNarrow();
	afx_msg void OnButtonCutout();
	afx_msg void OnButtonAddtext();
	afx_msg void OnButtonLeftroate();
	afx_msg void OnUpdateButtonEnlarge(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonNarrow(CCmdUI *pCmdUI);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnUpdateButtonCutout(CCmdUI *pCmdUI);
	afx_msg void OnButtonMakesure();
	afx_msg void OnButtonCancel();
	afx_msg void OnUpdateButtonMakesure(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonCancel(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonAddtext(CCmdUI *pCmdUI);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnUpdateButtonLeftroate(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonRightroate(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonPause(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonStop(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonStart(CCmdUI *pCmdUI);
	afx_msg void OnDetect();
	afx_msg void OnSaveasbom();
	afx_msg void OnDetectstop();
	afx_msg void OnUpdateDetectstop(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDetect(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonRevoke(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonRedo(CCmdUI *pCmdUI);
	afx_msg void OnStatisticspin();
	afx_msg void OnSetboardsize();
	afx_msg void OnButtonPreviousClick();
	afx_msg void OnButtonNextClick();
private:
	CBitmapButton  m_btn_previous;//上一张按钮
	CButton m_btn_next;//下一张按钮
	CStatic m_s_middle;

	CListCtrl m_list;
	CRichEditCtrl m_editMessage;
	CStatic m_staticMessage;
	CListCtrl m_messageList;
	CEdit m_modifyMessageListEdit;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnCompareSelectimgs();
	afx_msg void OnUpdateCompareStart(CCmdUI *pCmdUI);
	afx_msg void OnUpdateComparePause(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCompareStop(CCmdUI *pCmdUI);
	afx_msg void OnCompareStart();
	afx_msg void OnComparePause();
	afx_msg void OnCompareStop();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
//	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
//	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnRefresh();
	afx_msg void OnUpdateButtonAdd(CCmdUI *pCmdUI);
	afx_msg void OnButtonAdd();
	afx_msg void OnAdddetectclasshouse();
//	afx_msg void OnButton32890();
	afx_msg void OnButtonFillred();
	afx_msg void OnUpdateButtonFillred(CCmdUI *pCmdUI);
};

#ifndef _DEBUG  // MCU_systemView.cpp 中的调试版本
inline CMCU_systemDoc* CMCU_systemView::GetDocument() const
   { return reinterpret_cast<CMCU_systemDoc*>(m_pDocument); }
#endif

