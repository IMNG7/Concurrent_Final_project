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
sgl_stack stack;
node* head;
sgl_queue queue;
atomic<ThreadInfo*> location_fc[21]={};
// atomic<int> collision[6]={};
// int him,pos;
mutex mtx_fc;
// ThreadInfo *q;
//ThreadInfo* null_ptr = NULL;
int num_threads = 20;

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
		// pos = GetPosition();
		// ThreadInfo* q = new ThreadInfo();
		// him = collision[pos];
		// while(!collision[pos].compare_exchange_strong(him,mypid))
		// 	him = collision[pos];
		// if(him!=EMPTY)
		// {	
		// 	q=location[him];
		// 	if(q!=NULL && q->id == him && q->op!=p->op)
		// 	{	
		// 		if(location[mypid].compare_exchange_strong(p,NULL))
		// 		{	
		// 			if(TryCollision(p,q,mypid) == true)
		// 			{	
		// 				cout<<"\n\rCollision Achieved";
		// 				return;
		// 			}
		// 			else
		// 			{
		// 				goto stack;
		// 			}
		// 		}
		// 		else
		// 		{	
		// 			if(p!=NULL)
		// 			FinishCollision(p,mypid);
		// 			return;
		// 		}
		// 	}
		// }
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
			stack.stack_push_fc(&head,p);
			location_fc[p->id].store(NULL);
		}
		else if(p->op == POP_FC)
		{
			stack.stack_pop_fc(&head);
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
					stack.stack_push_fc(&head,temp);
					location_fc[i].store(NULL);
				}
				else if(temp!=NULL && temp->op == POP_FC && (i==temp->id))
				{
					// cout<<"\n\rDoing Remaing Threads Tasks";
					stack.stack_pop_fc(&head);
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
// bool TryCollision(ThreadInfo*p,ThreadInfo *q,int mypid)
// {	
// 	if(p->op == PUSH)
// 	{	
// 		if(location[him].compare_exchange_strong(q,p))
// 			return true;
// 		else
// 			return false;
// 	}
// 	if(p->op == POP)
// 	{	
// 		if(location[him].compare_exchange_strong(q,NULL))
// 		{
// 			p->cell = q->cell;
// 			location[mypid] = NULL;
// 			return true;
// 		}
// 		else
// 			return false;
// 	}
// }

// int GetPosition()
// {	
// 	srand(time(0));
// 	return ((rand() % 5)+1);
// }
void delay_fc()
{
	for(int i=0;i<100000;i++);
}