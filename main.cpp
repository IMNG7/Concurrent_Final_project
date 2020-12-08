#include <iostream>
#include <stdlib.h>
#include "treiber.h"
#include "M_S_Queue.h"
#include "Elimination.h"
//#include "Basket_Queue.h"
#include "sgl_stack_queue.h"
#include "Flat_Combining.h"
#include <omp.h>
#include <atomic>
using namespace std;
extern atomic<ThreadInfo*> *location_fc;
extern atomic<ThreadInfo*> *location;
extern int num_threads;
int count = 0;
int main()
{
int filler = 1000;
num_threads = omp_get_max_threads();
location_fc = new atomic<ThreadInfo*> [num_threads+2];
location = new atomic<ThreadInfo*> [num_threads+2];
//ThreadInfo* temp_loc = new ThreadInfo[num_threads];


while(filler != 0){

	#pragma omp parallel default(none) shared(filler) 
	{
		#pragma omp barrier
		ThreadInfo* thread1 = new ThreadInfo();
		ThreadInfo* thread2 = new ThreadInfo();
		thread1->id = omp_get_thread_num()+1;
		thread1->op = PUSH_FC;
		thread1->cell = new tnode();
		thread1->cell->val = filler;
		thread1->cell->down = NULL;
		thread2->id = omp_get_thread_num()+1;
		thread2->op = POP_FC;
		StackOp(thread1);
		StackOp(thread2);
	}
	//cout<<filler;
	filler = filler - 1;			
	}
	//cout<<count;
	delete(location_fc);
	delete(location);
	return 0;
}