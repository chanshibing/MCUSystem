#include "stdafx.h"
#include "ReadImg_algorithm.h"


ReadImgAlgorithm::ReadImgAlgorithm()
{
}


ReadImgAlgorithm::~ReadImgAlgorithm()
{
}

int ReadImgAlgorithm::openFile(HWND m_hWnd, bool &flag_setPara)
{
	filePath = "";

	if (!selectRootFilePath(m_hWnd)) //ѡ���ļ���
	{
		return 1;
	}


	imageFilePathNames.clear();
	imageNames.clear();

	getFiles(filePath, "jpg", imageFilePathNames, imageNames);  //��ȡ�ļ���������jpgͼ���ļ�
	getFiles(filePath, "bmp", imageFilePathNames, imageNames);  //��ȡ�ļ���������bmpͼ���ļ�
	getFiles(filePath, "png", imageFilePathNames, imageNames);  //��ȡ�ļ���������bmpͼ���ļ�
	if (imageFilePathNames.size() == 0)
	{
		return 2;
	}
	//if (imageFilePathNames.size() == 0)
	//{
	//	getFiles(filePath, "jpg", imageFilePathNames, imageNames);  //��ȡ�ļ���������jpgͼ���ļ�
	//	if (imageFilePathNames.size() == 0)
	//	{
	//		getFiles(filePath, "bmp", imageFilePathNames, imageNames);  //��ȡ�ļ���������bmpͼ���ļ�
	//		if (imageFilePathNames.size() == 0)
	//		{
	//			getFiles(filePath, "png", imageFilePathNames, imageNames);  //��ȡ�ļ���������bmpͼ���ļ�
	//			if (imageFilePathNames.size() == 0)
	//			{
	//				return 2;
	//			}
	//		}
	//	}
	//}
	flag_setPara = false;//��������

	imageNum = imageFilePathNames.size(); //��¼��ǰ�����ļ����������ں�����ʾ������ʹ�ã�
	return 0;
}

int ReadImgAlgorithm::readImgFile(HWND m_hWnd)
{
	filePath = "";

	if (!selectRootFilePath(m_hWnd)) //ѡ���ļ���
	{
		return 1;
	}

	imageFilePathNames.clear();
	imageNames.clear();

	getFiles(filePath, "JPG", imageFilePathNames, imageNames);   //��ȡ�ļ���������JPGͼ���ļ�
	if (imageFilePathNames.size() == 0)
	{
		getFiles(filePath, "jpg", imageFilePathNames, imageNames);  //��ȡ�ļ���������jpgͼ���ļ�
		if (imageFilePathNames.size() == 0)
		{
			getFiles(filePath, "bmp", imageFilePathNames, imageNames);  //��ȡ�ļ���������bmpͼ���ļ�
			if (imageFilePathNames.size() == 0)
			{
				return 2;
			}
		}
	}

	imageNum = imageFilePathNames.size(); //��¼��ǰ�����ļ����������ں�����ʾ������ʹ�ã�
	return 0;
}

int ReadImgAlgorithm::reFreshImgFile()
{
	imageFilePathNames.clear();
	imageNames.clear();

	getFiles(filePath, "jpg", imageFilePathNames, imageNames);  //��ȡ�ļ���������jpgͼ���ļ�
	getFiles(filePath, "bmp", imageFilePathNames, imageNames);  //��ȡ�ļ���������bmpͼ���ļ�
	getFiles(filePath, "png", imageFilePathNames, imageNames);  //��ȡ�ļ���������bmpͼ���ļ�
	if (imageFilePathNames.size() == 0)
	{
		return 2;
	}

	imageNum = imageFilePathNames.size(); //��¼��ǰ�����ļ����������ں�����ʾ������ʹ�ã�
	return 0;
}
/*
	#�ص����������ϴ�ѡ���ļ���·��#
*/
int CALLBACK BrowseCallBackFun(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	switch (uMsg)
	{
	case BFFM_INITIALIZED:  //ѡ���ļ��жԻ����ʼ��  
		//����Ĭ��·��ΪlpData��'D:\'  
		::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
		//��STATUSTEXT������ʾ��ǰ·��  
		::SendMessage(hwnd, BFFM_SETSTATUSTEXT, 0, lpData);
		//����ѡ���ļ��жԻ���ı���  
		::SetWindowText(hwnd, TEXT("�������ø�����Ŀ¼"));
		break;
	case BFFM_SELCHANGED:   //ѡ���ļ��б��ʱ  
	{
								TCHAR pszPath[MAX_PATH];
								//��ȡ��ǰѡ��·��  
								SHGetPathFromIDList((LPCITEMIDLIST)lParam, pszPath);
								//��STATUSTEXT������ʾ��ǰ·��  
								::SendMessage(hwnd, BFFM_SETSTATUSTEXT, TRUE, (LPARAM)pszPath);
	}
		break;
	}
	return 0;
}

bool ReadImgAlgorithm::selectRootFilePath(HWND m_hWnd)
{
    //���ѡ���Ŀ¼·�� 
	CString str;
	//ZeroMemory(szPath, sizeof(szPath));

	BROWSEINFO bi;
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szPath;
	bi.lpszTitle = "��ѡ����Ҫ�򿪵�Ŀ¼��";
	bi.ulFlags = BIF_NEWDIALOGSTYLE | BIF_EDITBOX;
	bi.lpfn = BrowseCallBackFun;
	bi.lParam = long(&szPath);
	bi.iImage = 0;
	//����ѡ��Ŀ¼�Ի���
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);

	if (lp && SHGetPathFromIDList(lp, szPath))
	{
		str.Format("%s", szPath);
	}
	else
	{
		return false;
	}

	filePath = str.GetBuffer();

	return true;
}

LPITEMIDLIST ReadImgAlgorithm::GetIDListFromPath(CString strPathName)
{
	if (TRUE == strPathName.IsEmpty())
		return NULL;

	LPSHELLFOLDER pDesktopFolder = NULL;
	HRESULT hr = SHGetDesktopFolder(&pDesktopFolder);
	if (FAILED(hr))
	{
		return NULL;
	}
	// �õ��ļ�·����Ӧ��ItemIDList
	LPITEMIDLIST pItemIDList = NULL;
	LPWSTR  pPathName = strPathName.AllocSysString();
	hr = pDesktopFolder->ParseDisplayName(NULL, NULL, pPathName, NULL, &pItemIDList, NULL);
	pDesktopFolder->Release();
	if (hr != S_OK)
	{
		return NULL;
	}

	return pItemIDList;
}

void ReadImgAlgorithm::getFiles(string path, string exd, vector<string> &files, vector<string> &imageNames)
{
	//�ļ����
	long   hFile = 0;
	//�ļ���Ϣ
	struct _finddata_t fileinfo;
	string pathName, exdName;

	if (0 != strcmp(exd.c_str(), ""))
	{
		exdName = "\\*." + exd;
	}
	else
	{
		exdName = "\\*";
	}

	if ((hFile = _findfirst(pathName.assign(path).append(exdName).c_str(), &fileinfo)) != -1)
	{
		do
		{
			//������ļ����������ļ���,����֮
			//�������,�����б�
			//if ((fileinfo.attrib &  _A_SUBDIR))
			//{
			//	if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
			//		getFiles(pathName.assign(path).append("\\").append(fileinfo.name), exd,files);
			//}
			//else
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				files.push_back(pathName.assign(path).append("\\").append(fileinfo.name));
				imageNames.push_back(fileinfo.name);
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}