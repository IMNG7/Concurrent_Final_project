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

extern int num_threads;
int main()
{
int filler = 1000;
num_threads = omp_get_max_threads();
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
		StackOp_fc(thread1);
		StackOp_fc(thread2);
		// if(TryStackOper(thread1) == false){
		// 			elimination_oper(push_thread_info);
		// }

		// if(TryStackOper(pop_thread_info) == false){
		// 			elimination_oper(pop_thread_info);
		// }
	}
	//cout<<filler;
	filler = filler - 1;			
	}

	return 0;
}