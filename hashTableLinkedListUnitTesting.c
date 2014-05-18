#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"linked_list.h"
#include "hashTableLinkedList.h"
/*
LHN_hashNode Section							Check
LHN_hashNode * LHN_HashNodeAlloc();					X
LHN_hashNode * LHN_HashNodeDataAlloc(int, void*);			X
LHN_hashNode * LHN_HashNodeSelfAlloc(void*, void**, int);		X
int HashNodeFail(LHN_hashNode*);					X
int LHN_HashNodeDel(LHN_hashNode*);					X
int LHN_HashNodeIns(LHN_hashNode*,void*);				X
int LHN_HashNodeCpy(LHN_hashNode*, LHN_hashNode*, void*, void*);	X
void LHN_HashNodePrint(LHN_hashNode*,void*);				X
void LHN_HashNodeInit(LHN_hashNode*);					X

LH_hashTable Section						        Check
LH_hashTable* LH_HashTableAlloc();					X
LH_hashTable* LH_HashTableAllocBuff(void*);				X
LH_hashTable* LH_HashTableAllocSetBuff(int, void*);			X
void LH_HashTableInit(LH_hashTable*);					X
int LH_HashTableDel(LH_hashTable*);					X
int LH_HashTableRehash(LH_hashTable**, void*, void*,void*);		
long int LH_hash(LHN_hashNode*,LH_hashTable*);				
void LH_HashSet(LH_hashTable*, void*, void*, void*);			X
int LH_hashFunc(LH_hashTable*, void *, void *,void *,void *);		
void LH_HashTablePrint(LH_hashTable*, void*);				X
typedef int(*LH_keyGenerate)(LH_hashTable*, void*);			X

*/
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
   printf("\nPrinting dummy struct:\n id = %d, garbage = %c", src->id, src->garbage);
}

int myhashfunc(LH_hashTable* table, void * data)
{
   int slot = 0;
   int rslot = 0;
   int i = 1;
   dummyStruct* p = (dummyStruct*) data;
   slot = (p->id) % (kprimeCap(table->LH_primenums));

   printf("\nAttempting to hash %c\n",p->garbage);

   if(LlistIsEmpty(table->LH_table[slot].LHN_list) || 
      (LlistAtLeastOne(table->LH_table[slot].LHN_list) && 
      ((dummyStruct*)LlistRetFirst(table->LH_table[slot].LHN_list))->id == ((dummyStruct*)data)->id))
         return slot;
   
   else
   {
      rslot = (p->id) % (kprimehash2(table->LH_primenums));
      if(rslot == 0 && slot == 0)
          rslot = (p->id + 13) % (kprimehash2(table->LH_primenums));
  
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
   int curID = 0;
   char curGarbage = '!';
   int i = 0;
   void * payloadArr[10];

   LHN_hashNode* tmp = NULL;
   printf("****************************HashNodeSection*************************\n");
   printf("Success at HashNodeAlloc() Test\n");
   tmp = LHN_HashNodeAlloc();
   if(tmp != NULL)
      printf("Success at HashNodeAlloc()++++++++++++++++++++++++++\n");
   else
      printf("Failure at HashNodeAlloc()--------------------------\n");

   printf("Success at HashNodeDel() Test\n");
   if(LHN_HashNodeDel(tmp))
   {  
      tmp = NULL;
      printf("Success at HashNodeDel()++++++++++++++++++++++++++\n");
   }

   else
      printf("Failure at HashNodeDel()--------------------------\n");
   
   tmp = LHN_HashNodeDataAlloc(10, dummyAlloc);
   if(tmp != NULL)
      printf("Success at HashNodeDataAlloc()++++++++++++++++++++++++++\n");

   else
      printf("Failure at HashNodeDataAlloc()--------------------------\n");
  
   printf("LHN_HashNodePrint() Test\n");
   LHN_HashNodePrint(tmp, printDummy);
   printf("\nIf show properly, success at LHN_HashNodePrint() Test\n");
   printf("LHN_HashNodeSelfAlloc Test\n");
   LHN_HashNodeDel(tmp);
   tmp = NULL;
   while(i < 10)
   {
      payloadArr[i] = malloc(sizeof(dummyStruct));
      ((dummyStruct*)payloadArr[i])->id = curID;
      ((dummyStruct*)payloadArr[i])->garbage = curGarbage;
      curID++;
      curGarbage++;
      i++;
   }
   
   tmp = LHN_HashNodeSelfAlloc(dummyAlloc, payloadArr, 10);
   LHN_HashNodePrint(tmp, printDummy);
   printf("\nIf show properly, success at LHN_HashNodeSelfAlloc() Test\n");
   printf("\nLHN_HashNodeIns()\n");
   dummyStruct * ins =(dummyStruct*) dummyAlloc();
   ins->id = curID;
   ins->garbage = curGarbage;
   curID++;
   curGarbage++;
   LHN_HashNodeIns(tmp,(void*) ins);
   LHN_HashNodePrint(tmp, printDummy);
   printf("\nIf show properly, success at LHN_HashNodeIns worked\n");
   printf("\nIf show properly, success at LHN_HashNodeCpy() Test\n");
   LHN_hashNode* cpy = LHN_HashNodeAlloc();
   if(LHN_HashNodeCpy(tmp, cpy, dummyCpy,dummyAlloc))
      printf("Success LHN_HashNodeCpy\n");
   else
      printf("Failure LHN_HashNodeCpy\n");
   printf("\nPrinting original\n");
   LHN_HashNodePrint(tmp, printDummy);
   printf("\nPrinting copy\n");
   LHN_HashNodePrint(cpy, printDummy);
   LHN_HashNodeDel(tmp);
   LHN_HashNodeDel(cpy);
   tmp = NULL;
   cpy = NULL;

   
   

   printf("\n\n****************************HashNodeSection*************************\n");
   printf("Testing LH_HashTableAlloc() Test\n");
   LH_hashTable* testTable = NULL;
   testTable = LH_HashTableAlloc();
   if(testTable != NULL) 
      printf("Success at HashTablelloc()++++++++++++++++++++++++++\n");
   else
      printf("Failure at HasTableAlloc()--------------------------\n");
   
   printf("Testing LH_HashTableDel()\n");
   if(LH_HashTableDel(testTable))
      printf("Success at HashTableDel()++++++++++++++++++++++++++\n");
   else
      printf("Failure at HasTableDel()--------------------------\n");
   
   printf("Testing LH_HashTableAllocBuff() Test\n");
   testTable = NULL;
   testTable = LH_HashTableAllocBuff(dummyAlloc);
   if(testTable != NULL)
      printf("Success at LH_HashTableAllocBuff()++++++++++++++++++++++++++\n");
   else
      printf("Failure at LH_HashTableAllocBuff()--------------------------\n");
   printf("Printing the hashTable out\n");
   LH_HashTablePrint(testTable, printDummy);

   i = 0;
   curID = 0;
   curGarbage = '!';
   void* arrpayload[93];
   
   while(i < 93)
   {
      arrpayload[i] = malloc(sizeof(dummyStruct));
      ((dummyStruct*)arrpayload[i])->id = curID;
      ((dummyStruct*)arrpayload[i])->garbage = curGarbage;
      curID++;
      curGarbage++;
      i++;
   }

   i = 0;
   while(i < 93)
   {
      if(LH_hashFunc(testTable, arrpayload[i],myhashfunc,dummyCpy,dummyAlloc))
         printf("Successful hash\n");
      else
         printf("ERROR hashing\n");
      i++;
   }

   //LH_HashTablePrint(testTable, printDummy);
 
//int LH_HashTableDel(LH_hashTable*);					
}




