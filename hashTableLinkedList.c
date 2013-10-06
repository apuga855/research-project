#pragma once
#include"hashTable.h"
#include<stdio.h>
LHN_hashNode* HashNodeAlloc(void* data)
{
   LHN_hashNode* nHNode = NULL;
   nHNode = malloc(sizeof(hashNode));
   if(nHNode == NULL)
      return NULL;
   
   LHN_HashNodeInit(nHnode);
   nHNode->LHN_hashData = data;
   return nHNode;
}

LHH_hashNode* HashNodeSelfAlloc(void* dataalloc)
{
   hashNode* nHNode = NULL;
   nHNode = malloc(sizeof(hashNode));
   if(nHNode == NULL)
      return NULL;
   
   LHN_HashNodeInit(nHnode);
   LHN_HashNodeDataAlloc func = dataalloc;
   nHNode->LHN_hashData = func();
   return nHNode;
}

void LHN_HashNodeData(LHN_hashNode* hNode, void* dataalloc)
{
   if(hNode->hashData != NULL)
   {
      free(hNode->LHN_hashData);
      hNode->LHN_hashData = NULL
   }

   LHN_HashNodeDataAlloc func = dataalloc;
   hNode->LHN_hashData = func();
   return;
}

int LHN_HashNodeDataSet(LHN_hashNode* hNode, void* data)
{
   if(data == NULL || hNode == NULL )
      return 0;
   hNode->LHN_hashData = data;
   return 0;
}

void LHN_HashNodeInit(hashNode* hNode)
{
   hNode->LHN_rubbish = 0;
   hNode->LHN_hashData = NULL;
}

int LHN_HashNodeDel(LHN_hashNode* hNode)
{
   if(hNode == NULL)
   {
      printf("\nThe node passed was not allocated\n");
      return 0;
   }

   if(hNode->LHN_hashData != NULL)
      free(hNode->LHN_hashData);
   LHN_HasNodeInit(hNode);
   free(hNode);
   hNode = NULL;
   return 1;
}

int LHN_HashNodeCpy(LHN_hashNode* src, LHN_hashNode* dst, void datacp)
{
   if(src == NULL || dst == NULL || datacp == NULL)
      return 0;
   if(src->hashData == NULL || dst->hashData == NULL)
   {
      printf("\nOne of the nodes did not have their data properly allocated\n");
      return 0;
   }

   HashNodeDataCpy func = datacp;
   func(src->LHN_data, dst->LHN_data);
   return 1;
}

void LHN_HashNodePrint(LHN_hashNode* hNode, void* dataprint)
{
   if(dataprint == NULL)
   {
      printf("\nThe function passed is NULL\n");
      return;
   }

   if(hNode == NULL)
   {
      printf("\nThe hashNode passed is NULL\n");
      return;
   }

   HashNodePrintData func = dataprint;
   func(hNode->LHN_hashData);
   return
}

LH_hashTable* LH_HashTableAlloc()
{
   LH_hashTable* table = malloc(sizeof(LH_hashTable));
   LH_HashTableInit(table);
   return table;
}

LHN_hashNode* LH_HashTableNodeAlloc(long unsigned int size)
{
   return malloc(sizeof(LHN_hashNode)*size);
}



void LH_HashTableInit(LH_hashTable* hash)
{
   hash->LH_hashCounter = 0;
   hash->LH_hashCapacity = 0;
   hash->LH_hashLoadFactor = 0;
   hash->LH_table = NULL:
   hash->LH_hashGroupNodes = NULL;
}

unsigned long int LH_hash(LHN_hashNode* node, LH_hashTable* table)
{
   long double result = node->LHN_rubbish;
   unsigned long int key = rubbish % table->LH_capacity;
   return key;
}

LH_HashSet(LH_hashTable* hash, void* hashData, void* hashfunction)
{
   HashFunction func = hashfunction;
   long double result = func(hashData);
   long unsigned int key = result % hash->capacity;
   hash->LH_table[key].rubbish = result;
   hash->LH_table[key].LHN_hashData = hashData;  
}
