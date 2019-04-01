#include "stdafx.h"
#include "Edit_algorithm.h"


EditAlgorithm::EditAlgorithm()
{
}


EditAlgorithm::~EditAlgorithm()
{
}

void EditAlgorithm::leftRotate(Mat &src_img, Rect2d &ROI_src, DetectAlgorithm &detectAlgorithm)
{
	if (view_index == 0)
	{
		//入队
		src_img.copyTo(view.img);
		view.ROI = ROI_src;
		view.enlargeNum = enlargeNum;
		view.narrowNum = narrowNum;
		view.left_top.clear();
		view.right_bottom.clear();
		for (int i = 0; i < detectAlgorithm.instanceRect_vector.size(); i++)
		{
			view.left_top.push_back(Point2d(detectAlgorithm.instanceRect_vector[i].left_top.x, detectAlgorithm.instanceRect_vector[i].left_top.y));
			view.right_bottom.push_back(Point2d(detectAlgorithm.instanceRect_vector[i].right_bottom.x, detectAlgorithm.instanceRect_vector[i].right_bottom.y));
		}
		if (!En_Queue(Q, view))
		{
			return;
		}
		view_index++;
	}
	//左旋90
	transpose(src_img, src_img);
	flip(src_img, src_img, 0);
	//show_img.copyTo(src_img);
	//旋转ROI
	double temp = ROI_src.width;
	ROI_src.width = ROI_src.height;
	ROI_src.height = temp;
	temp = ROI_src.x;
	ROI_src.x = ROI_src.y;
	ROI_src.y = src_img.rows - temp - ROI_src.height;

	view.left_top.clear();
	view.right_bottom.clear();
	for (int i = 0; i < detectAlgorithm.instanceRect_vector.size(); i++)
	{
		temp = detectAlgorithm.instanceRect_vector[i].left_top.x;
		detectAlgorithm.instanceRect_vector[i].left_top.x = detectAlgorithm.instanceRect_vector[i].left_top.y;
		detectAlgorithm.instanceRect_vector[i].left_top.y = ROI_src.height - detectAlgorithm.instanceRect_vector[i].right_bottom.x;
		detectAlgorithm.instanceRect_vector[i].right_bottom.x = detectAlgorithm.instanceRect_vector[i].right_bottom.y;
		detectAlgorithm.instanceRect_vector[i].right_bottom.y = ROI_src.height - temp;
		view.left_top.push_back(Point(detectAlgorithm.instanceRect_vector[i].left_top.x, detectAlgorithm.instanceRect_vector[i].left_top.y));
		view.right_bottom.push_back(Point(detectAlgorithm.instanceRect_vector[i].right_bottom.x, detectAlgorithm.instanceRect_vector[i].right_bottom.y));
	}
	//入队
	if (!De_Index_Queue(Q, view_index))
	{
		return;
	}
	src_img.copyTo(view.img);
	view.ROI = ROI_src;
	if (!En_Queue(Q, view))
	{
		return;
	}
	if (view_index <= 10)
	{
		view_index++;
	}
	max_view_index = view_index;
	view.enlargeNum = enlargeNum;
	view.narrowNum = narrowNum;
}

