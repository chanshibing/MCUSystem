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

	int previousCutHeight; //ǰһ����ͼ�ü��ĸ߶�

	Point2f getTransformPoint(const Point2f originalPoint, const Mat &transformMaxtri);//����ԭʼͼ���λ�ھ�������任����Ŀ��ͼ���϶�Ӧλ��
	int imageStitching_rows(Mat leftImage, Mat rightImage, Mat &outImage, const float widthByWhole);//��ƴ���㷨
	int imageStitching_cols(Mat leftImage, Mat rightImage, Mat &outImage, const float heightByImage, int currentCol);//��ƴ���㷨
	int imageStitching_cols2(Mat leftImage, Mat rightImage, Mat &outImage, const float heightByImage, int currentCol);//��ƴ���㷨
};
