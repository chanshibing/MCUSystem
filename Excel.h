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
	CWorkbook workBook;//��ǰ�����ļ�
	CWorkbooks books;//ExcelBook���ϣ����ļ�ʱʹ��
	CWorksheet workSheet;//��ǰʹ��sheet
	CWorksheets sheets;//Excel��sheet����
	CRange currentRange;//��ǰ��������

	bool isLoad;//�Ƿ��Ѿ�������ĳ��sheet����

	COleSafeArray safeArray;

protected:
	static CApplication application;//Excel����ʵ��

public:
	Excel();
	virtual ~Excel();

	//�õ�һ��cell��string
	CString getCellString(long iRow, long iColumn);
	//��excel�еõ�������ת��������
	static CString num_to_date(CString record);

	//ȡ���е�����
	int getRowCount();

	//ȡ���е�����
	int getColumnCount();

	//ʹ��ĳ��shell
	bool loadSheet(CString sheet, bool preLoaded = false);

	//ͨ�����ȡ��ĳ��shell������
	CString getSheetName(long tableID);

	//��excel�ļ�
	bool open(const char* fileName);

	//�رմ򿪵�excel�ļ�
	void close(bool ifSave = false);
public:
	//��excelд��
	void write_to_excel(string filename, vector<CString> cllassName, DetectAlgorithm detectAlgorithm, float boardWidth, float boardHeight);

	//��ȡexcel
	void get_from_excel(string filename, vector<ChipCoordinate> &chip);

	//�ַ����ָ�
	void split(const string& s, vector<int>& sv, const char flag);

public:
	//��ʼ�� Excel_OLE
	static bool initExcel();

	//�ͷ�Excel_OLE
	static void release();

};

