#pragma once
#include<stdio.h>
#include"linked_list.h"
#include"primeArr.h"
#define HASH_LENGTH 7//607
#define BUFF_SIZE 20
//Node structure, will hold our sensor ready packets
typedef struct _LHN_hashNode
{
   Llist* LHN_list;
}LHN_hashNode;

//hashtable, made of a one dimensional array of nodes, the 
//sensor ready packet array will be pointed to by the 
//stuff pointer
typedef struct _LH_hashTable
{
   kprime * LH_primenums;
   unsigned long int LH_counter;
   unsigned long int LH_capacity;
   double LH_loadFactor;
   LHN_hashNode * LH_table;
}LH_hashTable;

//allocating function
LHN_hashNode * LHN_HashNodeAlloc();
//allocating a node using the integer passed as size and the void*
//contains the function to allocate the used defined structure
LHN_hashNode * LHN_HashNodeDataAlloc(int, void*);
//self allocating node, pass an array of structures that you want to place
//inside of the node, the function first allocates a list of size int passed
//to the function using the voi* function, then populates using the same
//passed int and the void** passed as an array
LHN_hashNode * LHN_HashNodeSelfAlloc(void*, void**, int);
//1 if failes 0 otherwise
int HashNodeFail(LHN_hashNode*);
//deletes a node from the hash
int LHN_HashNodeDel(LHN_hashNode*);
//inserts data into the hash
int LHN_HashNodeIns(LHN_hashNode*,void*);
//copies the linked list from one node to another node
int LHN_HashNodeCpy(LHN_hashNode*, LHN_hashNode*, void*, void*);
//prints the list inside the hashnode
void LHN_HashNodePrint(LHN_hashNode*,void*);
//initializes a hashnode to its respective 0 value
void LHN_HashNodeInit(LHN_hashNode*);
//allocates a hashtable
LH_hashTable* LH_HashTableAlloc();
//allocates a hashtable using the dataalloc function passed
LH_hashTable* LH_HashTableAllocBuff(void*);
//allocates a hashtable using the dataalloc function passed with set capacity
LH_hashTable* LH_HashTableAllocSetBuff(int, void*);
//initialized the hash table to its respective 0 value
void LH_HashTableInit(LH_hashTable*);
//deletes the the entire hash table
int LH_HashTableDel(LH_hashTable*);
//rehash function for when we run out of space
int LH_HashTableRehash(LH_hashTable**, void*, void*,void*);
long int LH_hash(LHN_hashNode*,LH_hashTable*);
void LH_HashSet(LH_hashTable*, void*, void*, void*);
int LH_hashFunc(LH_hashTable*, void *, void *,void *,void *);
void LH_HashTablePrint(LH_hashTable*, void*);
//generates an index for the hashtable, takes care of collisions
typedef int(*LH_keyGenerate)(LH_hashTable*, void*);
