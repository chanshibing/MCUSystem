#include "Queue.h"

#include <iostream>
using std::cin;
using std::cout;
using std::endl;

//队列状态说明：
//front = -1 rear = -1 空队列
//front = -1 rear != -1 有元素队列
//front != -1 rear != -1 有元素出队列

//置空队列
bool Set_NULL(Queue &Q)
{
	Q.front = -1;
	Q.rear = -1;
	return true;
}

//判断队列是否为空
bool Is_NULL(Queue Q)
{
	if (Q.front == Q.rear)
	{
		return true;  //队头等于队尾，为空
	}
	return false;
}

//入队
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

//出队
bool De_Queue(Queue &Q)
{
	if (Is_NULL(Q))
	{
		return false;
	}
	Q.front += 1;
	return true;
}

//当前位置出队
bool De_Index_Queue(Queue &Q, int index)
{
	if (Is_NULL(Q))
	{
		return false;
	}
	Q.rear = Q.front + index;
	return true;
}

//取队列头元素
datatype front_element(Queue Q)
{
	if (Is_NULL(Q))
	{
		return ViewStruct();
	}
	return Q.data[Q.rear];
}

//取队列index元素
datatype element_atIndex(Queue Q, int index)
{
	if (Is_NULL(Q) || Q.front + index > Q.rear)
	{
		return ViewStruct();
	}
	return Q.data[Q.front + index];
}

////显示队列元素
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