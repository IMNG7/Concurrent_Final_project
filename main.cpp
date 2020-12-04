#include <iostream>
#include <stdlib.h>
#include "treiber.h"
#include "M_S_Queue.h"
using namespace std;
int main()
{
	tstack stack;
	msqueue mqueue;
	stack.push(3);
	stack.push(2);
	stack.push(4);
	cout<<stack.pop();
	cout<<stack.pop();
	cout<<stack.pop();
	cout<<"\n\r";
	mqueue.enqueue(1);
	mqueue.enqueue(2);
	mqueue.enqueue(3);
	cout<<mqueue.dequeue();
	cout<<mqueue.dequeue();
	cout<<mqueue.dequeue();
	return 0;
}