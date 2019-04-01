#pragma once
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/opencv.hpp>    
#include <opencv2/imgproc/imgproc.hpp> 
using namespace cv;
using namespace std;
typedef cv::Point CVPoint;

//Ð¾Æ¬×ø±ê
typedef struct{
	CVPoint leftTop;
	CVPoint rightBottom;
}ChipCoordinate;

class CompareModel
{
public:
	CompareModel();
	~CompareModel();

	void startCompareModel(Mat &srcMat, string excelPath);
	void drawRect(Mat &srcMat, ChipCoordinate chip);

	vector<ChipCoordinate> chip;

};

