#include "opencv2/highgui/highgui.hpp"
#include <opencv2/opencv.hpp>    
#include <opencv2/imgproc/imgproc.hpp> 

using namespace cv;

typedef struct
{
	Mat img;//ͼ��    
	Rect2d ROI;//ͼ���С
	int enlargeNum;
	int narrowNum;

	std::vector<cv::Point2d> left_top;//���ο�����Ͻ�����
	std::vector<cv::Point2d> right_bottom;//���ο�����½�����
}ViewStruct;