#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"linked_list.h"

/*
Purpose: Unit testing for linked list, testing all functionalities under most conditions

List of functions:							CHECK	

LlistNode:
1 LlistNode * LlistNodeAlloc(void*);					X
2 LlistNode * LlistNodeSelfAlloc(void*);				X
3 void LlistNodeInit (LlistNode *);					X
4 int LlistNodeDel(LlistNode *);					X
5 int LlistNodeCpy(LlistNode *, LlistNode *, void *);			X
6 int LlistNodeFail(LlistNode*);					X
7 void LlistNodePrint(LlistNode*, void*);				X
8 typedef void(*LlistNodePrintData)(void*);				X
9 typedef void(*LlistNodeDataCpy)(void*,void*);				X
10  typedef void*(*LlistDataAlloc)();					X

Llist:
11 Llist * LlistAlloc(int, void*);					X
12 int LlistPopulate(Llist *, void**, int);				X
13 int LlistInit(Llist *);						X
14 int LlistDel(Llist *);						X
15 int LlistInsNode(Llist *, void *);
16 int LlistInsData(Llist*, void*);
17 int LlistDelNode(Llist*, void*,void*,void*, int);
18 int LlistDelNodeTargetN(LlistNode *,Llist*);
19 int LlistDelNodeTarget(Llist*,int);
20 void* LlistDelNodeTargetQ(LlistNode ,Llist *);
21 int LlistDelDAta(Llist*, void*);
22 int LlistCpySize(Llist *, Llist *, void*);
23 int LlistCpy(Llist *, Llist *, void *,void*);
24 int LlistFail(Llist *);
25 LlistNode * LlistSearchNode(Llist*,void*, void*, int);
26 int LlistIsEmpty(Llist*);
27 void * LlistRetFirst(Llist*);
28 void LlistPrint(Llist *, void*);					X
29 typedef void(*LlistSortIns)(Llist*);
30 typedef void(*LlistSort)(Llist*);
31 typedef LlistNode*(*LlistSearch)(Llist*, void*);
32 typedef int(*LlistUsrDataDel)(void*);
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
   printf("\nPrinting dummy struct:\n id = %d, garbage = %c\n\n", src->id, src->garbage);
}




int main()
{
   printf("**************************ListNode section ***************************************\n");
   int curID = 0;
   char curGarbage = '!';
   int i = 0;
   void * payloadArr[10];
   dummyStruct * payload = malloc(sizeof(dummyStruct));
   payload->id = curID;
   payload->garbage = curGarbage;
   LlistNode * test = NULL; 
   printf("\nLlistNodeAlloc test ***************************************\n");
   test = LlistNodeAlloc((void*) payload);
   if(test == NULL)
      printf("Allocation had an error------------------------------\n");
   else
   {
      printf("Allocation successful+++++++++++++++++++++++++++++++++\n");
      printf("\nLlistNodePrint test ***************************************\n");
      LlistNodePrint(test, printDummy);
      printf("If shown properly, both functions successful+++++++++++++++++++++++++++++++++\n");      
   }
   
   printf("\nLlistNodeDel test ***************************************\n");
   if(LlistNodeDel(test))
   {
      test = NULL;
      printf("Successful deletion+++++++++++++++++++++++++++++++++\n");
   }
   else
      printf("Deletion had an error------------------------------\n");
   
   printf("\nLlistNodeSelfAlloc test ***************************************\n");
   test = LlistNodeSelfAlloc(dummyAlloc);  
   if(test == NULL)  
      printf("LlistNodeSelfAlloc had an error------------------------------\n");
   else
      printf("Successful SelfAlloc+++++++++++++++++++++++++++++++++\n");
   
   printf("\nLlistNodeInit test ***************************************\n");
   LlistNodeInit(test);
   LlistNodePrint(test, printDummy); 
   printf("Successful LlistNodeInit if the node was empty+++++++++++++++++++++++++++++++++\n");
   printf("\nLlistNodeCpy test ***************************************\n");
   payload = malloc(sizeof(dummyStruct));
   payload->id = curID;
   payload->garbage = curGarbage;
   test = LlistNodeAlloc((void*) payload);
   LlistNode* cpNode = LlistNodeSelfAlloc(dummyAlloc);
   if(LlistNodeCpy(test, cpNode, dummyCpy))
   {
      printf("Successful LlistNodeCpy if the nodes are the same+++++++++++++++++++++++++++++++++\n");
      printf("Original Node:\n");
      LlistNodePrint(test,printDummy);
      printf("Copied Node:\n");
      LlistNodePrint(cpNode,printDummy);
   } 

   else
      printf("LlistNodeCpy had an error------------------------------\n");
    
   LlistNodeDel(test);
   LlistNodeDel(cpNode);
   test = NULL;
   cpNode = NULL;
   
   
   printf("********************ListList section ***************************************\n");
   Llist* testList = NULL;
   printf("\nLlistAlloc test ***************************************\n");
   testList = LlistAlloc(10,dummyAlloc);
   if(testList != NULL)
   {
      printf("LlistAlloc successful+++++++++++++++++++++++++++++++++\n");
      printf("Populating the list\n");
      i = 0;
      curID = 0;
      curGarbage = '!';
      while(i < 10)
      {
         payloadArr[i] = malloc(sizeof(dummyStruct));
         ((dummyStruct*)payloadArr[i])->id = curID;
         ((dummyStruct*)payloadArr[i])->garbage = curGarbage;
         curID++;
         curGarbage++;
         i++;
      } 

      printf("\nLlistAllocPopulate test ***************************************\n");
      if(LlistPopulate(testList, payloadArr, 10))
      {
         printf("LlistAlloc successful+++++++++++++++++++++++++++++++++\n");
         printf("\nLlistAllocPrint test ***************************************\n");
         LlistPrint(testList, printDummy);
         printf("If Seen properly, populate worked+++++++++++++++++++++++++++++\n");
         printf("Otherwise populate had an error-------------------------------\n");
         
         
      }
      
      else 
         printf("LlistNodeCpy had an error------------------------------\n");
   }
   else
      printf("LlistNodeCpy had an error------------------------------\n");
   
   printf("\nLlistDel test ***************************************\n");
   if(LlistDel(testList))
   {
      printf("ListDel successful ++++++++++++++++++++++++++++++++++\n");
      testList = NULL;
   }
   else
      printf("LlistDel failed at something -----------------------------------\n");
   
       
   i = 0;
   curID = 0;
   curGarbage = '!';
   
   while(i < 10)
   {
      payloadArr[i] = malloc(sizeof(dummyStruct));
      ((dummyStruct*)payloadArr[i])->id = curID;
      ((dummyStruct*)payloadArr[i])->garbage = curGarbage;
      curID++;
      curGarbage++;
      i++;
   } 
   
   testList = LlistAlloc(11,dummyAlloc);
   LlistPopulate(testList, payloadArr, 10);
   LlistPrint(testList, printDummy);
   printf("\nLlistInsNode test ***************************************\n");
   dummyStruct* ins = malloc(sizeof(dummyStruct));
   ins->id = curID;
   ins->garbage = curGarbage;
   if(LlistInsNode(testList, (void*)ins))
   {
      printf("ListInsNode successful ++++++++++++++++++++++++++++++++++\n");
      LlistPrint(testList, printDummy);
   }

   else
      printf("LlistInsNoed Error at inserting -----------------------------------\n");        
}
