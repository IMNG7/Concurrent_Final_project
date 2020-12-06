#include <atomic>
#include <stdlib.h>
#include <iostream>
#include "treiber.h"
#include "Elimination.h"
#include <ctime>
using namespace std;

extern tstack S;
atomic<ThreadInfo*> location[21]={};
atomic<int> collision[6]={};
int him,pos;
// ThreadInfo *q;
ThreadInfo* null_ptr = NULL;
void StackOp(ThreadInfo* p)
{	
	int mypid;
	mypid = p->id;
	if(TryPerformStackOp(p)==false)
	{
		LesOP(p,mypid);
	}
	return;
}

void LesOP(ThreadInfo *p,int pid)
{	
	while(1)
	{	atomic<int> mypid;
		mypid.store(pid);
		location[mypid].store(p);
		pos = GetPosition();
		ThreadInfo* q = new ThreadInfo();
		him = collision[pos];
		while(!collision[pos].compare_exchange_strong(him,mypid))
			him = collision[pos];
		if(him!=EMPTY)
		{	
			q=location[him];
			if(q!=NULL && q->id == him && q->op!=p->op)
			{	
				if(location[mypid].compare_exchange_strong(p,NULL))
				{	
					if(TryCollision(p,q,mypid) == true)
					{	
						cout<<"\n\rCollision Achieved";
						return;
					}
					else
					{
						goto stack;
					}
				}
				else
				{	
					if(p!=NULL)
					FinishCollision(p,mypid);
					return;
				}
			}
		}
		delay();
		if(!location[mypid].compare_exchange_strong(p,NULL))
		{	
			if(p!=NULL)
			FinishCollision(p,mypid);
			return;
		}
stack:
		if(TryPerformStackOp(p) == true);
			return;
	}
}

bool TryPerformStackOp(ThreadInfo *p)
{	
	if(p->op == PUSH)
	{
		return S.push_elim(p);
	}
	if(p->op == POP)
	{
		return S.pop_elim(p);
	}
}

void FinishCollision(ThreadInfo *p,int mypid)
{	
	if(p->op == POP)
	{	
		ThreadInfo *a = new ThreadInfo();
		a = location[mypid];
		p->cell = a->cell;
		location[mypid] = NULL ;
	}
}
bool TryCollision(ThreadInfo*p,ThreadInfo *q,int mypid)
{	
	if(p->op == PUSH)
	{	
		if(location[him].compare_exchange_strong(q,p))
			return true;
		else
			return false;
	}
	if(p->op == POP)
	{	
		if(location[him].compare_exchange_strong(q,NULL))
		{
			p->cell = q->cell;
			location[mypid] = NULL;
			return true;
		}
		else
			return false;
	}
}

int GetPosition()
{	
	srand(time(0));
	return ((rand() % 5)+1);
}
void delay()
{
	for(int i=0;i<100000;i++);
}