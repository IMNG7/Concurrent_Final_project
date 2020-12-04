#include<stdlib.h>
#include <iostream>
#include<atomic>
#include "treiber.h"
using namespace std;

void tstack::push(int val)
{	tnode* t;
	tnode* n = new tnode(val);
	do
	{
		t = top.load(memory_order_acquire);
		n->down = t;
	}while(!top.compare_exchange_strong(t,n,memory_order_acq_rel));
}
int tstack::pop()
{
	tnode* n;
	tnode* t;
	int v;
	do
	{
		t= top.load(memory_order_acquire);
		if(t==NULL)
		{
			return NULL;
		}
		n = t->down;
		v = t->val;
	}while(!top.compare_exchange_strong(t,n,memory_order_acq_rel));
	return v;
}