#pragma once

#include <string>
#include <vector>
#include "io.h"

using namespace std;

class ReadImgAlgorithm
{
public:
	ReadImgAlgorithm();
	~ReadImgAlgorithm();

	string filePath;//文件目录
	vector<string> imageFilePathNames; //图片文件目录
	vector<string> imageNames;//图片文件名
	int imageNum = 0;//照片的数量

	int openFile(HWND m_hWnd, bool &flag_setPara);//打开文件目录
	int readImgFile(HWND m_hWnd);//载入图片文件目录
	int reFreshImgFile();//更新图片目录
	char szPath[MAX_PATH];

protected:

	bool selectRootFilePath(HWND m_hWnd);//选择图片路径
	void getFiles(string path, string exd, vector<string> &files, vector<string> &imageNames);//获取文件夹中的所有文件名
	LPITEMIDLIST GetIDListFromPath(CString strPathName);
};

