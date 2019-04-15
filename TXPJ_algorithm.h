#include "highgui/highgui.hpp"  
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/xfeatures2d/nonfree.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "k-means.h"
#pragma once

using namespace std;
using namespace cv;

class TXPJAlgorithm
{
protected:
public:
	TXPJAlgorithm()
	{
		previousCutHeight = 0;
	}
	virtual ~TXPJAlgorithm();

	int previousCutHeight; //前一列右图裁剪的高度

	Point2f getTransformPoint(const Point2f originalPoint, const Mat &transformMaxtri);//计算原始图像点位在经过矩阵变换后在目标图像上对应位置
	int imageStitching_rows(Mat leftImage, Mat rightImage, Mat &outImage, const float widthByWhole);//行拼接算法
	int imageStitching_cols(Mat leftImage, Mat rightImage, Mat &outImage, const float heightByImage, int currentCol);//行拼接算法
	int imageStitching_cols2(Mat leftImage, Mat rightImage, Mat &outImage, const float heightByImage, int currentCol);//行拼接算法
};
