#include<stdint.h>

typedef struct _dummyPack
{
   int currentSize;
   char data[100];
   uint16_t derpFragOff;
}dummyPack;

char* reassemblyAlg(Llist* list, uint16_t size, FILE  file)
{
   fprintf(file, "Inside of ReassemblyAlg\n");
   if(LlistIsEmpty(list))
   {
      fprintf(file, "The list was empty, exiting with an error\n");
      return NULL;
   }

   uint16_t runningCount = size;
   char* dataReady = malloc(sizeof(char) * size);
   uint16_t fragOff = 0;
   LlistNode * current = list->head->next;
   dummyPack* curPack = NULL;
   
   while(current != list->head)
   {
      curPack = (dummyPack*)(current->data)
      memcpy(dataReady + curPack->derpFragoff, curPack->data, curPack->size);
      curPack = curPack->next;
   }
    
   return dataReady;
}
