#include"linked_list.h"
#include<stdio.h>
#include<stdlib.h>

typedef struct _LQueue
{
  Llist * queue;
}LQueue;

LQueue* LQueueAlloc(int,void*);
void LQueueDel(LQueue*);
int LQueuePopulate(LQueue*, void**, int);
int LQueueEnqueue(LQueue*,void*);
void* LQueueDequeue(LQueue*);

