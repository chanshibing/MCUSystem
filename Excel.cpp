#include "stdafx.h"
#include <tchar.h>
#include "Excel.h"
#include <sstream>
//#include "excel_.h"
COleVariant
covTrue((short)TRUE),
covFalse((short)FALSE),
covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
CApplication Excel::application;

string v[10] = { "A1", "B1", "C1", "D1", "E1", "F1", "G1", "H1", "I1", "J1" };
string first_row[10] = { "类别", "坐标X1", "坐标Y1", "坐标X2", "坐标Y2", "类型", "宽(mm)", "高(mm)", "引脚1", "引脚2" };
string head_v[10] = { "A", "B", "C", "D", "E", "F", "G", "H", "I", "J" };

Excel::Excel() :isLoad(false)
{
}


Excel::~Excel()
{
	//close();
}


bool Excel::initExcel()
{
	//创建Excel 2000服务器(启动Excel)   
	if (!application.CreateDispatch(_T("Excel.application"), nullptr))
	{
		MessageBox(nullptr, _T("创建Excel服务失败,你可能没有安装EXCEL，请检查!"), _T("错误"), MB_OK);
		return FALSE;
	}

	application.put_DisplayAlerts(FALSE);
	return true;
}


void Excel::release()
{
	application.Quit();
	application.ReleaseDispatch();
	application = nullptr;
}

bool Excel::open(const char*  fileName)
{

	//先关闭文件
	close();

	//利用模板建立新文档
	books.AttachDispatch(application.get_Workbooks(), true);

	
	LPDISPATCH lpDis = nullptr;
	lpDis = books.Add(COleVariant(CString(fileName)));

	if (lpDis)
	{
		workBook.AttachDispatch(lpDis);

		sheets.AttachDispatch(workBook.get_Worksheets());

		openFileName = fileName;
		return true;
	}

	return false;
}

void Excel::close(bool ifSave)
{
	//如果文件已经打开，关闭文件
	if (!openFileName.IsEmpty())
	{
		//如果保存,交给用户控制,让用户自己存，如果自己SAVE，会出现莫名的等待  
		if (ifSave)
		{
			//show(true);
		}
		else
		{
			workBook.Close(COleVariant(short(FALSE)), COleVariant(openFileName), covOptional);
			books.Close();
		}

		//清空打开文件名称
		openFileName.Empty();
	}


	sheets.ReleaseDispatch();
	workSheet.ReleaseDispatch();
	currentRange.ReleaseDispatch();
	workBook.ReleaseDispatch();
	books.ReleaseDispatch();
}

CString Excel::getSheetName(long tableID)
{
	CWorksheet sheet;
	sheet.AttachDispatch(sheets.get_Item(COleVariant((long)tableID)));
	CString name = sheet.get_Name();
	sheet.ReleaseDispatch();
	return name;
}






bool Excel::loadSheet(CString sheet, bool preLoaded)
{
	LPDISPATCH lpDis = nullptr;
	currentRange.ReleaseDispatch();
	currentRange.ReleaseDispatch();

	lpDis = sheets.get_Item(COleVariant(sheet));
	if (lpDis)
	{
		workSheet.AttachDispatch(lpDis, true);
		currentRange.AttachDispatch(workSheet.get_Cells(), true);
	}
	else
	{
		return false;
	}

	isLoad = false;

	return true;
}


int Excel::getColumnCount()
{
	CRange range;
	CRange usedRange;

	usedRange.AttachDispatch(workSheet.get_UsedRange(), true);
	range.AttachDispatch(usedRange.get_Columns(), true);
	int count = range.get_Count();

	usedRange.ReleaseDispatch();
	range.ReleaseDispatch();

	return count;
}

int Excel::getRowCount()
{
	CRange range;
	CRange usedRange;

	usedRange.AttachDispatch(workSheet.get_UsedRange(), true);
	range.AttachDispatch(usedRange.get_Rows(), true);

	int count = range.get_Count();

	usedRange.ReleaseDispatch();
	range.ReleaseDispatch();

	return count;
}


