#include<stdio.h>
#include<stdlib.h>
#include "queueLinkedList.h"

typedef struct _garbage
{
   int alsoGarbage;
}garbage;

void* mydataalloc(){return malloc(sizeof(garbage));}


int main()
{
   LQueue* que = LQueueAlloc(10,mydataalloc);
   garbage * derp[10];
   int i = 0 ;
   printf("Right before the while\n");
   while(i < 10)
   {
      derp[i] = malloc(sizeof(garbage));
      derp[i]->alsoGarbage = i;
      LQueueEnqueue(que, (void*) derp[i]);
      i++;
   }
   printf("After populating\n");
   i = 0;
   int* current;
    
   printf("Printing as dequeueing\n");
   while(i < 10)
   {
      printf("At Dequeue\n");
      current = (int*)LQueueDequeue(que);
      if(current == NULL)
         printf("Problem\n");
      else
         printf("The current dequeue is %d", (*current));
      i++;
   } 
   //LQueuePopulate(que, (void*)garbage, 10);
}
