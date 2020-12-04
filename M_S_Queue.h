#ifndef M_S_QUEUE_H
#define M_S_QUEUE_H

using namespace std;
#include<atomic>

class mnode
{
public:
	int val;
	atomic<mnode*> next;
	mnode(int v)
	{
		val = v;
	}
};
class msqueue
{

public:
	atomic<mnode*> head,tail;
	void enqueue(int val);
	int dequeue(void);
	msqueue()
	{
		mnode* dummy = new mnode(0);
		head.store(dummy);
		tail.store(dummy);
	}
};

#endif