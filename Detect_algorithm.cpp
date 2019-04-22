#include "stdafx.h"
#include "Detect_algorithm.h"
#include "PredictWord.h"
#include "Excel.h"

DetectAlgorithm::DetectAlgorithm()
{
}


DetectAlgorithm::~DetectAlgorithm()
{
}

/*
	��ͼ�������
*/
void DetectAlgorithm::detect_bat(CString imageDir)
{
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	string batName = "run_detect.bat";
	ShExecInfo.lpFile = (LPCSTR)batName.c_str();
	ShExecInfo.lpParameters = imageDir;//���ó���������в���
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = 0;
	ShExecInfo.hInstApp = NULL;
	BOOL ret = ShellExecuteEx(&ShExecInfo);//ִ��exe
	WaitForSingleObject(ShExecInfo.hProcess, INFINITE);//�ȴ��������
	CloseHandle(ShExecInfo.hProcess);
}

/*
	ɱ��ͼ�������
*/
void DetectAlgorithm::kill_detect()
{
	remove("python//log//information.txt");
	ShExecInfo_kill.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo_kill.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo_kill.hwnd = NULL;
	ShExecInfo_kill.lpVerb = NULL;
	string batName = "kill_detect.bat";
	ShExecInfo_kill.lpFile = (LPCSTR)batName.c_str();
	ShExecInfo_kill.lpParameters = NULL;//���ó���������в���
	ShExecInfo_kill.lpDirectory = NULL;
	ShExecInfo_kill.nShow = 0;
	ShExecInfo_kill.hInstApp = NULL;
	BOOL ret = ShellExecuteEx(&ShExecInfo_kill);//ִ��exe
	WaitForSingleObject(ShExecInfo_kill.hProcess, INFINITE);//�ȴ��������
	CloseHandle(ShExecInfo_kill.hProcess);
}

/*
	���������
*/
int DetectAlgorithm::drawDetectInstances(Mat &src_img, vector<CString> className, int wordSize)
{
	InstanceRect instanceRect;
	instanceRect_vector.clear();
	//��information.txt�ļ�
	ifstream information;
	information.open(("python\\log\\information.txt"));
	if (!information)
	{
		information.close();
		return 0;
	}
	src_img.copyTo(detect_src_img);//��������excel����и�ԭͼƬ
	string s;
	vector<int> sv;
	Excel excel;
	while (getline(information, s))//һ��ͼ������Ԫ����ѭ��
	{
		excel.split(s, sv, '_');
		if (sv.size() == 5)//���sv������ȷ
		{
			//���������ӽ��ṹ����
			instanceRect.left_top.x = sv[1];
			instanceRect.left_top.y = sv[0];
			instanceRect.right_bottom.x = sv[3];
			instanceRect.right_bottom.y = sv[2];
			instanceRect.instanceClass = sv[4];
			numOfClassName[instanceRect.instanceClass - 1]++;//���������һ
			instanceRect.currentIndex = numOfClassName[instanceRect.instanceClass - 1];//��ǰ�������±�

			//����������
			if (className[instanceRect.instanceClass - 1] == "R"){

				Mat splitMat = cutRect(src_img, instanceRect.left_top, instanceRect.right_bottom);//�и����

				if (splitMat.cols < splitMat.rows)//����ַ���ֱ,����90
				{
					transpose(splitMat, splitMat);
					flip(splitMat, splitMat, 0);
				}
				splitWord.splitWordProcess(splitMat);//�ַ��ָ�

				char num_string[10] = "";
				for (int i = 0; i < splitWord.splitedMat.size(); i++)
				{
					//�ַ����
					Mat s;
					NumberPredict numberPredict;
					s = splitWord.splitedMat[i];
					Vec2d result = numberPredict.preditWord(s); //����׵׺���
					num_string[i] = numberPredict.num_cls[result[0]];
					instanceRect.instanceWord = num_string;
				}
			}
			else{
				instanceRect.instanceWord = "";
			}
			instanceRect.manaulAdd = 0;//����������
			instanceRect_vector.push_back(instanceRect);
			CString instanceString = "";
			//��ԭͼ�ϻ������ϵ��������
			if (instanceRect.instanceClass > className.size())
			{
				return 1;
			}
			instanceString.Format("%s%d", className[instanceRect.instanceClass - 1], instanceRect.currentIndex);
			const char* p = instanceString;//stringתconst char*
			drawInstanceClassString(src_img, p, instanceRect.left_top, Scalar(0, 0, 255), wordSize, false, false);//���û�������ֺ���
		}
	}
	information.close();//�ر����ļ�
	remove("python\\log\\information.txt");//ɾ���ļ�

	return 2;
}

void DetectAlgorithm::GetStringSize(HDC hDC, const char* str, int* w, int* h)
{
	SIZE size;
	GetTextExtentPoint32A(hDC, str, strlen(str), &size);
	if (w != 0) *w = size.cx;
	if (h != 0) *h = size.cy;
}