void EditAlgorithm::rightRotate(Mat &src_img, Rect2d &ROI_src, DetectAlgorithm &detectAlgorithm)
{
	if (view_index == 0)
	{
		//入队
		src_img.copyTo(view.img);
		view.ROI = ROI_src;
		view.enlargeNum = enlargeNum;
		view.narrowNum = narrowNum;
		view.left_top.clear();
		view.right_bottom.clear();
		for (int i = 0; i < detectAlgorithm.instanceRect_vector.size(); i++)
		{
			view.left_top.push_back(Point2d(detectAlgorithm.instanceRect_vector[i].left_top.x, detectAlgorithm.instanceRect_vector[i].left_top.y));
			view.right_bottom.push_back(Point2d(detectAlgorithm.instanceRect_vector[i].right_bottom.x, detectAlgorithm.instanceRect_vector[i].right_bottom.y));
		}
		if (!En_Queue(Q, view))
		{
			return;
		}
		view_index++;
	}
	transpose(src_img, src_img);
	flip(src_img, src_img, 1);
	//show_img.copyTo(src_img);

	//旋转ROI
	double temp = ROI_src.width;
	ROI_src.width = ROI_src.height;
	ROI_src.height = temp;
	temp = ROI_src.y;
	ROI_src.y = ROI_src.x;
	ROI_src.x = src_img.cols - temp - ROI_src.width;

	view.left_top.clear();
	view.right_bottom.clear();
	for (int i = 0; i < detectAlgorithm.instanceRect_vector.size(); i++)
	{
		temp = detectAlgorithm.instanceRect_vector[i].left_top.x;
		detectAlgorithm.instanceRect_vector[i].left_top.x = ROI_src.width - detectAlgorithm.instanceRect_vector[i].right_bottom.y;
		detectAlgorithm.instanceRect_vector[i].right_bottom.y = detectAlgorithm.instanceRect_vector[i].right_bottom.x;
		detectAlgorithm.instanceRect_vector[i].right_bottom.x = ROI_src.width - detectAlgorithm.instanceRect_vector[i].left_top.y;
		detectAlgorithm.instanceRect_vector[i].left_top.y = temp;
		view.left_top.push_back(Point2d(detectAlgorithm.instanceRect_vector[i].left_top.x, detectAlgorithm.instanceRect_vector[i].left_top.y));
		view.right_bottom.push_back(Point2d(detectAlgorithm.instanceRect_vector[i].right_bottom.x, detectAlgorithm.instanceRect_vector[i].right_bottom.y));
	}

	//入队
	if (!De_Index_Queue(Q, view_index))
	{
		return;
	}
	src_img.copyTo(view.img);
	view.ROI = ROI_src;
	if (!En_Queue(Q, view))
	{
		return;
	}
	if (view_index <= 10)
	{
		view_index++;
	}
	max_view_index = view_index;
	view.enlargeNum = enlargeNum;
	view.narrowNum = narrowNum;
}

void EditAlgorithm::enlargeImg(Mat show_img, Rect2d &ROI, Rect2d &ROI_src, DetectAlgorithm &detectAlgorithm)
{
	if (view_index == 0)
	{
		//入队
		show_img.copyTo(view.img);
		view.ROI = ROI_src;
		view.enlargeNum = enlargeNum;
		view.narrowNum = narrowNum;
		view.left_top.clear();
		view.right_bottom.clear();
		for (int i = 0; i < detectAlgorithm.instanceRect_vector.size(); i++)
		{
			view.left_top.push_back(Point2d(detectAlgorithm.instanceRect_vector[i].left_top.x, detectAlgorithm.instanceRect_vector[i].left_top.y));
			view.right_bottom.push_back(Point2d(detectAlgorithm.instanceRect_vector[i].right_bottom.x, detectAlgorithm.instanceRect_vector[i].right_bottom.y));
		}
		if (!En_Queue(Q, view))
		{
			return;
		}
		view_index++;
	}
	double x_move;
	double y_move;
	x_move = (ROI_src.width / 8.0);
	y_move = (ROI_src.height / 8.0);
	ROI.x = ROI_src.x + ROI_src.width / 8.0;
	ROI.y = ROI_src.y + ROI_src.height / 8.0;
	ROI.width = ROI_src.width * 3.0 / 4.0;
	ROI.height = ROI_src.height * 3.0 / 4.0;

	if (ROI.x < 0)
	{
		ROI.x = 0;
		x_move = ROI_src.x - ROI.x;
	}
	else if (ROI.x >= show_img.cols - ROI.width)
	{
		ROI.x = show_img.cols - ROI.width;
		x_move = ROI_src.x - ROI.x;
	}
	if (ROI.y < 0)
	{
		ROI.y = 0;
		y_move = ROI_src.y - ROI.y;
	}
	else if (ROI.y >= show_img.rows - ROI.height)
	{
		ROI.y = show_img.rows - ROI.height;
		y_move = ROI_src.y - ROI.y;
	}
	view.left_top.clear();
	view.right_bottom.clear();
	for (int i = 0; i < detectAlgorithm.instanceRect_vector.size(); i++)
	{
		detectAlgorithm.instanceRect_vector[i].left_top.x = (detectAlgorithm.instanceRect_vector[i].left_top.x - x_move);
		detectAlgorithm.instanceRect_vector[i].left_top.y = (detectAlgorithm.instanceRect_vector[i].left_top.y - y_move);
		detectAlgorithm.instanceRect_vector[i].right_bottom.x = (detectAlgorithm.instanceRect_vector[i].right_bottom.x - x_move);
		detectAlgorithm.instanceRect_vector[i].right_bottom.y = (detectAlgorithm.instanceRect_vector[i].right_bottom.y - y_move);
		view.left_top.push_back(Point2d(detectAlgorithm.instanceRect_vector[i].left_top.x, detectAlgorithm.instanceRect_vector[i].left_top.y));
		view.right_bottom.push_back(Point2d(detectAlgorithm.instanceRect_vector[i].right_bottom.x, detectAlgorithm.instanceRect_vector[i].right_bottom.y));
	}

	ROI_src = ROI;
	enlargeNum++;
	narrowNum++;
	//入队
	if (!De_Index_Queue(Q, view_index))
	{
		return;
	}
	show_img.copyTo(view.img);
	view.ROI = ROI_src;
	view.enlargeNum = enlargeNum;
	view.narrowNum = narrowNum;
	if (!En_Queue(Q, view))
	{
		return;
	}
	if (view_index <= 10)
	{
		view_index++;
	}
	max_view_index = view_index;
}

