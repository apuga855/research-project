#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include "linked_list.h"
#include<string.h>
typedef struct _dummyPack
{
   uint16_t dataSize;
   char * data;
   uint16_t fragoff;
   int derpFragOff;//assumed to be already in (2^13-1)*8 form
}dummyPack;

void * dataalloc()
{
   return (void*) malloc(sizeof(dummyPack));
}


char* reassemblyAlg(Llist* list, uint16_t size)
{
   printf("Inside of ReassemblyAlg\n");
   if(LlistIsEmpty(list))
   {
      printf("The list was empty, exiting with an error\n");
      return NULL;
   }

   printf("After if statement\n");
   printf("The list length is %d\n", list->length);
   //uint16_t runningCount = size;
   char* dataReady = malloc(sizeof(char) * ((int) size));
   LlistNode * current = list->head->next;
   dummyPack* curPack = NULL;
   int i = 0;
   printf("before the while\n");
   while(current != list->head)
   {
      printf("Current loop = %d",i);
      curPack = (dummyPack*)(current->data);
      memcpy((dataReady + curPack->derpFragOff), curPack->data, curPack->dataSize);
      current = current->next;
      i++;
   }
   printf("After while\n");
    
   return dataReady;
}

int main()
{
   char* finished = NULL;
   char array[100];
   memset(array, 'A', 20);
   memset(array+20, 'B', 20);
   memset(array+40, 'C', 20);
   memset(array+60, 'D', 20);
   memset(array+80, 'E', 20);
   uint16_t  sz = 100;
   Llist* list = LlistAlloc(5,dataalloc);
   printf("Creating test data\n");
   dummyPack * arr = malloc(sizeof(dummyPack) * 5);
   arr[0].dataSize = 20;
   arr[0].derpFragOff = 0;
   arr[0].data = malloc(sizeof(char)*20);
   memcpy(arr[0].data, array, 20);
   arr[1].dataSize = 20;
   arr[1].data = malloc(sizeof(char)*20);
   arr[1].derpFragOff = 20;
   memcpy(arr[1].data, array + 20, 20);
   arr[2].dataSize = 20;
   arr[2].data = malloc(sizeof(char)*20);
   memcpy(arr[2].data, array + 40, 20);
   arr[2].derpFragOff = 40;
   arr[3].dataSize = 20;
   arr[3].data = malloc(sizeof(char)*20);
   memcpy(arr[3].data, array + 60, 20);
   arr[3].derpFragOff = 60;
   arr[4].dataSize = 20;
   arr[4].data = malloc(sizeof(char)*20);
   memcpy(arr[4].data, array + 80, 20);
   arr[4].derpFragOff = 80;
   LlistPopulate(list, (void*) arr, 5);  
   finished = reassemblyAlg(list, sz);
   printf("after finished\n");
   if(finished != NULL)
   {
      printf("Original data %s\n",array);
      printf("Data from function%s\n",finished);
   }
   
}

