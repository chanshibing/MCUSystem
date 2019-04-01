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

	vector<Mat> splitedMat;//��ŷָ����ַ�ͼƬ

	void splitWordProcess(Mat srcMat);//�ָ�������ĸ
	void projection(Mat srcMat, int *ProjectionCount);//��ֱͶӰ
	vector<Vec2i> ProjectionNumberRange(int *ProjectionCount, int length);	//����ַ��ָ�λ�õ��±�
	Vec2i findTopBottomRange(Mat srcMat);//�ҵ����±߽�λ��
	Vec2i findLeftRightRange(Mat srcMat);//�ҵ����ұ߽�λ��
};