void EditAlgorithm::narrowImg(Mat show_img, Rect2d &ROI, Rect2d &ROI_src, DetectAlgorithm &detectAlgorithm)
{
	if (view_index == 0)
	{
		//入队
		show_img.copyTo(view.img);
		view.ROI = ROI_src;
		view.enlargeNum = enlargeNum;
		view.narrowNum = narrowNum;
		view.left_top.clear();
		view.right_bottom.clear();
		for (int i = 0; i < detectAlgorithm.instanceRect_vector.size(); i++)
		{
			view.left_top.push_back(Point2d(detectAlgorithm.instanceRect_vector[i].left_top.x, detectAlgorithm.instanceRect_vector[i].left_top.y));
			view.right_bottom.push_back(Point2d(detectAlgorithm.instanceRect_vector[i].right_bottom.x, detectAlgorithm.instanceRect_vector[i].right_bottom.y));
		}
		if (!En_Queue(Q, view))
		{
			return;
		}
		view_index++;
	}

	double x_move;
	double y_move;
	x_move = (ROI_src.width / 6.0);
	y_move = (ROI_src.height / 6.0);
	ROI.x = ROI_src.x - ROI_src.width / 6.0;
	ROI.y = ROI_src.y - ROI_src.height / 6.0;
	ROI.width = ROI_src.width * 4.0 / 3.0;
	ROI.height = ROI_src.height * 4.0 / 3.0;

	if (ROI.x < 0)
	{
		ROI.x = 0;
		x_move = ROI_src.x - ROI.x;
	}
	else if (ROI.x >= show_img.cols - ROI.width)
	{
		ROI.x = show_img.cols - ROI.width;
		x_move = ROI_src.x - ROI.x;
	}
	if (ROI.y < 0)
	{
		ROI.y = 0;
		y_move = ROI_src.y - ROI.y;
	}
	else if (ROI.y >= show_img.rows - ROI.height)
	{
		ROI.y = show_img.rows - ROI.height;
		y_move = ROI_src.y - ROI.y;
	}
	view.left_top.clear();
	view.right_bottom.clear();
	for (int i = 0; i < detectAlgorithm.instanceRect_vector.size(); i++)
	{
		detectAlgorithm.instanceRect_vector[i].left_top.x = (detectAlgorithm.instanceRect_vector[i].left_top.x + x_move);
		detectAlgorithm.instanceRect_vector[i].left_top.y = (detectAlgorithm.instanceRect_vector[i].left_top.y + y_move);
		detectAlgorithm.instanceRect_vector[i].right_bottom.x = (detectAlgorithm.instanceRect_vector[i].right_bottom.x + x_move);
		detectAlgorithm.instanceRect_vector[i].right_bottom.y = (detectAlgorithm.instanceRect_vector[i].right_bottom.y + y_move);
		view.left_top.push_back(Point2d(detectAlgorithm.instanceRect_vector[i].left_top.x, detectAlgorithm.instanceRect_vector[i].left_top.y));
		view.right_bottom.push_back(Point2d(detectAlgorithm.instanceRect_vector[i].right_bottom.x, detectAlgorithm.instanceRect_vector[i].right_bottom.y));
	}

	ROI_src = ROI;
	enlargeNum--;
	narrowNum--;
	//入队
	if (!De_Index_Queue(Q, view_index))
	{
		return;
	}
	show_img.copyTo(view.img);
	view.ROI = ROI_src;
	view.enlargeNum = enlargeNum;
	view.narrowNum = narrowNum;
	if (!En_Queue(Q, view))
	{
		return;
	}
	if (view_index <= 10)
	{
		view_index++;
	}
	max_view_index = view_index;
}
/*
	#拖拽函数#
	参数：
		1、原图
		2、原图的ROI
		3、鼠标按下时ROI的坐标
		4、鼠标按下时的坐标
		5、鼠标当前坐标
		6、检测对象
		7、鼠标按下时矩形框左上角坐标集合
		8、鼠标按下时矩形框右下角坐标集合
*/
void  EditAlgorithm::draftingImg(Mat show_img, Rect2d &ROI_src, Rect2d source_point, Rect lButtonDown_point, CPoint point, DetectAlgorithm &detectAlgorithm, vector<Point> leftBtnDown_lt, vector<Point> leftBtnDown_rb)
{
	double x_move = lButtonDown_point.x - point.x;
	double y_move = lButtonDown_point.y - point.y;
	ROI_src.x = source_point.x + x_move;
	ROI_src.y = source_point.y + y_move;
	if (ROI_src.x < 0)
	{
		ROI_src.x = 0;
		x_move = ROI_src.x - source_point.x;
	}
	else if (ROI_src.x + ROI_src.width >= show_img.cols)
	{
		ROI_src.x = show_img.cols - ROI_src.width;
		x_move = ROI_src.x - source_point.x;
	}
	if (ROI_src.y < 0)
	{
		ROI_src.y = 0;
		y_move = ROI_src.y - source_point.y;
	}
	else if (ROI_src.y + ROI_src.height >= show_img.rows)
	{
		ROI_src.y = show_img.rows - ROI_src.height;
		y_move = ROI_src.y - source_point.y;
	}
	for (int i = 0; i < detectAlgorithm.instanceRect_vector.size(); i++)
	{
		detectAlgorithm.instanceRect_vector[i].left_top.x = (leftBtnDown_lt[i].x - x_move);
		detectAlgorithm.instanceRect_vector[i].right_bottom.x = (leftBtnDown_rb[i].x - x_move);
		detectAlgorithm.instanceRect_vector[i].left_top.y = (leftBtnDown_lt[i].y - y_move);
		detectAlgorithm.instanceRect_vector[i].right_bottom.y = (leftBtnDown_rb[i].y - y_move);
	}
}

