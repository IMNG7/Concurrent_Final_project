#include <iostream>
#include <stdlib.h>
#include "treiber.h"
#include "M_S_Queue.h"
#include "Elimination.h"
#include <omp.h>
using namespace std;
int main()
{
int filler = 1000;

while(filler != 0){

	#pragma omp parallel default(none) shared(filler) 
	{
		#pragma omp barrier
		ThreadInfo* thread1 = new ThreadInfo();
		ThreadInfo* thread2 = new ThreadInfo();
		thread1->id = omp_get_thread_num()+1;
		thread1->op = PUSH;
		thread1->cell = new tnode();
		thread1->cell->val = 1;
		thread1->cell->down = NULL;
		thread2->id = omp_get_thread_num()+1;
		thread2->op = POP;
		StackOp(thread1);
		StackOp(thread2);
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