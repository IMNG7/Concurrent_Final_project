#include <iostream>
#include <stdlib.h>
#include "treiber.h"
#include "M_S_Queue.h"
#include "Elimination.h"
#include "Basket_Queue.h"
#include "sgl_stack_queue.h"
#include "Flat_Combining.h"
#include "main.h"
#include <omp.h>
#include <atomic>
#include <unistd.h>
#include <getopt.h>
#include <sstream>

tstack S;
msqueue mqueue;
node_t* head;
using namespace std;
extern atomic<ThreadInfo*> *location_fc;
extern atomic<ThreadInfo*> *location;
extern int num_threads;
algo_types Algo;
s_q_types Stack_queue_type;
int count = 0;
int main(int argc, char *args[])
{
	int c;
	string algorithm,SQ;
	Algo = None;
	Stack_queue_type = SGL_Stack;
	int Iterations;
	while(1)
	{
		int count = 0;
		//Structure used in getopt_long to capture the command line arguments.
		static struct option long_options[] = {						
			{"alg", 	required_argument,	0,	'a'},
			{"Iterations", required_argument,	0,	'i'},
			{"Stack_queue", required_argument,	0,	's'},
			{0,			0,					0,	0}
		};
		// Used to capture the command line arguments
		c= getopt_long(argc,args,"a:i:s:",long_options,&count);		
		if(c==-1)	break;
		switch(c)
		{
			case 'a' :	
						// Saves the algorithm name in the variable
						algorithm = optarg;	
						if(algorithm == "Elim")
						Algo = Elimination;
						else if(algorithm == "fc")
						Algo = Flat_Combining;
						else
						Algo = None;						
						break;
			case 'i' :	Iterations = atoi(optarg);
						// Saves the algorithm name in the variable
						break;
			case 's' :	SQ = optarg;
						// Saves the algorithm name in the variable
						//cout<<SQ;
						//cout<<optarg;
						if(SQ == "sglst")
						Stack_queue_type = SGL_Stack;
						else if(SQ== "sglqu")
						Stack_queue_type = SGL_Queue;
						else if(SQ == "treiberst")
						Stack_queue_type = Treiber;
						else if(SQ == "msqu")
						Stack_queue_type = M_S_Queue;
						else if(SQ == "bsqu")
						Stack_queue_type = Basket_Queue;
						else
						Stack_queue_type = SGL_Stack;							
						break;
			default	:	
						break;
		}
	}

	int filler = 100;
	num_threads = omp_get_max_threads();
	location_fc = new atomic<ThreadInfo*> [num_threads+2];
	location = new atomic<ThreadInfo*> [num_threads+2];
	//ThreadInfo* temp_loc = new ThreadInfo[num_threads];
	sgl_stack stack;
	sgl_queue queue;
	init_queue();
	while(filler != 0){
		
		#pragma omp parallel default(none) shared(filler,stack,queue,S,Algo,Stack_queue_type,head,mqueue,cout) 
		{
			ThreadInfo* thread1 = new ThreadInfo();
			ThreadInfo* thread2 = new ThreadInfo();
			thread1->id = omp_get_thread_num()+1;
			thread1->op = PUSH_FC;
			thread1->cell = new tnode();
			thread1->cell->val = filler;
			thread1->cell->down = NULL;
			thread2->id = omp_get_thread_num()+1;
			thread2->op = POP_FC;
			if(Algo == Flat_Combining)
			{	cout<<"\n\r Implementing Flat_Combining";
				#pragma omp barrier
				StackOp_fc(thread1);
				StackOp_fc(thread2);
			}
			else if(Algo == Elimination)
			{	cout<<"\n\r Implementing Elimination";
				#pragma omp barrier
				StackOp(thread1);
				StackOp(thread2);
			}
			else
			{
				if(Stack_queue_type == SGL_Stack)
				{	//cout<<"\n\r Implementing SGL STACK Without Algo";
					#pragma omp barrier
					stack.stack_push(&head,filler);
					stack.stack_pop(&head);
				}
				else if(Stack_queue_type == SGL_Queue)
				{	//cout<<"\n\r Implementing SGL Queue Without Algo";
					#pragma omp barrier
					queue.queue_push(filler);
					queue.queue_pop();
				}
				else if(Stack_queue_type == Treiber)
				{	//cout<<"\n\r Implementing Treiber STACK Without Algo";
					#pragma omp barrier
					S.push(filler);
					S.pop();
				}
				else if(Stack_queue_type == M_S_Queue)
				{	//cout<<"\n\r Implementing M_S_Queue Without Algo";
					#pragma omp barrier
					mqueue.enqueue(filler);
					mqueue.dequeue();
				}
				else if(Stack_queue_type == Basket_Queue)
				{	//cout<<"\n\r Implementing Basket_Queue Without Algo";
					#pragma omp barrier
					enqueue(filler);			
					dequeue();
				}
			}
		}
		//cout<<filler;
		filler = filler - 1;			
		}
		//cout<<count;
		delete(location_fc);
		delete(location);
		return 0;
}