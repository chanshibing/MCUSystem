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

//元器件的信息
typedef struct In{
	In()
	{
		memset(this, 0, sizeof(In));//初始化0
	}
	CVPoint2d left_top;//左上角坐标
	CVPoint2d right_bottom;//右下角坐标
	int instanceClass;//物体类别{电阻， 电容， 芯片},从1开始
	string instanceWord;//物体上的文字
	int numOfPin1;//元器件引脚1
	int numOfPin2;//元器件引脚2
	bool flag_noMark;//不标记当前元器件
	int manaulAdd;//是否是人工添加元器件
	int currentIndex;//当前器件的下标
}InstanceRect;

class DetectAlgorithm
{
public:
	DetectAlgorithm();
	~DetectAlgorithm();
	void detect_bat(ATL::CStringT< TCHAR, StrTraitMFC_DLL< TCHAR > > imageDir = "");//目标检测程序
	void kill_detect();//结束检测线程bat
	int drawDetectInstances(Mat &src_img, vector<CString> className, int wordSize);//在原图上画出物体检测边框
	void GetStringSize(HDC hDC, const char* str, int* w, int* h);
	void drawInstanceClassString(Mat &src_img, const char* str, Point org, Scalar color, int fontSize, bool italic, bool underline);//在原图上画出类别文字
	void drawRect(Mat &src_img, CVPoint2d begin, CVPoint2d end, int manaulAdd);//画物体边框
	Mat cutRect(Mat src_img, CVPoint2d begin, CVPoint2d end);//裁剪物体
	void drawAddDetect(CWnd* pWnd, Point pictureInClient_ltPoint, Point pictureInClient_rbPoint, Point lButtonDown_point, CPoint point, double pictureRatio);

	vector<InstanceRect> instanceRect_vector;//物体位置类别信息结构体
	SplitWord splitWord;//分割字符对象
	Mat detect_src_img;//检测的原始图像

	SHELLEXECUTEINFO ShExecInfo;
	SHELLEXECUTEINFO ShExecInfo_kill;
	std::vector<int> numOfClassName;//各个类别的数量
};

