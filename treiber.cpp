#include<stdlib.h>
#include <iostream>
#include<atomic>
#include "treiber.h"
#include "Elimination.h"
using namespace std;

extern tstack S;
void tstack::push(int val)
{	tnode* t = new tnode;
	tnode* n = new tnode;
	n->val = val;
	do
	{
		t = top.load(memory_order_acquire);
		n->down = t;
	}while(!top.compare_exchange_strong(t,n,memory_order_acq_rel));
}
bool tstack::push_elim(ThreadInfo *p)
{	
	tnode *phead;
	phead = S.top;
	p->cell->down = phead;
	if(S.top.compare_exchange_strong(phead,p->cell))
		return true;
	else
		return false;
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
bool tstack::pop_elim(ThreadInfo *p)
{
	tnode *phead,*pnext;
	phead = S.top;
	if(phead == NULL)
	{
		p->cell =NULL;
		return true;
	}
	pnext = phead->down;
	if(S.top.compare_exchange_strong(phead,pnext))
	{
		p->cell = phead;
		return true;
	}
	else
	{
		p->cell = NULL;
		return false;
	}
}