void EditAlgorithm::drawCutRect(Mat src_img, Mat &show_img, Rect2d ROI_src, Rect2d &ROI_cut, Rect lButtonDown_point, Rect pictureInClient_ltPoint, CPoint point, int &lineThickness, float pictureRatio)
{
	src_img.copyTo(show_img);
	Point2d beginPoint;//开始位置
	Point2d endPoint;//结束位置
	//按比例在原图中求解
	beginPoint.x = ROI_src.x + (lButtonDown_point.x - pictureInClient_ltPoint.x) / pictureRatio;
	beginPoint.y = ROI_src.y + (lButtonDown_point.y - pictureInClient_ltPoint.y) / pictureRatio;
	endPoint.x = ROI_src.x + (point.x - pictureInClient_ltPoint.x) / pictureRatio;
	endPoint.y = ROI_src.y + (point.y - pictureInClient_ltPoint.y) / pictureRatio;

	ROI_cut = Rect2d(beginPoint, endPoint);

	//控制线条粗细
	if (ROI_src.width < 300 || ROI_src.height < 300){
		lineThickness = 1;
	}
	else if (ROI_src.width < 500 || ROI_src.height < 500){
		lineThickness = 2;
	}
	else{
		lineThickness = 4;
	}

	rectangle(show_img,
		Point(beginPoint.x, beginPoint.y),
		Point(endPoint.x, endPoint.y),
		Scalar(0, 0, 0),
		lineThickness,
		8);
}

