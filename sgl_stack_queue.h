#ifndef SGL_STACK_QUEUE_H
#define SGL_STACK_QUEUE_H
#include "treiber.h"
class node_t
{
public:
	node_t(int v)
	{
		val=v;
	}
	int val;
	node_t* next;
};
class sgl_stack	//TODO: Get somechanges for this class or just remove it.
{	public:
	void stack_push(node_t** head,int d);
	void stack_push_fc(node_t** head,ThreadInfo *p);
	bool stack_push_elim(node_t** head,ThreadInfo *p);
	int stack_pop(node_t** head);
	int stack_pop_fc(node_t** head);
	bool stack_pop_elim(node_t** head,ThreadInfo *p);
};

class sgl_queue
{	public:
	node_t *front,*rear;
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