#include "Queue.h"

#include <iostream>
using std::cin;
using std::cout;
using std::endl;

//����״̬˵����
//front = -1 rear = -1 �ն���
//front = -1 rear != -1 ��Ԫ�ض���
//front != -1 rear != -1 ��Ԫ�س�����

//�ÿն���
bool Set_NULL(Queue &Q)
{
	Q.front = -1;
	Q.rear = -1;
	return true;
}

//�ж϶����Ƿ�Ϊ��
bool Is_NULL(Queue Q)
{
	if (Q.front == Q.rear)
	{
		return true;  //��ͷ���ڶ�β��Ϊ��
	}
	return false;
}

//���
bool En_Queue(Queue &Q, datatype a)
{
	if ((Q.rear - Q.front) > 10)
	{
		if (!De_Queue(Q))
		{
			return false;
		}
	}
	Q.rear += 1;
	a.img.copyTo(Q.data[Q.rear].img);
	Q.data[Q.rear].ROI = a.ROI;
	Q.data[Q.rear].enlargeNum = a.enlargeNum;
	Q.data[Q.rear].narrowNum = a.narrowNum;
	Q.data[Q.rear].left_top = a.left_top;
	Q.data[Q.rear].right_bottom = a.right_bottom;
	return true;
}

//����
bool De_Queue(Queue &Q)
{
	if (Is_NULL(Q))
	{
		return false;
	}
	Q.front += 1;
	return true;
}

//��ǰλ�ó���
bool De_Index_Queue(Queue &Q, int index)
{
	if (Is_NULL(Q))
	{
		return false;
	}
	Q.rear = Q.front + index;
	return true;
}

//ȡ����ͷԪ��
datatype front_element(Queue Q)
{
	if (Is_NULL(Q))
	{
		return ViewStruct();
	}
	return Q.data[Q.rear];
}

//ȡ����indexԪ��
datatype element_atIndex(Queue Q, int index)
{
	if (Is_NULL(Q) || Q.front + index > Q.rear)
	{
		return ViewStruct();
	}
	return Q.data[Q.front + index];
}

////��ʾ����Ԫ��
//bool show(Queue Q)
//{
//	if (Is_NULL(Q))
//	{
//		cout << "The queue is empty~";
//		return false;
//	}
//	for (int i = Q.front; i < Q.rear; ++i)
//	{
//		cout << Q.data[i + 1] << " ";
//	}
//	return true;
//}