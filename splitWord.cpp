#include "stdafx.h"
#include "splitWord.h"


SplitWord::SplitWord()
{
}


SplitWord::~SplitWord()
{
}

/*
	#���ַָ����#
	������
		1��ԭͼ
*/
void SplitWord::splitWordProcess(Mat srcMat)
{
	int srcMat_width = srcMat.cols;//����ͼƬ�Ŀ��
	int srcMat_height = srcMat.rows;//����ͼƬ�ĸ߶�
	//�жϵ���ĺ���
	if (srcMat_width <= srcMat_height)//���������
	{
		//����90
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
	//��ֵ��
	adaptiveThreshold(greyMat, ostuMat, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 15, 5);
	//threshold(greyMat, ostuMat, 180, 255, CV_THRESH_BINARY);

	Mat SmallKernel = getStructuringElement(MORPH_RECT, Size(2, 2));
	Mat BigKernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	Mat dst1, dst2;
	morphologyEx(ostuMat, tmpMat, MORPH_OPEN, SmallKernel);
	morphologyEx(tmpMat, tmpMat, MORPH_CLOSE, BigKernel);

	//structElement = getStructuringElement(MORPH_RECT, Size(2, 2));

	//���͸�ʴ
	//dilate(ostuMat, tmpMat, structElement);
	//erode(tmpMat, tmpMat, structElement);
	
	
	tmpMat = 255 - tmpMat;

	//��¼ÿһ�е�255�ĸ���
	int *vProjection = new int[tmpMat.cols];

	//�ҳ����±߽�
	Vec2i topBottomRange = findTopBottomRange(tmpMat);

	tmpMat = tmpMat(Rect(0, topBottomRange[0], tmpMat.cols, topBottomRange[1] - topBottomRange[0]));

	//�ҳ����ұ߽�
	Vec2i leftRightRange = findLeftRightRange(tmpMat);

	tmpMat = tmpMat(Rect(leftRightRange[0], 0, leftRightRange[1] - leftRightRange[0], tmpMat.rows));

	//����
	Mat revertMat = 255 - tmpMat;
	//��ֱӳ��
	projection(tmpMat, vProjection);


	//��ȡ�ַ�����
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
		//�ָ���
		ROI = revertMat(Range(0, tmpMat.rows), Range(range[i][0], range[i][1]));
		splitedMat.push_back(ROI);
	}

	delete vProjection;
}
/*
	#Ѱ�����ұ߽�λ��#
	������
		ԭͼ
*/
Vec2i SplitWord::findLeftRightRange(Mat srcMat)
{
	int left = 0;//��߽�
	int right = 0;//�ұ߽�
	int num = 0;
	int start = 0;
	int end = 0;
	bool isStart = false;
	int maxLength = 0;
	int left1 = 0;
	int right1 = 0;
	//��һ��Ѱ������λ��
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
				if (max > maxLength)//�����ĺ�ɫ�����Ⱥ�����λ��
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
	//���һ��Ѱ������λ��
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
				if (max > maxLength)//�����ĺ�ɫ�����Ⱥ�����λ��
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
	#Ѱ�����±߽�λ��#
	������
		ԭͼ
*/
Vec2i SplitWord::findTopBottomRange(Mat srcMat)
{
	int top = 0;
	int bottom = 0;
	int x = srcMat.cols / 3;
	int *ProjectionCount = new int[srcMat.rows];//ÿһ�еĺ�ɫ����
	//��ȡ�м䲿��
	Mat img = srcMat(Rect(x,0,x,srcMat.rows));
	//����ͶӰ
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
	//�ҵ�������ɫ��λ��
	bool isStart = false;
	int start = 0;
	int end = 0;
	vector<Vec2i> range;
	for (int i = 0; i < srcMat.rows; i++)
	{
		if (!isStart)
		{
			if (*(ProjectionCount + i) > 1)//1Ϊÿһ�еĺ�����ֵ
			{
				isStart = true;
				start = i;
			}
		}
		else
		{
			if (*(ProjectionCount + i) <= 1 || i == srcMat.rows - 1)//���ߵ������һ��
			{
				isStart = false;
				end = i;
				range.push_back(Vec2i(start, end));
			}
		}
	}
	//�ҵ����±߽�
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
	//�Ƿ񳬳��߽�
	top = top > 0 ? top : 0;
	bottom = bottom < srcMat.rows ? bottom : srcMat.rows;

	delete ProjectionCount;
	return Vec2i(top, bottom);
}

/*
	#��ֱӳ�亯��#
	������
		1��ԭͼ
		2����¼ÿһ�е�255�ĸ���
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
	#����ַ��ָ�λ�õ��±�#
	������
		1��ÿһ�е�255�ĸ���
		2��ԭͼwidth
*/
vector<Vec2i> SplitWord::ProjectionNumberRange(int * ProjectionCount, int length)
{
	vector<Vec2i> range;//��ŷָ���ַ���ʼ����λ��
	bool isStart = false;//����Ƿ����ַ���ʼ
	int start = 0;//�ַ���ʼλ��
	int end = 0;//�ַ�����λ��
	//int firstEnd = 0;//��һ���ַ�����
	//for (int i = 0; i < length; i++)
	//{
	//	if (!isStart)
	//	{
	//		if (*(ProjectionCount + i) > 1)//�ַ���ʼ
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
	//bool pullLast = true;//�Ƿ��Ƴ����һ��

	for (int i = 0; i < length; i++)
	{
		if (!isStart)
		{
			if (*(ProjectionCount + i) > 1)//�ַ���ʼ
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
			if (*(ProjectionCount + i) == 0 || i == length - 1)//�ַ�����
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
