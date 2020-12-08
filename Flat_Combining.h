#ifndef FLAT_COMNINING_H
#define FLAT_COMNINING_H

#include "treiber.h"

#define Empty 0
#define POP_FC 3
#define PUSH_FC 4

void StackOp_fc(ThreadInfo* p);
void LesOP_fc(ThreadInfo *p,int mypid);
bool TryPerformStackOp_fc(ThreadInfo *p);
void FinishCollision_fc(ThreadInfo *p,int mypid);
// bool TryCollision(ThreadInfo*p,ThreadInfo *q,int mypid);
// int GetPosition();
void delay_fc();
#endif