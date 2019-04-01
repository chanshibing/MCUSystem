#pragma once
#include<opencv2\opencv.hpp>
#include<vector>

using namespace cv;
using namespace std;

class SplitWord
{
public:
	SplitWord();
	~SplitWord();

	vector<Mat> splitedMat;//存放分割后的字符图片

	void splitWordProcess(Mat srcMat);//分割数字字母
	void projection(Mat srcMat, int *ProjectionCount);//垂直投影
	vector<Vec2i> ProjectionNumberRange(int *ProjectionCount, int length);	//获得字符分割位置的下标
	Vec2i findTopBottomRange(Mat srcMat);//找到上下边界位置
	Vec2i findLeftRightRange(Mat srcMat);//找到左右边界位置
};

