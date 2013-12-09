#pragma once
#include"hashTableLinkedList.h"

//parameters: LHN_hashNode* "hnode"
//returns     1 if successful, 0 otherwise
//=====================================================
//checking if the node was allocated properly
int HashNodeFail(LHN_hashNode* hnode)
{
   if(hnode == NULL || hnode->LHN_list == NULL)
      return 1;
   if(LlistFail(hnode->LHN_list))
      return 1;
   return 0; 
}

//parameters: Nothing
//returns:    LHN_hashNode* if successful, NULL otherwise
//======================================================
//allocates a node with NULL data, we use Init to initialize
//all of the members and finally we use the LlistAlloc function
//for our linked list with a datatype of NULL and a size of 0,
//its basically just a skeleton for a node
LHN_hashNode* LHN_HashNodeAlloc()
{
   LHN_hashNode* nHNode = NULL;
   nHNode = malloc(sizeof(LHN_hashNode));
   if(nHNode == NULL)
      return NULL;
   
   LHN_HashNodeInit(nHNode);
   nHNode->LHN_list = LlistAlloc(0,NULL);
   return nHNode;
}

//parameters: int "size" with the amount of nodes for the linked list
//returns     LHN_hashNode* that got allocated with the ammount of nodes
//            and of payload type created by dataalloc
//====================================================================
//creates a hash node with a linked list of size "size" with each
//payload in the linked list being of the type allocated by the
//dataalloc function that was passed by the user
//we use the LlistAlloc function to get behavior
LHN_hashNode* LHN_HashNodeDataAlloc(int size, void* dataalloc)
{
   LHN_hashNode* nHNode = NULL;
   nHNode = malloc(sizeof(LHN_hashNode));
   if(nHNode == NULL)
      return NULL;
   
   LHN_HashNodeInit(nHNode);
   nHNode->LHN_list = LlistAlloc(size,dataalloc);
   return nHNode;
}

//parameters: void *  "dataalloc" function that allocates data
//            void ** "dataArr"   array of malloced data of user type
//            int     "arrLen"    how long is the array of data passed
//return:     LHN_hashNode allocated with a linked list with all members of 
//            the dataArr array as members
//            NULL otherwise 
//=========================================================================
//creates a LHN_hashNode that was allocated with dataArr members for its linked
//list, the members of the array are placed in the linked list from index 0 to
//the end of the array. 
LHN_hashNode* LHN_HashNodeSelfAlloc(void* dataalloc, void**dataArr, int arrLen)
{
   LHN_hashNode* nHNode = NULL;
   nHNode = malloc(sizeof(LHN_hashNode));
   if(nHNode == NULL)
      return NULL;
   
   LHN_HashNodeInit(nHNode);
   nHNode->LHN_list = LlistAlloc(arrLen, dataalloc);
   if(LlistPopulate(nHNode->LHN_list,dataArr,arrLen))
      return nHNode;
   else
   {
      printf("\nThere was a problem with the allocation\n");
      return NULL;
   }
}

//parameters: LHN_hashNode* "hNode" pointer to a node 
//return:     1 if successful, 0 otherwise
//===================================================
//calls the delete function on the list inside the node
//that function deletes the list and once successfuly
//deleted we delete the entire node. Note that all members
//of the list had dynamically allocated data segments for each
//list nodes
int LHN_HashNodeDel(LHN_hashNode* hNode)
{
  if(hNode != NULL)
  {
     if(LlistDel(hNode->LHN_list))
     {
        LHN_HashNodeInit(hNode);
        hNode = NULL;
        return 1;
     }
     else
     {
        printf("\nERROR AT HASHNODEDEL\n");
        return 0;
     }
  } 
  
  else
  {
     printf("\nERROR AT HASHNODEDEL\n");
     return 0;
  }
}  