/*
	#��ԭͼ�ϻ����������#
	������
		1��ԭͼ
		2������
		3��λ��
		4����ɫ
		5�������С
		6��б��
		7���»���
*/
void DetectAlgorithm::drawInstanceClassString(Mat &src_img, const char* str, Point org, Scalar color, int fontSize, bool italic, bool underline)
{
	if (!str)//Ϊ�վͲ�����
		return;
	CV_Assert(src_img.data != 0 && (src_img.channels() == 1 || src_img.channels() == 3));
	int x, y, r, b;
	if (org.x > src_img.cols || org.y > src_img.rows) return;
	x = org.x < 0 ? -org.x : 0;
	y = org.y < 0 ? -org.y : 0;
	LOGFONTA lf;
	lf.lfHeight = -fontSize;
	lf.lfWidth = 0;
	lf.lfEscapement = 0;
	lf.lfOrientation = 0;
	lf.lfWeight = 10;
	lf.lfItalic = italic;	//б��
	lf.lfUnderline = underline;	//�»���
	lf.lfStrikeOut = 0;
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfOutPrecision = 0;
	lf.lfClipPrecision = 0;
	lf.lfQuality = PROOF_QUALITY;
	lf.lfPitchAndFamily = 0;
	strcpy(lf.lfFaceName, "����");
	HFONT hf = CreateFontIndirectA(&lf);
	HDC hDC = CreateCompatibleDC(0);
	HFONT hOldFont = (HFONT)SelectObject(hDC, hf);
	int strBaseW = 0, strBaseH = 0;
	int singleRow = 0;
	char buf[1 << 12];
	strcpy(buf, str);
	//�������
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
	r = org.x + strBaseW > src_img.cols ? src_img.cols - org.x - 1 : strBaseW - 1;
	b = org.y + strBaseH > src_img.rows ? src_img.rows - org.y - 1 : strBaseH - 1;
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
	uchar* dstData = (uchar*)src_img.data;
	int dstStep = src_img.step / sizeof(dstData[0]);
	unsigned char* pImg = (unsigned char*)src_img.data + org.x * src_img.channels() + org.y * dstStep;
	unsigned char* pStr = (unsigned char*)pDibData + x * 3;
	for (int tty = y; tty <= b; ++tty)
	{
		unsigned char* subImg = pImg + (tty - y) * dstStep;
		unsigned char* subStr = pStr + (strBaseH - tty - 1) * strDrawLineStep;
		for (int ttx = x; ttx <= r; ++ttx)
		{
			for (int n = 0; n < src_img.channels(); ++n){
				double vtxt = subStr[n] / 255.0;
				int cvv = vtxt * color.val[n] + (1 - vtxt) * subImg[n];
				subImg[n] = cvv > 255 ? 255 : (cvv < 0 ? 0 : cvv);
			}
			subStr += 3;
			subImg += src_img.channels();
		}
	}
	SelectObject(hDC, hOldBmp);
	SelectObject(hDC, hOldFont);
	DeleteObject(hf);
	DeleteObject(hBmp);
	DeleteDC(hDC);
}

/*
������
1��ԭͼ
2�����Ͻ�����
3�����½�����
*/
void DetectAlgorithm::drawRect(Mat &src_img, CVPoint2d begin, CVPoint2d end, int manaulAdd)
{
	int lineThickness = 4;
	//����������ϸ
	if (src_img.cols < 300 || src_img.rows < 300){
		lineThickness = 1;
	}
	else if (src_img.cols < 500 || src_img.rows < 500){
		lineThickness = 2;
	}
	else{
		lineThickness = 4;
	}
	if (manaulAdd == 1)
	{
		rectangle(src_img,
			Point(begin.x, begin.y),
			Point(end.x, end.y),
			Scalar(255, 0, 0),
			lineThickness,
			8);
	}
	else if (manaulAdd == 0)
	{
		rectangle(src_img,
			Point(begin.x, begin.y),
			Point(end.x, end.y),
			Scalar(0, 0, 255),
			lineThickness,
			8);
	}
}

/*
	������
		1��ԭͼ
		2�����Ͻ�����
		3�����½�����
*/
Mat DetectAlgorithm::cutRect(Mat src_img, CVPoint2d begin, CVPoint2d end)
{
	Rect2d cut_RIO;
	cut_RIO.x = begin.x;
	cut_RIO.y = begin.y;
	cut_RIO.width = end.x - begin.x;
	cut_RIO.height = end.y - begin.y;
	Mat cutImg = src_img(cut_RIO);
	return cutImg;
}

void DetectAlgorithm::drawAddDetect(CWnd* pWnd, Point pictureInClient_ltPoint, Point pictureInClient_rbPoint, Point lButtonDown_point, CPoint point, double pictureRatio)
{
	CClientDC dc(pWnd);
	dc.SelectStockObject(NULL_BRUSH);
	CPen pen(PS_SOLID, 2, RGB(0, 0, 255));//��һ֧��ɫ��ϸΪ1�ı�
	dc.SelectObject(&pen);//��pen�ŵ�dc��
	
	CRect dcRect;
	dcRect.left = lButtonDown_point.x;
	dcRect.top = lButtonDown_point.y;
	dcRect.right = point.x;
	dcRect.bottom = point.y;
	dc.Rectangle(dcRect);
}