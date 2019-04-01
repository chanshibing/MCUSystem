#include "stdafx.h"
#include "StatisticsTip.h"
#include<numeric>
#include<vector>

using namespace std;
bool cmp(double x, double y)
{
	return x < y;
}

StatisticsTip::~StatisticsTip()
{
}

/*
	#统计引脚数量#
*/
int StatisticsTip::statisticsPin(DetectAlgorithm &detectAlgorithm)
{
	for (int s = 0; s < detectAlgorithm.instanceRect_vector.size(); s++)
	{
		//如果是电阻，不计算引脚，跳过
		if (detectAlgorithm.instanceRect_vector[s].instanceClass == 1 || detectAlgorithm.instanceRect_vector[s].instanceClass == 2)
			continue;

		Mat src_img = detectAlgorithm.cutRect(detectAlgorithm.detect_src_img, detectAlgorithm.instanceRect_vector[s].left_top, 
			detectAlgorithm.instanceRect_vector[s].right_bottom);//原图

		Mat src_img_;//右旋转90的原图

		transpose(src_img, src_img_);//逆时针镜像90
		flip(src_img_, src_img_, 1);//顺时针180

		Mat image_gray, image_gray_;//灰度图
		Mat image_threshold, image_threshold_;//二值图
		Mat image_erode, image_erode_;//腐蚀图
		Mat image_dilate, image_dilate_;//膨胀图
		Mat out_img, out_img_;//输出的图

		//灰度图
		cvtColor(src_img, image_gray, COLOR_RGB2GRAY);
		cvtColor(src_img_, image_gray_, COLOR_RGB2GRAY);

		//二值化
		threshold(image_gray, image_threshold, 140, 255, CV_THRESH_BINARY);
		threshold(image_gray_, image_threshold_, 140, 255, CV_THRESH_BINARY);

		//腐蚀膨胀
		Mat structElement_e, structElement_d;
		structElement_e = getStructuringElement(MORPH_RECT, Size(2, 2));
		structElement_d = getStructuringElement(MORPH_RECT, Size(2, 2));
		erode(image_threshold, out_img, structElement_e);
		erode(image_threshold_, out_img_, structElement_e);
		dilate(out_img, out_img, structElement_d);
		dilate(out_img_, out_img_, structElement_d);

		//反向
		out_img = 255 - out_img;
		out_img_ = 255 - out_img_;


		//统计直方图
		vector<int> histHeight1;
		vector<int> histHeight2;
		int r = 0;

		//整个图1的直方图，统计每行黑色点的个数
		for (int i = 0; i < out_img.rows; i++)//行
		{
			r = 0;
			for (int j = 0; j < out_img.cols; j++)//列
			{
				uchar c = out_img.at<uchar>(i, j);
				if (c == 0)
				{
					r++;
				}
			}
			histHeight1.push_back(r);
		}
		//整个图2的直方图，统计每行黑色点的个数
		for (int i = 0; i < out_img_.rows; i++)//行
		{
			r = 0;
			for (int j = 0; j < out_img_.cols; j++)//列
			{
				uchar c = out_img_.at<uchar>(i, j);
				if (c == 0)
				{
					r++;
				}
			}
			histHeight2.push_back(r);
		}

		//直方图
		Mat dstImg(out_img.cols, out_img.rows, CV_8UC1, Scalar(0));
		Mat dstImg_(out_img_.cols, out_img_.rows, CV_8UC1, Scalar(0));

		//画直方图
		for (int i = 0; i < out_img.rows; i++)
		{
			int h = histHeight1[i];
			rectangle(dstImg, Point(i, out_img.cols - h), Point(i, out_img.cols), Scalar(122));
		}
		for (int i = 0; i < out_img_.rows; i++)
		{
			int h = histHeight2[i];
			rectangle(dstImg_, Point(i, out_img_.cols - h), Point(i, out_img_.cols), Scalar(122));
		}


		//高度阈值,计算最大的两个连续长度之和
		int s_threshold1 = (out_img.cols) * 0.25;
		int s_threshold2 = (out_img_.cols) * 0.25;

		int length = 0;
		vector<int> length1;//连续最大长度
		vector<int> length2;//连续最大长度

		bool flag = false;

		//在阈值高度，计算连续长度
		for (int i = 0; i < histHeight1.size(); i++)
		{
			if (histHeight1[i] < s_threshold1)
			{
				length++;
				flag = true;
				if (i == histHeight1.size() - 1)
				{
					length1.push_back(length);
					break;
				}
			}
			else if (flag)
			{
				length1.push_back(length);
				length = 0;
				flag = false;
			}
		}

		flag = false;
		length = 0;
		//在阈值高度，计算连续长度
		for (int i = 0; i < histHeight2.size(); i++)
		{
			if (histHeight2[i] < s_threshold2)
			{
				length++;
				flag = true;
				if (i == histHeight2.size() - 1)
				{
					length2.push_back(length);
					break;
				}
			}
			else if (flag)
			{
				length2.push_back(length);
				length = 0;
				flag = false;
			}
		}


		int sum_length1 = 0;
		int sum_length2 = 0;

		//计算最大两个长度和
		if (length1.size() > 1)
		{
			sort(length1.begin(), length1.end(), greater<int>());
			sum_length1 = length1[0] + length1[1];
		}
		else if (length1.size() == 1)
		{
			sum_length1 = length1[0];
		}

		if (length2.size() > 1)
		{
			sort(length2.begin(), length2.end(), greater<int>());
			sum_length2 = length2[0] + length2[1];
		}
		else if(length2.size() == 1)
		{
			sum_length2 = length2[0];
		}

		Mat h_outImage, v_outImage;//横 垂直 结果图
		Mat h_dstImage, v_dstImage;//直方图
		int kind = 0;

		structElement_e = getStructuringElement(MORPH_RECT, Size(5, 2));
		structElement_d = getStructuringElement(MORPH_RECT, Size(5, 2));

		//判断图片的横竖
		if (sum_length1 < sum_length2){
			erode(image_threshold, out_img, structElement_e);
			dilate(out_img, out_img, structElement_d);
			out_img = 255 - out_img;
			out_img.copyTo(h_outImage);
			out_img_.copyTo(v_outImage);
			dstImg.copyTo(h_dstImage);
			dstImg_.copyTo(v_dstImage);
			kind = 1;//设置旋转种类
		}
		else
		{
			erode(image_threshold_, out_img_, structElement_e);
			dilate(out_img_, out_img_, structElement_d);
			out_img_ = 255 - out_img_;
			out_img.copyTo(v_outImage);
			out_img_.copyTo(h_outImage);
			dstImg.copyTo(v_dstImage);
			dstImg_.copyTo(h_dstImage);
			kind = 2;//设置旋转种类
		}



		//找到芯片中间的左右位置
		int imgHeight = v_dstImage.rows;
		int thresholdHeight = imgHeight * 0.25;//阈值

		int maxLength = 0;
		length = 0;
		flag = false;
		int indexl = 0;//芯片左边位置
		int indexr = v_dstImage.cols / 2;//芯片右边位置

		//计算左边的位置
		for (int i = 0; i < v_dstImage.cols / 2; i++)
		{
			uchar c = v_dstImage.at<uchar>(imgHeight - thresholdHeight, i);
			if (c == 0)
			{
				length++;
				flag = true;
				if (i == v_dstImage.cols / 2 - 1)//如果是最后一列
				{
					if (length > maxLength){
						maxLength = length;
						indexl = i - length;
					}
					flag = false;
					length = 0;
					break;
				}
			}
			else if (flag)
			{
				if (length > maxLength){
					maxLength = length;
					indexl = i - length;
				}
				flag = false;
				length = 0;
			}
		}

		//计算右边的位置
		maxLength = 0;
		for (int i = v_dstImage.cols / 2; i < v_dstImage.cols; i++)
		{
			uchar c = v_dstImage.at<uchar>(imgHeight - thresholdHeight, i);
			if (c == 0)
			{
				length++;
				flag = true;
				if (i == v_dstImage.cols / 2 - 1)//如果是最后一列
				{
					if (length > maxLength){
						maxLength = length;
						indexr = i - 1;
					}
					flag = false;
					length = 0;
					break;
				}
			}
			else if (flag)
			{
				if (length > maxLength){
					maxLength = length;
					indexr = i - 1;
				}
				flag = false;
				length = 0;
			}
		}

		//如果是旋转种类二
		if (kind = 2)
		{
			int temp = h_outImage.cols - indexl;
			indexl = h_outImage.cols - indexr;
			indexr = temp;
		}

		//画出横图左右两边的直方图
		Mat img1 = h_outImage(Rect(0, 0, h_outImage.cols / 2, h_outImage.rows));
		Mat img2 = h_outImage(Rect(h_outImage.cols / 2, 0, h_outImage.cols / 2, h_outImage.rows));

		Mat dstImg1(img1.cols, img1.rows, CV_8UC1, Scalar(0));
		Mat dstImg2(img2.cols, img2.rows, CV_8UC1, Scalar(0));

		histHeight1.clear();
		histHeight2.clear();

		//统计左边图的黑色点个数
		for (int i = 0; i < img1.rows; i++)
		{
			r = 0;
			for (int j = 0; j < img1.cols; j++)//左端至中间
			{
				uchar c = img1.at<uchar>(i, j);
				if (c == 0)
				{
					r++;
				}
			}
			histHeight1.push_back(r);
		}

		//画直方图
		for (int i = 0; i < img1.rows; i++)
		{
			int h = histHeight1[i];
			rectangle(dstImg1, Point(i, img1.cols - h), Point(i, img1.cols), Scalar(122));
		}

		//统计右边图的黑色点个数
		for (int i = 0; i < img2.rows; i++)
		{
			r = 0;
			for (int j = 0; j < img2.cols; j++)//中间至右端
			{
				uchar c = img2.at<uchar>(i, j);
				if (c == 0)
				{
					r++;
				}
			}
			histHeight2.push_back(r);
		}

		//画直方图
		for (int i = 0; i < img2.rows; i++)
		{
			int h = histHeight2[i];
			rectangle(dstImg2, Point(i, img2.cols - h), Point(i, img2.cols), Scalar(122));
		}

		//轮廓
		vector<vector<Point>> contours1;
		vector<vector<Point>> contours2;
		vector<Vec4i> hierarchy1;
		vector<Vec4i> hierarchy2;
		findContours(dstImg1, contours1, hierarchy1, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
		findContours(dstImg2, contours2, hierarchy2, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

		vector<double> area1;
		vector<double> area2;

		//计算面积  
		for (int i = 0; i < (int)contours1.size(); i++)
		{
			area1.push_back(contourArea(contours1[i]));
		}
		//计算面积  
		for (int i = 0; i < (int)contours2.size(); i++)
		{
			area2.push_back(contourArea(contours2[i]));
		}

		sort(area1.begin(), area1.end(), cmp);
		sort(area2.begin(), area2.end(), cmp);

		//计算引脚数量
		int num1 = area1.size();
		if (num1 != 0)
		{
			double max_area1 = area1[num1 - 1];
			for (int i = 0; i < area1.size(); i++)
			{
				if (area1[i] >= max_area1 * 0.5)
					break;
				num1--;
			}
		}
		

		//计算引脚数量
		int num2 = area2.size();
		if (num2 != 0)
		{
			double max_area2 = area2[num2 - 1];
			for (int i = 0; i < area2.size(); i++)
			{
				if (area2[i] >= max_area2 * 0.5)
					break;
				num2--;
			}
		}
		

		detectAlgorithm.instanceRect_vector[s].numOfPin1 = num1;
		detectAlgorithm.instanceRect_vector[s].numOfPin2 = num2;
	}

	return 0;
}


int StatisticsTip::statisticsPin3()
{
	String fileName[] = { "1.png", "13.png", "14.png", "15.png", "16.png", "17.png", "18.png", "123.png", "133.png"
		, "F3.jpg", "F4.jpg", "F5.jpg", "F6.jpg", "F8.jpg", "F9.jpg", "F11.jpg", "F13.jpg", "50.png", "51.png", "52.png"
		, "53.png", "54.png", "55.png", "56.png", "57.png", "58.png", "59.png", "60.png" };
	String fileName_[] = { "1_.png", "13_.png", "14_.png", "15_.png", "16_.png", "17_.png", "18_.png", "123_.png", "133_.png"
		, "F3_.jpg", "F4_.jpg", "F5_.jpg", "F6_.jpg", "F8_.jpg", "F9_.jpg", "F11_.jpg", "F13_.jpg", "50_.png", "51_.png", "52_.png"
		, "53_.png", "54_.png", "55_.png", "56_.png", "57_.png", "58_.png", "59_.png", "60_.png" };
	String fileNameh[] = { "1h.png", "13h.png", "14h.png", "15h.png", "16h.png", "17h.png", "18h.png", "123h.png", "133h.png"
		, "F3h.jpg", "F4h.jpg", "F5h.jpg", "F6h.jpg", "F8h.jpg", "F9h.jpg", "F11h.jpg", "F13h.jpg", "50h.png", "51h.png", "52h.png"
		, "53h.png", "54h.png", "55h.png", "56h.png", "57h.png", "58h.png", "59h.png", "60h.png" };
	String fileName_h[] = { "1_h.png", "13_h.png", "14_h.png", "15_h.png", "16_h.png", "17_h.png", "18_h.png", "123_h.png", "133_h.png"
		, "F3_h.jpg", "F4_h.jpg", "F5_h.jpg", "F6_h.jpg", "F8_h.jpg", "F9_h.jpg", "F11_h.jpg", "F13_h.jpg", "50_h.png", "51_h.png", "52_h.png"
		, "53_h.png", "54_h.png", "55_h.png", "56_h.png", "57_h.png", "58_h.png", "59_h.png", "60_h.png" };
	String fileNamel[] = { "1l.png", "13l.png", "14l.png", "15l.png", "16l.png", "17l.png", "18l.png", "123l.png", "133l.png"
		, "F3l.jpg", "F4l.jpg", "F5l.jpg", "F6l.jpg", "F8l.jpg", "F9l.jpg", "F11l.jpg", "F13l.jpg", "50l.png", "51l.png", "52l.png"
		, "53l.png", "54l.png", "55l.png", "56l.png", "57l.png", "58l.png", "59l.png", "60l.png" };
	String fileNamel_[] = { "1l_.png", "13l_.png", "14l_.png", "15l_.png", "16l_.png", "17l_.png", "18l_.png", "123l_.png", "133l_.png"
		, "F3l_.jpg", "F4l_.jpg", "F5l_.jpg", "F6l_.jpg", "F8l_.jpg", "F9l_.jpg", "F11l_.jpg", "F13l_.jpg", "50l_.png", "51l_.png", "52l_.png"
		, "53l_.png", "54l_.png", "55l_.png", "56l_.png", "57l_.png", "58l_.png", "59l_.png", "60l_.png" };
	String fileNamer[] = { "1r.png", "13r.png", "14r.png", "15r.png", "16r.png", "17r.png", "18r.png", "123r.png", "133r.png"
		, "F3r.jpg", "F4r.jpg", "F5r.jpg", "F6r.jpg", "F8r.jpg", "F9r.jpg", "F11r.jpg", "F13r.jpg", "50r.png", "51r.png", "52r.png" 
		, "53r.png", "54r.png", "55r.png", "56r.png", "57r.png", "58r.png", "59r.png", "60r.png" };
	String fileNamer_[] = { "1r_.png", "13r_.png", "14r_.png", "15r_.png", "16r_.png", "17r_.png", "18r_.png", "123r_.png", "133r_.png"
		, "F3r_.jpg", "F4r_.jpg", "F5r_.jpg", "F6r_.jpg", "F8r_.jpg", "F9r_.jpg", "F11r_.jpg", "F13r_.jpg", "50r_.png", "51r_.png", "52r_.png"
		, "53r_.png", "54r_.png", "55r_.png", "56r_.png", "57r_.png", "58r_.png", "59r_.png", "60r_.png" };
	String fileNameTxT[] = { "1.txt", "13.txt", "14.txt", "15.txt", "16.txt", "17.txt", "18.txt", "123.txt", "133.txt"
		, "F3.txt", "F4.txt", "F5.txt", "F6.txt", "F8.txt", "F9.txt", "F11.txt", "F13.txt", "50.txt", "51.txt", "52.txt"
		, "53.txt", "54.txt", "55.txt", "56.txt", "57.txt", "58.txt", "59.txt", "60.txt" };

	for (int s = 0; s < 28; s++)
	{
		
		Mat src_img = imread("images\\" + fileName[s]);//原图
		
		Mat src_img_;//右旋转90的原图

		transpose(src_img, src_img_);//逆时针镜像90
		flip(src_img_, src_img_, 1);//顺时针180

		Mat image_gray, image_gray_;//灰度图
		Mat image_threshold, image_threshold_;//二值图
		Mat image_erode, image_erode_;//腐蚀图
		Mat image_dilate, image_dilate_;//膨胀图
		Mat out_img, out_img_;//输出的图

		//灰度图
		cvtColor(src_img, image_gray, COLOR_RGB2GRAY);
		cvtColor(src_img_, image_gray_, COLOR_RGB2GRAY);

		//二值化
		threshold(image_gray, image_threshold, 140, 255, CV_THRESH_BINARY);
		threshold(image_gray_, image_threshold_, 140, 255, CV_THRESH_BINARY);

		//腐蚀膨胀
		Mat structElement_e, structElement_d;
		structElement_e = getStructuringElement(MORPH_RECT, Size(2, 2));
		structElement_d = getStructuringElement(MORPH_RECT, Size(2, 2));
		erode(image_threshold, out_img, structElement_e);
		erode(image_threshold_, out_img_, structElement_e);
		dilate(out_img, out_img, structElement_d);
		dilate(out_img_, out_img_, structElement_d);

		//反向
		out_img = 255 - out_img;
		out_img_ = 255 - out_img_;


		//直方图
		vector<int> histHeight1;
		vector<int> histHeight2;
		int r = 0;
		//int *histHeight = new int[img.rows];
		//memset(histHeight, 0, img.rows * sizeof(int));

		//整个图1的直方图，统计每行黑色点的个数
		for (int i = 0; i < out_img.rows; i++)//行
		{
			r = 0;
			for (int j = 0; j < out_img.cols; j++)//列
			{
				uchar c = out_img.at<uchar>(i, j);
				if (c == 0)
				{
					r++;
				}
			}
			histHeight1.push_back(r);
		}
		//整个图2的直方图，统计每行黑色点的个数
		for (int i = 0; i < out_img_.rows; i++)//行
		{
			r = 0;
			for (int j = 0; j < out_img_.cols; j++)//列
			{
				uchar c = out_img_.at<uchar>(i, j);
				if (c == 0)
				{
					r++;
				}
			}
			histHeight2.push_back(r);
		}

		//用于画直方图
		Mat dstImg(out_img.cols, out_img.rows, CV_8UC1, Scalar(0));
		Mat dstImg_(out_img_.cols, out_img_.rows, CV_8UC1, Scalar(0));

		//画直方图
		for (int i = 0; i < out_img.rows; i++)
		{
			int h = histHeight1[i];
			rectangle(dstImg, Point(i, out_img.cols - h), Point(i, out_img.cols), Scalar(122));
		}
		for (int i = 0; i < out_img_.rows; i++)
		{
			int h = histHeight2[i];
			rectangle(dstImg_, Point(i, out_img_.cols - h), Point(i, out_img_.cols), Scalar(122));
		}


		//高度阈值,计算最大的两个连续长度之和
		int s_threshold1 = (out_img.cols) * 0.25;
		int s_threshold2 = (out_img_.cols) * 0.25;

		int length = 0;
		vector<int> length1;//连续最大长度
		vector<int> length2;//连续最大长度

		bool flag = false;

		//在阈值高度，计算连续长度
		for (int i = 0; i < histHeight1.size(); i++)
		{
			if (histHeight1[i] < s_threshold1)
			{
				length++;
				flag = true;
				if (i == histHeight1.size() - 1)
				{
					length1.push_back(length);
					break;
				}
			}
			else if (flag)
			{
				length1.push_back(length);
				length = 0;
				flag = false;
			}
		}

		flag = false;
		length = 0;
		//在阈值高度，计算连续长度
		for (int i = 0; i < histHeight2.size(); i++)
		{
			if (histHeight2[i] < s_threshold2)
			{
				length++;
				flag = true;
				if (i == histHeight2.size() - 1)
				{
					length2.push_back(length);
					break;
				}
			}
			else if (flag)
			{
				length2.push_back(length);
				length = 0;
				flag = false;
			}
		}


		int sum_length1 = 0;
		int sum_length2 = 0;

		//计算最大两个长度和
		if (length1.size() > 1)
		{
			sort(length1.begin(), length1.end(), greater<int>());
			sum_length1 = length1[0] + length1[1];
		}
		else
		{
			sum_length1 = length1[0];
		}

		if (length2.size() > 1)
		{
			sort(length2.begin(), length2.end(), greater<int>());
			sum_length2 = length2[0] + length2[1];
		}
		else
		{
			sum_length2 = length2[0];
		}

		Mat h_outImage, v_outImage;//横 垂直 结果图
		Mat h_dstImage, v_dstImage;//直方图
		int kind = 0;

		structElement_e = getStructuringElement(MORPH_RECT, Size(5, 2));
		structElement_d = getStructuringElement(MORPH_RECT, Size(5, 2));

		if (sum_length1 < sum_length2){
			erode(image_threshold, out_img, structElement_e);
			dilate(out_img, out_img, structElement_d);
			out_img = 255 - out_img;
			out_img.copyTo(h_outImage);
			out_img_.copyTo(v_outImage);
			dstImg.copyTo(h_dstImage);
			dstImg_.copyTo(v_dstImage);
			kind = 1;
			imwrite("pin_image1\\" + fileNameh[s], dstImg);
			imwrite("pin_image1\\" + fileName_h[s], dstImg_);
		}
		else
		{
			erode(image_threshold_, out_img_, structElement_e);
			dilate(out_img_, out_img_, structElement_d);
			out_img_ = 255 - out_img_;
			out_img.copyTo(v_outImage);
			out_img_.copyTo(h_outImage);
			dstImg.copyTo(v_dstImage);
			dstImg_.copyTo(h_dstImage);
			kind = 2;
			imwrite("pin_image1\\" + fileNameh[s], dstImg_);
			imwrite("pin_image1\\" + fileName_h[s], dstImg);
		}

		

		//找到芯片中间的左右位置
		int imgHeight = v_dstImage.rows;
		int thresholdHeight = imgHeight * 0.25;
		
		int maxLength = 0;
		length = 0;
		flag = false;
		int indexl = 0;//芯片左边位置
		int indexr = v_dstImage.cols / 2;//芯片右边位置

		//计算左边的位置
		for (int i = 0; i < v_dstImage.cols / 2; i++)
		{
			uchar c = v_dstImage.at<uchar>(imgHeight - thresholdHeight, i);
			if (c == 0)
			{
				length++;
				flag = true;
				if (i == v_dstImage.cols / 2 - 1)
				{
					if (length > maxLength){
						maxLength = length;
						indexl = i - length;
					}
					flag = false;
					length = 0;
					break;
				}
			}
			else if (flag)
			{
				if (length > maxLength){
					maxLength = length;
					indexl = i - length;
				}
				flag = false;
				length = 0;
			}
		}

		//计算右边的位置
		maxLength = 0;
		for (int i = v_dstImage.cols / 2; i < v_dstImage.cols; i++)
		{
			uchar c = v_dstImage.at<uchar>(imgHeight - thresholdHeight, i);
			if (c == 0)
			{
				length++;
				flag = true;
				if (i == v_dstImage.cols / 2 - 1)
				{
					if (length > maxLength){
						maxLength = length;
						indexr = i - 1;
					}
					flag = false;
					length = 0;
					break;
				}
			}
			else if (flag)
			{
				if (length > maxLength){
					maxLength = length;
					indexr = i - 1;
				}
				flag = false;
				length = 0;
			}
		}

		if (kind = 2)
		{
			int temp = h_outImage.cols - indexl;
			indexl= h_outImage.cols - indexr;
			indexr = temp;
		}
		//去掉中间部分
		for (int i = 0; i < h_outImage.rows; i++)
		{
			for (int j = 0; j < h_outImage.cols; j++)
			{
				if (j >= indexl && j <= indexr)
				{
					h_outImage.at<uchar>(i, j) = 255;
				}
			}
		}
		imwrite("pin_image1\\" + fileName[s], h_outImage);
		imwrite("pin_image1\\" + fileName_[s], v_outImage);

		//画出横图左右两边的直方图
		Mat img1 = h_outImage(Rect(0, 0, h_outImage.cols / 2, h_outImage.rows));
		Mat img2 = h_outImage(Rect(h_outImage.cols / 2, 0, h_outImage.cols / 2, h_outImage.rows));
		
		Mat dstImg1(img1.cols, img1.rows, CV_8UC1, Scalar(0));
		Mat dstImg2(img2.cols, img2.rows, CV_8UC1, Scalar(0));

		histHeight1.clear();
		histHeight2.clear();

		//统计左边图的黑色点个数
		for (int i = 0; i < img1.rows; i++)
		{
			r = 0;
			for (int j = 0; j < indexl; j++)
			{
				uchar c = img1.at<uchar>(i, j);
				if (c == 0)
				{
					r++;
				}
			}
			histHeight1.push_back(r);
		}

		//画直方图
		for (int i = 0; i < img1.rows; i++)
		{
			int h = histHeight1[i];
			rectangle(dstImg1, Point(i, img1.cols - h), Point(i, img1.cols), Scalar(122));
		}

		//统计右边图的黑色点个数
		for (int i = 0; i < img2.rows; i++)
		{
			r = 0;
			for (int j = indexr - img1.cols + 1; j < img2.cols; j++)
			{
				uchar c = img2.at<uchar>(i, j);
				if (c == 0)
				{
					r++;
				}
			}
			histHeight2.push_back(r);
		}

		//画直方图
		for (int i = 0; i < img2.rows; i++)
		{
			int h = histHeight2[i];
			rectangle(dstImg2, Point(i, img2.cols - h), Point(i, img2.cols), Scalar(122));
		}

		imwrite("pin_image1\\" + fileNamel[s], dstImg1);
		imwrite("pin_image1\\" + fileNamer[s], dstImg2);

		vector<vector<Point>> contours1;
		vector<vector<Point>> contours2;
		vector<Vec4i> hierarchy1;
		vector<Vec4i> hierarchy2;
		findContours(dstImg1, contours1, hierarchy1, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
		findContours(dstImg2, contours2, hierarchy2, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

		vector<double> area1;
		vector<double> area2;

		//绘制轮廓  
		for (int i = 0; i < (int)contours1.size(); i++)
		{
			area1.push_back(contourArea(contours1[i]));
			drawContours(dstImg1, contours1, i, Scalar(255), 1, 8);
		}
		//绘制轮廓  
		for (int i = 0; i < (int)contours2.size(); i++)
		{
			area2.push_back(contourArea(contours2[i]));
			drawContours(dstImg2, contours2, i, Scalar(255), 1, 8);
		}

		sort(area1.begin(), area1.end(), cmp);
		sort(area2.begin(), area2.end(), cmp);

				//计算引脚数量
		int num1 = area1.size();
		double max_area1 = area1[num1 - 1];
		for (int i = 0; i < area1.size(); i++)
		{
			if (area1[i] >= max_area1 * 0.5)
				break;
			num1--;
		}

		//计算引脚数量
		int num2 = area2.size();
		double max_area2 = area2[num2 - 1];
		for (int i = 0; i < area2.size(); i++)
		{
			if (area2[i] >= max_area2 * 0.5)
				break;
			num2--;
		}

		//定义文件输出流   
		ofstream oFile;
		//打开要输出的文件   
		oFile.open("txt\\" + fileNameTxT[s], ios::out);    // 这样就很容易的输出一个需要的excel 文件  
		
		for (int i = 0; i < area1.size(); i++)
		{
			oFile << area1[i] << " ";
		}
		oFile << num1 << endl;
		for (int i = 0; i < area2.size(); i++)
		{
			oFile << area2[i] << " ";
		}
		oFile << num2 << endl;
		oFile.close();

		imwrite("pin_image1\\" + fileNamel_[s], dstImg1);
		imwrite("pin_image1\\" + fileNamer_[s], dstImg2);
		

		//Mat imgl = imread("pin_image\\" + fileNamel[s]);
		//Mat imgr = imread("pin_image\\" + fileNamer[s]);

		//cvtColor(imgl, imgl, COLOR_RGB2GRAY);
		//cvtColor(imgr, imgr, COLOR_RGB2GRAY);

		//int imgHeight = imgl.rows;
		//int thresholdHeight = imgHeight * 0.25;

		//Mat dstImage(imgl.rows, imgl.cols + imgr.cols, CV_8UC1, Scalar(0));

		//imgl.copyTo(dstImage(Rect(0, 0, imgl.cols, imgl.rows)));
		//imgr.copyTo(dstImage(Rect(imgl.cols, 0, imgr.cols, imgr.rows)));

		//rectangle(dstImage, Point(imgl.cols, 0), Point(imgl.cols, imgl.rows), Scalar(144));
		//rectangle(dstImage, Point(0, imgHeight - thresholdHeight), Point(dstImage.cols, imgHeight - thresholdHeight), Scalar(144));


		int a = 0;
	}

	return 0;
}