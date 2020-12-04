#include <atomic>
#include <stdlib.h>
using namespace std;
#include "M_S_Queue.h"
void msqueue::enqueue(int val)
{
	mnode *old_tail,*extra,*real_tail;
	real_tail = new mnode(val);
	while(true)
	{
		old_tail = tail.load();
		extra = old_tail->next.load();
		mnode* null_ptr = NULL;
		if(old_tail == tail.load())
		{
			if(extra == NULL && old_tail->next.compare_exchange_strong(null_ptr,real_tail,memory_order_acq_rel))
				break;
			else if(extra!=NULL)
				tail.compare_exchange_strong(old_tail,extra);
		}
	}
	atomic_compare_exchange_strong(&tail,&old_tail,real_tail);
}
int msqueue::dequeue()
{
	mnode *old_tail,*real_head,*next_head;
	while(true)
	{
		real_head = head.load();
		old_tail = tail.load();
		next_head = real_head->next.load();
		if(real_head == head.load())
		{
			if(old_tail == real_head)
			{
				if(next_head == NULL)
				{
					return NULL;
				}
				else
				{
					tail.compare_exchange_strong(old_tail,next_head,memory_order_acq_rel);
				}
			}
			else
			{
				int ret = next_head->val;
				if(head.compare_exchange_strong(real_head,next_head,memory_order_acq_rel))
				{
					return ret;
				}
			}
		} 
	}
}
