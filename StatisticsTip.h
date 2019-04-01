#pragma once
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/opencv.hpp>    
#include <opencv2/imgproc/imgproc.hpp> 
#include "Detect_algorithm.h"

using namespace cv;

class StatisticsTip
{
public:
	StatisticsTip()
	{

	}
	~StatisticsTip();

public:
	int statisticsPin(DetectAlgorithm  &detectAlgorithm);//统计引脚数
	int statisticsPin3();//统计引脚数
};

