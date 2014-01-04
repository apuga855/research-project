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

typedef void*(*LHN_HashNodeDataAlloc)();			X
typedef void*(*LHN_HashNodeDataCpy)(void*, void*);		X
typedef void*(*LHN_HashNodePrintData)(void*);			X
typedef long double (*LHN_HashFunction)(void*);			X

LH_hashTable* LH_HashTableAlloc();				X
LH_hashTable* LH_HashTableAllocBuff(void*);			X

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
      do
      {
         slot = (slot + (i * rslot)) % (table->LH_capacity);   
         i++;          
      }while(LlistIsEmpty(table->LH_table[slot].LHN_list));
   }
   return slot;
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
   
   printf("\n\n\n\n\n");
   printf("\nTesting LH_HashTableAllocSetBuff()\n");
   printf("\nPlease enter the size\n");
   long unsigned int szb;
   scanf("%lu", &szb);
   hashtable = LH_HashTableAllocSetBuff(szb,mydataalloc);
   if(hashtable != NULL)
      printf("\nSUCCESS AT LH_HASHTABLEALLOCSETBUFF()\n");
   else
      printf("\nFAILURE AT LH_HASHTABLEALLOCSETBUFF()\n");
   
   printf("\nTesting LH_HashTableDel after the set buff alloc\n");
   if(LH_HashTableDel(hashtable))
      printf("\nSUCCESS AT LH_HASHTABLEDEL()\n");
   else
      printf("\nFAILURE AT LH_HASHTABLEDEL()\n"); 
   
   person *e = malloc(sizeof(person));
   e->age = 11;
   e->id = 451;
   strncpy(e->name,"steve",20);
   strncpy(e->lname,"kk",20);
   
   
   person *f = malloc(sizeof(person));
   f->age = 42;
   f->id = 626;
   strncpy(f->name,"cirno",20);
   strncpy(f->lname,"tohou",20);
   
   person *g = malloc(sizeof(person));
   g->age = 31;
   g->id = 124;
   strncpy(g->name,"harry",20);
   strncpy(g->lname,"potter",20);
   
   person *h = malloc(sizeof(person));
   h->age = 10;
   h->id = 2;
   strncpy(h->name,"yes",20);
   strncpy(h->lname,"no",20);
   
   person *i = malloc(sizeof(person));
   i->age = 50;
   i->id = 333;
   strncpy(i->name,"jackie",20);
   strncpy(i->lname,"chan",20);
   
   person *j = malloc(sizeof(person));
   j->age = 21;
   j->id = 511;
   strncpy(j->name,"john",20);
   strncpy(j->lname,"van dam",20);
   
   person *k = malloc(sizeof(person));
   k->age = 30;
   k->id = 420;
   strncpy(k->name,"snoop",20);
   strncpy(k->lname,"dogg",20);
   
   printf("\nTesting rehash\n");
   scanf("%lu", &szb);
   szb = 10;
   hashtable = LH_HashTableAllocSetBuff(szb, mydataalloc);
   LH_hashFunc(hashtable, a, myhashfunc, mycopy,mydataalloc); 
   LH_hashFunc(hashtable, b, myhashfunc, mycopy,mydataalloc); 
   LH_hashFunc(hashtable, c, myhashfunc, mycopy,mydataalloc); 
   LH_hashFunc(hashtable, d, myhashfunc, mycopy,mydataalloc); 
   LH_hashFunc(hashtable, e, myhashfunc, mycopy,mydataalloc); 
   LH_hashFunc(hashtable, f, myhashfunc, mycopy,mydataalloc); 
   LH_hashFunc(hashtable, g, myhashfunc, mycopy,mydataalloc); 
   LH_hashFunc(hashtable, h, myhashfunc, mycopy,mydataalloc); 
   LH_hashFunc(hashtable, i, myhashfunc, mycopy,mydataalloc); 
   LH_hashFunc(hashtable, j, myhashfunc, mycopy,mydataalloc);
   LH_hashFunc(hashtable, k, myhashfunc, mycopy,mydataalloc);
   
   LHN_hashNode* testarrcp = LHN_HashNodeDataAlloc(10,mydataalloc);
   LHN_hashNode* testingnode = LHN_HashNodeDataAlloc(1,mydataalloc);
   LHN_HashNodeIns(testarrcp, (void*)a);
   LHN_HashNodeIns(testarrcp, (void*)b);
   LHN_HashNodeIns(testarrcp, (void*)c);
   LHN_HashNodeIns(testarrcp, (void*)d);
   LHN_HashNodeIns(testarrcp, (void*)e);
   LHN_HashNodeIns(testarrcp, (void*)f);
   LHN_HashNodeIns(testarrcp, (void*)g);
   LHN_HashNodeIns(testarrcp, (void*)h);
   LHN_HashNodeIns(testarrcp, (void*)i);
   LHN_HashNodeIns(testarrcp, (void*)j);
   LHN_HashNodeIns(testarrcp, (void*)k);

   a->age = 22;
   a->id = 11;
   b->age = 33;
   b->id = 22;
   c->age = 44;
   c->id = 33;
   
   printf("\nprinting rehash\n");
   LH_HashTablePrint(hashtable,printPerson);
   
   printf("\nPrinting the source\n");
   LHN_HashNodePrint(testarrcp,printPerson); 
   printf("\nPrinting the testing node\n");
   LHN_HashNodePrint(testingnode,printPerson); 
   
   LHN_HashNodeCpy(testarrcp, testingnode, mycopy,mydataalloc); 

   printf("\nPrinting the source AFTER THE COPY\n");
   LHN_HashNodePrint(testarrcp,printPerson); 
   printf("\nPrinting the testing node AFTER THE COPY\n");
   LHN_HashNodePrint(testingnode,printPerson); 
   

   return 0;
}
