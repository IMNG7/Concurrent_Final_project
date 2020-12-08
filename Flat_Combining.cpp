#include <atomic>
#include <stdlib.h>
#include <iostream>
#include "treiber.h"
#include "Flat_Combining.h"
#include "sgl_stack_queue.h"
#include <ctime>
#include "pthread.h"
#include <mutex>
using namespace std;

extern tstack S;
int num_threads = 20;
sgl_stack stack;
node_t* head_fc;
sgl_queue queue;
atomic<ThreadInfo*> *location_fc;
mutex mtx_fc;

void StackOp_fc(ThreadInfo* p)
{	
	int mypid;
	mypid = p->id;
	if(TryPerformStackOp_fc(p)==false)
	{
		LesOP_fc(p,mypid);
	}
	return;
}

void LesOP_fc(ThreadInfo *p,int pid)
{	
	while(1)
	{	atomic<int> mypid;
		mypid.store(pid);
		location_fc[mypid].store(p);
		delay_fc();
		if(!location_fc[mypid].compare_exchange_strong(p,NULL))
		{	
			if(p!=NULL)
			TryPerformStackOp_fc(p);
			return;
		}
stack:
		if(p!=NULL)
		if(TryPerformStackOp_fc(p) == true)
			return;
	}
}

bool TryPerformStackOp_fc(ThreadInfo *p)
{	if(mtx_fc.try_lock())
	{
		if(p->op == PUSH_FC)
		{
			stack.stack_push_fc(&head_fc,p);
			location_fc[p->id].store(NULL);
		}
		else if(p->op == POP_FC)
		{
			stack.stack_pop_fc(&head_fc);
			location_fc[p->id].store(NULL);
		}
		for(int i=1;i<num_threads;i++)
		{	
			if(location_fc[i].load()!=NULL)
			{	
				ThreadInfo* temp = location_fc[i].load();
				if(temp!=NULL && temp->op == PUSH_FC && (i==temp->id))
				{
					// cout<<"\n\rDoing Remaing Threads Tasks";
					stack.stack_push_fc(&head_fc,temp);
					location_fc[i].store(NULL);
				}
				else if(temp!=NULL && temp->op == POP_FC && (i==temp->id))
				{
					// cout<<"\n\rDoing Remaing Threads Tasks";
					stack.stack_pop_fc(&head_fc);
					location_fc[i].store(NULL);
				}
			}
		}
		mtx_fc.unlock();
	}
	else
	{	
		return false;
	}
	return true;
}

void FinishCollision_fc(ThreadInfo *p,int mypid)
{	
	if(p->op == POP_FC)
	{	
		ThreadInfo *a = new ThreadInfo();
		a = location_fc[mypid];
		p->cell = a->cell;
		location_fc[mypid] = NULL ;
	}
}

void delay_fc()
{
	for(int i=0;i<100000;i++);
}