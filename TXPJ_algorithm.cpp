#include "stdafx.h"
#include "TXPJ_algorithm.h"
#include<math.h>
#include "ProgStatusBar.h"

#include <fstream>
#include <iostream>

DWORD WINAPI imageStitching(LPVOID p);//ͼƬƴ��



TXPJAlgorithm::~TXPJAlgorithm()
{
}


Point2f TXPJAlgorithm::getTransformPoint(const Point2f originalPoint, const Mat &transformMaxtri)
{
	Mat originelP, targetP;
	originelP = (Mat_<double>(3, 1) << originalPoint.x, originalPoint.y, 1.0);
	targetP = transformMaxtri*originelP;
	float x = targetP.at<double>(0, 0) / targetP.at<double>(2, 0);
	float y = targetP.at<double>(1, 0) / targetP.at<double>(2, 0);
	return Point2f(x, y);
}

/*
	���ĸ��������ص����ֱ���
*/
int TXPJAlgorithm::imageStitching_rows(Mat leftImage, Mat rightImage, Mat &outImage, const float widthByWhole)
{
	if (leftImage.rows != rightImage.rows)
	{
		return 0;
	}
	//�Ҷ�ͼת��
	Mat leftImageGray, rightImageGray;
	cvtColor(leftImage, leftImageGray , COLOR_RGB2GRAY);
	cvtColor(rightImage, rightImageGray, COLOR_RGB2GRAY);

	//��ȡ
	int cut_width = rightImage.cols * widthByWhole;//��ȡ�Ĳ��ֿ��
	if (leftImage.cols < cut_width)
	{
		return 0;
	}
	try
	{
		leftImageGray = leftImageGray(Rect(leftImageGray.cols - cut_width, 0, cut_width, leftImageGray.rows));//���ͼƬ��ȡ�Ҳ�
		rightImageGray = rightImageGray(Rect(0, 0, cut_width, rightImageGray.rows));//�ұ�ͼƬ��ȡ��
	}
	catch (const std::exception& ex)
	{
		return 0;
	}
	

	//��ȡ������ 
	Ptr<Feature2D> siftDetector = xfeatures2d::SIFT::create(2000);//����SIFT�����࣬����������2000
	vector<KeyPoint> leftKeyPoint, rightKeyPoint;
	siftDetector->detect(leftImageGray, leftKeyPoint);
	siftDetector->detect(rightImageGray, rightKeyPoint);

	//������������Ϊ�±ߵ�������ƥ����׼��  
	Mat leftImageDesc, rightImageDesc;
	siftDetector->compute(leftImageGray, leftKeyPoint, leftImageDesc);
	siftDetector->compute(rightImageGray, rightKeyPoint, rightImageDesc);

	//���ƥ�������㣬����ȡ�������  	
	FlannBasedMatcher matcher;
	vector<DMatch> matchePoints;
	matcher.match(leftImageDesc, rightImageDesc, matchePoints, Mat());
	if (matchePoints.size() <= 0)
	{
		return 0;
	}
	sort(matchePoints.begin(), matchePoints.end()); //����������	

	//��ȡ����ǰN��������ƥ��������
	vector<Point2f> leftImagePoints, rightImagePoints;
	vector<DMatch> m_InlierMatches;
	int N = 500;
	if (matchePoints.size() < N){
		N = matchePoints.size();
	}
	m_InlierMatches.resize(N);
	for (int i = 0; i < N; i++)
	{
		leftImagePoints.push_back(leftKeyPoint[matchePoints[i].queryIdx].pt);
		rightImagePoints.push_back(rightKeyPoint[matchePoints[i].trainIdx].pt);
		m_InlierMatches[i].queryIdx = i;
		m_InlierMatches[i].trainIdx = i;
	}

	// ���ڵ�ת��ΪdrawMatches����ʹ�õĸ�ʽ  
	leftKeyPoint.clear();
	rightKeyPoint.clear();
	KeyPoint::convert(leftImagePoints, leftKeyPoint);
	KeyPoint::convert(rightImagePoints, rightKeyPoint);

	/*Mat o1;
	drawMatches(leftImageGray, leftKeyPoint, rightImageGray, rightKeyPoint, m_InlierMatches, o1);*/


	//����б��
	vector<float> slopes;  //б��
	float slope;
	for (int i = 0; i < (int)leftKeyPoint.size(); i++)
	{
		slope = (rightKeyPoint[i].pt.y - leftKeyPoint[i].pt.y) / (rightKeyPoint[i].pt.x + leftImageGray.cols - leftKeyPoint[i].pt.x);
		slopes.push_back(slope);
	}


	//�ų����õ�б�ʵĵ�
	for (int i = 0; i < (int)slopes.size(); i++)
	{
		if (slopes[i] > 0.03 || slopes[i] < -0.03)
		{
			leftKeyPoint.erase(leftKeyPoint.begin() + i);
			rightKeyPoint.erase(rightKeyPoint.begin() + i);
			m_InlierMatches.erase(m_InlierMatches.begin() + i);
			slopes.erase(slopes.begin() + i);
			i--;
		}
	}
	for (int i = 0; i < (int)leftKeyPoint.size(); i++)
	{
		m_InlierMatches[i].queryIdx = i;
		m_InlierMatches[i].trainIdx = i;
	}
	if (leftKeyPoint.size() <= 0)
	{
		return 0;
	}


	/*Mat o2;
	drawMatches(leftImageGray, leftKeyPoint, rightImageGray, rightKeyPoint, m_InlierMatches, o2);*/

	//�������Ƶ�����ľ���
	double *rightLeftPointDistance = new double[leftKeyPoint.size()];
	vector<double> distancesVector;
	double distance = 0;
	for (int i = 0; i < leftKeyPoint.size(); i++)
	{
		distance = sqrt(pow(rightKeyPoint[i].pt.x + leftImageGray.cols - leftKeyPoint[i].pt.x, 2) + pow(rightKeyPoint[i].pt.y - leftKeyPoint[i].pt.y, 2));
		*(rightLeftPointDistance+i) = distance;
		distancesVector.push_back(distance);
	}

	//K��ֵ���෨
	const int size = leftKeyPoint.size(); //Number of samples
	const int dim = 1;   //Dimension of feature
	const int cluster_num = 10; //Cluster number
	KMeans* kmeans = new KMeans(dim, cluster_num);
	int* labels = new int[size];
	kmeans->thresh = 5;
	kmeans->SetInitMode(KMeans::InitUniform);
	kmeans->Cluster(rightLeftPointDistance, size, labels);
	
	int numberOflabels[cluster_num];//��¼labels������
	for (int i = 0; i < cluster_num; i++)
	{
		numberOflabels[i] = 0;
	}
	for (int i = 0; i < size; i++)
	{
		numberOflabels[labels[i]]++;
	}
	
	//�ҵ�����labels
	int maxLabels = 0;//����
	int indexLabels = 0;//λ��
	for (int i = 0; i < cluster_num; i++)
	{
		if (maxLabels < numberOflabels[i])
		{
			maxLabels = numberOflabels[i];
			indexLabels = i;
		}
	}

	//�ų��������labels�ĵ�
	for (int i = 0, j = 0; i < (int)leftKeyPoint.size(); i++, j++)
	{
		if (labels[j] != indexLabels)
		{
			leftKeyPoint.erase(leftKeyPoint.begin() + i);
			rightKeyPoint.erase(rightKeyPoint.begin() + i);
			m_InlierMatches.erase(m_InlierMatches.begin() + i);
			distancesVector.erase(distancesVector.begin() + i);
			i--;
		}
	}
	for (int i = 0; i < (int)leftKeyPoint.size(); i++)
	{
		m_InlierMatches[i].queryIdx = i;
		m_InlierMatches[i].trainIdx = i;
	}
	if (leftKeyPoint.size() <= 0){
		return 0;
	}

	delete rightLeftPointDistance;
	delete labels;
	delete kmeans;

	Mat o3;
	drawMatches(leftImageGray, leftKeyPoint, rightImageGray, rightKeyPoint, m_InlierMatches, o3);

	int averageDistance = distancesVector[0];
	if (averageDistance > leftImage.cols || averageDistance > rightImage.cols){
		return 0;
	}

	//����ǿƥ��������ص���������ۼӣ����ν��ȶ����ɣ�����ͻ��
	Mat imageLeftOverlap, imageRightOverlap; //ͼ1��ͼ2���ص�����	
	imageLeftOverlap = leftImage(Rect(leftImage.cols - averageDistance, 0, averageDistance, leftImage.rows));
	imageRightOverlap = rightImage(Rect(0, 0, averageDistance, rightImage.rows));
	Mat imageLeftROICopy = imageLeftOverlap.clone();  //����һ��ͼ1���ص�����
	for (int i = 0; i<imageLeftOverlap.rows; i++)
	{
		for (int j = 0; j<imageRightOverlap.cols; j++)
		{
			double weight;
			weight = (double)j / imageLeftOverlap.cols;  //�����ı���ı�ĵ���ϵ��
			imageLeftROICopy.at<Vec3b>(i, j)[0] = (1 - weight)*imageLeftOverlap.at<Vec3b>(i, j)[0] + weight*imageRightOverlap.at<Vec3b>(i, j)[0];
			imageLeftROICopy.at<Vec3b>(i, j)[1] = (1 - weight)*imageLeftOverlap.at<Vec3b>(i, j)[1] + weight*imageRightOverlap.at<Vec3b>(i, j)[1];
			imageLeftROICopy.at<Vec3b>(i, j)[2] = (1 - weight)*imageLeftOverlap.at<Vec3b>(i, j)[2] + weight*imageRightOverlap.at<Vec3b>(i, j)[2];
		}
	}

	Mat RightROIMat = rightImage(Rect(Point(imageRightOverlap.cols, 0), Point(rightImage.cols, rightImage.rows)));	 //�ұ�ƴ�Ӳ���

	int overlapWidth = leftImage.cols + rightImage.cols - averageDistance;//�غϵĿ�
	int overlapHeight = rightImage.rows;//�غϵĸ�

	Mat outMat(overlapHeight, overlapWidth, CV_8UC3);
	outMat.setTo(0);
	Mat leftROIMat = leftImage(Rect(0, 0, leftImage.cols - averageDistance, leftImage.rows));//���ƴ�Ӳ���

	leftROIMat.copyTo(outMat(Rect(0, 0, leftROIMat.cols, leftROIMat.rows)));
	imageLeftROICopy.copyTo(outMat(Rect(leftROIMat.cols, 0, imageLeftROICopy.cols, imageLeftROICopy.rows)));//�м�ƴ�Ӳ���

	RightROIMat.copyTo(outMat(Rect(leftROIMat.cols + imageLeftOverlap.cols, 0, RightROIMat.cols, RightROIMat.rows)));//�ұ�ƴ�Ӳ���

	outMat.copyTo(outImage);

	return 1;
}

