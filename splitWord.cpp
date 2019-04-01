#include "stdafx.h"
#include "splitWord.h"


SplitWord::SplitWord()
{
}


SplitWord::~SplitWord()
{
}

/*
	#数字分割程序#
	参数：
		1、原图
*/
void SplitWord::splitWordProcess(Mat srcMat)
{
	int srcMat_width = srcMat.cols;//电阻图片的宽度
	int srcMat_height = srcMat.rows;//电阻图片的高度
	//判断电阻的横竖
	if (srcMat_width <= srcMat_height)//如果是竖的
	{
		//左旋90
		transpose(srcMat, srcMat);
		flip(srcMat, srcMat, 0);
		int temp = srcMat_width;
		srcMat_width = srcMat_height;
		srcMat_height = temp;
	}

	splitedMat.clear();

	Mat greyMat, ostuMat, tmpMat;

	cvtColor(srcMat, greyMat, CV_BGR2GRAY);
	greyMat = 255 - greyMat;

	Mat structElement;
	//二值化
	adaptiveThreshold(greyMat, ostuMat, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 15, 5);
	//threshold(greyMat, ostuMat, 180, 255, CV_THRESH_BINARY);

	Mat SmallKernel = getStructuringElement(MORPH_RECT, Size(2, 2));
	Mat BigKernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	Mat dst1, dst2;
	morphologyEx(ostuMat, tmpMat, MORPH_OPEN, SmallKernel);
	morphologyEx(tmpMat, tmpMat, MORPH_CLOSE, BigKernel);

	//structElement = getStructuringElement(MORPH_RECT, Size(2, 2));

	//膨胀腐蚀
	//dilate(ostuMat, tmpMat, structElement);
	//erode(tmpMat, tmpMat, structElement);
	
	
	tmpMat = 255 - tmpMat;

	//记录每一列的255的个数
	int *vProjection = new int[tmpMat.cols];

	//找出上下边界
	Vec2i topBottomRange = findTopBottomRange(tmpMat);

	tmpMat = tmpMat(Rect(0, topBottomRange[0], tmpMat.cols, topBottomRange[1] - topBottomRange[0]));

	//找出左右边界
	Vec2i leftRightRange = findLeftRightRange(tmpMat);

	tmpMat = tmpMat(Rect(leftRightRange[0], 0, leftRightRange[1] - leftRightRange[0], tmpMat.rows));

	//反向
	Mat revertMat = 255 - tmpMat;
	//垂直映射
	projection(tmpMat, vProjection);


	//获取字符区间
	vector<Vec2i> range = ProjectionNumberRange(vProjection, tmpMat.cols);


	/*Mat lineMat = tmpMat.clone();
	for (size_t i = 0; i < range.size(); i++)
	{
	line(lineMat, Point(range[i][0], 0), Point(range[i][0], lineMat.rows), Scalar(255, 255, 255), 1);
	line(lineMat, Point(range[i][1], 0), Point(range[i][1], lineMat.rows), Scalar(255, 255, 255), 1);
	}*/

	Mat ROI;
	for (size_t i = 0; i < range.size(); i++)
	{
		//分割结果
		ROI = revertMat(Range(0, tmpMat.rows), Range(range[i][0], range[i][1]));
		splitedMat.push_back(ROI);
	}

	delete vProjection;
}
/*
	#寻找左右边界位置#
	参数：
		原图
*/
Vec2i SplitWord::findLeftRightRange(Mat srcMat)
{
	int left = 0;//左边界
	int right = 0;//右边界
	int num = 0;
	int start = 0;
	int end = 0;
	bool isStart = false;
	int maxLength = 0;
	int left1 = 0;
	int right1 = 0;
	//第一行寻找左右位置
	for (int j = 0; j < srcMat.cols; j++)
	{
		if (!isStart)
		{
			if (srcMat.at<uchar>(2, j) == 0)
			{
				isStart = true;
				start = j;
			}
		}
		else
		{
			if (srcMat.at<uchar>(2, j) == 255 || j == srcMat.cols - 1)
			{
				end = j;
				isStart = false;
				int max = end - start;
				if (max > maxLength)//求最大的黑色带长度和左右位置
				{
					maxLength = max;
					left1 = start;
					right1 = end;
				}
			}
		}
	}
	maxLength = 0;
	start = 0;
	end = 0;
	int left2 = 0;
	int right2 = 0;
	//最后一行寻找左右位置
	for (int j = 0; j < srcMat.cols; j++)
	{
		if (!isStart)
		{
			if (srcMat.at<uchar>(srcMat.rows - 2, j) == 0)
			{
				isStart = true;
				start = j;
			}
		}
		else
		{
			if (srcMat.at<uchar>(srcMat.rows - 2, j) == 255 || j == srcMat.cols - 1)
			{
				end = j;
				isStart = false;
				int max = end - start;
				if (max > maxLength)//求最大的黑色带长度和左右位置
				{
					maxLength = max;
					left2 = start;
					right2 = end;
				}
			}
		}
	}
	left = left1 > left2 ? left1 + 2 : left2 + 2;
	right = right1 < right2 ? right1 - 3 : right2 - 3;
	return Vec2i(left, right);
}