CString Excel::getCellString(long iRow, long iColumn)
{
	COleVariant vResult;
	CString str;
	//字符串  
	if (isLoad == false)
	{
		CRange range;
		range.AttachDispatch(currentRange.get_Item(COleVariant((long)iRow), COleVariant((long)iColumn)).pdispVal, true);
		vResult = range.get_Value2();
		range.ReleaseDispatch();
	}
	//如果数据依据预先加载了  
	else
	{
		long read_address[2];
		VARIANT val;
		read_address[0] = iRow;
		read_address[1] = iColumn;
		safeArray.GetElement(read_address, &val);
		vResult = val;
	}

	if (vResult.vt == VT_BSTR)
	{
		str = vResult.bstrVal;
	}
	//整数  
	else if (vResult.vt == VT_INT)
	{
		str.Format(_T("%d"), vResult.pintVal);
	}
	//8字节的数字   
	else if (vResult.vt == VT_R8)
	{
		str.Format(_T("%0.0f"), vResult.dblVal);
	}
	//时间格式  
	else if (vResult.vt == VT_DATE)
	{
		SYSTEMTIME st;
		VariantTimeToSystemTime(vResult.date, &st);
		CTime tm(st);
		str = tm.Format(_T("%Y-%m-%d"));

	}
	//单元格空的  
	else if (vResult.vt == VT_EMPTY)
	{
		str = "";
	}

	return str;
}

CString Excel::num_to_date(CString record)//将excel中得到的数字转化成日期
{
	int record_num = _ttoi(record);
	int year = 1900, month = 1, day = 1, flag, number = 2;//开始日期1900/01/01
	while (number != record_num)
	{
		number++;//计数

		if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))//判断闰年
			flag = 1;
		else
			flag = 0;
		day++;
		if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
		{
			if (day>31)
			{
				month = month + 1;
				{
					if (month>12)
					{
						year = year + 1;
						month = month - 12;
					}
				}
				day = day - 31;
			}
		}
		else if (month == 4 || month == 6 || month == 9 || month == 11)
		{
			if (day>30)
			{
				month = month + 1;
				{
					if (month>12)
					{
						year = year + 1;
						month = month - 12;
					}
				}
				day = day - 30;
			}
		}
		else
		{
			if (flag == 1)
			{
				if (day>29)
				{
					month = month + 1;
					day = day - 29;
				}
			}
			else
			{
				if (day>28)
				{
					month = month + 1;
					day = day - 28;
				}
			}
		}

	}
	if (month < 10)
	{
		record.Format("%d%s%d", year, "/0", month);
	}
	else
	{
		record.Format("%d%s%d", year, "/", month);
	}
	if (day < 10)
	{
		record.Format("%s%s%d", record, "/0", day);
	}
	else
	{
		record.Format("%s%s%d", record, "/", day);
	}
	return record;
}

