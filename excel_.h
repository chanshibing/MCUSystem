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
		#��excel��д������#
		������
			1������excel·����
			2��������
	*/

	void AddHyperlinksToTheCells(string filename, DetectAlgorithm detectAlgorithm);
	
};


