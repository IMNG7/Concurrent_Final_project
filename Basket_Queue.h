#ifndef BASKET_QUEUE_H
#define BASKET_QUEUE_H

using namespace std;
#include<atomic>


#define EMPTY -1
#define MAX_HOPS 3

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

pointer_t::pointer_t(node* pointer,bool d ,unsigned int tag){

unsigned long long td = ((unsigned long long)tag << 1) | (unsigned long long)d;
cnt_ptr_var.store((td<<48)|(unsigned long long)pointer);
}

node* pointer_t::get_ptr(){
unsigned long long shifted = cnt_ptr_var.load() << 16;
shifted = shifted >> 16;
node *ptr = (node*)(long)shifted;
return ptr;
}

int pointer_t::get_tag(){
unsigned long long shifted = (unsigned long long)cnt_ptr_var.load() >> 49;
return (int)shifted;
}

int pointer_t::get_deleted(){
unsigned long long v = (unsigned long long)cnt_ptr_var.load() >> 48; 
unsigned long long temp = 1;
unsigned long long d = (v & temp);
return (int)d;
}

bool pointer_t::CAS(unsigned long long cp1, unsigned long long cp2){
	if(cnt_ptr_var.compare_exchange_strong(cp1,cp2))
	{
		return true;
	}
	return false;
}

class queue{
public:

pointer_t head;
pointer_t tail;

queue(){}

};

void init_queue();
void delete_queue();
bool enqueue(int val,queue* q);
int dequeue();
void free_chain(pointer_t head, pointer_t new_head);


#endif