#include"queueLinkedList.h"
LQueue* LQueueAlloc(int size, void * func)
{
   LQueue* temp = malloc(sizeof(LQueue));
   temp->queue = LlistAlloc(size,func);
   return temp;
}

void LQueueDel(LQueue* q){LlistDel(q->queue);}
int LQueuePopulate(LQueue* q, void** data, int length){LlistPopulate(q->queue,data,length);}
int LQueueEnqueue(LQueue* q, void* input){LlistInsNodeQ(q->queue, input);}
void* LQueueDequeue(LQueue* q)
{
   int i = 0;
   void* current;
   LlistNode* target = q->queue->head->next;
   while(i < q->queue->used)
   {
      target = target->next;
      current = (void*) LlistDelNodeTargetQ(target, q->queue);
      i++;
   }
   return current;
}

