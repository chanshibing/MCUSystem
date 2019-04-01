
// MainFrm.h : CMainFrame ��Ľӿ�
//
#include<string>
#include<vector>
#include <io.h>
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/opencv.hpp>    
#include <opencv2/imgproc/imgproc.hpp> 

#include "ProgStatusBar.h"
#pragma once

using namespace std;
using namespace cv;

class CMainFrame : public CFrameWnd
{
	
protected: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// ����
public:


// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �ؼ���Ƕ���Ա
	CToolBar          m_wndToolBar;

	CImageList m_imgList;

	

	int SetToolBar();//���ù�����
	HICON HBITMAPTOHICON(CBitmap& bitmap);//CBitmap ת HICON
// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

public:
	CProgStatusBar         m_wndStatusBar;
	afx_msg LRESULT OnProgress(WPARAM wp, LPARAM lp);
};


