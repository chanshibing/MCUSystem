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

	if (!selectRootFilePath(m_hWnd)) //选择文件夹
	{
		return 1;
	}


	imageFilePathNames.clear();
	imageNames.clear();

	getFiles(filePath, "jpg", imageFilePathNames, imageNames);  //读取文件夹内所有jpg图像文件
	getFiles(filePath, "bmp", imageFilePathNames, imageNames);  //读取文件夹内所有bmp图像文件
	getFiles(filePath, "png", imageFilePathNames, imageNames);  //读取文件夹内所有bmp图像文件
	if (imageFilePathNames.size() == 0)
	{
		return 2;
	}
	//if (imageFilePathNames.size() == 0)
	//{
	//	getFiles(filePath, "jpg", imageFilePathNames, imageNames);  //读取文件夹内所有jpg图像文件
	//	if (imageFilePathNames.size() == 0)
	//	{
	//		getFiles(filePath, "bmp", imageFilePathNames, imageNames);  //读取文件夹内所有bmp图像文件
	//		if (imageFilePathNames.size() == 0)
	//		{
	//			getFiles(filePath, "png", imageFilePathNames, imageNames);  //读取文件夹内所有bmp图像文件
	//			if (imageFilePathNames.size() == 0)
	//			{
	//				return 2;
	//			}
	//		}
	//	}
	//}
	flag_setPara = false;//参数配置

	imageNum = imageFilePathNames.size(); //记录当前的总文件数量（用于后面显示总数量使用）
	return 0;
}

int ReadImgAlgorithm::readImgFile(HWND m_hWnd)
{
	filePath = "";

	if (!selectRootFilePath(m_hWnd)) //选择文件夹
	{
		return 1;
	}

	imageFilePathNames.clear();
	imageNames.clear();

	getFiles(filePath, "JPG", imageFilePathNames, imageNames);   //读取文件夹内所有JPG图像文件
	if (imageFilePathNames.size() == 0)
	{
		getFiles(filePath, "jpg", imageFilePathNames, imageNames);  //读取文件夹内所有jpg图像文件
		if (imageFilePathNames.size() == 0)
		{
			getFiles(filePath, "bmp", imageFilePathNames, imageNames);  //读取文件夹内所有bmp图像文件
			if (imageFilePathNames.size() == 0)
			{
				return 2;
			}
		}
	}

	imageNum = imageFilePathNames.size(); //记录当前的总文件数量（用于后面显示总数量使用）
	return 0;
}

int ReadImgAlgorithm::reFreshImgFile()
{
	imageFilePathNames.clear();
	imageNames.clear();

	getFiles(filePath, "jpg", imageFilePathNames, imageNames);  //读取文件夹内所有jpg图像文件
	getFiles(filePath, "bmp", imageFilePathNames, imageNames);  //读取文件夹内所有bmp图像文件
	getFiles(filePath, "png", imageFilePathNames, imageNames);  //读取文件夹内所有bmp图像文件
	if (imageFilePathNames.size() == 0)
	{
		return 2;
	}

	imageNum = imageFilePathNames.size(); //记录当前的总文件数量（用于后面显示总数量使用）
	return 0;
}
/*
	#回调函数保存上次选择文件夹路径#
*/
int CALLBACK BrowseCallBackFun(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	switch (uMsg)
	{
	case BFFM_INITIALIZED:  //选择文件夹对话框初始化  
		//设置默认路径为lpData即'D:\'  
		::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
		//在STATUSTEXT区域显示当前路径  
		::SendMessage(hwnd, BFFM_SETSTATUSTEXT, 0, lpData);
		//设置选择文件夹对话框的标题  
		::SetWindowText(hwnd, TEXT("请先设置个工作目录"));
		break;
	case BFFM_SELCHANGED:   //选择文件夹变更时  
	{
								TCHAR pszPath[MAX_PATH];
								//获取当前选择路径  
								SHGetPathFromIDList((LPCITEMIDLIST)lParam, pszPath);
								//在STATUSTEXT区域显示当前路径  
								::SendMessage(hwnd, BFFM_SETSTATUSTEXT, TRUE, (LPARAM)pszPath);
	}
		break;
	}
	return 0;
}

bool ReadImgAlgorithm::selectRootFilePath(HWND m_hWnd)
{
    //存放选择的目录路径 
	CString str;
	//ZeroMemory(szPath, sizeof(szPath));

	BROWSEINFO bi;
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szPath;
	bi.lpszTitle = "请选择需要打开的目录：";
	bi.ulFlags = BIF_NEWDIALOGSTYLE | BIF_EDITBOX;
	bi.lpfn = BrowseCallBackFun;
	bi.lParam = long(&szPath);
	bi.iImage = 0;
	//弹出选择目录对话框
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
	// 得到文件路径对应的ItemIDList
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
	//文件句柄
	long   hFile = 0;
	//文件信息
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
			//如果是文件夹中仍有文件夹,迭代之
			//如果不是,加入列表
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