/*
	#寻找上下边界位置#
	参数：
		原图
*/
Vec2i SplitWord::findTopBottomRange(Mat srcMat)
{
	int top = 0;
	int bottom = 0;
	int x = srcMat.cols / 3;
	int *ProjectionCount = new int[srcMat.rows];//每一行的黑色点数
	//获取中间部分
	Mat img = srcMat(Rect(x,0,x,srcMat.rows));
	//横向投影
	for (int j = 0; j < img.rows; j++)
	{
		int num = 0;
		for (int i = 0; i < img.cols; i++)
		{
			uchar c = img.at<uchar>(j, i);
			num += c / 255;
		}
		*(ProjectionCount + j) = num;
	}
	//找到连续黑色的位置
	bool isStart = false;
	int start = 0;
	int end = 0;
	vector<Vec2i> range;
	for (int i = 0; i < srcMat.rows; i++)
	{
		if (!isStart)
		{
			if (*(ProjectionCount + i) > 1)//1为每一行的忽略阈值
			{
				isStart = true;
				start = i;
			}
		}
		else
		{
			if (*(ProjectionCount + i) <= 1 || i == srcMat.rows - 1)//或者到达最后一行
			{
				isStart = false;
				end = i;
				range.push_back(Vec2i(start, end));
			}
		}
	}
	//找到上下边界
	int maxLength = 0;
	for (int i = 0; i < range.size(); i++)
	{
		int length = range[i][1] - range[i][0];
		if (length > maxLength)
		{
			top = range[i][0] - 4;
			bottom = range[i][1] + 4;
			maxLength = length;
		}
	}
	//是否超出边界
	top = top > 0 ? top : 0;
	bottom = bottom < srcMat.rows ? bottom : srcMat.rows;

	delete ProjectionCount;
	return Vec2i(top, bottom);
}

/*
	#垂直映射函数#
	参数：
		1、原图
		2、记录每一列的255的个数
*/
void SplitWord::projection(Mat srcMat, int * ProjectionCount)
{
	for (int j = 0; j < srcMat.cols; j++)
	{
		int num = 0;
		for (int i = 0; i < srcMat.rows; i++)
		{
			uchar c = srcMat.at<uchar>(i, j);
			num += c / 255;
		}
		*(ProjectionCount + j) = num;
	}
}

/*
	#获得字符分割位置的下标#
	参数：
		1、每一列的255的个数
		2、原图width
*/
vector<Vec2i> SplitWord::ProjectionNumberRange(int * ProjectionCount, int length)
{
	vector<Vec2i> range;//存放分割的字符开始结束位置
	bool isStart = false;//标记是否是字符开始
	int start = 0;//字符开始位置
	int end = 0;//字符结束位置
	//int firstEnd = 0;//第一个字符结束
	//for (int i = 0; i < length; i++)
	//{
	//	if (!isStart)
	//	{
	//		if (*(ProjectionCount + i) > 1)//字符开始
	//		{
	//			isStart = true;
	//		}
	//	}
	//	if (isStart)
	//	{
	//		if (*(ProjectionCount + i) == 0)
	//		{
	//			firstEnd = i;
	//			break;
	//		}
	//	}
	//}

	//isStart = false;
	//bool pullLast = true;//是否推出最后一个

	for (int i = 0; i < length; i++)
	{
		if (!isStart)
		{
			if (*(ProjectionCount + i) > 1)//字符开始
			{
				start = i;
				isStart = true;
			}
		}

		if (isStart)
		{
			/*if (i == length - 1)
			{
				pullLast = false;
				break;
			}*/
			if (*(ProjectionCount + i) == 0 || i == length - 1)//字符结束
			{
				end = i;
				isStart = false;
				if (end - start > 1)
				{
					range.push_back(Vec2i(start, end));
				}
			}
			
		}
	}
	/*if (pullLast)
	{
		range.pop_back();
	}*/
	return range;
}
