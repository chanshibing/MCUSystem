#pragma once
#include<iostream>
#include <string>
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/opencv.hpp>    
#include <opencv2/imgproc/imgproc.hpp> 
#include "splitWord.h"
#include "afxdialogex.h"
//#include "AddDetectClassHouse.h"

using namespace std;

using namespace cv;
typedef cv::Point2d CVPoint2d;

//Ԫ��������Ϣ
typedef struct In{
	In()
	{
		memset(this, 0, sizeof(In));//��ʼ��0
	}
	CVPoint2d left_top;//���Ͻ�����
	CVPoint2d right_bottom;//���½�����
	int instanceClass;//�������{���裬 ���ݣ� оƬ},��1��ʼ
	string instanceWord;//�����ϵ�����
	int numOfPin1;//Ԫ��������1
	int numOfPin2;//Ԫ��������2
	bool flag_noMark;//����ǵ�ǰԪ����
	int manaulAdd;//�Ƿ����˹����Ԫ����
	int currentIndex;//��ǰ�������±�
}InstanceRect;

class DetectAlgorithm
{
public:
	DetectAlgorithm();
	~DetectAlgorithm();
	void detect_bat(ATL::CStringT< TCHAR, StrTraitMFC_DLL< TCHAR > > imageDir = "");//Ŀ�������
	void kill_detect();//��������߳�bat
	int drawDetectInstances(Mat &src_img, vector<CString> className, int wordSize);//��ԭͼ�ϻ���������߿�
	void GetStringSize(HDC hDC, const char* str, int* w, int* h);
	void drawInstanceClassString(Mat &src_img, const char* str, Point org, Scalar color, int fontSize, bool italic, bool underline);//��ԭͼ�ϻ����������
	void drawRect(Mat &src_img, CVPoint2d begin, CVPoint2d end, int manaulAdd);//������߿�
	Mat cutRect(Mat src_img, CVPoint2d begin, CVPoint2d end);//�ü�����
	void drawAddDetect(CWnd* pWnd, Point pictureInClient_ltPoint, Point pictureInClient_rbPoint, Point lButtonDown_point, CPoint point, double pictureRatio);

	vector<InstanceRect> instanceRect_vector;//����λ�������Ϣ�ṹ��
	SplitWord splitWord;//�ָ��ַ�����
	Mat detect_src_img;//����ԭʼͼ��

	SHELLEXECUTEINFO ShExecInfo;
	SHELLEXECUTEINFO ShExecInfo_kill;
	std::vector<int> numOfClassName;//������������
};