void EditAlgorithm::cutImg(Mat &src_img, Mat &show_img, Rect2d &ROI_src, Rect2d ROI_cut, DetectAlgorithm &detectAlgorithm)
{
	if (view_index == 0)
	{
		//入队
		src_img.copyTo(view.img);
		view.ROI = ROI_src;
		view.enlargeNum = enlargeNum;
		view.narrowNum = narrowNum;
		view.left_top.clear();
		view.right_bottom.clear();
		for (int i = 0; i < detectAlgorithm.instanceRect_vector.size(); i++)
		{
			view.left_top.push_back(Point2d(detectAlgorithm.instanceRect_vector[i].left_top.x, detectAlgorithm.instanceRect_vector[i].left_top.y));
			view.right_bottom.push_back(Point2d(detectAlgorithm.instanceRect_vector[i].right_bottom.x, detectAlgorithm.instanceRect_vector[i].right_bottom.y));
		}
		if (!En_Queue(Q, view))
		{
			return;
		}
		view_index++;
	}
	//src_img.copyTo(show_img);
	src_img = src_img(ROI_cut);
	src_img.copyTo(show_img);
	double x_move = ROI_cut.x - ROI_src.x;
	double y_move = ROI_cut.y - ROI_src.y;
	view.left_top.clear();
	view.right_bottom.clear();
	for (int i = 0; i < detectAlgorithm.instanceRect_vector.size(); i++)
	{
		detectAlgorithm.instanceRect_vector[i].left_top.x = (detectAlgorithm.instanceRect_vector[i].left_top.x - x_move);
		detectAlgorithm.instanceRect_vector[i].left_top.y = (detectAlgorithm.instanceRect_vector[i].left_top.y - y_move);
		detectAlgorithm.instanceRect_vector[i].right_bottom.x = (detectAlgorithm.instanceRect_vector[i].right_bottom.x - x_move);
		detectAlgorithm.instanceRect_vector[i].right_bottom.y = (detectAlgorithm.instanceRect_vector[i].right_bottom.y - y_move);
		view.left_top.push_back(Point2d(detectAlgorithm.instanceRect_vector[i].left_top.x, detectAlgorithm.instanceRect_vector[i].left_top.y));
		view.right_bottom.push_back(Point2d(detectAlgorithm.instanceRect_vector[i].right_bottom.x, detectAlgorithm.instanceRect_vector[i].right_bottom.y));
	}

	ROI_src = ROI_cut;
	//重新初始化
	ROI_src.x = 0;
	ROI_src.y = 0;
	enlargeNum = 0;
	narrowNum = 0;
	//入队
	if (!De_Index_Queue(Q, view_index))
	{
		return;
	}
	show_img.copyTo(view.img);
	view.ROI = ROI_src;
	view.enlargeNum = enlargeNum;
	view.narrowNum = narrowNum;
	if (!En_Queue(Q, view))
	{
		return;
	}
	if (view_index <= 10)
	{
		view_index++;
	}
	max_view_index = view_index;
}

void EditAlgorithm::GetStringSize(HDC hDC, const char* str, int* w, int* h)
{
	SIZE size;
	GetTextExtentPoint32A(hDC, str, strlen(str), &size);
	if (w != 0) *w = size.cx;
	if (h != 0) *h = size.cy;
}

