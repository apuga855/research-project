#pragma once
#include<stdio.h>
#include<linked_list.h>
#define HASH_LENGTH 600
#define BUFF_SIZE 10
//Node structure, will hold our sensor ready packets
typedef struct _LHN_hashNode
{
   long double LHN_rubbish;
   Llist* LHN_list;
}LHN_hashNode;

//hashtable, made of a one dimensional array of nodes, the 
//sensor ready packet array will be pointed to by the 
//stuff pointer
typedef struct _LH_hashTable
{
   unsigned long int LH_counter;
   unsigned long int LH_capacity;
   double LH_loadFactor;
   LHN_hashNode * LH_table;
}LH_hashTable;

LHN_hashNode * LHN_HashNodeAlloc();
LHN_hashNode * LHN_HashNodeDataAlloc(int, void*);
LHN_hashNode * LHN_HashNodeSelfAlloc(void*, void**, int);
int LHN_HashNodeDel(hashNode*,void*);
int LHN_HashNodeIns(hashNode*,void*);
int LHN_HashNodeCpy(hashNode*, hashNode*, void*);
void LHN_HashNodePrint(hashNode*,void*);

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
