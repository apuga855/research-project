#pragma once
#define HASH_LENGTH 600
#define HASH_NODE_GROUPS 30
//Node structure, will hold our sensor ready packets
typedef struct _LHN_hashNode
{
   long double LHN_rubbish;
   void * LHN_hashData;
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
   unsigned long int * LH_groupNodes; 

}LH_hashTable;

LHN_hashNode * LHN_HashNodeAlloc(void);
LHN_hashNode * LHN_HashNodeSelfAlloc(void);
void LHN_HashNodeData(hashNode*,void*);
int LHN_HashNodeDataSet(hashNode*,void*);
int LHN_HashTableDel(hashNode*);
int LHN_HashNodeCpy(hashNode*, hashNode*, void*);
void LHN_HashNodePrint(hashNode*,void*);
typedef void*(*LHN_HashNodeDataAlloc)();
typedef void*(*LHN_HashNodeDataCpy)(void*, void*);
typedef void*(*LHN_HashNodePrintData)(void*);
typedef long double (*LHN_HashFunction)(void*);

LH_hashTable * LH_HashTableRehash(LH_hashTable *);
LH_HashTableInit(LH_hashTable*);
unsigned long int LH_hash(LHN_hashNode*);
LH_hHashSet(LH_hashTable*, void*, void*);
LH_hashTable* LH_HashTableAlloc();
