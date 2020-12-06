#ifndef TREIBER_H
#define TREIBER_H

using namespace std;
#include<atomic>

class ThreadInfo;
class tnode
{
public:
	int val;
	tnode* down;
};
class tstack
{
public:
	atomic<tnode*> top;
	void push(int val);
	bool push_elim(ThreadInfo *p);
	int pop(void);
	bool pop_elim(ThreadInfo *p);
	void display();
};
class ThreadInfo
{
public:
	uint id;
	char op;
	tnode* cell;
	int spin;
};

#endif