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
#include "Basket_Queue.h"

using namespace std;

#define EMPTY -1
#define MAX_HOPS 3


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
queue Que;

void reclaim_node(node *t){
delete (t);
}

void backoff_scheme(){

	for(int i=0;i<100000;i++);
}

void free_chain(pointer_t *head, pointer_t *new_head){
	pointer_t temp_node1 (new_head->get_ptr(),0,head->get_tag()+1);
	pointer_t nxt (NULL,0,0);
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
Que.head.cnt_ptr_var.store(empty.cnt_ptr_var.load());
Que.tail.cnt_ptr_var.store(empty.cnt_ptr_var.load());
}

bool enqueue(int val){

node *nd = new node(val); // new node tob enqueued


pointer_t nxt (NULL,0,0);
pointer_t t (NULL,0,0);

nd->next = new pointer_t(NULL,0,0);
while(1){	

	t.cnt_ptr_var.store(Que.tail.cnt_ptr_var.load());

	nxt.cnt_ptr_var.store(t.get_ptr()->next->cnt_ptr_var.load());

	if(t.cnt_ptr_var.load() == Que.tail.cnt_ptr_var.load()){
		if(nxt.get_ptr() == NULL){
			
			pointer_t temp_node1 (NULL,0,t.get_tag()+2);
			

			
			nd->next->cnt_ptr_var.store(temp_node1.cnt_ptr_var.load());


			pointer_t temp_node (nd,0,t.get_tag()+1);


			if(t.get_ptr()->next->CAS(nxt.cnt_ptr_var.load(),temp_node.cnt_ptr_var.load())){
				Que.tail.CAS(t.cnt_ptr_var.load(),temp_node.cnt_ptr_var.load());//??<nd,0,tail.tag+1>
				
				return true;
			}
			nxt.cnt_ptr_var.store(t.get_ptr()->next->cnt_ptr_var.load());
			
			while(nxt.get_tag() == t.get_tag()+1 && !(nxt.get_deleted())){

				backoff_scheme();
				nd->next->cnt_ptr_var.store(nxt.cnt_ptr_var.load());
				pointer_t temp_node3 (nd,0,t.get_tag()+1);
				if(t.get_ptr()->next->CAS(nxt.cnt_ptr_var,temp_node3.cnt_ptr_var)){ 
					return true;
				}
			nxt.cnt_ptr_var.store(t.get_ptr()->next->cnt_ptr_var.load());

			}
		}
		else{
				while((nxt.get_ptr()->next->get_ptr() != NULL) && (Que.tail.cnt_ptr_var.load() == t.cnt_ptr_var.load())){
				nxt.cnt_ptr_var.store(nxt.get_ptr()->next->cnt_ptr_var.load());

			}
			pointer_t temp_node4 (nxt.get_ptr(),0,t.get_tag()+1);
			Que.tail.CAS(t.cnt_ptr_var.load(),temp_node4.cnt_ptr_var.load());
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


int hops;
while(1){
	//printf("dequeue \n");
	h.cnt_ptr_var.store(Que.head.cnt_ptr_var.load());
	t.cnt_ptr_var.store(Que.tail.cnt_ptr_var.load());	
	//nxt = head->ptr->next;
	nxt.cnt_ptr_var.store(h.get_ptr()->next->cnt_ptr_var.load());

	
	if(h.cnt_ptr_var.load() == Que.head.cnt_ptr_var.load()){
		if(h.get_ptr() == t.get_ptr()){
			if(nxt.get_ptr() == NULL)
				return EMPTY;

			while(nxt.get_ptr()->next->get_ptr() != NULL && Que.tail.cnt_ptr_var.load() ==t.cnt_ptr_var.load()){											 

				nxt.cnt_ptr_var.store(nxt.get_ptr()->next->cnt_ptr_var.load());
			}	
			pointer_t temp_node (nxt.get_ptr(),0,t.get_tag()+1);
			Que.tail.CAS(t.cnt_ptr_var.load(),temp_node.cnt_ptr_var.load());

			}
			else{
				iter.cnt_ptr_var.store(h.cnt_ptr_var.load());
				hops = 0; 

				while((nxt.get_deleted()) && (iter.get_ptr() != t.get_ptr()) && (Que.head.cnt_ptr_var.load() == h.cnt_ptr_var.load())){
					iter.cnt_ptr_var.store(nxt.cnt_ptr_var.load());
					nxt.cnt_ptr_var.store(iter.get_ptr()->next->cnt_ptr_var.load());
					hops++;

				}

					if(Que.head.cnt_ptr_var.load() != h.cnt_ptr_var.load()){

						continue; 
					}
					else if(iter.get_ptr() == t.get_ptr()){

						free_chain(&h,&iter);
					}
					else{
						value = nxt.get_ptr()->val;
							pointer_t temp_node2 (nxt.get_ptr(),1,nxt.get_tag()+1);
						if(iter.get_ptr()->next->CAS(nxt.cnt_ptr_var.load(),temp_node2.cnt_ptr_var.load())){
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