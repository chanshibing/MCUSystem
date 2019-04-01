
// MCU_systemView.h : CMCU_systemView ��Ľӿ�
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
protected: // �������л�����
	CMCU_systemView();
	DECLARE_DYNCREATE(CMCU_systemView)

// ����
public:
	CMCU_systemDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CMCU_systemView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	//�����ʽ
	HCURSOR wait_cursor = LoadCursor(NULL, IDC_WAIT);
	HCURSOR arrow_cursor = LoadCursor(NULL, IDC_ARROW);
// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	CMainFrame *pFrame;
	Mat show_img;//��ʾ��ͼƬ
	Mat src_img;//ԭʼͼ��
	Mat save_img;
	Rect2d srcROI2d;//ԭʼͼ���С
	Rect2d ROI2d;//��ʱͼ���С

	Rect ROI_cut;//�ü���ͼ���С
	Rect2d cutROI2d;//�ü���ͼ���С
	bool flag_LeftButtonDown = false;//���������±�־
	Rect source_point;//ԭ���Ͻ�����
	Rect2d sourcePoint2d;//ԭ���Ͻ�����
	Rect lButtonDown_point;//����������ʱ������
	Rect pictureInClient_ltPoint;//��ʾͼƬ����ڿͻ��� ���Ͻ�����
	Rect pictureInClient_rbPoint;//��ʾͼƬ����ڿͻ��� ���½�����
	bool flag_cutButtonDown = false;//�ü���ť���±�־
	double pictureRatio = 0;//ԭͼ����ʾͼ�����
	int lineThickness = 4;//������ϸ
	bool flag_addEditButtonDown = false;//����ı���ť���±�־
	cv::Point point_textInPicture;//�ı���ԭͼƬ�е����Ͻ�����
	CEdit *p_newEdit;//������ı���

	Mat leftImage;
	Mat rightImage;
	Mat outImage_row[10];//��ʱ���ÿ�е�ƴ��ͼƬ
	Mat outImage;//���ڷ��ص����ͼƬ
	int sumProcessTimes = 0;//�ܵĴ������
	int currentProcessingTimes = 0;//��ǰ�������
	HANDLE imageStitchingThread;//ͼ��ƴ���߳�

	bool goonStitching = false;//������ť
	bool flag_isStopStitching = false;//�Ƿ�ֹͣƴ��
	bool flag_isStopDetect = false;//�Ƿ�ֹͣ���

	ReadImgAlgorithm openFilePath;//���ļ�Ŀ¼����
	SetupParaDlg setupParaDlg;//���ò�����ܶ���
	SetBoardSize setBoardSize;//����ģ��ߴ����
	EditAlgorithm editAlgorithm;//�༭ͼ�����
	TXPJAlgorithm txpjAlgorithm;//ͼ��ƴ�Ӷ���
	DetectAlgorithm detectAlgorithm;//������
	StatisticsTip statisticsTip;//ͳ��Pin������

	int numOfRowImage = 0;//ÿ�е�ͼƬ����
	int numOfColImage = 0;//ÿ�е�ͼƬ����
	int numOfImage = 0;//�ܵ�ͼƬ����
	float widthByImage = 0.0;//ͼƬ�ص����ֵı���
	float heightByImage = 0.0;//ͼƬ�ص����ֵı���
	bool flag_setPara = false;//�Ƿ����ò���

	float boardWidth = 0.0;//ģ��Ŀ��
	float boardHeight = 0.0;//ģ��ĸ߶�

	bool startEnable = false;//��ʼ��ť
	void setStartEnable(bool enable);//��ʼ��ť
	bool pauseEnable = true;//��ͣ��ť
	void setPauseEnable(bool enable);//��ͣ��ť
	bool stopEnable = true;//ֹͣ��ť
	void setStopEnable(bool enable);//ֹͣ��ť

	bool revokeEnable = true;//������ť
	void setRevokeEnable(bool enable);//������ť
	bool redoEnable = true;//�ָ���ť
	void setRedoEnable(bool enable);//�ָ���ť

	bool leftRoateEnable = true;//������ť
	void setLeftRoateEnable(bool enable);//������ť
	bool rightRoateEnable = true;//������ť
	void setRightRoateEnable(bool enable);//������ť
	bool enlargeEnable = true;//�Ŵ�ť
	void setEnlargeEnable(bool enable);//�Ŵ�ť
	bool narrowEnable = true;//��С��ť
	void setNarrowEnable(bool enable);//��С��ť
	bool cutOutEnable = true;//�ü���ť
	void setCutOutEnable(bool enable);//�ü���ť
	bool makeSureEnable = true;//ȷ����ť
	void setMakeSureEnable(bool enable);//ȷ����ť
	bool cancelEnable = true;//ȡ����ť
	void setCancelEnable(bool enable);//ȡ����ť
	bool addTextEnable = true;//����ı���ť
	void setAddtextEnable(bool enable);//����ı���ť
	bool addEnable = true;//���ʶ��������ť
	void setAddEnable(bool enable);//���ʶ��������ť
	bool fillRedEnable = true;//һ����찴ť
	void setFillRedEnable(bool enable);//һ����찴ť


	bool detectStartEnable = false;//��⿪ʼ��ť
	void setDetectStartEnable(bool enable);//��⿪ʼ��ť
	bool detectStopEnable = true;//���ֹͣ��ť
	void setDetectStopEnable(bool enable);//���ֹͣ��ť

	
	HANDLE imageDetectThread;//ͼ�����߳�

	void MatToCImage(Mat mat, CImage& cimage);//Mat to CImage
	void showPicture(Mat mat);//��ʾͼƬ
	void initView();//��ʼ������
	void havePictuireView();//��ͼ�����

	int currentPage = 0;//��ǰҳ
	bool flag_showComponent = false;//�Ƿ���ʾ��ҳ���
	void setPageTurningComponent();//���÷�ҳ���

	LRESULT imageStitchingOver(WPARAM wp, LPARAM lp);//ͼ��ƴ���߳̽���
	LRESULT imageStitchingStop(WPARAM wp, LPARAM lp);//ͼ��ƴ���߳̽���
	LRESULT imageDetectOver(WPARAM wp, LPARAM lp);//ͼ�����߳̽���
	LRESULT imageDetectStopOver(WPARAM wp, LPARAM lp);//ͼ�����߳̽���
	LRESULT showTXPJMessage(WPARAM wp, LPARAM lp);//��ʾͼ��ƴ����Ϣ

	CompareSelectImgs compareSelectImgs;//ģ��Ա�ѡ��ͼƬ����
	CompareModel compareModel;//ģ��Ա�
	bool flag_compareBoard = false;//ģ��Ա�
	void showBoardPicture(Mat mat1, Mat mat2);//��ʾģ��Ա�ͼƬ
	Mat boardImg1;//����ͼƬ
	Mat boardImg2;//����ͼƬ

	bool startCompareEnable = true;//��ʼ�ԱȰ�ť
	void setStartCompareEnable(bool enable);//��ʼ�ԱȰ�ť
	bool pauseCompareEnable = true;//��ͣ�ԱȰ�ť
	void setPauseCompareEnable(bool enable);//��ͣ�ԱȰ�ť
	bool stopCompareEnable = true;//ֹͣ�ԱȰ�ť
	void setStopCompareEnable(bool enable);//ֹͣ�ԱȰ�ť

	void setListControl();//����listControl��λ��
	void setListControlContent();//����listControl������
	void setListControlCheckBox();//����listCtrol��CheckBox
	void OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult);
	int DrawRect();//�����ο�
	void DrawAlphaRect(CDC *pDC);//����͸������
	void OnNMClickList(NMHDR *pNMHDR, LRESULT *pResult);//CheckBox��Ϣ

	bool flag_draw = false;//�Ƿ񻭾���
	bool flag_listFocus = false;//�Ƿ񻭰�͸������
	int currentRowInList = -1;//��ǰ������б���
	bool flag_detectOver = false;//������
	vector<cv::Point> leftBtnDown_lt;//����������ʱ�����ο������
	vector<cv::Point> leftBtnDown_rb;//����������ʱ,���ο������

	bool flag_ctrl = false;//ctrl���Ƿ񱻰���

	int currentTXPJRow = 0;//��ǰƴ����
	int currentTXPJCol = 0;//��ǰƴ����
	bool flag_rowOrCol = false;//��ǰ����ƴ���л�����
	int flag_TXPJSucess = 0;//�Ƿ�ƴ�ӳɹ�

	bool flag_addDetect = false;//���ʶ������
	addDetect adddetect;//���Ԫ�����������

	AddDetectClassHouse addDetectClassHouse;//��Ӽ������

	void setMessageListControlContent();//����MessagelistControl������
	void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);//m_messageList˫���¼�
	void OnKillfocusEditList();//m_modifyMessageListEdit�����ʧ�¼�

	void fillRed();//һ�����
	int wordSize = 0;//�����С
	int loadXML_Configure();//��������
	int writeXML_Configure();//д������

	int m_row;//ѡ�е��к�
	int m_column;//ѡ�е��к�

	std::mutex _lock;//���߳���
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
	CBitmapButton  m_btn_previous;//��һ�Ű�ť
	CButton m_btn_next;//��һ�Ű�ť
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

#ifndef _DEBUG  // MCU_systemView.cpp �еĵ��԰汾
inline CMCU_systemDoc* CMCU_systemView::GetDocument() const
   { return reinterpret_cast<CMCU_systemDoc*>(m_pDocument); }
#endif

