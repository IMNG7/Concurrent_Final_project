#ifndef SGL_STACK_QUEUE_H
#define SGL_STACK_QUEUE_H
#include "treiber.h"
class node
{
public:
	node(int v)
	{
		val=v;
	}
	int val;
	node* next;
};
class sgl_stack	//TODO: Get somechanges for this class or just remove it.
{	public:
	void stack_push(node** head,int d);
	void stack_push_fc(node** head,ThreadInfo *p);
	bool stack_push_elim(node** head,ThreadInfo *p);
	int stack_pop(node** head);
	int stack_pop_fc(node** head);
	bool stack_pop_elim(node** head,ThreadInfo *p);
};

class sgl_queue
{	public:
	node *front,*rear;
	sgl_queue()
	{
		front = NULL;
		rear = NULL;
	}
	void queue_push(int d);
	void queue_push_fc(int d);
	int queue_pop(void);
	int queue_pop_fc(void);
};
#endif