#include "msexcel/CBorders.h"
#include "msexcel/CApplication.h"
#include "msexcel/CFont0.h"
#include "msexcel/CRange.h"
#include "msexcel/CWorkbook.h"
#include "msexcel/CWorkbooks.h"
#include "msexcel/CWorksheet.h"
#include "msexcel/CWorksheets.h"
//#include "msexcel/Cnterior.h"
#include "msexcel/CHyperlinks.h"
#include "afxdisp.h"
#include <fstream>
#include <streambuf>
#include <iostream>
#include <string>
#include <vector>
#include "Detect_algorithm.h"

#include <iomanip>
#include <direct.h>
#include "CompareModel.h"

using namespace std;


class Excel
{
private:
	CString openFileName;
	CWorkbook workBook;//当前处理文件
	CWorkbooks books;//ExcelBook集合，多文件时使用
	CWorksheet workSheet;//当前使用sheet
	CWorksheets sheets;//Excel的sheet集合
	CRange currentRange;//当前操作区域

	bool isLoad;//是否已经加载了某个sheet数据

	COleSafeArray safeArray;

protected:
	static CApplication application;//Excel进程实例

public:
	Excel();
	virtual ~Excel();

	//得到一个cell的string
	CString getCellString(long iRow, long iColumn);
	//将excel中得到的数字转化成日期
	static CString num_to_date(CString record);

	//取得行的总数
	int getRowCount();

	//取得列的整数
	int getColumnCount();

	//使用某个shell
	bool loadSheet(CString sheet, bool preLoaded = false);

	//通过序号取得某个shell的名称
	CString getSheetName(long tableID);

	//打开excel文件
	bool open(const char* fileName);

	//关闭打开的excel文件
	void close(bool ifSave = false);
public:
	//向excel写入
	void write_to_excel(string filename, vector<CString> cllassName, DetectAlgorithm detectAlgorithm, float boardWidth, float boardHeight);

	//读取excel
	void get_from_excel(string filename, vector<ChipCoordinate> &chip);

	//字符串分割
	void split(const string& s, vector<int>& sv, const char flag);

public:
	//初始化 Excel_OLE
	static bool initExcel();

	//释放Excel_OLE
	static void release();

};

