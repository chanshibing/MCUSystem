#include "stdafx.h"
#include "TXPJ_algorithm.h"
#include<math.h>
#include "ProgStatusBar.h"

#include <fstream>
#include <iostream>

DWORD WINAPI imageStitching(LPVOID p);//图片拼接



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
	第四个参数：重叠部分比例
*/
int TXPJAlgorithm::imageStitching_rows(Mat leftImage, Mat rightImage, Mat &outImage, const float widthByWhole)
{
	if (leftImage.rows != rightImage.rows)
	{
		return 0;
	}
	//灰度图转换
	Mat leftImageGray, rightImageGray;
	cvtColor(leftImage, leftImageGray , COLOR_RGB2GRAY);
	cvtColor(rightImage, rightImageGray, COLOR_RGB2GRAY);

	//截取
	int cut_width = rightImage.cols * widthByWhole;//截取的部分宽度
	if (leftImage.cols < cut_width)
	{
		return 0;
	}
	try
	{
		leftImageGray = leftImageGray(Rect(leftImageGray.cols - cut_width, 0, cut_width, leftImageGray.rows));//左边图片截取右部
		rightImageGray = rightImageGray(Rect(0, 0, cut_width, rightImageGray.rows));//右边图片截取左部
	}
	catch (const std::exception& ex)
	{
		return 0;
	}
	

	//提取特征点 
	Ptr<Feature2D> siftDetector = xfeatures2d::SIFT::create(2000);//创建SIFT特征类，特征点上限2000
	vector<KeyPoint> leftKeyPoint, rightKeyPoint;
	siftDetector->detect(leftImageGray, leftKeyPoint);
	siftDetector->detect(rightImageGray, rightKeyPoint);

	//特征点描述，为下边的特征点匹配做准备  
	Mat leftImageDesc, rightImageDesc;
	siftDetector->compute(leftImageGray, leftKeyPoint, leftImageDesc);
	siftDetector->compute(rightImageGray, rightKeyPoint, rightImageDesc);

	//获得匹配特征点，并提取最优配对  	
	FlannBasedMatcher matcher;
	vector<DMatch> matchePoints;
	matcher.match(leftImageDesc, rightImageDesc, matchePoints, Mat());
	if (matchePoints.size() <= 0)
	{
		return 0;
	}
	sort(matchePoints.begin(), matchePoints.end()); //特征点排序	

	//获取排在前N个的最优匹配特征点
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

	// 把内点转换为drawMatches可以使用的格式  
	leftKeyPoint.clear();
	rightKeyPoint.clear();
	KeyPoint::convert(leftImagePoints, leftKeyPoint);
	KeyPoint::convert(rightImagePoints, rightKeyPoint);

	/*Mat o1;
	drawMatches(leftImageGray, leftKeyPoint, rightImageGray, rightKeyPoint, m_InlierMatches, o1);*/


	//计算斜率
	vector<float> slopes;  //斜率
	float slope;
	for (int i = 0; i < (int)leftKeyPoint.size(); i++)
	{
		slope = (rightKeyPoint[i].pt.y - leftKeyPoint[i].pt.y) / (rightKeyPoint[i].pt.x + leftImageGray.cols - leftKeyPoint[i].pt.x);
		slopes.push_back(slope);
	}


	//排除不好的斜率的点
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

	//计算出现频率最多的距离
	double *rightLeftPointDistance = new double[leftKeyPoint.size()];
	vector<double> distancesVector;
	double distance = 0;
	for (int i = 0; i < leftKeyPoint.size(); i++)
	{
		distance = sqrt(pow(rightKeyPoint[i].pt.x + leftImageGray.cols - leftKeyPoint[i].pt.x, 2) + pow(rightKeyPoint[i].pt.y - leftKeyPoint[i].pt.y, 2));
		*(rightLeftPointDistance+i) = distance;
		distancesVector.push_back(distance);
	}

	//K均值聚类法
	const int size = leftKeyPoint.size(); //Number of samples
	const int dim = 1;   //Dimension of feature
	const int cluster_num = 10; //Cluster number
	KMeans* kmeans = new KMeans(dim, cluster_num);
	int* labels = new int[size];
	kmeans->thresh = 5;
	kmeans->SetInitMode(KMeans::InitUniform);
	kmeans->Cluster(rightLeftPointDistance, size, labels);
	
	int numberOflabels[cluster_num];//记录labels的数量
	for (int i = 0; i < cluster_num; i++)
	{
		numberOflabels[i] = 0;
	}
	for (int i = 0; i < size; i++)
	{
		numberOflabels[labels[i]]++;
	}
	
	//找到最多的labels
	int maxLabels = 0;//数量
	int indexLabels = 0;//位置
	for (int i = 0; i < cluster_num; i++)
	{
		if (maxLabels < numberOflabels[i])
		{
			maxLabels = numberOflabels[i];
			indexLabels = i;
		}
	}

	//排除不是最多labels的点
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

	//在最强匹配点左侧的重叠区域进行累加，是衔接稳定过渡，消除突变
	Mat imageLeftOverlap, imageRightOverlap; //图1和图2的重叠部分	
	imageLeftOverlap = leftImage(Rect(leftImage.cols - averageDistance, 0, averageDistance, leftImage.rows));
	imageRightOverlap = rightImage(Rect(0, 0, averageDistance, rightImage.rows));
	Mat imageLeftROICopy = imageLeftOverlap.clone();  //复制一份图1的重叠部分
	for (int i = 0; i<imageLeftOverlap.rows; i++)
	{
		for (int j = 0; j<imageRightOverlap.cols; j++)
		{
			double weight;
			weight = (double)j / imageLeftOverlap.cols;  //随距离改变而改变的叠加系数
			imageLeftROICopy.at<Vec3b>(i, j)[0] = (1 - weight)*imageLeftOverlap.at<Vec3b>(i, j)[0] + weight*imageRightOverlap.at<Vec3b>(i, j)[0];
			imageLeftROICopy.at<Vec3b>(i, j)[1] = (1 - weight)*imageLeftOverlap.at<Vec3b>(i, j)[1] + weight*imageRightOverlap.at<Vec3b>(i, j)[1];
			imageLeftROICopy.at<Vec3b>(i, j)[2] = (1 - weight)*imageLeftOverlap.at<Vec3b>(i, j)[2] + weight*imageRightOverlap.at<Vec3b>(i, j)[2];
		}
	}

	Mat RightROIMat = rightImage(Rect(Point(imageRightOverlap.cols, 0), Point(rightImage.cols, rightImage.rows)));	 //右边拼接部分

	int overlapWidth = leftImage.cols + rightImage.cols - averageDistance;//重合的宽
	int overlapHeight = rightImage.rows;//重合的高

	Mat outMat(overlapHeight, overlapWidth, CV_8UC3);
	outMat.setTo(0);
	Mat leftROIMat = leftImage(Rect(0, 0, leftImage.cols - averageDistance, leftImage.rows));//左边拼接部分

	leftROIMat.copyTo(outMat(Rect(0, 0, leftROIMat.cols, leftROIMat.rows)));
	imageLeftROICopy.copyTo(outMat(Rect(leftROIMat.cols, 0, imageLeftROICopy.cols, imageLeftROICopy.rows)));//中间拼接部分

	RightROIMat.copyTo(outMat(Rect(leftROIMat.cols + imageLeftOverlap.cols, 0, RightROIMat.cols, RightROIMat.rows)));//右边拼接部分

	outMat.copyTo(outImage);

	return 1;
}

