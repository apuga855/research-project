#pragma once
#include<stdio.h>
#include<string.h>
#include"hashTableLinkedList.h"
#include<stdlib.h>
/*								SUCCESS			FAILURE
LHN_hashNode * LHN_HashNodeAlloc();				X			
LHN_hashNode * LHN_HashNodeDataAlloc(int, void*);		X
LHN_hashNode * LHN_HashNodeSelfAlloc(void*, void**, int);	X
int LHN_HashNodeDel(hashNode*,void*);				X
int LHN_HashNodeIns(hashNode*,void*);				X
int LHN_HashNodeCpy(hashNode*, hashNode*, void*);		X
void LHN_HashNodePrint(hashNode*,void*);			X

typedef void*(*LHN_HashNodeDataAlloc)();
typedef void*(*LHN_HashNodeDataCpy)(void*, void*);
typedef void*(*LHN_HashNodePrintData)(void*);
typedef long double (*LHN_HashFunction)(void*);

LH_hashTable* LH_HashTableAlloc();
LH_hashTable* LH_HashTableAllocBuff(void*);

LH_hashTable * LH_HashTableRehash(LH_hashTable *);
LH_HashTableInit(LH_hashTable*);
unsigned long int LH_hash(LHN_hashNode*);
LH_hHashSet(LH_hashTable*, void*, void*);



*/


typedef struct _person
{
   int age;
   int id;
   char name[20];
   char lname[20];
}person;

void printPerson(person* per)
{
   if(per == NULL) return;
   printf("\nId entered is: %d",per->id);
   printf("\nAge entered is: %d",per->age);
   printf("\nName entered is: %s",per->name);
   printf("\nLast name entered is: %s\n",per->lname);
   return;
}

void mycopy(void* src, void* dst)
{
   printf("\ninside of function mycopy\n");
   printf("\nsrc address is %p\n",src);
   printf("\ndst address is %p\n",dst);
   person* srcStc = (person*)src;
   person* dstStc = (person*)dst;
   printPerson(srcStc);
   dstStc->age = srcStc->age;
   dstStc->id = srcStc->id;
   strncpy(dstStc->name, srcStc->name,20);
   strncpy(dstStc->lname, srcStc->lname,20);
   printf("\nfinished function mycopy\n");
   printPerson(dstStc);
   return;
}

void* mydataalloc()
{
   person* data =  malloc(sizeof(person));
   return (void*) data;
}

void* MyPayloadCreate()
{
   person* payload = NULL;
   payload = malloc(sizeof(person));
   printf("Enter the name of who you are looking for\n");
   scanf("%s",payload->name);
   printf("Enter the last name of who you are looking for\n");
   scanf("%s",payload->lname);
   return (void*) payload;
}

LlistNode* MySearch(Llist* list, void* payload)
{
   person* currentPerson = NULL;
   person* target = (person*)payload;
   LlistNode* current = NULL;
   current = list->head->next;
   while(current != list->head)
   {
      currentPerson = (person*)current->data;
      if(strncmp(currentPerson->name,target->name,20)   &&
         strncmp(currentPerson->lname,target->lname,20) == 0)
        {
           int i = 0;
           printf("Found:\n");
           printPerson(currentPerson);
           printf("\nIs this the correct person or would you like to keep looking?\n"
                   "Press 0 to quit and 1 to keep looking\n");
           scanf("%d",&i);
           if(i == 1)
           {
              current = current->next;
	      continue;
	   }
           else
              return current;
        }
   }

   printf("No hits with the given payload\n\n");
   return NULL;
}



int main()
{
   LHN_hashNode* nhptr = NULL;
   LHN_hashNode* nhptrData = NULL;
   printf("Testing HashNodeAlloc()\n");
   nhptr = LHN_HashNodeAlloc();
   if(HashNodeFail(nhptr))
      printf("Failure at allocating the node\n");
   else
      printf("SUCCESS AT HASHNODEALLOC\n");
   
   
   printf("\n\n\n\n\n");


   printf("Testing HashNodeDataAlloc()\n");
   nhptrData = LHN_HashNodeDataAlloc(10,mydataalloc);
   if(HashNodeFail(nhptr))
      printf("Failure at allocating the node\n");
   else
      printf("SUCCESS AT HASHNODEDATAALLOC\n");
    
   printf("\n\n\n\n\n");
   
   printf("Testing HashNodeSelfAlloc()\n");
   person *a = malloc(sizeof(person));
   a->age = 22;
   a->id = 11;
   strncpy(a->name,"alfonso",20);
   strncpy(a->lname,"puga",20);
   
   person *b = malloc(sizeof(person));
   b->age = 33;
   b->id = 22;
   strncpy(b->name,"sean",20);
   strncpy(b->lname,"philipps",20);
   
   person *c = malloc(sizeof(person));
   c->age = 44;
   c->id = 33;
   strncpy(c->name,"lev",20);
   strncpy(c->lname,"williams",20);
   
   void* payloadArr[3];
   payloadArr[0] = (void*)a;
   payloadArr[1] = (void*)b;
   payloadArr[2] = (void*)c;
   
   nhptrData = LHN_HashNodeSelfAlloc(mydataalloc,payloadArr,3);
   if(HashNodeFail(nhptrData))
      printf("Failure at allocating the node\n");
   else
      printf("SUCCESS AT HASHNODESELFALLOC\n");

     
   printf("\n\n\n\n\n");
   printf("Testing hashnodedel\n");
   if(LHN_HashNodeDel(nhptr))
      printf("Deleted successfully\n");
   else
      printf("FAILURE\n");
   
   printf("\n\n\n\n\n");
   printf("testing LHN_HashNodeDel\n");
   if(LHN_HashNodeDel(nhptrData))
      printf("Deleted successfully\n");
   else
      printf("FAILURE\n");
 
   printf("\n\n\n\n\n");
   
   nhptrData = LHN_HashNodeSelfAlloc(mydataalloc,payloadArr,3);
   person* d = malloc(sizeof(person)); 
   d->age = 44;
   d->id = 33;
   strncpy(d->name,"derp",20);
   strncpy(d->lname,"herp",20);
   if(LHN_HashNodeIns(nhptrData, (void*)d))
      printf("Successfully added person d\n");
   else
      printf("FAILURE\n");

   printf("\n\n\n\n\n");
   
   printf("Trying HashNodeCpy\n");
   nhptr = LHN_HashNodeAlloc();
   if(LHN_HashNodeCpy(nhptrData,nhptr,mycopy,mydataalloc))
      printf("Successful copy\n");
   else
      printf("FAILURE\n");
   
    
   printf("\n\n\n\n\n");

   printf("Trying HashNodePrint after the copy\n");
   printf("Should print 2 of the same linked lists\n");
   printf("Printing original\n");
   LHN_HashNodePrint(nhptrData, printPerson);
   printf("\n\n\n\n\n");
   printf("Printing copy\n");
   LHN_HashNodePrint(nhptr, printPerson);
   
    
   printf("\n\n\n\n\n");
   printf("\nTesting LH_HashTableAlloc()\n");
   LH_hashTable* hashtable = LH_HashTableAlloc();
   if(hashtable != NULL)
      printf("\nSUCCESS AT LH_HASHTABLEALLOC()\n");
   else
      printf("\nFAILURE AT LH_HASHTABLEALLOC()\n");
    
   printf("\nTesting LH_HashTableDel\n");
   if(LH_HashTableDel(hashtable))
      printf("\nSUCCESS AT LH_HASHTABLEDEL()\n");
   else
      printf("\nFAILURE AT LH_HASHTABLEDEL()\n"); 
   
   

   return 0;
}