//parameters: LHN_hashNode* "hNode" pointer to a node
//            void* data to the data to place in the new node
//returns:    1 if successful, 0 otherwise
//===========================================================
//calls the LlistInsNode function on the list inside of the passed
//node, the function needs the list and the void* to the dynamically
//allocated data
int LHN_HashNodeIns(LHN_hashNode* hNode, void* data)
{
   if(LlistInsNode(hNode->LHN_list,data))
      return 1;
   else 
      return 0;
}

//parameters: LHN_hashNode* "hNode" to a node
//returns:    Nothing
//===========================================
//initializes all values to its respective 0 value
void LHN_HashNodeInit(LHN_hashNode* hNode)
{
   hNode->LHN_list = NULL;
}

//parameters: LHN_hashNode* "src" to the source node
//	      LHN_hashNode* "dst" to the destination node
//            void* "datacp" to the function that copies the data
//	      void* "dataalloc" to the function that allocates data
//returns:    1 if successful, 0 otherwise
//=====================================================================================
//copies a node and its list to another node, has to use 2 user defined functions
//one that copies and one that allocates
int LHN_HashNodeCpy(LHN_hashNode* src, LHN_hashNode* dst, void* datacp, void* dataalloc)
{
   if(src == NULL || dst == NULL || dataalloc == NULL)
      return 0;
   if(src->LHN_list == NULL || dst->LHN_list == NULL)
   {
      printf("\nOne of the nodes did not have their data properly allocated\n");
      return 0;
   }

   if(LlistCpy(src->LHN_list, dst->LHN_list, datacp, dataalloc))
      return 1;
   else
      return 0;
   
}

//parameters: LHN_hashNode* "hNode" pointer to a node
//            void* "dataprint" to user print function
//returns:    Nothing
//=========================================================
//prints using the LlistPrint function
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
   return;
}

//parameters: Nothing
//returns:    LH_hashTable * of a dafault allocated LH_hashTable
//===========================================================
//allocates a default LH_HashTable with HASH_LENGTH amount of nodes
//with each having a list with BUFF_SIZE nodes allocated with 
//a NULL pointer for data on each one
LH_hashTable* LH_HashTableAlloc()
{
   int i = 0;
   LH_hashTable* hash = malloc(sizeof(LH_hashTable));
   LH_HashTableInit(hash);
   hash->LH_primenums = kprimeAlloc();
   kprimeInit(hash->LH_primenums);
   kprimeFindCap(HASH_LENGTH, hash->LH_primenums);
   hash->LH_capacity = kprimeCap(hash->LH_primenums);
   hash->LH_table = malloc(sizeof(LHN_hashNode) * hash->LH_capacity);
   while(i < hash->LH_capacity)
   {
      hash->LH_table[i].LHN_list = LlistAlloc(BUFF_SIZE,NULL);
      i++;
   }
   return hash;
}

//parameters: void* dataalloc
//returns:    LH_hashTable* of allocated data
//=================================================
//allocates a hashtable with a table with HASH_LENGTH amount
//of nodes, each node contains a list with BUFF_SIZE nodes and
//each node has a dynamically allocated data* created with the
//dataalloc function
LH_hashTable* LH_HashTableAllocBuff(void* dataalloc)
{
   int i = 0;
   LH_hashTable* hash = malloc(sizeof(LH_hashTable));
   LH_HashTableInit(hash);
   hash->LH_primenums = kprimeAlloc();
   kprimeInit(hash->LH_primenums);
   kprimeFindCap(HASH_LENGTH, hash->LH_primenums);
   hash->LH_capacity = kprimeCap(hash->LH_primenums);
   hash->LH_table = malloc(sizeof(LHN_hashNode) * hash->LH_capacity);
   while(i < hash->LH_capacity)
   {
      hash->LH_table[i].LHN_list = LlistAlloc(BUFF_SIZE, dataalloc);
      i++;
   }
   return hash;
}