/*
参数：
	1、原图
	2、图像显示区域
	3、字符串
	4、文字的位置
	5、颜色
	6、字体大小
	7、斜体
	8、下划线
*/
void EditAlgorithm::paDrawString(Mat& dst, Rect ROI_src, const char* str, Point org, Scalar color, int fontSize, bool italic, bool underline, DetectAlgorithm &detectAlgorithm)
{
	if (view_index == 0)
	{
		//入队
		dst.copyTo(view.img);
		view.ROI = ROI_src;
		view.enlargeNum = enlargeNum;
		view.narrowNum = narrowNum; 
		view.left_top.clear();
		view.right_bottom.clear();
		for (int i = 0; i < detectAlgorithm.instanceRect_vector.size(); i++)
		{
			view.left_top.push_back(Point2d(detectAlgorithm.instanceRect_vector[i].left_top.x, detectAlgorithm.instanceRect_vector[i].left_top.y));
			view.right_bottom.push_back(Point2d(detectAlgorithm.instanceRect_vector[i].right_bottom.x, detectAlgorithm.instanceRect_vector[i].right_bottom.y));
		}
		if (!En_Queue(Q, view))
		{
			return;
		}
		view_index++;
	}
	CV_Assert(dst.data != 0 && (dst.channels() == 1 || dst.channels() == 3));
	int x, y, r, b;
	if (org.x > dst.cols || org.y > dst.rows) return;
	x = org.x < 0 ? -org.x : 0;
	y = org.y < 0 ? -org.y : 0;
	LOGFONTA lf;
	lf.lfHeight = -fontSize;
	lf.lfWidth = 0;
	lf.lfEscapement = 0;
	lf.lfOrientation = 0;
	lf.lfWeight = 10;
	lf.lfItalic = FALSE;	//斜体
	lf.lfUnderline = FALSE;	//下划线
	lf.lfStrikeOut = 0;
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfOutPrecision = 0;
	lf.lfClipPrecision = 0;
	lf.lfQuality = PROOF_QUALITY;
	lf.lfPitchAndFamily = 0;
	strcpy(lf.lfFaceName, "宋体");
	HFONT hf = CreateFontIndirectA(&lf);
	HDC hDC = CreateCompatibleDC(0);
	HFONT hOldFont = (HFONT)SelectObject(hDC, hf);
	int strBaseW = 0, strBaseH = 0;
	int singleRow = 0;
	char buf[1 << 12];
	strcpy(buf, str);
	//处理多行
	{
		int nnh = 0;
		int cw, ch;
		const char* ln = strtok(buf, "\n");
		while (ln != 0)
		{
			GetStringSize(hDC, ln, &cw, &ch);
			strBaseW = max(strBaseW, cw);
			strBaseH = max(strBaseH, ch);
			ln = strtok(0, "\n");
			nnh++;
		}
		singleRow = strBaseH;
		strBaseH *= nnh;
	}
	if (org.x + strBaseW < 0 || org.y + strBaseH < 0)
	{
		SelectObject(hDC, hOldFont);
		DeleteObject(hf);
		DeleteObject(hDC);
		return;
	}
	r = org.x + strBaseW > dst.cols ? dst.cols - org.x - 1 : strBaseW - 1;
	b = org.y + strBaseH > dst.rows ? dst.rows - org.y - 1 : strBaseH - 1;
	org.x = org.x < 0 ? 0 : org.x;
	org.y = org.y < 0 ? 0 : org.y;
	BITMAPINFO bmp = { 0 };
	BITMAPINFOHEADER& bih = bmp.bmiHeader;
	int strDrawLineStep = strBaseW * 3 % 4 == 0 ? strBaseW * 3 : (strBaseW * 3 + 4 - ((strBaseW * 3) % 4));
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biWidth = strBaseW;
	bih.biHeight = strBaseH;
	bih.biPlanes = 1;
	bih.biBitCount = 24;
	bih.biCompression = BI_RGB;
	bih.biSizeImage = strBaseH * strDrawLineStep;
	bih.biClrUsed = 0;
	bih.biClrImportant = 0;
	void* pDibData = 0;
	HBITMAP hBmp = CreateDIBSection(hDC, &bmp, DIB_RGB_COLORS, &pDibData, 0, 0);
	CV_Assert(pDibData != 0);
	HBITMAP hOldBmp = (HBITMAP)SelectObject(hDC, hBmp);
	//color.val[2], color.val[1], color.val[0]
	SetTextColor(hDC, RGB(255, 255, 255));
	SetBkColor(hDC, 0);
	//SetStretchBltMode(hDC, COLORONCOLOR);
	strcpy(buf, str);
	const char* ln = strtok(buf, "\n");
	int outTextY = 0;
	while (ln != 0)
	{
		TextOutA(hDC, 0, outTextY, ln, strlen(ln));
		outTextY += singleRow;
		ln = strtok(0, "\n");
	}
	uchar* dstData = (uchar*)dst.data;
	int dstStep = dst.step / sizeof(dstData[0]);
	unsigned char* pImg = (unsigned char*)dst.data + org.x * dst.channels() + org.y * dstStep;
	unsigned char* pStr = (unsigned char*)pDibData + x * 3;
	for (int tty = y; tty <= b; ++tty)
	{
		unsigned char* subImg = pImg + (tty - y) * dstStep;
		unsigned char* subStr = pStr + (strBaseH - tty - 1) * strDrawLineStep;
		for (int ttx = x; ttx <= r; ++ttx)
		{
			for (int n = 0; n < dst.channels(); ++n){
				double vtxt = subStr[n] / 255.0;
				int cvv = vtxt * color.val[n] + (1 - vtxt) * subImg[n];
				subImg[n] = cvv > 255 ? 255 : (cvv < 0 ? 0 : cvv);
			}
			subStr += 3;
			subImg += dst.channels();
		}
	}
	SelectObject(hDC, hOldBmp);
	SelectObject(hDC, hOldFont);
	DeleteObject(hf);
	DeleteObject(hBmp);
	DeleteDC(hDC);
	//入队
	view.left_top.clear();
	view.right_bottom.clear();
	for (int i = 0; i < detectAlgorithm.instanceRect_vector.size(); i++)
	{
		view.left_top.push_back(Point2d(detectAlgorithm.instanceRect_vector[i].left_top.x, detectAlgorithm.instanceRect_vector[i].left_top.y));
		view.right_bottom.push_back(Point2d(detectAlgorithm.instanceRect_vector[i].right_bottom.x, detectAlgorithm.instanceRect_vector[i].right_bottom.y));
	}
	if (!De_Index_Queue(Q, view_index))
	{
		return;
	}
	dst.copyTo(view.img);
	view.ROI = ROI_src;
	view.enlargeNum = enlargeNum;
	view.narrowNum = narrowNum;
	if (!En_Queue(Q, view))
	{
		return;
	}
	if (view_index <= 10)
	{
		view_index++;
	}
	max_view_index = view_index;
}