int TXPJAlgorithm::imageStitching_cols(Mat leftImage, Mat rightImage, Mat &outImage, const float heightByImage, int currentCol)
{
	//˳ʱ����ת90
	transpose(leftImage, leftImage);//��ʱ�뾵��90
	flip(leftImage, leftImage, 1);//˳ʱ��180
	transpose(rightImage, rightImage);
	flip(rightImage, rightImage, 1);

	if (previousCutHeight >= rightImage.rows)
	{
		return 0;
	}

	try
	{
		//�ж�ǰ��һ����ͼ�Ƿ��вü�,����вü����ͽ�����ǰ��ͼ���вü�
		if (currentCol % 2 == 0 && previousCutHeight > 0)//ż��
		{
			rightImage = rightImage(Rect(0, previousCutHeight, rightImage.cols, rightImage.rows - previousCutHeight));
		}
		if (currentCol % 2 == 1 && previousCutHeight > 0)//����
		{
			rightImage = rightImage(Rect(0, 0, rightImage.cols, rightImage.rows - previousCutHeight));
		}

		//�Ƚ�����ͼ�ĸ߶ȣ�������ͼ�ߣ��е����ͼ������ұ�ͼ�ߣ��е��ұ�ͼ�����Ҽ��¸߶�
		int cut_height = 0;
		if (leftImage.rows > rightImage.rows)//�ü���ͼ
		{
			cut_height = leftImage.rows - rightImage.rows;
			previousCutHeight = 0;//��ͼ�޲ü�
			if (currentCol % 2 == 1)//�����ü��ϲ�
			{
				leftImage = leftImage(Rect(0, cut_height, leftImage.cols, leftImage.rows - cut_height));
			}
			else//ż���ü���ͼ�²�
			{
				leftImage = leftImage(Rect(0, 0, leftImage.cols, leftImage.rows - cut_height));
			}
		}
		else//�ü���ͼ
		{
			cut_height = rightImage.rows - leftImage.rows;
			previousCutHeight = cut_height;//��ͼ�вü�
			if (currentCol % 2 == 1)//�����ü���ͼ�ϲ�
			{
				rightImage = rightImage(Rect(0, cut_height, rightImage.cols, rightImage.rows - cut_height));
			}
			else//ż���ü���ͼ�²�
			{
				rightImage = rightImage(Rect(0, 0, rightImage.cols, rightImage.rows - cut_height));
			}
		}
	}
	catch (const std::exception& ex)
	{
		return 0;
	}
	


	if (rightImage.rows != leftImage.rows)
	{
		return 0;
	}
	//�Ҷ�ͼת��
	Mat leftImageGray, rightImageGray;
	cvtColor(leftImage, leftImageGray, CV_RGB2GRAY);
	cvtColor(rightImage, rightImageGray, CV_RGB2GRAY);

	//��ȡһ���ֽ���ƥ��
	int cut_width = rightImage.cols * heightByImage;//��ȡ�Ĳ��ֿ��

	if (leftImage.cols < cut_width)
	{
		return 0;
	}

	try
	{
		leftImageGray = leftImageGray(Rect(leftImageGray.cols - cut_width, 0, cut_width, leftImageGray.rows));
		rightImageGray = rightImageGray(Rect(0, 0, cut_width, rightImageGray.rows));
	}
	catch (const std::exception &ex)
	{
		return 0;
	}
	

	//��ȡ������  
	Ptr<Feature2D> siftDetector = xfeatures2d::SIFT::create(6000);//����SIFT������
	vector<KeyPoint> leftKeyPoint, rightKeyPoint;
	siftDetector->detect(leftImageGray, leftKeyPoint);
	siftDetector->detect(rightImageGray, rightKeyPoint);

	//������������Ϊ�±ߵ�������ƥ����׼��  
	Mat leftImageDesc, rightImageDesc;
	siftDetector->compute(leftImageGray, leftKeyPoint, leftImageDesc);
	siftDetector->compute(rightImageGray, rightKeyPoint, rightImageDesc);
	
	//���ƥ�������㣬����ȡ�������  	
	FlannBasedMatcher matcher;
	vector<DMatch> matchePoints;
	matcher.match(leftImageDesc, rightImageDesc, matchePoints, Mat());
	if (matchePoints.size() <= 0)
	{
		return 0;
	}
	sort(matchePoints.begin(), matchePoints.end()); //����������	

	//��ȡ����ǰN��������ƥ��������
	vector<Point2f> leftImagePoint, rightImagePoint;
	vector<DMatch> m_InlierMatches;

	int N = 1500;
	if (leftKeyPoint.size() < N){
		N = leftKeyPoint.size();
	}
	m_InlierMatches.resize(N);
	for (int i = 0; i<N; i++)
	{
		leftImagePoint.push_back(leftKeyPoint[matchePoints[i].queryIdx].pt);
		rightImagePoint.push_back(rightKeyPoint[matchePoints[i].trainIdx].pt);
		m_InlierMatches[i].queryIdx = i;
		m_InlierMatches[i].trainIdx = i;
	}


	// ���ڵ�ת��ΪdrawMatches����ʹ�õĸ�ʽ  
	leftKeyPoint.clear();
	rightKeyPoint.clear();
	KeyPoint::convert(leftImagePoint, leftKeyPoint);
	KeyPoint::convert(rightImagePoint, rightKeyPoint);

	/*Mat OutImage1;
	drawMatches(leftImageGray, leftKeyPoint, rightImageGray, rightKeyPoint, m_InlierMatches, OutImage1);
*/
	//б���ų�
	vector<float> slopes; //б��
	float slope;
	for (int i = 0; i < (int)leftKeyPoint.size(); i++)
	{
		slope = (rightKeyPoint[i].pt.y - leftKeyPoint[i].pt.y) / (rightKeyPoint[i].pt.x + leftImageGray.cols - leftKeyPoint[i].pt.x);
		slopes.push_back(slope);
	}

	for (int i = 0; i < (int)slopes.size(); i++)
	{
		if (slopes[i] > 0.03 || slopes[i] < -0.03)
		{
			leftKeyPoint.erase(leftKeyPoint.begin() + i);
			rightKeyPoint.erase(rightKeyPoint.begin() + i);
			m_InlierMatches.erase(m_InlierMatches.begin() + i);
			slopes.erase(slopes.begin() + i);
			i--;
		}
	}
	for (int i = 0; i < (int)leftKeyPoint.size(); i++)
	{
		m_InlierMatches[i].queryIdx = i;
		m_InlierMatches[i].trainIdx = i;
	}
	if (leftKeyPoint.size() <= 0)
	{
		return 0;
	}

	//�������Ƶ�����ľ���
	double *rightLeftPointDistance = new double[leftKeyPoint.size()];
	vector<double> distancesVector;
	double distance = 0;
	for (int i = 0; i < leftKeyPoint.size(); i++)
	{
		distance = sqrt(pow(rightKeyPoint[i].pt.x + leftImageGray.cols - leftKeyPoint[i].pt.x, 2) + pow(rightKeyPoint[i].pt.y - leftKeyPoint[i].pt.y, 2));
		*(rightLeftPointDistance + i) = distance;
		distancesVector.push_back(distance);
	}

	//K��ֵ���෨
	const int size = leftKeyPoint.size(); //Number of samples
	const int dim = 1;   //Dimension of feature
	const int cluster_num = 15; //Cluster number
	KMeans* kmeans = new KMeans(dim, cluster_num);
	int* labels = new int[size];
	kmeans->thresh = 5;
	kmeans->SetInitMode(KMeans::InitUniform);
	kmeans->Cluster(rightLeftPointDistance, size, labels);

	int numberOflabels[cluster_num];//��¼labels������
	for (int i = 0; i < cluster_num; i++)
	{
		numberOflabels[i] = 0;
	}
	for (int i = 0; i < size; i++)
	{
		numberOflabels[labels[i]]++;
	}

	//�ҵ�����labels
	int maxLabels = 0;//����
	int indexLabels = 0;//λ��
	for (int i = 0; i < cluster_num; i++)
	{
		if (maxLabels < numberOflabels[i])
		{
			maxLabels = numberOflabels[i];
			indexLabels = i;
		}
	}

	//�ų��������labels�ĵ�
	for (int i = 0, j = 0; i < (int)leftKeyPoint.size(); i++, j++)
	{
		if (labels[j] != indexLabels)
		{
			leftKeyPoint.erase(leftKeyPoint.begin() + i);
			rightKeyPoint.erase(rightKeyPoint.begin() + i);
			m_InlierMatches.erase(m_InlierMatches.begin() + i);
			distancesVector.erase(distancesVector.begin() + i);
			i--;
		}
	}
	for (int i = 0; i < (int)leftKeyPoint.size(); i++)
	{
		m_InlierMatches[i].queryIdx = i;
		m_InlierMatches[i].trainIdx = i;
	}
	if (leftKeyPoint.size() <= 0){
		return 0;
	}

	delete rightLeftPointDistance;
	delete labels;
	delete kmeans;

	int averageDistance = distancesVector[0];

	//����ǿƥ��������ص���������ۼӣ����ν��ȶ����ɣ�����ͻ��
	Mat imageLeftOverlap, imageRightOverlap; //ͼ1��ͼ2���ص�����	
	imageLeftOverlap = leftImage(Rect(Point(leftImage.cols - averageDistance, 0), Point(leftImage.cols, leftImage.rows)));
	imageRightOverlap = rightImage(Rect(0, 0, averageDistance, rightImage.rows));
	Mat imageLeftROICopy = imageLeftOverlap.clone();  //����һ��ͼ1���ص�����
	for (int i = 0; i<imageLeftOverlap.rows; i++)
	{
		for (int j = 0; j<imageLeftOverlap.cols; j++)
		{
			double weight;
			weight = (double)j / imageRightOverlap.cols;  //�����ı���ı�ĵ���ϵ��
			imageLeftROICopy.at<Vec3b>(i, j)[0] = (1 - weight)*imageLeftOverlap.at<Vec3b>(i, j)[0] + weight*imageRightOverlap.at<Vec3b>(i, j)[0];
			imageLeftROICopy.at<Vec3b>(i, j)[1] = (1 - weight)*imageLeftOverlap.at<Vec3b>(i, j)[1] + weight*imageRightOverlap.at<Vec3b>(i, j)[1];
			imageLeftROICopy.at<Vec3b>(i, j)[2] = (1 - weight)*imageLeftOverlap.at<Vec3b>(i, j)[2] + weight*imageRightOverlap.at<Vec3b>(i, j)[2];
		}
	}

	Mat RightROIMat = rightImage(Rect(Point(imageRightOverlap.cols, 0), Point(rightImage.cols, rightImage.rows)));	 //�ұ�ƴ�Ӳ���

	int overlapWidth = leftImage.cols + rightImage.cols - averageDistance;//�غϵĿ�
	int overlapHeight = rightImage.rows;//�غϵĸ�

	Mat outMat(overlapHeight, overlapWidth, CV_8UC3);
	outMat.setTo(0);
	Mat leftROIMat = leftImage(Rect(0, 0, leftImage.cols - averageDistance, leftImage.rows));//���ƴ�Ӳ���

	leftROIMat.copyTo(outMat(Rect(0, 0, leftROIMat.cols, leftROIMat.rows)));
	imageLeftROICopy.copyTo(outMat(Rect(leftROIMat.cols, 0, imageLeftROICopy.cols, imageLeftROICopy.rows)));//�м�ƴ�Ӳ���

	RightROIMat.copyTo(outMat(Rect(leftROIMat.cols + imageLeftOverlap.cols, 0, RightROIMat.cols, RightROIMat.rows)));//�ұ�ƴ�Ӳ���

	//��ʱ��90
	transpose(outMat, outMat);
	flip(outMat, outMat, 0);
	outMat.copyTo(outImage);

	return 1;
}




