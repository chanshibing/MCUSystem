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

	string filePath;//�ļ�Ŀ¼
	vector<string> imageFilePathNames; //ͼƬ�ļ�Ŀ¼
	vector<string> imageNames;//ͼƬ�ļ���
	int imageNum = 0;//��Ƭ������

	int openFile(HWND m_hWnd, bool &flag_setPara);//���ļ�Ŀ¼
	int readImgFile(HWND m_hWnd);//����ͼƬ�ļ�Ŀ¼
	int reFreshImgFile();//����ͼƬĿ¼
	char szPath[MAX_PATH];

protected:

	bool selectRootFilePath(HWND m_hWnd);//ѡ��ͼƬ·��
	void getFiles(string path, string exd, vector<string> &files, vector<string> &imageNames);//��ȡ�ļ����е������ļ���
	LPITEMIDLIST GetIDListFromPath(CString strPathName);
};

