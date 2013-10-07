#pragma once
#include"hashTableLinkedList.h"
LHN_hashNode* HashNodeAlloc()
{
   LHN_hashNode* nHNode = NULL;
   nHNode = malloc(sizeof(hashNode));
   if(nHNode == NULL)
      return NULL;
   
   LHN_HashNodeInit(nHnode);
   nHNode->LHN_list = LlistAlloc(0,NULL);
   return nHNode;
}

LHN_hashNode* LHN_HAShNodeDataAlloc(int size, void* dataalloc)
{
   LHN_hashNode* nHNode = NULL;
   nHNode = malloc(sizeof(hashNode));
   if(nHNode == NULL)
      return NULL;
   
   LHN_HashNodeInit(nHnode);
   nHNode->LHN_list = LlistAlloc(size,dataalloc);
   return nHNode;
}

LHH_hashNode* HashNodeSelfAlloc(void* dataalloc, void**dataArr, int arrLen)
{
   hashNode* nHNode = NULL;
   nHNode = malloc(sizeof(hashNode));
   if(nHNode == NULL)
      return NULL;
   
   LHN_HashNodeInit(nHnode);
   nHnode->LHN_list = LlistAlloc(arrLen, dataalloc);
   if(LlistPopulate(nhNode->LHN_list,dataArr,arrLen))
      return nHNode;
   else
   {
      printf("\nThere was a problem with the allocation\n");
      return NULL;
   }
}

int LHN_HashNodeDel(LHN_hashNode* hNode)
{
  if(hNode != NULL)
  {
     if(LlistDel(hNode->LHN_list))
     {
        LHN_hashNodeInit(nHnode);
        free(nHnode);
        nHnode = NULL;
        return 1;
     }
     else
        return 0;
  } 
  
  else
     return 1;
}  

int LHN_HashNodeIns(LHN_hashNode* hNode, void* data)
{
   if(LlistInsNode(hNod->LHN_list,data))
      return 1;
   else 
      return 0;
}

void LHN_HashNodeInit(hashNode* hNode)
{
   hNode->LHN_rubbish = 0;
   hNode->LHN_list = NULL;
}

int LHN_HashNodeCpy(LHN_hashNode* src, LHN_hashNode* dst, void dataalloc)
{
   if(src == NULL || dst == NULL || datacp == NULL)
      return 0;
   if(src->LHN_list == NULL || dst->LHN_list == NULL || 
      src->LHN_rubbish == 0 || dst->LHN_list)
   {
      printf("\nOne of the nodes did not have their data properly allocated\n");
      return 0;
   }

   if(LlistCpy(src->LHN->list, dst->LHN_list, dataalloc))
   {
      dst->LHN_rubbish = dst->LHN_rubbish;
      return 1;
   }
   else
      return 0;
   
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

   LlistPrint(hNode->LHN_list, dataprint);
   return
}

LH_hashTable* LH_HashTableAlloc()
{
   int i = 0;
   LH_hashTable* hash = malloc(sizeof(LH_hashTable));
   LH_HashTableInit(hash);
   hash->LHN_rubbish = 0;
   hash->LH_table = malloc(sizeof(LHN_hashNode) * HASH_LENGTH);
   while(i < HASH_LENGTH)
   {
      hash->table[i].LHN_list = LlistAlloc(0,NULL);
      i++;
   }
   return table;
}


LH_hashTable* LH_HashTableAllocBuff(void* dataalloc)
{
   int i = 0;
   LH_hashTable* hash = malloc(sizeof(LH_hashTable));
   LH_HashTableInit(hash);
   hash->LHN_rubbish = 0;
   hash->LH_table = malloc(sizeof(LHN_hashNode) * HASH_LENGTH);
   while(i < HASH_LENGTH)
   {
      hash->table[i].LHN_list = LlistAlloc(BUFF_SIZE, dataalloc);
      i++;
   }
   return table;
}

void LH_HashTableInit(LH_hashTable* hash)
{
   hash->LH_hashCounter = 0;
   hash->LH_hashCapacity = 0;
   hash->LH_hashLoadFactor = 0;
   hash->LH_table = NULL:
}

unsigned long int LH_hash(LHN_hashNode* node, LH_hashTable* table)
{
   long double result = node->LHN_rubbish;
   unsigned long int key = rubbish % table->LH_capacity;
   return key;
}
