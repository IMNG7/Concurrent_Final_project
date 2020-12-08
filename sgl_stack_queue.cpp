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
void sgl_stack::stack_push(node** head,int d)
{	mtx.lock();
	node* new_node = new node(d);
	new_node->next = *head;
	*head = new_node;
	mtx.unlock();
}
int sgl_stack::stack_pop(node** head)
{	mtx.lock();
	if(!(*head))
		return NULL;
	node* temp = *head;
	*head = (*head)->next;
	int popped = temp ->val;
	free(temp);
	mtx.unlock();
	return(popped);
}

void sgl_stack::stack_push_fc(node** head,ThreadInfo* p)
{	
	node* new_node = new node(p->cell->val);
	new_node->next = *head;
	*head = new_node;
	count++;
}
int sgl_stack::stack_pop_fc(node** head)
{
	if(!(*head))
		return NULL;
	node* temp = *head;
	*head = (*head)->next;
	int popped = temp ->val;
	free(temp);
	count--;
	return(popped);
}

bool sgl_stack::stack_push_elim(node** head,ThreadInfo* p)
{	if(mtx.try_lock())
	{
		node* new_node = new node(p->cell->val);
		new_node->next = *head;
		*head = new_node;
		count++;
		return true;
	}
	else
		return false;
}
bool sgl_stack::stack_pop_elim(node** head,ThreadInfo* p)
{	
	if(mtx.try_lock())
	{
		if(!(*head))
			return NULL;
		node* temp = *head;
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
	node* temp = new node(x);
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
	node* temp = front;
	front = front->next;
	if(front == NULL)
		rear = NULL;
	int temp_val = temp->val;
	delete(temp);
	mtx.unlock();
	return temp_val;

}

void sgl_queue::queue_push_fc(int x)
{	
	node* temp = new node(x);
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
	node* temp = front;
	front = front->next;
	if(front == NULL)
		rear = NULL;
	int temp_val = temp->val;
	delete(temp);
	return temp_val;
}