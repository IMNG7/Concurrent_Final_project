/*
Bucket Queue
Author: Abhijeet Dutt Srivastava
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <atomic>
#include <new>
#include <time.h>
#include <omp.h>
#include <iostream>

using namespace std;

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


// class node;

// class pointer_t{
// public:
// pointer_t(){
// 	ptr = NULL;
// 	deleted = 0;
// 	tag =0;
// }
// node *ptr,
// bool deleted,
// unsigned int tag
// }



class queue{
public:

pointer_t head;
pointer_t tail;

queue(){}

//void free_chain(queue *q, pointer_t *head, pointer_t *new_head);
//void backoff_scheme();
};

queue Que;

void init_queue();
bool enqueue(int val,queue* q);
int dequeue();
void free_chain(pointer_t head, pointer_t new_head);

void reclaim_node(node *t){
delete (t);
}

void backoff_scheme(){

	for(int i=0;i<100000;i++);
}

void free_chain(pointer_t *head, pointer_t *new_head){
	//cout<<"\n\rInside Free Chain";
	//<new_head.ptr,0,head->tag+1>
	pointer_t temp_node1 (new_head->get_ptr(),0,head->get_tag()+1);
	// temp_node1->ptr = new_head->ptr;
	// temp_node1->deleted = 0;
	// temp_node1->tag = head->tag+1;
	pointer_t nxt (NULL,0,0);
	//<new_head.ptr,0,head->tag+1>
	if(Que.head.CAS(head->cnt_ptr_var.load(),temp_node1.cnt_ptr_var.load())){
		while(head->get_ptr() != new_head->get_ptr()){
			nxt.cnt_ptr_var.store(head->get_ptr()->next->cnt_ptr_var.load());
			reclaim_node(head->get_ptr());
			head->cnt_ptr_var.store(nxt.cnt_ptr_var.load());
		}
	}	
}

void init_queue(){



node *nd = new node(0);
nd->next = new pointer_t(NULL,0,0); 

pointer_t empty(nd,0,0);
//pointer_t new_p (nd,0,0);

Que.head.cnt_ptr_var.store(empty.cnt_ptr_var.load());
Que.tail.cnt_ptr_var.store(empty.cnt_ptr_var.load());

}

bool enqueue(int val){

node *nd = new node(val); // new node tob enqueued
//node *nd = comb_ptr(node* pointer,bool d ,unsigned int tag)

pointer_t nxt (NULL,0,0);
pointer_t t (NULL,0,0);

nd->next = new pointer_t(NULL,0,0);
while(1){	
	//t->cnt_ptr_var.store(q->tail->cnt_ptr_var.load()); //snapshot of original tail
	t.cnt_ptr_var.store(Que.tail.cnt_ptr_var.load());
	//next = tail->ptr->next.load();
	nxt.cnt_ptr_var.store(t.get_ptr()->next->cnt_ptr_var.load());
	//next->cnt_ptr_var.store(t->get_ptr()->next->cnt_ptr_var.load());
//node *pointer,bool d,unsigned int tag
	if(t.cnt_ptr_var.load() == Que.tail.cnt_ptr_var.load()){
		if(nxt.get_ptr() == NULL){
			
			pointer_t temp_node1 (NULL,0,t.get_tag()+2);
			
			//nd->next->cnt_ptr_var.store(temp_node1.cnt_ptr_var.load()); //<NULL, 0, tail.tag+2>
			
			nd->next->cnt_ptr_var.store(temp_node1.cnt_ptr_var.load());

			//printf("nd->val %d \n",nd->next->get_tag());
			pointer_t temp_node (nd,0,t.get_tag()+1);

			//printf("t tag %d \n",t->get_tag());
			if(t.get_ptr()->next->CAS(nxt.cnt_ptr_var.load(),temp_node.cnt_ptr_var.load())){ //<nd,0,tail.tag+1>	
				//printf("CAS1 Success \n");
				//pointer_t temp_node2 (nd,0,t->get_tag()+1);
				//cout<<q->tail->cnt_ptr_var<<"\n\r"<<t->cnt_ptr_var<<"\n\r"<<temp_node2.cnt_ptr_var;
				Que.tail.CAS(t.cnt_ptr_var.load(),temp_node.cnt_ptr_var.load());//??<nd,0,tail.tag+1>
				
					// cout<<"\n\r"<<q->tail->cnt_ptr_var<<"\n\r"<<t->cnt_ptr_var<<"\n\r"<<temp_node2.cnt_ptr_var;
					// cout<<"\n\r"<<temp_node2.get_ptr()->val<<"\n\r";
					// cout<<tail->get_ptr()->val<<"\n\r";
				
				return true;
			}
			nxt.cnt_ptr_var.store(t.get_ptr()->next->cnt_ptr_var.load());
			//next->cnt_ptr_var.store(q->tail->get_ptr()->next->cnt_ptr_var.load());
			
			// insert the failed operations into bucket based on tag
			
			while(nxt.get_tag() == t.get_tag()+1 && !(nxt.get_deleted())){
				//printf("TAG \n");
				backoff_scheme();
				nd->next->cnt_ptr_var.store(nxt.cnt_ptr_var.load());
				pointer_t temp_node3 (nd,0,t.get_tag()+1);
				if(t.get_ptr()->next->CAS(nxt.cnt_ptr_var,temp_node3.cnt_ptr_var)){ //<nd,0,tail.tag+1>
					return true;
				}
			nxt.cnt_ptr_var.store(t.get_ptr()->next->cnt_ptr_var.load());
			//next->cnt_ptr_var.store(q->tail->get_ptr()->next->cnt_ptr_var.load());
			}
		}
		else{
				while((nxt.get_ptr()->next->get_ptr() != NULL) && (Que.tail.cnt_ptr_var.load() == t.cnt_ptr_var.load())){
				nxt.cnt_ptr_var.store(nxt.get_ptr()->next->cnt_ptr_var.load());
				//next->cnt_ptr_var.store(next->get_ptr()->next->cnt_ptr_var.load());
			}
			pointer_t temp_node4 (nxt.get_ptr(),0,t.get_tag()+1);
			Que.tail.CAS(t.cnt_ptr_var.load(),temp_node4.cnt_ptr_var.load());//<next.ptr, 0 , tail.tag+1>
		}

	}
	}
}


int dequeue(){
pointer_t h (NULL,0,0);
pointer_t t (NULL,0,0);
pointer_t nxt (NULL,0,0);
pointer_t iter (NULL,0,0);
int value;

//TODO: initialise pointer

int hops;
while(1){
	//printf("dequeue \n");
	h.cnt_ptr_var.store(Que.head.cnt_ptr_var.load());
	t.cnt_ptr_var.store(Que.tail.cnt_ptr_var.load());	
	//nxt = head->ptr->next;
	nxt.cnt_ptr_var.store(h.get_ptr()->next->cnt_ptr_var.load());
	//cout<<nxt->cnt_ptr_var<<"\t"<<h->get_ptr()->next->cnt_ptr_var;
	//<nxt.ptr,0,tail->tag+1>
	
	if(h.cnt_ptr_var.load() == Que.head.cnt_ptr_var.load()){
		if(h.get_ptr() == t.get_ptr()){
			if(nxt.get_ptr() == NULL)
				return EMPTY;
			//while(nxt->ptr->next->ptr != NULL && tail==t)
			while(nxt.get_ptr()->next->get_ptr() != NULL && Que.tail.cnt_ptr_var.load() ==t.cnt_ptr_var.load()){											 
				//nxt = nxt->ptr->next;
				//printf("7 \n");
				nxt.cnt_ptr_var.store(nxt.get_ptr()->next->cnt_ptr_var.load());
			}	
			pointer_t temp_node (nxt.get_ptr(),0,t.get_tag()+1);
			Que.tail.CAS(t.cnt_ptr_var.load(),temp_node.cnt_ptr_var.load());//<nxt.ptr,0,tail->tag+1>
			//value = q->head->get_ptr()->val;
			}
			else{
				iter.cnt_ptr_var.store(h.cnt_ptr_var.load());
				hops = 0; 
				// cout<<"\n\r"<<!(nxt->get_deleted())<<"\n\r";
				// cout<<(iter->get_ptr() != t->get_ptr())<<"\n\r";
				// cout<<(q->head==h)<<"\n\r";
				while((nxt.get_deleted()) && (iter.get_ptr() != t.get_ptr()) && (Que.head.cnt_ptr_var.load() == h.cnt_ptr_var.load())){
					iter.cnt_ptr_var.store(nxt.cnt_ptr_var.load());
					nxt.cnt_ptr_var.store(iter.get_ptr()->next->cnt_ptr_var.load());
					hops++;
					//printf("8 \n");
				}
				 // Does this loop include the things below
					if(Que.head.cnt_ptr_var.load() != h.cnt_ptr_var.load()){
						//printf("9 \n");
						continue; // continue what?
					}
					else if(iter.get_ptr() == t.get_ptr()){
						//printf("10 \n");
						//cout<<"iter->get_ptr() == t->get_ptr()";
						free_chain(&h,&iter);
					}
					else{
						value = nxt.get_ptr()->val;
							pointer_t temp_node2 (nxt.get_ptr(),1,nxt.get_tag()+1);
						if(iter.get_ptr()->next->CAS(nxt.cnt_ptr_var.load(),temp_node2.cnt_ptr_var.load())){//<next.ptr,1,next.tag+1>
							if(hops >= MAX_HOPS){
								free_chain(&h,&nxt);
							}
							return value;
						}
					backoff_scheme();		
					}
				
			}
		}
	}
}


int main(){
	
	init_queue();
int filler = 1000;

while(filler != 0){

	#pragma omp parallel default(none) shared(filler) 
	{
		#pragma omp barrier

		enqueue(filler);			
		dequeue();
	}
	filler = filler - 1;			
	}
	return 0;
}