int TXPJAlgorithm::imageStitching_cols2(Mat leftImage, Mat rightImage, Mat &outImage, const float heightByImage, int currentCol)
{
	//˳ʱ����ת90
	transpose(leftImage, leftImage);//��ʱ�뾵��90
	flip(leftImage, leftImage, 1);//˳ʱ��180
	transpose(rightImage, rightImage);
	flip(rightImage, rightImage, 1);

	
	//�Ҷ�ͼת��
	Mat leftImageGray, rightImageGray;
	cvtColor(leftImage, leftImageGray, CV_RGB2GRAY);
	cvtColor(rightImage, rightImageGray, CV_RGB2GRAY);

	//��ȡһ���ֽ���ƥ��
	int cut_width = rightImage.cols * heightByImage;//��ȡ�Ĳ��ֿ��

	if (leftImage.cols < cut_width)
	{
		return 0;
	}

	try
	{
		leftImageGray = leftImageGray(Rect(leftImageGray.cols - cut_width, 0, cut_width, leftImageGray.rows));
		rightImageGray = rightImageGray(Rect(0, 0, cut_width, rightImageGray.rows));
	}
	catch (const std::exception &ex)
	{
		return 0;
	}


	//��ȡ������  
	Ptr<Feature2D> siftDetector = xfeatures2d::SIFT::create(6000);//����SIFT������
	vector<KeyPoint> leftKeyPoint, rightKeyPoint;
	siftDetector->detect(leftImageGray, leftKeyPoint);
	siftDetector->detect(rightImageGray, rightKeyPoint);

	//������������Ϊ�±ߵ�������ƥ����׼��  
	Mat leftImageDesc, rightImageDesc;
	siftDetector->compute(leftImageGray, leftKeyPoint, leftImageDesc);
	siftDetector->compute(rightImageGray, rightKeyPoint, rightImageDesc);

	//���ƥ�������㣬����ȡ�������  	
	FlannBasedMatcher matcher;
	vector<DMatch> matchePoints;
	matcher.match(leftImageDesc, rightImageDesc, matchePoints, Mat());
	if (matchePoints.size() <= 0)
	{
		return 0;
	}
	sort(matchePoints.begin(), matchePoints.end()); //����������	

	//��ȡ����ǰN��������ƥ��������
	vector<Point2f> leftImagePoint, rightImagePoint;
	vector<DMatch> m_InlierMatches;

	int N = 1500;
	if (leftKeyPoint.size() < N){
		N = leftKeyPoint.size();
	}
	m_InlierMatches.resize(N);
	for (int i = 0; i<N; i++)
	{
		leftImagePoint.push_back(leftKeyPoint[matchePoints[i].queryIdx].pt);
		rightImagePoint.push_back(rightKeyPoint[matchePoints[i].trainIdx].pt);
		m_InlierMatches[i].queryIdx = i;
		m_InlierMatches[i].trainIdx = i;
	}


	// ���ڵ�ת��ΪdrawMatches����ʹ�õĸ�ʽ  
	leftKeyPoint.clear();
	rightKeyPoint.clear();
	KeyPoint::convert(leftImagePoint, leftKeyPoint);
	KeyPoint::convert(rightImagePoint, rightKeyPoint);

	/*Mat OutImage1;
	drawMatches(leftImageGray, leftKeyPoint, rightImageGray, rightKeyPoint, m_InlierMatches, OutImage1);*/
	

	//б���ų�
	double* slopes = new double[(int)leftKeyPoint.size()]; //б��
	float slope;
	for (int i = 0; i < (int)leftKeyPoint.size(); i++)
	{
		slope = (rightKeyPoint[i].pt.y - leftKeyPoint[i].pt.y) / (rightKeyPoint[i].pt.x + leftImageGray.cols - leftKeyPoint[i].pt.x);
		slopes[i] = slope;
	}

	//K��ֵ���෨
	const int size_slope = leftKeyPoint.size(); //Number of samples
	const int dim_slope = 1;   //Dimension of feature
	const int cluster_num_slope = 15; //Cluster number
	KMeans* kmeans_slope = new KMeans(dim_slope, cluster_num_slope);
	int* labels_slope = new int[size_slope];
	kmeans_slope->thresh = 0.005;
	kmeans_slope->SetInitMode(KMeans::InitUniform);
	kmeans_slope->Cluster(slopes, size_slope, labels_slope);

	int numberOflabels_slope[cluster_num_slope];//��¼labels������
	for (int i = 0; i < cluster_num_slope; i++)
	{
		numberOflabels_slope[i] = 0;
	}
	for (int i = 0; i < size_slope; i++)
	{
		numberOflabels_slope[labels_slope[i]]++;
	}

	//�ҵ�����labels
	int maxLabels_slope = 0;//����
	int indexLabels_slope = 0;//λ��
	for (int i = 0; i < cluster_num_slope; i++)
	{
		if (maxLabels_slope < numberOflabels_slope[i])
		{
			maxLabels_slope = numberOflabels_slope[i];
			indexLabels_slope = i;
		}
	}

	for (int i = 0, j = 0; i < (int)leftKeyPoint.size(); i++, j++){
		if (labels_slope[j] != indexLabels_slope){
			leftKeyPoint.erase(leftKeyPoint.begin() + i);
			rightKeyPoint.erase(rightKeyPoint.begin() + i);
			m_InlierMatches.erase(m_InlierMatches.begin() + i);
			i--;
		}
	}
	for (int i = 0; i < (int)leftKeyPoint.size(); i++)
	{
		m_InlierMatches[i].queryIdx = i;
		m_InlierMatches[i].trainIdx = i;
	}
	if (leftKeyPoint.size() <= 0)
	{
		return 0;
	}

	delete labels_slope;
	delete kmeans_slope;
	delete slopes;


	//Mat outImae1;
	//drawMatches(leftImageGray, leftKeyPoint, rightImageGray, rightKeyPoint, m_InlierMatches, outImae1);

	//�������Ƶ�����ľ���
	double *rightLeftPointDistance = new double[leftKeyPoint.size()];
	vector<double> distancesVector;
	double distance = 0;
	for (int i = 0; i < leftKeyPoint.size(); i++)
	{
		distance = sqrt(pow(rightKeyPoint[i].pt.x + leftImageGray.cols - leftKeyPoint[i].pt.x, 2) + pow(rightKeyPoint[i].pt.y - leftKeyPoint[i].pt.y, 2));
		*(rightLeftPointDistance + i) = distance;
		distancesVector.push_back(distance);
	}

	//K��ֵ���෨
	const int size = leftKeyPoint.size(); //Number of samples
	const int dim = 1;   //Dimension of feature
	const int cluster_num = 15; //Cluster number
	KMeans* kmeans = new KMeans(dim, cluster_num);
	int* labels = new int[size];
	kmeans->thresh = 5;
	kmeans->SetInitMode(KMeans::InitUniform);
	kmeans->Cluster(rightLeftPointDistance, size, labels);

	int numberOflabels[cluster_num];//��¼labels������
	for (int i = 0; i < cluster_num; i++)
	{
		numberOflabels[i] = 0;
	}
	for (int i = 0; i < size; i++)
	{
		numberOflabels[labels[i]]++;
	}

	//�ҵ�����labels
	int maxLabels = 0;//����
	int indexLabels = 0;//λ��
	for (int i = 0; i < cluster_num; i++)
	{
		if (maxLabels < numberOflabels[i])
		{
			maxLabels = numberOflabels[i];
			indexLabels = i;
		}
	}

	//�ų��������labels�ĵ�
	for (int i = 0, j = 0; i < (int)leftKeyPoint.size(); i++, j++)
	{
		if (labels[j] != indexLabels)
		{
			leftKeyPoint.erase(leftKeyPoint.begin() + i);
			rightKeyPoint.erase(rightKeyPoint.begin() + i);
			m_InlierMatches.erase(m_InlierMatches.begin() + i);
			distancesVector.erase(distancesVector.begin() + i);
			i--;
		}
	}
	for (int i = 0; i < (int)leftKeyPoint.size(); i++)
	{
		m_InlierMatches[i].queryIdx = i;
		m_InlierMatches[i].trainIdx = i;
	}
	if (leftKeyPoint.size() <= 0){
		return 0;
	}

	delete rightLeftPointDistance;
	delete labels;
	delete kmeans;

	/*Mat outImae2;
	drawMatches(leftImageGray, leftKeyPoint, rightImageGray, rightKeyPoint, m_InlierMatches, outImae2);*/

	int height_move = rightKeyPoint[0].pt.y - leftKeyPoint[0].pt.y;//�ü��߶�
	int height_move_2 = 0;
	if (height_move > 0){//�ü��ұ�ͼƬ
		rightImage = rightImage(Rect(0, height_move, rightImage.cols, rightImage.rows - height_move));	
	}
	else if (height_move < 0){//�ü���ͼ
		height_move = abs(height_move);
		leftImage = leftImage(Rect(0, height_move, leftImage.cols, leftImage.rows - height_move));
	}
	if (rightImage.rows >= leftImage.rows){//����ұ�ͼƬ�ϸߣ� �ü��ұ�ͼƬ
		height_move_2 = rightImage.rows - leftImage.rows;
		rightImage = rightImage(Rect(0, 0, rightImage.cols, rightImage.rows - height_move_2));
	}
	else{//������ͼƬ�ϸߣ� �ü����ͼƬ
		height_move_2 = leftImage.rows - rightImage.rows;
		leftImage = leftImage(Rect(0, 0, leftImage.cols, leftImage.rows - height_move_2));
	}


	int averageDistance = distancesVector[0];//�ص�����
	if (averageDistance > leftImage.cols || averageDistance > rightImage.cols){
		return 0;
	}

	//����ǿƥ��������ص���������ۼӣ����ν��ȶ����ɣ�����ͻ��
	Mat imageLeftOverlap, imageRightOverlap; //ͼ1��ͼ2���ص�����	
	imageLeftOverlap = leftImage(Rect(Point(leftImage.cols - averageDistance, 0), Point(leftImage.cols, leftImage.rows)));
	imageRightOverlap = rightImage(Rect(0, 0, averageDistance, rightImage.rows));
	Mat imageLeftROICopy = imageLeftOverlap.clone();  //����һ��ͼ1���ص�����
	for (int i = 0; i<imageLeftOverlap.rows; i++)
	{
		for (int j = 0; j<imageLeftOverlap.cols; j++)
		{
			double weight;
			weight = (double)j / imageRightOverlap.cols;  //�����ı���ı�ĵ���ϵ��
			imageLeftROICopy.at<Vec3b>(i, j)[0] = (1 - weight)*imageLeftOverlap.at<Vec3b>(i, j)[0] + weight*imageRightOverlap.at<Vec3b>(i, j)[0];
			imageLeftROICopy.at<Vec3b>(i, j)[1] = (1 - weight)*imageLeftOverlap.at<Vec3b>(i, j)[1] + weight*imageRightOverlap.at<Vec3b>(i, j)[1];
			imageLeftROICopy.at<Vec3b>(i, j)[2] = (1 - weight)*imageLeftOverlap.at<Vec3b>(i, j)[2] + weight*imageRightOverlap.at<Vec3b>(i, j)[2];
		}
	}

	Mat RightROIMat = rightImage(Rect(Point(imageRightOverlap.cols, 0), Point(rightImage.cols, rightImage.rows)));	 //�ұ�ƴ�Ӳ���

	int overlapWidth = leftImage.cols + rightImage.cols - averageDistance;//�غϵĿ�
	int overlapHeight = rightImage.rows;//�غϵĸ�

	Mat outMat(overlapHeight, overlapWidth, CV_8UC3);
	outMat.setTo(0);
	Mat leftROIMat = leftImage(Rect(0, 0, leftImage.cols - averageDistance, leftImage.rows));//���ƴ�Ӳ���

	leftROIMat.copyTo(outMat(Rect(0, 0, leftROIMat.cols, leftROIMat.rows)));
	imageLeftROICopy.copyTo(outMat(Rect(leftROIMat.cols, 0, imageLeftROICopy.cols, imageLeftROICopy.rows)));//�м�ƴ�Ӳ���

	RightROIMat.copyTo(outMat(Rect(leftROIMat.cols + imageLeftOverlap.cols, 0, RightROIMat.cols, RightROIMat.rows)));//�ұ�ƴ�Ӳ���

	//��ʱ��90
	transpose(outMat, outMat);
	flip(outMat, outMat, 0);
	outMat.copyTo(outImage);

	return 1;
}