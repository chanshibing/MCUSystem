#include "View_struct.h"

#define MAXSIZE 1000
typedef ViewStruct datatype;

typedef struct
{
	datatype data[MAXSIZE];
	int front, rear;    //表示队列的头尾位置
}Queue;

//置空队列
bool Set_NULL(Queue &Q);

//判断队列是否为空
bool Is_NULL(Queue Q);

//入队
bool En_Queue(Queue &Q, datatype a);

//出队
bool De_Queue(Queue &Q);

//当前位置出队
bool De_Index_Queue(Queue &Q, int index);

//取队列头元素
datatype front_element(Queue Q);

//取队列index位置元素
datatype element_atIndex(Queue Q, int index);

//显示队列元素
//bool show(Queue Q);