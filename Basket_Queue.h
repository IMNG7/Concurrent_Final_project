#ifndef BASKET_QUEUE_H
#define BASKET_QUEUE_H

using namespace std;
#include<atomic>

class pointer_t;

class node{
public:
	int val;
	pointer_t *next;
	node(int number){val = number;}
};

class pointer_t{
public:
	node* get_ptr();
	int get_tag();
	int get_deleted();
	bool CAS(unsigned long long cp1, unsigned long long cp2);
	pointer_t(){}
	pointer_t(node *pointer,bool d,unsigned int tag);
	atomic <unsigned long long> cnt_ptr_var;
};

class queue{
public:

pointer_t head;
pointer_t tail;

queue(){}

};

void init_queue();
void delete_queue();
bool enqueue(int val);
int dequeue();
void free_chain(pointer_t head, pointer_t new_head);


#endif