#pragma once
#include <opencv2/opencv.hpp>  
#include <opencv2/dnn.hpp>  
#include <iostream>

#include <fstream>
#include <iostream>
#include <cstdlib>
#include <opencv2/core/ocl.hpp>

using namespace std;
using namespace cv;
using namespace cv::dnn;

class PredictWord
{
private:
	int n;
protected:
	dnn::Net *net;
public:
	PredictWord(int n) {
		net = new dnn::Net[n];
		this->n = n;
	}
	~PredictWord() {
		delete[]net;
	}
	static void getMaxClass(const Mat &probBlob, int *classId, double *classProb)
	{
		Mat probMat = probBlob.reshape(1, 1);
		Point classNumber;
		minMaxLoc(probMat, NULL, classProb, NULL, &classNumber);
		*classId = classNumber.x;
	}

	Vec2d preditWord(Mat src_img, int style =0)
	{
		if (style < 0 || style >= this->n)return{};
		src_img = transform(src_img);
		//这个28是和训练时的输入图片一致，，，，  最浩裁剪的图片是28*28，，
		Mat inputBlob = dnn::blobFromImage(src_img, 0.00390625f, Size(28, 28), Scalar(), false); //Convert Mat to batch of images
		Mat pred;
		net[style].setInput(inputBlob, "data");//set the network input, "data" is the name of the input layer   
		pred = net[style].forward("prob");//compute output, "prob" is the name of the output layer   
		//cout << pred << endl;
		int classId;
		double classProb;
		getMaxClass(pred, &classId, &classProb);
		//cout << "Best Class: " << classId << endl;
		//cout << "Probability: " << classProb * 100 << "%" << endl;
		return Vec2d(classId, classProb);
	}

	Mat transform(Mat b_img, int filled_size = 30);//传入白底黑字,传出白底黑字  
};

//竞彩足球  
class NumberPredict :public PredictWord {
private:
public:
	const string num_cls = "0123456789-/:x()@.+";//一共有19个字符
	const string word_cls = "竞彩足球胜平负让半全场比分总进数混合过关方式单固定倍计元";
	const string dig_eng_cls = "0123456789ABCDEF";
	//dnn::Net net[5];//all,word,num;
	const int ALL = 0;//数字，符号，汉字
	const int WORD = 1;//汉字
	const int NUM = 2;//数字，符号
	const int Only_NUM = 5;//纯数字
	const int DIG_ENG = 3;//数字和字母
	const int WEEK = 4;//周几  一二。。。六日
	const int num_num = 19;
	const int word_num = 28;

	NumberPredict() :PredictWord(1) {
		string modelTxt = "./model/num.prototxt";
		string modelBin = "./model/num.caffemodel";
		try {
			net[0] = dnn::readNetFromCaffe(modelTxt, modelBin);
		}
		catch (cv::Exception &ee) {
			cerr << "Exception: " << ee.what() << endl;
			if (net[0].empty()) {
				cout << "Can't load the network by using the flowing files:" << endl;
				cout << "modelTxt: " << modelTxt << endl;
				cout << "modelBin: " << modelBin << endl;
				exit(-1);
			}
		}
	}
};