/*
	撤销
*/
void EditAlgorithm::revoke(Mat &show_img, Rect2d &ROI, DetectAlgorithm &detectAlgorithm)
{
	view_index--;//当前位置减一
	ViewStruct view_ = element_atIndex(Q, view_index);//获取当前位置元素
	view_.img.copyTo(show_img);
	ROI = view_.ROI;
	enlargeNum = view_.enlargeNum;
	narrowNum = view_.narrowNum;
	for (int i = 0; i <view_.left_top.size(); i++)
	{
		detectAlgorithm.instanceRect_vector[i].left_top = view_.left_top[i];
		detectAlgorithm.instanceRect_vector[i].right_bottom = view_.right_bottom[i];
	}

}

/*
	恢复
*/
void EditAlgorithm::redo(Mat &show_img, Rect2d &ROI, DetectAlgorithm &detectAlgorithm)
{
	view_index++;//当前位置减一
	ViewStruct view_ = element_atIndex(Q, view_index);//获取当前位置元素
	view_.img.copyTo(show_img);
	ROI = view_.ROI;
	enlargeNum = view_.enlargeNum;
	narrowNum = view_.narrowNum;
	for (int i = 0; i < view_.left_top.size(); i++)
	{
		detectAlgorithm.instanceRect_vector[i].left_top = view_.left_top[i];
		detectAlgorithm.instanceRect_vector[i].right_bottom = view_.right_bottom[i];
	}
}

/*
	设置为空
*/
void EditAlgorithm::setQueue_NULL()
{
	if (!Set_NULL(Q))
	{
		return;
	}
	view_index = 0;
	max_view_index = 0;
}