#pragma once

#include "opencv2/highgui/highgui.hpp"
#include <opencv2/opencv.hpp>    
#include <opencv2/imgproc/imgproc.hpp> 

#include "afxdialogex.h"
#include "Queue.h"
#include "Detect_algorithm.h"
using namespace cv;

class EditAlgorithm
{
public:
	EditAlgorithm();
	~EditAlgorithm();

	int enlargeNum = 0;//放大次数
	int narrowNum = 0;//缩小次数

	int view_index = 0;//当前队列的位置
	int max_view_index = 0;//最大队列的位置
	Queue Q;//用于撤销恢复
	ViewStruct view;
	Rect2d dstRect2d = Rect2d(0,0,0,0);

	void leftRotate(Mat &src_img, Rect2d &ROI_src, DetectAlgorithm &detectAlgorithm);//左旋90
	void rightRotate(Mat &src_img, Rect2d &ROI_src, DetectAlgorithm &detectAlgorithm);//右旋90
	void enlargeImg(Mat show_img, Rect2d &ROI, Rect2d &ROI_src, DetectAlgorithm &detectAlgorithm);//放大
	void narrowImg(Mat show_img, Rect2d &ROI, Rect2d &ROI_src, DetectAlgorithm &detectAlgorithm);//缩小
	void draftingImg(Mat show_img, Rect2d &ROI_src, Rect2d source_point, Rect lButtonDown_point, CPoint point, DetectAlgorithm &detectAlgorithm, vector<Point> leftBtnDown_lt, vector<Point> leftBtnDown_rb);//图片拖拽
	void drawCutRect(Mat src_img, Mat &show_img, Rect2d ROI_src, Rect2d &ROI_cut, Rect lButtonDown_point, Rect pictureInClient_ltPoint, CPoint point, int &lineThickness, float pictureRatio);//画裁剪矩形
	void cutImg(Mat &src_img, Mat &show_img, Rect2d &ROI_src, Rect2d ROI_cut, DetectAlgorithm &detectAlgorithm);//裁剪图片
	void GetStringSize(HDC hDC, const char* str, int* w, int* h);
	void paDrawString(Mat& dst, Rect ROI_src, const char* str, cv::Point org, Scalar color, int fontSize, bool italic, bool underline, DetectAlgorithm &detectAlgorithm);//向图片中写入文字
	void revoke(Mat &show_img, Rect2d &ROI, DetectAlgorithm &detectAlgorithm);//撤销
	void redo(Mat &show_img, Rect2d &ROI, DetectAlgorithm &detectAlgorithm);//恢复
	void setQueue_NULL();//Queue设置为空
};

