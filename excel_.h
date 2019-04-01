#pragma once
//#include "Aspose.Cells.h"
#include "Detect_algorithm.h"
#include <iostream> 
#include <iomanip>
#include <direct.h>

using namespace std;
//typedef Aspose::Cells::System::String ACString;

class excel_
{
public:
	excel_();
	~excel_();
	
	/*
		#向excel中写入数据#
		参数：
			1、生成excel路径名
			2、检测对象
	*/

	void AddHyperlinksToTheCells(string filename, DetectAlgorithm detectAlgorithm);
	
};