//parameters: LH_hashTable* "hash" to hash structure
//returns:    Nothing
//==================================================
//initializes all members of our hash to the respective 0 value
void LH_HashTableInit(LH_hashTable* hash)
{
   hash->LH_primenums = NULL;
   hash->LH_counter = 0;
   hash->LH_capacity = 0;//600
   hash->LH_loadFactor = 0.8;
   hash->LH_table = NULL;
   return;
}


int LH_HashTableDel(LH_hashTable* tab)
{
  int i = 0;
  while(i < tab->LH_capacity)
  {
     printf("i = %d capacity = %lu\n",i,tab->LH_capacity);
     if(LHN_HashNodeDel(&tab->LH_table[i]))
     {
        i++;
        continue;
     }

     else
     {
        printf("\nError deleting a node\n");
        return 0;
     }
  }
  
  printf("\nFreeing the table\n");
  free(tab->LH_table);
  tab->LH_table = NULL;
  printf("\nFreeing the primenums struct\n");
  free(tab->LH_primenums);
  tab->LH_primenums = NULL;
  printf("\nFreeing the structure\n");
  free(tab);
  tab = NULL; 
  return 1;
}

//parameters: void* dataalloc
//            int "cap" with capacity
//returns:    LH_hashTable* of allocated data
//=================================================
//allocates a hashtable with a table with HASH_LENGTH amount
//of nodes, each node contains a list with BUFF_SIZE nodes and
//each node has a dynamically allocated data* created with the
//dataalloc function
LH_hashTable* LH_HashTableAllocSetBuff(int cap, void* dataalloc)
{
   int i = 0;
   LH_hashTable* hash = malloc(sizeof(LH_hashTable));
   LH_HashTableInit(hash);
   hash->LH_primenums = kprimeAlloc();
   kprimeInit(hash->LH_primenums);
   kprimeFindCap(cap, hash->LH_primenums);
   hash->LH_capacity = kprimeCap(hash->LH_primenums);
   hash->LH_table = malloc(sizeof(LHN_hashNode) * hash->LH_capacity);
   while(i < hash->LH_capacity)
   {
      hash->LH_table[i].LHN_list = LlistAlloc(BUFF_SIZE, dataalloc);
      i++;
   }
   return hash;
}


int LH_HashTableRehash(LH_hashTable* hash, void* datacp, void* dataalloc, void * keygen)
{
   double result = (double)(hash->LH_counter / hash->LH_capacity); 
   if(result > hash->LH_loadFactor)
   {
      int i = 0;
      LH_hashTable* nhash = LH_HashTableAllocSetBuff(hash->LH_capacity * 2, dataalloc);

      while(i < hash->LH_capacity)
      {
         if(LlistIsEmpty(hash->LH_table[i].LHN_list))
            continue;

         else       
         {
            LH_keyGenerate func = keygen;
            int key = func(hash, LlistRetFirst(hash->LH_table[i].LHN_list));
            if(LlistCpy(hash->LH_table[i].LHN_list, nhash->LH_table[key].LHN_list, datacp, dataalloc))
               printf("\nSuccessful mapping\n");
            else
            {
               printf("\nERROR UNSUCCESSFUL MAPPING\n");
               return 0;
            }
         }   
      }

      nhash->LH_counter = hash->LH_counter;
      
      return 1;
   }
   
   else
      return 0;
}

int LH_hashFunc(LH_hashTable* hash, void* data, void* keygen, void * datacp, void * dataalloc)
{
   double result = (double)((hash->LH_counter + 1)/ hash->LH_capacity);
   if(result > hash->LH_loadFactor)
   {
      printf("\nRehashing\n");
      if(LH_HashTableRehash(hash,datacp,dataalloc,keygen))
         printf("\nRehashing SUCCESSFUL\n");
      else
      {
         printf("\nERROR IN REHASHING UNSUCCESSFUL\n");
         return 0;
      }
   }
   
   LH_keyGenerate func = keygen;
   int key = func(hash,data);
   
   if(LlistInsData(hash->LH_table[key].LHN_list, data))
      return 1;
   else
       return 0;   
}
