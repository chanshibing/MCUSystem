#include "stdafx.h"
#include "CompareModel.h"
#include "Excel.h"
#include "Detect_algorithm.h"

CompareModel::CompareModel()
{
}


CompareModel::~CompareModel()
{
}

void CompareModel::startCompareModel(Mat & srcMat, string excelPath)
{
	//从excel获取芯片坐标
	chip.clear();
	Excel excel;
	excel.get_from_excel(excelPath, chip);
	//画出芯片位置
	DetectAlgorithm detect;
	for (int i = 0; i < chip.size(); i++)
	{
		drawRect(srcMat, chip[i]);
	}
	
}

void CompareModel::drawRect(Mat &srcMat, ChipCoordinate chip)
{
	int lineThickness = 4;
	if (srcMat.cols < 300 || srcMat.rows < 300){
		lineThickness = 1;
	}
	else if (srcMat.cols < 500 || srcMat.rows < 500){
		lineThickness = 2;
	}
	else{
		lineThickness = 4;
	}
	//防止越界
	int width = srcMat.cols;
	int height = srcMat.rows;
	if (chip.leftTop.x < width && chip.leftTop.y < height
		&& chip.rightBottom.x < width && chip.rightBottom.y < height)
	{
		rectangle(srcMat, chip.leftTop, chip.rightBottom, Scalar(255, 0, 0), lineThickness, 8);
	}
}