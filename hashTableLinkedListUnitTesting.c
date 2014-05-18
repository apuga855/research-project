#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"linked_list.h"

typedef struct _dummyStruct
{
   int id;
   char garbage;
}dummyStruct;

void* dummyAlloc()
{
   return malloc(sizeof(dummyStruct));
}

void dummyCpy(void* src, void* dst)
{
   dummyStruct* srcC = (dummyStruct*) src;
   dummyStruct* dstC = (dummyStruct*) dst;
   dstC->id = srcC->id;
   dstC->garbage = srcC->garbage;

}
void printDummy(dummyStruct* src)
{
   if(src == NULL)
   {
      printf("The struct was empty\n\n");
      return;
   }
   printf("\nPrinting dummy struct:\n id = %d, garbage = %c\n\n", src->id, src->garbage);
}

int myhashfunc(LH_hashTable* table, void * data)
{
   int slot = 0;
   int rslot = 0;
   int i = 1;
   person* p = (person*) data;
   slot = (p->id) % (kprimeCap(table->LH_primenums));

   printf("\nAttempting to hash %s\n",p->name);

   if(LlistIsEmpty(table->LH_table[slot].LHN_list))
      return slot;

   else
   {
      rslot = (p->id) % (kprimehash2(table->LH_primenums));
      if(rslot == 0 && slot == 0)
          rslot = (p->id + 13) % (kprimehash2(table->primenums));
  
      do
      {
         slot = (slot + (i * rslot)) % (table->LH_capacity);
         i++;
      }while(LlistIsEmpty(table->LH_table[slot].LHN_list));
   }
   return slot;
}

int main()
{

}
