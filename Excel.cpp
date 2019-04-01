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
string first_row[10] = { "���", "����X1", "����Y1", "����X2", "����Y2", "����", "��(mm)", "��(mm)", "����1", "����2" };
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
	//����Excel 2000������(����Excel)   
	if (!application.CreateDispatch(_T("Excel.application"), nullptr))
	{
		MessageBox(nullptr, _T("����Excel����ʧ��,�����û�а�װEXCEL������!"), _T("����"), MB_OK);
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

	//�ȹر��ļ�
	close();

	//����ģ�彨�����ĵ�
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
	//����ļ��Ѿ��򿪣��ر��ļ�
	if (!openFileName.IsEmpty())
	{
		//�������,�����û�����,���û��Լ��棬����Լ�SAVE�������Ī���ĵȴ�  
		if (ifSave)
		{
			//show(true);
		}
		else
		{
			workBook.Close(COleVariant(short(FALSE)), COleVariant(openFileName), covOptional);
			books.Close();
		}

		//��մ��ļ�����
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
	//�ַ���  
	if (isLoad == false)
	{
		CRange range;
		range.AttachDispatch(currentRange.get_Item(COleVariant((long)iRow), COleVariant((long)iColumn)).pdispVal, true);
		vResult = range.get_Value2();
		range.ReleaseDispatch();
	}
	//�����������Ԥ�ȼ�����  
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
	//����  
	else if (vResult.vt == VT_INT)
	{
		str.Format(_T("%d"), vResult.pintVal);
	}
	//8�ֽڵ�����   
	else if (vResult.vt == VT_R8)
	{
		str.Format(_T("%0.0f"), vResult.dblVal);
	}
	//ʱ���ʽ  
	else if (vResult.vt == VT_DATE)
	{
		SYSTEMTIME st;
		VariantTimeToSystemTime(vResult.date, &st);
		CTime tm(st);
		str = tm.Format(_T("%Y-%m-%d"));

	}
	//��Ԫ��յ�  
	else if (vResult.vt == VT_EMPTY)
	{
		str = "";
	}

	return str;
}

CString Excel::num_to_date(CString record)//��excel�еõ�������ת��������
{
	int record_num = _ttoi(record);
	int year = 1900, month = 1, day = 1, flag, number = 2;//��ʼ����1900/01/01
	while (number != record_num)
	{
		number++;//����

		if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))//�ж�����
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
	#д��excel#
	������
		1���ļ���
		2��������
		3��ģ����
		4��ģ��߶�
*/
void Excel::write_to_excel(string filename, vector<CString> className, DetectAlgorithm detectAlgorithm, float boardWidth, float boardHeight)
{
	//��ȡ��ǰ����·��
	char buffer[_MAX_PATH];
	_getcwd(buffer, _MAX_PATH);


	CoInitialize(NULL);//��ʼ��COM�������CoUninitialize
	COleVariant vMissing((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

	CApplication app;
	CWorkbooks workbooks;
	CWorkbook workbook;
	CWorksheet worksheet;
	CRange range;
	CHyperlinks hls;
	CFont0 ft;

	if (!app.CreateDispatch(_T("EXCEL.application"))) //����EXCEL
	{
		AfxMessageBox(_T("��Ȼ����OFFICE��û�а�װ��?"));
		return;
	}
	app.put_Visible(FALSE);//�Ƿ���ʾexcel
	app.put_DisplayFullScreen(FALSE);//����ȫ����ʾ
	app.put_DisplayAlerts(FALSE);//���ξ���

	
	//�򿪹�����
	CString strPath;//excel�ļ���
	strPath.Format("%s", filename.c_str());
	workbooks = app.get_Workbooks();
	try
	{
		workbook = workbooks.Add(_variant_t(strPath));//��excel�ļ�
	}
	catch (CException* e)
	{
		workbook = workbooks.Add(vtMissing);//�Ҳ������½�һ��
	}

	
	worksheet = workbook.get_ActiveSheet();//��ȡ��ǰ������

	// ���õ�Ԫ����п�Ϊ12 
	//range.AttachDispatch(worksheet.get_Range(_variant_t("A1"), _variant_t("A1")), true);
	//range.put_ColumnWidth(_variant_t((long)12));

	// ���е�Ԫ�������ʾ 
	range.AttachDispatch(worksheet.get_Cells(), true);
	range.put_HorizontalAlignment(_variant_t((long)-4108)); // -4108:���У�-4131:����-4152:���� 
	range.put_VerticalAlignment(_variant_t((long)-4108)); // -4108:���У�-4160:���ϣ�-4107:���� 

	//����excel��һ�е�ֵ
	for (int i = 0; i < 10; i++){
		range = worksheet.get_Range(_variant_t(v[i].c_str()), _variant_t(v[i].c_str()));
		range.put_Value2(_variant_t(first_row[i].c_str()));//�����Ƕ�ά��������������õ�Ԫ��ֵ
	}


	//��������
	for (int i = 0; i < detectAlgorithm.instanceRect_vector.size(); i++)
	{
		stringstream ss;
		ss << (i + 2);//�ڶ��п�ʼ
		string s1 = ss.str();
		ss.str("");
		//����
		range = worksheet.get_Range(_variant_t((head_v[1] + s1).c_str()), _variant_t((head_v[1] + s1).c_str()));
		range.put_Value2(_variant_t(detectAlgorithm.instanceRect_vector[i].left_top.x));//�����Ƕ�ά��������������õ�Ԫ��ֵ
		range = worksheet.get_Range(_variant_t((head_v[2] + s1).c_str()), _variant_t((head_v[2] + s1).c_str()));
		range.put_Value2(_variant_t(detectAlgorithm.instanceRect_vector[i].left_top.y));//�����Ƕ�ά��������������õ�Ԫ��ֵ
		range = worksheet.get_Range(_variant_t((head_v[3] + s1).c_str()), _variant_t((head_v[3] + s1).c_str()));
		range.put_Value2(_variant_t(detectAlgorithm.instanceRect_vector[i].right_bottom.x));//�����Ƕ�ά��������������õ�Ԫ��ֵ
		range = worksheet.get_Range(_variant_t((head_v[4] + s1).c_str()), _variant_t((head_v[4] + s1).c_str()));
		range.put_Value2(_variant_t(detectAlgorithm.instanceRect_vector[i].right_bottom.y));//�����Ƕ�ά��������������õ�Ԫ��ֵ
		//����
		if (detectAlgorithm.instanceRect_vector[i].instanceWord != "")//���������������
		{
			range = worksheet.get_Range(_variant_t((head_v[5] + s1).c_str()), _variant_t((head_v[5] + s1).c_str()));
			range.put_Value2(_variant_t(detectAlgorithm.instanceRect_vector[i].instanceWord.c_str()));//�����Ƕ�ά��������������õ�Ԫ��ֵ
		}
		else//���� ����ͼƬ������ͼƬ������
		{
			string outImg_pathName = string(buffer) + "\\images\\" + head_v[5] + s1 + ".jpg";
			//�и� ������ͼƬ
			Mat outImg = detectAlgorithm.cutRect(detectAlgorithm.detect_src_img, detectAlgorithm.instanceRect_vector[i].left_top, detectAlgorithm.instanceRect_vector[i].right_bottom);
			cv::imwrite(outImg_pathName, outImg);
			//���볬����
			hls = (worksheet.get_Hyperlinks());//������
			COleVariant vCell((head_v[5] + s1).c_str());
			COleVariant vText("ͼƬ");
			COleVariant vTip(outImg_pathName.c_str());
			hls.Add(worksheet.get_Range(vCell, vMissing), outImg_pathName.c_str(), vMissing, vTip, vText);//��ӳ�����
		}
		//�ߴ�
		float width = detectAlgorithm.instanceRect_vector[i].right_bottom.x - detectAlgorithm.instanceRect_vector[i].left_top.x;//Ԫ����
		float height = detectAlgorithm.instanceRect_vector[i].right_bottom.y - detectAlgorithm.instanceRect_vector[i].left_top.y;
		float img_width = detectAlgorithm.detect_src_img.cols;//ԭͼ
		float img_height = detectAlgorithm.detect_src_img.rows;
		if (boardWidth == 0)
		{
			range = worksheet.get_Range(_variant_t((head_v[6] + s1).c_str()), _variant_t((head_v[6] + s1).c_str()));
			range.put_Value2(_variant_t(0));//�����Ƕ�ά��������������õ�Ԫ��ֵ
		}
		else
		{
			range = worksheet.get_Range(_variant_t((head_v[6] + s1).c_str()), _variant_t((head_v[6] + s1).c_str()));
			width = width / img_width * boardWidth;
			ss << setiosflags(ios::fixed) << setprecision(1) << width;
			range.put_Value2(_variant_t(ss.str().c_str()));//�����Ƕ�ά��������������õ�Ԫ��ֵ
			ss.str("");
		}
		if (boardHeight== 0)
		{
			range = worksheet.get_Range(_variant_t((head_v[7] + s1).c_str()), _variant_t((head_v[7] + s1).c_str()));
			range.put_Value2(_variant_t(0));//�����Ƕ�ά��������������õ�Ԫ��ֵ
		}
		else
		{
			range = worksheet.get_Range(_variant_t((head_v[7] + s1).c_str()), _variant_t((head_v[7] + s1).c_str()));
			height = height / img_height * boardHeight; 
			ss << setiosflags(ios::fixed) << setprecision(1) << height;
			range.put_Value2(_variant_t(ss.str().c_str()));//�����Ƕ�ά��������������õ�Ԫ��ֵ
			ss.str("");
		}
		//����
		range = worksheet.get_Range(_variant_t((head_v[8] + s1).c_str()), _variant_t((head_v[8] + s1).c_str()));
		range.put_Value2(_variant_t(detectAlgorithm.instanceRect_vector[i].numOfPin1));//�����Ƕ�ά��������������õ�Ԫ��ֵ
		range = worksheet.get_Range(_variant_t((head_v[9] + s1).c_str()), _variant_t((head_v[9] + s1).c_str()));
		range.put_Value2(_variant_t(detectAlgorithm.instanceRect_vector[i].numOfPin2));//�����Ƕ�ά��������������õ�Ԫ��ֵ

		 
		//���
		if (className[detectAlgorithm.instanceRect_vector[i].instanceClass - 1] == "R")//����
		{
			range = worksheet.get_Range(_variant_t((head_v[0] + s1).c_str()), _variant_t((head_v[0] + s1).c_str()));
			range.put_Value2(_variant_t("����"));//�����Ƕ�ά��������������õ�Ԫ��ֵ
			//���ô���������ɫ
			range.AttachDispatch(worksheet.get_Range(_variant_t((head_v[0] + s1).c_str()), _variant_t((head_v[9] + s1).c_str())), true);
			ft.AttachDispatch(range.get_Font());
			ft.put_Color(_variant_t((long)RGB(255, 0, 0)));//��ɫ
		}
		else if (className[detectAlgorithm.instanceRect_vector[i].instanceClass - 1] == "C")//����
		{
			range = worksheet.get_Range(_variant_t((head_v[0] + s1).c_str()), _variant_t((head_v[0] + s1).c_str()));
			range.put_Value2(_variant_t("����"));//�����Ƕ�ά��������������õ�Ԫ��ֵ
			//���ô���������ɫ
			range.AttachDispatch(worksheet.get_Range(_variant_t((head_v[0] + s1).c_str()), _variant_t((head_v[9] + s1).c_str())), true);
			ft.AttachDispatch(range.get_Font());
			ft.put_Color(_variant_t((long)RGB(0, 0, 255)));//��ɫ
		}
		else if (className[detectAlgorithm.instanceRect_vector[i].instanceClass - 1] == "Chip")//оƬ
		{
			range = worksheet.get_Range(_variant_t((head_v[0] + s1).c_str()), _variant_t((head_v[0] + s1).c_str()));
			range.put_Value2(_variant_t("оƬ"));//�����Ƕ�ά��������������õ�Ԫ��ֵ
			//���ô���������ɫ
			range.AttachDispatch(worksheet.get_Range(_variant_t((head_v[0] + s1).c_str()), _variant_t((head_v[9] + s1).c_str())), true);
			ft.AttachDispatch(range.get_Font());
			ft.put_Color(_variant_t((long)RGB(0, 255, 0)));//��ɫ
		}

	}

	//�����ļ�
	workbook.SaveAs(COleVariant(strPath), covOptional,
		covOptional, covOptional,
		covOptional, covOptional, (long)0, covOptional, covOptional, covOptional,
		covOptional, covOptional);

	
	//�ͷ���Դ
	ft.ReleaseDispatch();
	hls.ReleaseDispatch();
	worksheet.ReleaseDispatch();
	workbook.ReleaseDispatch();
	workbooks.ReleaseDispatch();
	range.ReleaseDispatch();
	app.Quit();
	app.ReleaseDispatch();
	CoUninitialize();//��ӦCoInitialize
}

/*
	#��ȡexcel�ļ�#
	������
		1���ļ���
*/
void Excel::get_from_excel(string filename, vector<ChipCoordinate> &chip)
{
	CoInitialize(NULL);//��ʼ��COM�������CoUninitialize
	COleVariant vMissing((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

	CApplication app;
	CWorkbooks workbooks;
	CWorkbook workbook;
	CWorksheet worksheet;
	CRange range;

	if (!app.CreateDispatch(_T("EXCEL.application"))) //����EXCEL
	{
		AfxMessageBox(_T("��Ȼ����OFFICE��û�а�װ��?"));
		return;
	}
	app.put_Visible(FALSE);//�Ƿ���ʾexcel
	app.put_DisplayFullScreen(FALSE);//����ȫ����ʾ
	app.put_DisplayAlerts(FALSE);//���ξ���


	//�򿪹�����
	CString strPath;//excel�ļ���
	strPath.Format("%s", filename.c_str());
	workbooks = app.get_Workbooks();
	try
	{
		workbook = workbooks.Add(_variant_t(strPath));//��excel�ļ�
	}
	catch (CException* e)
	{
		AfxMessageBox(_T("û�д��ļ���������ѡ���ļ�!"));
		//�ͷ���Դ
		worksheet.ReleaseDispatch();
		workbook.ReleaseDispatch();
		workbooks.ReleaseDispatch();
		range.ReleaseDispatch();
		app.Quit();
		app.ReleaseDispatch();
		CoUninitialize();//��ӦCoInitialize
		return;
	}

	worksheet = workbook.get_ActiveSheet();//��ȡ��ǰ������

	for (int i = 1;; i++)
	{
		stringstream ss;
		ss << (i + 1);
		string s = ss.str();
		ss.str("");
		range = worksheet.get_Range(_variant_t((head_v[0] + s).c_str()), _variant_t((head_v[0] + s).c_str()));
		string instanceClass = (char*)(_bstr_t)range.get_Value2();
		if (instanceClass == "оƬ")
		{
			//��ȡоƬ����
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

	//�����ļ�
	workbook.SaveAs(COleVariant(strPath), covOptional,
		covOptional, covOptional,
		covOptional, covOptional, (long)0, covOptional, covOptional, covOptional,
		covOptional, covOptional);

	//�ͷ���Դ
	worksheet.ReleaseDispatch();
	workbook.ReleaseDispatch();
	workbooks.ReleaseDispatch();
	range.ReleaseDispatch();
	app.Quit();
	app.ReleaseDispatch();
	CoUninitialize();//��ӦCoInitialize
}

/*
�ַ����ָ�
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