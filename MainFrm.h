
// MainFrm.h : CMainFrame 类的接口
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
	
protected: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 特性
public:


// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CToolBar          m_wndToolBar;

	CImageList m_imgList;

	

	int SetToolBar();//设置工具栏
	HICON HBITMAPTOHICON(CBitmap& bitmap);//CBitmap 转 HICON
// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

public:
	CProgStatusBar         m_wndStatusBar;
	afx_msg LRESULT OnProgress(WPARAM wp, LPARAM lp);
};