/*
	#写到excel#
	参数：
		1、文件名
		2、检测对象
		3、模块宽度
		4、模块高度
*/
void Excel::write_to_excel(string filename, vector<CString> className, DetectAlgorithm detectAlgorithm, float boardWidth, float boardHeight)
{
	//获取当前绝对路径
	char buffer[_MAX_PATH];
	_getcwd(buffer, _MAX_PATH);


	CoInitialize(NULL);//初始化COM，最后还有CoUninitialize
	COleVariant vMissing((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

	CApplication app;
	CWorkbooks workbooks;
	CWorkbook workbook;
	CWorksheet worksheet;
	CRange range;
	CHyperlinks hls;
	CFont0 ft;

	if (!app.CreateDispatch(_T("EXCEL.application"))) //启动EXCEL
	{
		AfxMessageBox(_T("居然你连OFFICE都没有安装吗?"));
		return;
	}
	app.put_Visible(FALSE);//是否显示excel
	app.put_DisplayFullScreen(FALSE);//设置全屏显示
	app.put_DisplayAlerts(FALSE);//屏蔽警告

	
	//打开工作簿
	CString strPath;//excel文件名
	strPath.Format("%s", filename.c_str());
	workbooks = app.get_Workbooks();
	try
	{
		workbook = workbooks.Add(_variant_t(strPath));//打开excel文件
	}
	catch (CException* e)
	{
		workbook = workbooks.Add(vtMissing);//找不到就新建一个
	}

	
	worksheet = workbook.get_ActiveSheet();//获取当前工作表

	// 设置单元格的列宽为12 
	//range.AttachDispatch(worksheet.get_Range(_variant_t("A1"), _variant_t("A1")), true);
	//range.put_ColumnWidth(_variant_t((long)12));

	// 所有单元格居中显示 
	range.AttachDispatch(worksheet.get_Cells(), true);
	range.put_HorizontalAlignment(_variant_t((long)-4108)); // -4108:居中，-4131:靠左，-4152:靠右 
	range.put_VerticalAlignment(_variant_t((long)-4108)); // -4108:居中，-4160:靠上，-4107:靠下 

	//设置excel第一行的值
	for (int i = 0; i < 10; i++){
		range = worksheet.get_Range(_variant_t(v[i].c_str()), _variant_t(v[i].c_str()));
		range.put_Value2(_variant_t(first_row[i].c_str()));//参数是二维数组可以批量设置单元格值
	}


	//插入数据
	for (int i = 0; i < detectAlgorithm.instanceRect_vector.size(); i++)
	{
		stringstream ss;
		ss << (i + 2);//第二行开始
		string s1 = ss.str();
		ss.str("");
		//坐标
		range = worksheet.get_Range(_variant_t((head_v[1] + s1).c_str()), _variant_t((head_v[1] + s1).c_str()));
		range.put_Value2(_variant_t(detectAlgorithm.instanceRect_vector[i].left_top.x));//参数是二维数组可以批量设置单元格值
		range = worksheet.get_Range(_variant_t((head_v[2] + s1).c_str()), _variant_t((head_v[2] + s1).c_str()));
		range.put_Value2(_variant_t(detectAlgorithm.instanceRect_vector[i].left_top.y));//参数是二维数组可以批量设置单元格值
		range = worksheet.get_Range(_variant_t((head_v[3] + s1).c_str()), _variant_t((head_v[3] + s1).c_str()));
		range.put_Value2(_variant_t(detectAlgorithm.instanceRect_vector[i].right_bottom.x));//参数是二维数组可以批量设置单元格值
		range = worksheet.get_Range(_variant_t((head_v[4] + s1).c_str()), _variant_t((head_v[4] + s1).c_str()));
		range.put_Value2(_variant_t(detectAlgorithm.instanceRect_vector[i].right_bottom.y));//参数是二维数组可以批量设置单元格值
		//类型
		if (detectAlgorithm.instanceRect_vector[i].instanceWord != "")//如果器件上有数字
		{
			range = worksheet.get_Range(_variant_t((head_v[5] + s1).c_str()), _variant_t((head_v[5] + s1).c_str()));
			range.put_Value2(_variant_t(detectAlgorithm.instanceRect_vector[i].instanceWord.c_str()));//参数是二维数组可以批量设置单元格值
		}
		else//否则 保存图片并插入图片超链接
		{
			string outImg_pathName = string(buffer) + "\\images\\" + head_v[5] + s1 + ".jpg";
			//切割 并保存图片
			Mat outImg = detectAlgorithm.cutRect(detectAlgorithm.detect_src_img, detectAlgorithm.instanceRect_vector[i].left_top, detectAlgorithm.instanceRect_vector[i].right_bottom);
			cv::imwrite(outImg_pathName, outImg);
			//插入超链接
			hls = (worksheet.get_Hyperlinks());//超链接
			COleVariant vCell((head_v[5] + s1).c_str());
			COleVariant vText("图片");
			COleVariant vTip(outImg_pathName.c_str());
			hls.Add(worksheet.get_Range(vCell, vMissing), outImg_pathName.c_str(), vMissing, vTip, vText);//添加超链接
		}
		//尺寸
		float width = detectAlgorithm.instanceRect_vector[i].right_bottom.x - detectAlgorithm.instanceRect_vector[i].left_top.x;//元器件
		float height = detectAlgorithm.instanceRect_vector[i].right_bottom.y - detectAlgorithm.instanceRect_vector[i].left_top.y;
		float img_width = detectAlgorithm.detect_src_img.cols;//原图
		float img_height = detectAlgorithm.detect_src_img.rows;
		if (boardWidth == 0)
		{
			range = worksheet.get_Range(_variant_t((head_v[6] + s1).c_str()), _variant_t((head_v[6] + s1).c_str()));
			range.put_Value2(_variant_t(0));//参数是二维数组可以批量设置单元格值
		}
		else
		{
			range = worksheet.get_Range(_variant_t((head_v[6] + s1).c_str()), _variant_t((head_v[6] + s1).c_str()));
			width = width / img_width * boardWidth;
			ss << setiosflags(ios::fixed) << setprecision(1) << width;
			range.put_Value2(_variant_t(ss.str().c_str()));//参数是二维数组可以批量设置单元格值
			ss.str("");
		}
		if (boardHeight== 0)
		{
			range = worksheet.get_Range(_variant_t((head_v[7] + s1).c_str()), _variant_t((head_v[7] + s1).c_str()));
			range.put_Value2(_variant_t(0));//参数是二维数组可以批量设置单元格值
		}
		else
		{
			range = worksheet.get_Range(_variant_t((head_v[7] + s1).c_str()), _variant_t((head_v[7] + s1).c_str()));
			height = height / img_height * boardHeight; 
			ss << setiosflags(ios::fixed) << setprecision(1) << height;
			range.put_Value2(_variant_t(ss.str().c_str()));//参数是二维数组可以批量设置单元格值
			ss.str("");
		}
		//引脚
		range = worksheet.get_Range(_variant_t((head_v[8] + s1).c_str()), _variant_t((head_v[8] + s1).c_str()));
		range.put_Value2(_variant_t(detectAlgorithm.instanceRect_vector[i].numOfPin1));//参数是二维数组可以批量设置单元格值
		range = worksheet.get_Range(_variant_t((head_v[9] + s1).c_str()), _variant_t((head_v[9] + s1).c_str()));
		range.put_Value2(_variant_t(detectAlgorithm.instanceRect_vector[i].numOfPin2));//参数是二维数组可以批量设置单元格值

		 
		//类别
		if (className[detectAlgorithm.instanceRect_vector[i].instanceClass - 1] == "R")//电阻
		{
			range = worksheet.get_Range(_variant_t((head_v[0] + s1).c_str()), _variant_t((head_v[0] + s1).c_str()));
			range.put_Value2(_variant_t("电阻"));//参数是二维数组可以批量设置单元格值
			//设置此行字体颜色
			range.AttachDispatch(worksheet.get_Range(_variant_t((head_v[0] + s1).c_str()), _variant_t((head_v[9] + s1).c_str())), true);
			ft.AttachDispatch(range.get_Font());
			ft.put_Color(_variant_t((long)RGB(255, 0, 0)));//红色
		}
		else if (className[detectAlgorithm.instanceRect_vector[i].instanceClass - 1] == "C")//电容
		{
			range = worksheet.get_Range(_variant_t((head_v[0] + s1).c_str()), _variant_t((head_v[0] + s1).c_str()));
			range.put_Value2(_variant_t("电容"));//参数是二维数组可以批量设置单元格值
			//设置此行字体颜色
			range.AttachDispatch(worksheet.get_Range(_variant_t((head_v[0] + s1).c_str()), _variant_t((head_v[9] + s1).c_str())), true);
			ft.AttachDispatch(range.get_Font());
			ft.put_Color(_variant_t((long)RGB(0, 0, 255)));//蓝色
		}
		else if (className[detectAlgorithm.instanceRect_vector[i].instanceClass - 1] == "Chip")//芯片
		{
			range = worksheet.get_Range(_variant_t((head_v[0] + s1).c_str()), _variant_t((head_v[0] + s1).c_str()));
			range.put_Value2(_variant_t("芯片"));//参数是二维数组可以批量设置单元格值
			//设置此行字体颜色
			range.AttachDispatch(worksheet.get_Range(_variant_t((head_v[0] + s1).c_str()), _variant_t((head_v[9] + s1).c_str())), true);
			ft.AttachDispatch(range.get_Font());
			ft.put_Color(_variant_t((long)RGB(0, 255, 0)));//绿色
		}

	}

	//保存文件
	workbook.SaveAs(COleVariant(strPath), covOptional,
		covOptional, covOptional,
		covOptional, covOptional, (long)0, covOptional, covOptional, covOptional,
		covOptional, covOptional);

	
	//释放资源
	ft.ReleaseDispatch();
	hls.ReleaseDispatch();
	worksheet.ReleaseDispatch();
	workbook.ReleaseDispatch();
	workbooks.ReleaseDispatch();
	range.ReleaseDispatch();
	app.Quit();
	app.ReleaseDispatch();
	CoUninitialize();//对应CoInitialize
}

/*
	#读取excel文件#
	参数：
		1、文件名
*/
void Excel::get_from_excel(string filename, vector<ChipCoordinate> &chip)
{
	CoInitialize(NULL);//初始化COM，最后还有CoUninitialize
	COleVariant vMissing((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

	CApplication app;
	CWorkbooks workbooks;
	CWorkbook workbook;
	CWorksheet worksheet;
	CRange range;

	if (!app.CreateDispatch(_T("EXCEL.application"))) //启动EXCEL
	{
		AfxMessageBox(_T("居然你连OFFICE都没有安装吗?"));
		return;
	}
	app.put_Visible(FALSE);//是否显示excel
	app.put_DisplayFullScreen(FALSE);//设置全屏显示
	app.put_DisplayAlerts(FALSE);//屏蔽警告


	//打开工作簿
	CString strPath;//excel文件名
	strPath.Format("%s", filename.c_str());
	workbooks = app.get_Workbooks();
	try
	{
		workbook = workbooks.Add(_variant_t(strPath));//打开excel文件
	}
	catch (CException* e)
	{
		AfxMessageBox(_T("没有此文件，请重新选择文件!"));
		//释放资源
		worksheet.ReleaseDispatch();
		workbook.ReleaseDispatch();
		workbooks.ReleaseDispatch();
		range.ReleaseDispatch();
		app.Quit();
		app.ReleaseDispatch();
		CoUninitialize();//对应CoInitialize
		return;
	}

	worksheet = workbook.get_ActiveSheet();//获取当前工作表

	for (int i = 1;; i++)
	{
		stringstream ss;
		ss << (i + 1);
		string s = ss.str();
		ss.str("");
		range = worksheet.get_Range(_variant_t((head_v[0] + s).c_str()), _variant_t((head_v[0] + s).c_str()));
		string instanceClass = (char*)(_bstr_t)range.get_Value2();
		if (instanceClass == "芯片")
		{
			//获取芯片坐标
			ChipCoordinate chip_dm;
			range = worksheet.get_Range(_variant_t((head_v[1] + s).c_str()), _variant_t((head_v[1] + s).c_str()));
			chip_dm.leftTop.x = stoi((char*)(_bstr_t)range.get_Value2());
			range = worksheet.get_Range(_variant_t((head_v[2] + s).c_str()), _variant_t((head_v[2] + s).c_str()));
			chip_dm.leftTop.y = stoi((char*)(_bstr_t)range.get_Value2());
			range = worksheet.get_Range(_variant_t((head_v[3] + s).c_str()), _variant_t((head_v[3] + s).c_str()));
			chip_dm.rightBottom.x = stoi((char*)(_bstr_t)range.get_Value2());
			range = worksheet.get_Range(_variant_t((head_v[4] + s).c_str()), _variant_t((head_v[4] + s).c_str()));
			chip_dm.rightBottom.y = stoi((char*)(_bstr_t)range.get_Value2());
			chip.push_back(chip_dm);
		}
		else if (instanceClass == "")
		{
			break;
		}
	}

	//保存文件
	workbook.SaveAs(COleVariant(strPath), covOptional,
		covOptional, covOptional,
		covOptional, covOptional, (long)0, covOptional, covOptional, covOptional,
		covOptional, covOptional);

	//释放资源
	worksheet.ReleaseDispatch();
	workbook.ReleaseDispatch();
	workbooks.ReleaseDispatch();
	range.ReleaseDispatch();
	app.Quit();
	app.ReleaseDispatch();
	CoUninitialize();//对应CoInitialize
}

/*
字符串分割
*/
void Excel::split(const string& s, vector<int>& sv, const char flag = ' ') {
	sv.clear();
	istringstream iss(s);
	string temp;

	while (getline(iss, temp, flag)) {
		sv.push_back(stoi(temp));
	}
	return;
}