int TXPJAlgorithm::imageStitching_cols(Mat leftImage, Mat rightImage, Mat &outImage, const float heightByImage, int currentCol)
{
	//顺时针旋转90
	transpose(leftImage, leftImage);//逆时针镜像90
	flip(leftImage, leftImage, 1);//顺时针180
	transpose(rightImage, rightImage);
	flip(rightImage, rightImage, 1);

	if (previousCutHeight >= rightImage.rows)
	{
		return 0;
	}

	try
	{
		//判断前面一行右图是否有裁剪,如果有裁剪，就讲过当前右图进行裁剪
		if (currentCol % 2 == 0 && previousCutHeight > 0)//偶数
		{
			rightImage = rightImage(Rect(0, previousCutHeight, rightImage.cols, rightImage.rows - previousCutHeight));
		}
		if (currentCol % 2 == 1 && previousCutHeight > 0)//奇数
		{
			rightImage = rightImage(Rect(0, 0, rightImage.cols, rightImage.rows - previousCutHeight));
		}

		//比较左右图的高度，如果左边图高，切掉左边图；如果右边图高，切掉右边图，并且记下高度
		int cut_height = 0;
		if (leftImage.rows > rightImage.rows)//裁剪左图
		{
			cut_height = leftImage.rows - rightImage.rows;
			previousCutHeight = 0;//右图无裁剪
			if (currentCol % 2 == 1)//奇数裁剪上部
			{
				leftImage = leftImage(Rect(0, cut_height, leftImage.cols, leftImage.rows - cut_height));
			}
			else//偶数裁剪左图下部
			{
				leftImage = leftImage(Rect(0, 0, leftImage.cols, leftImage.rows - cut_height));
			}
		}
		else//裁剪右图
		{
			cut_height = rightImage.rows - leftImage.rows;
			previousCutHeight = cut_height;//右图有裁剪
			if (currentCol % 2 == 1)//奇数裁剪右图上部
			{
				rightImage = rightImage(Rect(0, cut_height, rightImage.cols, rightImage.rows - cut_height));
			}
			else//偶数裁剪右图下部
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
	//灰度图转换
	Mat leftImageGray, rightImageGray;
	cvtColor(leftImage, leftImageGray, CV_RGB2GRAY);
	cvtColor(rightImage, rightImageGray, CV_RGB2GRAY);

	//截取一部分进行匹配
	int cut_width = rightImage.cols * heightByImage;//截取的部分宽度

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
	

	//提取特征点  
	Ptr<Feature2D> siftDetector = xfeatures2d::SIFT::create(6000);//创建SIFT特征类
	vector<KeyPoint> leftKeyPoint, rightKeyPoint;
	siftDetector->detect(leftImageGray, leftKeyPoint);
	siftDetector->detect(rightImageGray, rightKeyPoint);

	//特征点描述，为下边的特征点匹配做准备  
	Mat leftImageDesc, rightImageDesc;
	siftDetector->compute(leftImageGray, leftKeyPoint, leftImageDesc);
	siftDetector->compute(rightImageGray, rightKeyPoint, rightImageDesc);
	
	//获得匹配特征点，并提取最优配对  	
	FlannBasedMatcher matcher;
	vector<DMatch> matchePoints;
	matcher.match(leftImageDesc, rightImageDesc, matchePoints, Mat());
	if (matchePoints.size() <= 0)
	{
		return 0;
	}
	sort(matchePoints.begin(), matchePoints.end()); //特征点排序	

	//获取排在前N个的最优匹配特征点
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


	// 把内点转换为drawMatches可以使用的格式  
	leftKeyPoint.clear();
	rightKeyPoint.clear();
	KeyPoint::convert(leftImagePoint, leftKeyPoint);
	KeyPoint::convert(rightImagePoint, rightKeyPoint);

	/*Mat OutImage1;
	drawMatches(leftImageGray, leftKeyPoint, rightImageGray, rightKeyPoint, m_InlierMatches, OutImage1);
*/
	//斜率排除
	vector<float> slopes; //斜率
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

	//计算出现频率最多的距离
	double *rightLeftPointDistance = new double[leftKeyPoint.size()];
	vector<double> distancesVector;
	double distance = 0;
	for (int i = 0; i < leftKeyPoint.size(); i++)
	{
		distance = sqrt(pow(rightKeyPoint[i].pt.x + leftImageGray.cols - leftKeyPoint[i].pt.x, 2) + pow(rightKeyPoint[i].pt.y - leftKeyPoint[i].pt.y, 2));
		*(rightLeftPointDistance + i) = distance;
		distancesVector.push_back(distance);
	}

	//K均值聚类法
	const int size = leftKeyPoint.size(); //Number of samples
	const int dim = 1;   //Dimension of feature
	const int cluster_num = 15; //Cluster number
	KMeans* kmeans = new KMeans(dim, cluster_num);
	int* labels = new int[size];
	kmeans->thresh = 5;
	kmeans->SetInitMode(KMeans::InitUniform);
	kmeans->Cluster(rightLeftPointDistance, size, labels);

	int numberOflabels[cluster_num];//记录labels的数量
	for (int i = 0; i < cluster_num; i++)
	{
		numberOflabels[i] = 0;
	}
	for (int i = 0; i < size; i++)
	{
		numberOflabels[labels[i]]++;
	}

	//找到最多的labels
	int maxLabels = 0;//数量
	int indexLabels = 0;//位置
	for (int i = 0; i < cluster_num; i++)
	{
		if (maxLabels < numberOflabels[i])
		{
			maxLabels = numberOflabels[i];
			indexLabels = i;
		}
	}

	//排除不是最多labels的点
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

	//在最强匹配点左侧的重叠区域进行累加，是衔接稳定过渡，消除突变
	Mat imageLeftOverlap, imageRightOverlap; //图1和图2的重叠部分	
	imageLeftOverlap = leftImage(Rect(Point(leftImage.cols - averageDistance, 0), Point(leftImage.cols, leftImage.rows)));
	imageRightOverlap = rightImage(Rect(0, 0, averageDistance, rightImage.rows));
	Mat imageLeftROICopy = imageLeftOverlap.clone();  //复制一份图1的重叠部分
	for (int i = 0; i<imageLeftOverlap.rows; i++)
	{
		for (int j = 0; j<imageLeftOverlap.cols; j++)
		{
			double weight;
			weight = (double)j / imageRightOverlap.cols;  //随距离改变而改变的叠加系数
			imageLeftROICopy.at<Vec3b>(i, j)[0] = (1 - weight)*imageLeftOverlap.at<Vec3b>(i, j)[0] + weight*imageRightOverlap.at<Vec3b>(i, j)[0];
			imageLeftROICopy.at<Vec3b>(i, j)[1] = (1 - weight)*imageLeftOverlap.at<Vec3b>(i, j)[1] + weight*imageRightOverlap.at<Vec3b>(i, j)[1];
			imageLeftROICopy.at<Vec3b>(i, j)[2] = (1 - weight)*imageLeftOverlap.at<Vec3b>(i, j)[2] + weight*imageRightOverlap.at<Vec3b>(i, j)[2];
		}
	}

	Mat RightROIMat = rightImage(Rect(Point(imageRightOverlap.cols, 0), Point(rightImage.cols, rightImage.rows)));	 //右边拼接部分

	int overlapWidth = leftImage.cols + rightImage.cols - averageDistance;//重合的宽
	int overlapHeight = rightImage.rows;//重合的高

	Mat outMat(overlapHeight, overlapWidth, CV_8UC3);
	outMat.setTo(0);
	Mat leftROIMat = leftImage(Rect(0, 0, leftImage.cols - averageDistance, leftImage.rows));//左边拼接部分

	leftROIMat.copyTo(outMat(Rect(0, 0, leftROIMat.cols, leftROIMat.rows)));
	imageLeftROICopy.copyTo(outMat(Rect(leftROIMat.cols, 0, imageLeftROICopy.cols, imageLeftROICopy.rows)));//中间拼接部分

	RightROIMat.copyTo(outMat(Rect(leftROIMat.cols + imageLeftOverlap.cols, 0, RightROIMat.cols, RightROIMat.rows)));//右边拼接部分

	//逆时针90
	transpose(outMat, outMat);
	flip(outMat, outMat, 0);
	outMat.copyTo(outImage);

	return 1;
}




int TXPJAlgorithm::imageStitching_cols2(Mat leftImage, Mat rightImage, Mat &outImage, const float heightByImage, int currentCol)
{
	//顺时针旋转90
	transpose(leftImage, leftImage);//逆时针镜像90
	flip(leftImage, leftImage, 1);//顺时针180
	transpose(rightImage, rightImage);
	flip(rightImage, rightImage, 1);

	
	//灰度图转换
	Mat leftImageGray, rightImageGray;
	cvtColor(leftImage, leftImageGray, CV_RGB2GRAY);
	cvtColor(rightImage, rightImageGray, CV_RGB2GRAY);

	//截取一部分进行匹配
	int cut_width = rightImage.cols * heightByImage;//截取的部分宽度

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


	//提取特征点  
	Ptr<Feature2D> siftDetector = xfeatures2d::SIFT::create(6000);//创建SIFT特征类
	vector<KeyPoint> leftKeyPoint, rightKeyPoint;
	siftDetector->detect(leftImageGray, leftKeyPoint);
	siftDetector->detect(rightImageGray, rightKeyPoint);

	//特征点描述，为下边的特征点匹配做准备  
	Mat leftImageDesc, rightImageDesc;
	siftDetector->compute(leftImageGray, leftKeyPoint, leftImageDesc);
	siftDetector->compute(rightImageGray, rightKeyPoint, rightImageDesc);

	//获得匹配特征点，并提取最优配对  	
	FlannBasedMatcher matcher;
	vector<DMatch> matchePoints;
	matcher.match(leftImageDesc, rightImageDesc, matchePoints, Mat());
	if (matchePoints.size() <= 0)
	{
		return 0;
	}
	sort(matchePoints.begin(), matchePoints.end()); //特征点排序	

	//获取排在前N个的最优匹配特征点
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


	// 把内点转换为drawMatches可以使用的格式  
	leftKeyPoint.clear();
	rightKeyPoint.clear();
	KeyPoint::convert(leftImagePoint, leftKeyPoint);
	KeyPoint::convert(rightImagePoint, rightKeyPoint);

	/*Mat OutImage1;
	drawMatches(leftImageGray, leftKeyPoint, rightImageGray, rightKeyPoint, m_InlierMatches, OutImage1);*/
	

	//斜率排除
	double* slopes = new double[(int)leftKeyPoint.size()]; //斜率
	float slope;
	for (int i = 0; i < (int)leftKeyPoint.size(); i++)
	{
		slope = (rightKeyPoint[i].pt.y - leftKeyPoint[i].pt.y) / (rightKeyPoint[i].pt.x + leftImageGray.cols - leftKeyPoint[i].pt.x);
		slopes[i] = slope;
	}

	//K均值聚类法
	const int size_slope = leftKeyPoint.size(); //Number of samples
	const int dim_slope = 1;   //Dimension of feature
	const int cluster_num_slope = 15; //Cluster number
	KMeans* kmeans_slope = new KMeans(dim_slope, cluster_num_slope);
	int* labels_slope = new int[size_slope];
	kmeans_slope->thresh = 0.005;
	kmeans_slope->SetInitMode(KMeans::InitUniform);
	kmeans_slope->Cluster(slopes, size_slope, labels_slope);

	int numberOflabels_slope[cluster_num_slope];//记录labels的数量
	for (int i = 0; i < cluster_num_slope; i++)
	{
		numberOflabels_slope[i] = 0;
	}
	for (int i = 0; i < size_slope; i++)
	{
		numberOflabels_slope[labels_slope[i]]++;
	}

	//找到最多的labels
	int maxLabels_slope = 0;//数量
	int indexLabels_slope = 0;//位置
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

	//计算出现频率最多的距离
	double *rightLeftPointDistance = new double[leftKeyPoint.size()];
	vector<double> distancesVector;
	double distance = 0;
	for (int i = 0; i < leftKeyPoint.size(); i++)
	{
		distance = sqrt(pow(rightKeyPoint[i].pt.x + leftImageGray.cols - leftKeyPoint[i].pt.x, 2) + pow(rightKeyPoint[i].pt.y - leftKeyPoint[i].pt.y, 2));
		*(rightLeftPointDistance + i) = distance;
		distancesVector.push_back(distance);
	}

	//K均值聚类法
	const int size = leftKeyPoint.size(); //Number of samples
	const int dim = 1;   //Dimension of feature
	const int cluster_num = 15; //Cluster number
	KMeans* kmeans = new KMeans(dim, cluster_num);
	int* labels = new int[size];
	kmeans->thresh = 5;
	kmeans->SetInitMode(KMeans::InitUniform);
	kmeans->Cluster(rightLeftPointDistance, size, labels);

	int numberOflabels[cluster_num];//记录labels的数量
	for (int i = 0; i < cluster_num; i++)
	{
		numberOflabels[i] = 0;
	}
	for (int i = 0; i < size; i++)
	{
		numberOflabels[labels[i]]++;
	}

	//找到最多的labels
	int maxLabels = 0;//数量
	int indexLabels = 0;//位置
	for (int i = 0; i < cluster_num; i++)
	{
		if (maxLabels < numberOflabels[i])
		{
			maxLabels = numberOflabels[i];
			indexLabels = i;
		}
	}

	//排除不是最多labels的点
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

	int height_move = rightKeyPoint[0].pt.y - leftKeyPoint[0].pt.y;//裁剪高度
	int height_move_2 = 0;
	if (height_move > 0){//裁剪右边图片
		rightImage = rightImage(Rect(0, height_move, rightImage.cols, rightImage.rows - height_move));	
	}
	else if (height_move < 0){//裁剪左图
		height_move = abs(height_move);
		leftImage = leftImage(Rect(0, height_move, leftImage.cols, leftImage.rows - height_move));
	}
	if (rightImage.rows >= leftImage.rows){//如果右边图片较高， 裁剪右边图片
		height_move_2 = rightImage.rows - leftImage.rows;
		rightImage = rightImage(Rect(0, 0, rightImage.cols, rightImage.rows - height_move_2));
	}
	else{//如果左边图片较高， 裁剪左边图片
		height_move_2 = leftImage.rows - rightImage.rows;
		leftImage = leftImage(Rect(0, 0, leftImage.cols, leftImage.rows - height_move_2));
	}


	int averageDistance = distancesVector[0];//重叠距离
	if (averageDistance > leftImage.cols || averageDistance > rightImage.cols){
		return 0;
	}

	//在最强匹配点左侧的重叠区域进行累加，是衔接稳定过渡，消除突变
	Mat imageLeftOverlap, imageRightOverlap; //图1和图2的重叠部分	
	imageLeftOverlap = leftImage(Rect(Point(leftImage.cols - averageDistance, 0), Point(leftImage.cols, leftImage.rows)));
	imageRightOverlap = rightImage(Rect(0, 0, averageDistance, rightImage.rows));
	Mat imageLeftROICopy = imageLeftOverlap.clone();  //复制一份图1的重叠部分
	for (int i = 0; i<imageLeftOverlap.rows; i++)
	{
		for (int j = 0; j<imageLeftOverlap.cols; j++)
		{
			double weight;
			weight = (double)j / imageRightOverlap.cols;  //随距离改变而改变的叠加系数
			imageLeftROICopy.at<Vec3b>(i, j)[0] = (1 - weight)*imageLeftOverlap.at<Vec3b>(i, j)[0] + weight*imageRightOverlap.at<Vec3b>(i, j)[0];
			imageLeftROICopy.at<Vec3b>(i, j)[1] = (1 - weight)*imageLeftOverlap.at<Vec3b>(i, j)[1] + weight*imageRightOverlap.at<Vec3b>(i, j)[1];
			imageLeftROICopy.at<Vec3b>(i, j)[2] = (1 - weight)*imageLeftOverlap.at<Vec3b>(i, j)[2] + weight*imageRightOverlap.at<Vec3b>(i, j)[2];
		}
	}

	Mat RightROIMat = rightImage(Rect(Point(imageRightOverlap.cols, 0), Point(rightImage.cols, rightImage.rows)));	 //右边拼接部分

	int overlapWidth = leftImage.cols + rightImage.cols - averageDistance;//重合的宽
	int overlapHeight = rightImage.rows;//重合的高

	Mat outMat(overlapHeight, overlapWidth, CV_8UC3);
	outMat.setTo(0);
	Mat leftROIMat = leftImage(Rect(0, 0, leftImage.cols - averageDistance, leftImage.rows));//左边拼接部分

	leftROIMat.copyTo(outMat(Rect(0, 0, leftROIMat.cols, leftROIMat.rows)));
	imageLeftROICopy.copyTo(outMat(Rect(leftROIMat.cols, 0, imageLeftROICopy.cols, imageLeftROICopy.rows)));//中间拼接部分

	RightROIMat.copyTo(outMat(Rect(leftROIMat.cols + imageLeftOverlap.cols, 0, RightROIMat.cols, RightROIMat.rows)));//右边拼接部分

	//逆时针90
	transpose(outMat, outMat);
	flip(outMat, outMat, 0);
	outMat.copyTo(outImage);

	return 1;
}