#include "stdafx.h"
#include "excel_.h"


excel_::excel_()
{
}


excel_::~excel_()
{
}

void excel_::AddHyperlinksToTheCells(string filename, DetectAlgorithm detectAlgorithm)
{
	////��ȡ��ǰ����·��
	//char buffer[_MAX_PATH];
	//_getcwd(buffer, _MAX_PATH);


	//string v[6] = { "A1", "B1", "C1", "D1", "E1", "F1" };
	//string first_row[6] = { "���", "����X1", "����Y1", "����X2", "����Y2", "����" };
	//string head_v[6] = { "A", "B", "C", "D", "E", "F" };

	//Path of input excel file
	//StringPtr sampleData = new ACString("sampleData.xlsx");


	//Read input excel file
	//intrusive_ptr<IWorkbook> workbook = Factory::CreateIWorkbook();

	//Get the first worksheet
	//intrusive_ptr<IWorksheetCollection> wsc = workbook->GetIWorksheets();
	//intrusive_ptr<IWorksheet> worksheet = wsc->GetObjectByIndex(0);

	////����excel��һ�е�ֵ
	//for (int i = 0; i < 6; i++){
	//	StringPtr outData = new ACString(first_row[i].c_str());
	//	worksheet->GetICells()->GetObjectByIndex(new ACString(v[i].c_str()))->PutValue(outData);
	//}

	////��������
	//for (int i = 0; i < detectAlgorithm.instanceRect_vector.size(); i++)
	//{
	//	stringstream ss;
	//	ss << (i + 2);
	//	string s1 = ss.str();
	//	//���
	//	if (detectAlgorithm.instanceRect_vector[i].instanceClass == 1)//����
	//	{
	//		StringPtr outData = new ACString("����");
	//		worksheet->GetICells()->GetObjectByIndex(new ACString((head_v[0] + s1).c_str()))->PutValue(outData);
	//	}
	//	else if (detectAlgorithm.instanceRect_vector[i].instanceClass == 2)//оƬ
	//	{
	//		StringPtr outData = new ACString("оƬ");
	//		worksheet->GetICells()->GetObjectByIndex(new ACString((head_v[0] + s1).c_str()))->PutValue(outData);
	//	}
	//	//����
	//	worksheet->GetICells()->GetObjectByIndex(new ACString((head_v[1] + s1).c_str()))->PutValue(detectAlgorithm.instanceRect_vector[i].left_top.x);
	//	worksheet->GetICells()->GetObjectByIndex(new ACString((head_v[2] + s1).c_str()))->PutValue(detectAlgorithm.instanceRect_vector[i].left_top.y);
	//	worksheet->GetICells()->GetObjectByIndex(new ACString((head_v[3] + s1).c_str()))->PutValue(detectAlgorithm.instanceRect_vector[i].right_bottom.x);
	//	worksheet->GetICells()->GetObjectByIndex(new ACString((head_v[4] + s1).c_str()))->PutValue(detectAlgorithm.instanceRect_vector[i].right_bottom.y);
	//	//����
	//	if (detectAlgorithm.instanceRect_vector[i].instanceWord != "")//���������������
	//	{
	//		StringPtr outData = new ACString(detectAlgorithm.instanceRect_vector[i].instanceWord.c_str());
	//		worksheet->GetICells()->GetObjectByIndex(new ACString((head_v[5] + s1).c_str()))->PutValue(outData);
	//	}
	//	else//���� ����ͼƬ������ͼƬ������
	//	{
	//		string outImg_pathName = string(buffer) + "\\images\\" + head_v[5] + s1 + ".jpg";
	//		//�и� ������ͼƬ
	//		Mat outImg = detectAlgorithm.cutRect(detectAlgorithm.detect_src_img, detectAlgorithm.instanceRect_vector[i].left_top, detectAlgorithm.instanceRect_vector[i].right_bottom);
	//		cv::imwrite(outImg_pathName, outImg);
	//		//���볬����
	//		intrusive_ptr<IHyperlinkCollection> hypLnks = worksheet->GetIHyperlinks();
	//		int idx = hypLnks->Add(new ACString((head_v[5] + s1).c_str()), 1, 1, new ACString(outImg_pathName.c_str()));
	//		intrusive_ptr<IHyperlink> lnk = hypLnks->GetObjectByIndex(idx);
	//		lnk->SetTextToDisplay(new ACString("ͼƬ"));
	//		lnk->SetScreenTip(new ACString("ͼƬ������"));
	//	}
	//}
	//Save the workbook
	//workbook->Save(sampleData);

}