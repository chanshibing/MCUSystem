#include "opencv2/highgui/highgui.hpp"
#include <opencv2/opencv.hpp>    
#include <opencv2/imgproc/imgproc.hpp> 

using namespace cv;

typedef struct
{
	Mat img;//图像    
	Rect2d ROI;//图像大小
	int enlargeNum;
	int narrowNum;

	std::vector<cv::Point2d> left_top;//矩形框的左上角坐标
	std::vector<cv::Point2d> right_bottom;//矩形框的右下角坐标
}ViewStruct;