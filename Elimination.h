#ifndef ELIMINATION_H
#define ELIMINATION_H

#include "treiber.h"

#define Empty 0
#define POP 1
#define PUSH 2

void StackOp(ThreadInfo* p);
void LesOP(ThreadInfo *p,int mypid);
bool TryPerformStackOp(ThreadInfo *p);
void FinishCollision(ThreadInfo *p,int mypid);
bool TryCollision(ThreadInfo*p,ThreadInfo *q,int mypid);
int GetPosition();
void delay();
#endif