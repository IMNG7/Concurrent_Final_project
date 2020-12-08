#include <atomic>
#include <iostream>
#include <stdlib.h>
#include "sgl_stack_queue.h"
#include "pthread.h"
#include <mutex>
#include "treiber.h"
using namespace std;
mutex mtx;
extern int count;
void sgl_stack::stack_push(node_t** head,int d)
{	mtx.lock();
	node_t* new_node = new node_t(d);
	new_node->next = *head;
	*head = new_node;
	mtx.unlock();
}
int sgl_stack::stack_pop(node_t** head)
{	mtx.lock();
	if(!(*head))
		return NULL;
	node_t* temp = *head;
	*head = (*head)->next;
	int popped = temp ->val;
	free(temp);
	mtx.unlock();
	return(popped);
}

void sgl_stack::stack_push_fc(node_t** head,ThreadInfo* p)
{	
	node_t* new_node = new node_t(p->cell->val);
	new_node->next = *head;
	*head = new_node;
	count++;
}
int sgl_stack::stack_pop_fc(node_t** head)
{
	if(!(*head))
		return NULL;
	node_t* temp = *head;
	*head = (*head)->next;
	int popped = temp ->val;
	free(temp);
	count--;
	return(popped);
}

bool sgl_stack::stack_push_elim(node_t** head,ThreadInfo* p)
{	if(mtx.try_lock())
	{
		node_t* new_node = new node_t(p->cell->val);
		new_node->next = *head;
		*head = new_node;
		count++;
		return true;
	}
	else
		return false;
}
bool sgl_stack::stack_pop_elim(node_t** head,ThreadInfo* p)
{	
	if(mtx.try_lock())
	{
		if(!(*head))
			return NULL;
		node_t* temp = *head;
		*head = (*head)->next;
		int popped = temp ->val;
		free(temp);
		count--;
		return true;
	}
	return false;
}

void sgl_queue::queue_push(int x)
{	mtx.lock();
	node_t* temp = new node_t(x);
	if(rear == NULL)
	{
		front = rear = temp;
	}
	rear->next = temp;
	rear = temp;
	mtx.unlock();
}
int sgl_queue::queue_pop()
{	mtx.lock();
	if(front == NULL)
		return NULL;
	node_t* temp = front;
	front = front->next;
	if(front == NULL)
		rear = NULL;
	int temp_val = temp->val;
//	delete(temp);
	mtx.unlock();
	return temp_val;

}

void sgl_queue::queue_push_fc(int x)
{	
	node_t* temp = new node_t(x);
	if(rear == NULL)
	{
		front = rear = temp;
	}
	rear->next = temp;
	rear = temp;
}
int sgl_queue::queue_pop_fc()
{
	if(front == NULL)
		return NULL;
	node_t* temp = front;
	front = front->next;
	if(front == NULL)
		rear = NULL;
	int temp_val = temp->val;
	delete(temp);
	return temp_val;
}