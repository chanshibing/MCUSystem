#include "stdafx.h"
#include "PredictWord.h"

Mat PredictWord::transform(Mat b_img, int filled_size)
{//���ָ���ֹ�һ�����м䣬ͼƬΪfilled_size��С
	//assert(b_img.channels() == 1);
	if (b_img.channels() == 3 || b_img.channels() == 4)
		cvtColor(b_img, b_img, CV_BGR2GRAY);
	b_img = 255 - b_img;
	//assert(filled_size >= b_img.cols&&filled_size >= b_img.rows);
	Rect r = boundingRect(b_img);//�ּ��ǰ�ɫ��
	//rectangle(b_img, r, 0);
	Mat roi;
	if (r.width == 0 || r.height == 0) {
		roi = Mat::zeros(filled_size, filled_size, CV_8UC1);
	}
	else {
		roi = b_img(r).clone();
	}
	double fxy = double(filled_size) / std::max(roi.cols, roi.rows);
	resize(roi, roi, Size(0, 0), fxy, fxy);
	threshold(roi, roi, 128, 255, cv::THRESH_BINARY);
	Mat res(filled_size, filled_size, CV_8UC1);
	res = 0;
	int off_w = (filled_size - roi.cols) / 2;
	int off_h = (filled_size - roi.rows) / 2;
	Mat nroi = res(Rect(off_w, off_h, roi.cols, roi.rows));
	nroi = nroi + roi;
	res = 255 - res;
	return res;
}
