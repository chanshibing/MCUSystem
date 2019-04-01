#include "View_struct.h"

#define MAXSIZE 1000
typedef ViewStruct datatype;

typedef struct
{
	datatype data[MAXSIZE];
	int front, rear;    //��ʾ���е�ͷβλ��
}Queue;

//�ÿն���
bool Set_NULL(Queue &Q);

//�ж϶����Ƿ�Ϊ��
bool Is_NULL(Queue Q);

//���
bool En_Queue(Queue &Q, datatype a);

//����
bool De_Queue(Queue &Q);

//��ǰλ�ó���
bool De_Index_Queue(Queue &Q, int index);

//ȡ����ͷԪ��
datatype front_element(Queue Q);

//ȡ����indexλ��Ԫ��
datatype element_atIndex(Queue Q, int index);

//��ʾ����Ԫ��
//bool show(Queue Q);