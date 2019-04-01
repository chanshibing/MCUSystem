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

	int enlargeNum = 0;//�Ŵ����
	int narrowNum = 0;//��С����

	int view_index = 0;//��ǰ���е�λ��
	int max_view_index = 0;//�����е�λ��
	Queue Q;//���ڳ����ָ�
	ViewStruct view;
	Rect2d dstRect2d = Rect2d(0,0,0,0);

	void leftRotate(Mat &src_img, Rect2d &ROI_src, DetectAlgorithm &detectAlgorithm);//����90
	void rightRotate(Mat &src_img, Rect2d &ROI_src, DetectAlgorithm &detectAlgorithm);//����90
	void enlargeImg(Mat show_img, Rect2d &ROI, Rect2d &ROI_src, DetectAlgorithm &detectAlgorithm);//�Ŵ�
	void narrowImg(Mat show_img, Rect2d &ROI, Rect2d &ROI_src, DetectAlgorithm &detectAlgorithm);//��С
	void draftingImg(Mat show_img, Rect2d &ROI_src, Rect2d source_point, Rect lButtonDown_point, CPoint point, DetectAlgorithm &detectAlgorithm, vector<Point> leftBtnDown_lt, vector<Point> leftBtnDown_rb);//ͼƬ��ק
	void drawCutRect(Mat src_img, Mat &show_img, Rect2d ROI_src, Rect2d &ROI_cut, Rect lButtonDown_point, Rect pictureInClient_ltPoint, CPoint point, int &lineThickness, float pictureRatio);//���ü�����
	void cutImg(Mat &src_img, Mat &show_img, Rect2d &ROI_src, Rect2d ROI_cut, DetectAlgorithm &detectAlgorithm);//�ü�ͼƬ
	void GetStringSize(HDC hDC, const char* str, int* w, int* h);
	void paDrawString(Mat& dst, Rect ROI_src, const char* str, cv::Point org, Scalar color, int fontSize, bool italic, bool underline, DetectAlgorithm &detectAlgorithm);//��ͼƬ��д������
	void revoke(Mat &show_img, Rect2d &ROI, DetectAlgorithm &detectAlgorithm);//����
	void redo(Mat &show_img, Rect2d &ROI, DetectAlgorithm &detectAlgorithm);//�ָ�
	void setQueue_NULL();//Queue����Ϊ��
};

