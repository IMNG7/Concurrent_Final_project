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

void help_menu();

int main(int argc, char *args[])
{
	int c;
	string algorithm,SQ;
	Algo = None;
	Stack_queue_type = SGL_Stack;
	int Iterations=1000;
	while(1)
	{
		int count = 0;
		//Structure used in getopt_long to capture the command line arguments.
		static struct option long_options[] = {						
			{"alg", 	required_argument,	0,	'a'},
			{"Iterations", required_argument,	0,	'i'},
			{"Stack_queue", required_argument,	0,	's'},
			{"help", required_argument,	0,	'h'},
			{0,			0,					0,	0}
		};
		// Used to capture the command line arguments
		c= getopt_long(argc,args,"a:i:s:h:",long_options,&count);		
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
			case 'h' : help_menu();
						return 0;
			default	:	
						break;
		}
	}

	num_threads = omp_get_max_threads();
	location_fc = new atomic<ThreadInfo*> [num_threads+2];
	location = new atomic<ThreadInfo*> [num_threads+2];
	//ThreadInfo* temp_loc = new ThreadInfo[num_threads];
	sgl_stack stack;
	sgl_queue queue;
	init_queue();
		
		#pragma omp parallel default(none) shared(stack,queue,S,Algo,Stack_queue_type,head,mqueue,cout, num_threads,Iterations) 
		{
			int Ind_thread_num = omp_get_thread_num();
			if(num_threads > Iterations)
	        {
	            num_threads = Iterations;
	        }
			int left =Ind_thread_num * (Iterations/num_threads);
			int right=((Ind_thread_num+1) * (Iterations/num_threads))-1;
			for(int i=left;i<=right;i++)
			{
				ThreadInfo* thread1 = new ThreadInfo();
				ThreadInfo* thread2 = new ThreadInfo();
				thread1->id = omp_get_thread_num()+1;
				thread1->op = PUSH_FC;
				thread1->cell = new tnode();
				thread1->cell->val = i;
				thread1->cell->down = NULL;
				thread2->id = omp_get_thread_num()+1;
				thread2->op = POP_FC;
				if(Algo == Flat_Combining)
				{	//cout<<"\n\r Implementing Flat_Combining";
					#pragma omp barrier
					StackOp_fc(thread1);
					StackOp_fc(thread2);
				}
				else if(Algo == Elimination)
				{	//cout<<"\n\r Implementing Elimination";
					#pragma omp barrier
					StackOp(thread1);
					StackOp(thread2);
				}
				else
				{
					if(Stack_queue_type == SGL_Stack)
					{	//cout<<"\n\r Implementing SGL STACK Without Algo";
						#pragma omp barrier
						stack.stack_push(&head,i);
						stack.stack_pop(&head);
					}
					else if(Stack_queue_type == SGL_Queue)
					{	//cout<<"\n\r Implementing SGL Queue Without Algo";
						#pragma omp barrier
						queue.queue_push(i);
						queue.queue_pop();
					}
					else if(Stack_queue_type == Treiber)
					{	//cout<<"\n\r Implementing Treiber STACK Without Algo";
						#pragma omp barrier
						S.push(i);
						S.pop();
					}
					else if(Stack_queue_type == M_S_Queue)
					{	//cout<<"\n\r Implementing M_S_Queue Without Algo";
						#pragma omp barrier
						mqueue.enqueue(i);
						mqueue.dequeue();
					}
					else if(Stack_queue_type == Basket_Queue)
					{	//cout<<"\n\r Implementing Basket_Queue Without Algo";
						#pragma omp barrier
						enqueue(i);			
						dequeue();
					}
				}
			}
		}
		// delete(location_fc);
		// delete(location);
		return 0;
}

void help_menu()
{
	cout<<"\n\rAfter the compilation of the program, you can use the ./mysort instruction on the command line, to execute the program. There are 4 arguments you can give to the ./mysort instruction. These are Stack and Queue(-s with various stacks and queues) to give the give the type of stack and queue used, algorithm(with varible -a) required to give type of  optimization algorithm  used and help(-h) to display the help menu." ;
    cout<<"\n\r-s can take 5 different value, sglst for sgl stack, sglqu for sgl queue, treiberst for treiber stack, msqu for  M& SQueue,bsqu for Basket Queue. By giving these variables , we can use the type of stack or queue we want to use. By default, if nothing is given, we will use SGL Stack. ";
    cout<<"\n\rHelp will display the help menu for the types of arguments, and what values can be given. ";
    cout<<"\n\rLast is the algorithm with variable --alg, it can take two values, one is fc and other is elim, according to the value they will perform flat combining algorithm on stack and queue and b. If something else is given, or nothing is given, if nothing given, none will be set by default. ";
    cout<<"\n\rThere are only few combinations possible for algos, if Elimination is given, then only trieber stack or SGL stack will be used, else it will exi.t. Similarly, if Flat Combining is given, only SGL stack and SGL queue can be given.\n\r";
}