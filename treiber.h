#ifndef TREIBER_H
#define TREIBER_H

using namespace std;
#include<atomic>


class tstack
{
	class tnode
	{
	public:
		tnode(int v)
		{
			val=v;
		}
		int val;
		tnode* down;
	};
public:
	atomic<tnode*> top;
	void push(int val);
	int pop(void);
	void display();
};


#endif