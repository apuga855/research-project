#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"linked_list.h"

/*
Purpose: Unit testing for linked list, testing all functionalities under most conditions

List of functions:							CHECK	

1 LlistNode * LlistNodeAlloc(void*);					X
2 LlistNode * LlistNodeSelfAlloc(void*);
3 void LlistNodeInit (LlistNode *);
4 int LlistNodeDel(LlistNode *);					X
5 int LlistNodeCpy(LlistNode *, LlistNode *, void *);
6 int LlistNodeFail(LlistNode*);
7 void LlistNodePrint(LlistNode*, void*);				X
8 typedef void(*LlistNodePrintData)(void*);
9 typedef void(*LlistNodeDataCpy)(void*,void*);
10  typedef void*(*LlistDataAlloc)();
11 Llist * LlistAlloc(int, void*);
12 int LlistPopulate(Llist *, void**, int);
13 int LlistInit(Llist *);
14 int LlistDel(Llist *);
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
28 void LlistPrint(Llist *, void*);
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
      printf("The struct was empty\n");
      return;
   }
   printf("Printing dummy struct:\n id = %d, garbage = %c\n", src->id, src->garbage);
}




int main()
{
   printf("ListNode section ***************************************\n");
   int curID = 0;
   char curGarbage = '!';
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
//1 LlistNode * LlistNodeAlloc(void*);
//2 LlistNode * LlistNodeSelfAlloc(void*);
//3 void LlistNodeInit (LlistNode *);

    
}
