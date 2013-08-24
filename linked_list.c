//author: Alfonso Puga
//purpose:
//implementing a linked list with unknown datatype structure
//the list is dynamic, has a dummy head node, inserts before
//the head each time by default, contains callback functions
//to satisfy whatever the user needs

//node for the list, int root is 0 unless youa re at the head
//contains a previous and next pointer for linked list behavior
//the void* stores whatever data the user needs

//allocates a new node with the passed void
//poinder placed in the data pointer of the
//newly allocated node, finally it returns 
//the node
#pragma once
#include"linked_list.h"
LlistNode * LlistNodeAlloc(void* data)
{
   LlistNode * nNode = NULL;
   nNode = malloc(sizeof(LlistNode));
   if(nNode == NULL)
      return NULL;
   LlistNodeInit(nNode);
   nNode->data = data;
   return nNode;
}

//initializes nodes to their respective 0 value
//default behavior is to initialize the node to not
//be a root, aka not the head
void LlistNodeInit (LlistNode *node)
{
    
   node->root = 0;
   node->next = NULL;
   node->prev = NULL;
   node->data = NULL;
   return;
}

//deletes the node safely
void LlistNodeDel(LlistNode *node)
{
   free(node->data);
   LlistNodeInit(node);
   free(node);
   return;
}

//copies src into dst using datacap callback function
//the callback function passes two void pointers,
//the src data is copied where dst pointer used to point to
//if the src or dst are NULL we fail, if the source is not
//part of a list we fail
int LlistNodeCpy(LlistNode *src, LlistNode *dst, void *datacp)
{
   if(src == NULL || dst == NULL)
      return 0;
   if(src->next == NULL || src->prev == NULL)
      return 0;

   dst->next = src->next;
   dst->prev = src->prev;
   LlistNodeDataCpy func = datacp;
   func(src->data,dst->data);
   return 1; 
}

//allocates a size amount of nodes, minimum number
//has to be 1 for the head node
Llist * LlistAlloc(int size)
{
   if(size <= 0)
      return NULL;
   int i = 1;
   Llist* nList = malloc(sizeof(Llist));
   LlistInit(nList);
   while(i < size)
   {
      if(LlistInsNode(nList, NULL))
         i++;
      else
         return NULL;
   }
   
   nList->length = size;
   return nList;
}

//populates list sequentially by placing members
//of the void array in the list, the entire
//data array shoudl have used malloc to create
//the data
int LlistPopulate(Llist *list, void** dataArr)
{
   if(LlistFail(list))
      return 0;
   int remaining = list->length -2;
   int i = 0;
   while(i < remaining)
   {
      LlistInsNode(list, dataArr[i]);
      i++;
   }
   
   return 1; 
}

//initializes the list, we check for a head node, if this
//was not allocated we fail, otherwise we initialize the head
//node, flag and finally the length
int LlistInit(Llist *list)
{
   if(list->head == NULL)
      return 0;
   LlistNodeInit(list->head);
   list->head->next = list->head;
   list->head->prev = list->head;
   list->head->root = 1;
   list->length = 1;
   return 1; 
}

//deleting entire list, first checks for faillist, if it passess
//we delete sequentially starting with deleting head's next until
//we reach the head again, where we delete the head, delete the
//list and exit successfully
int LlistDel(Llist *list)
{
   if(LlistFail(list))
      return 0;
   LlistNode * start = list->head;
   LlistNode * current = list->head->next;
   while(start != current)
   {
      start->next = current->next;
      LlistNodeDel(current);
      current = start->next;
   }
   
   LlistNodeDel(start);
   list->head = NULL;
   free(list);
   list = NULL;
   return 1;
}

//inserting node into list by handing a void pointer of data
//if we LlistFail is trye then we exit because the list was malformend
//the payload will automatically be added to a node which is now 
//the last node before the head node
int LlistInsNode(Llist *list, void * nData)
{
   if(LlistFail(list)) 
      return 0;
   LlistNode * nNode = NULL;
   LlistNode * prev = NULL;
   nNode = malloc(sizeof(LlistNode));
   if(nNode == NULL)
      return 0;
   LlistNodeInit(nNode);
   nNode->data = nData;
   prev = list->head->prev;
   list->head->prev = nNode;
   nNode->next = list->head;
   nNode->prev = prev;
   prev->next = nNode;
   list->length++;
   return 1;
}

//deleting node from list, first we check if LlistFail,
//if so we exit failure, if the node that we are trying
//to delete fails the node test, then we exit failure
//otherwise the node is deleted and the chain is fixed
//accordingly
int LlistDelNode(LlistNode *node,Llist *list)
{
   if(LlistFail(list))
      return 0;
   if(LlistNodeFail(node)) 
      return 0;
   node->prev->next = node->next;
   node->next->prev = node->prev;
   LlistNodeDel(node);
   list->length--;
   return 1; 
  
}

//making two lists have same size, size use is src
//diff is used to properly add or remove nodes,
//it is assumed dst's data members are no longer
//relevant so they are deleted without question
//if either fail the list test then we exit failure
int LlistCpySize(Llist *src, Llist *dst)
{
   if(LlistFail(src)|| LlistFail(dst))   
      return 0;
   
   int diff = src->length - dst->length;
   if(diff < 0)
   {
      while(dst->length > diff)
      {
         if(LlistDelNode(dst->head->prev, dst))
            diff++;
         else
            return 0;
      }
   }

   else if(diff > 0)
   {
      while(dst->length < diff)
      {
         if(LlistInsNode(src,NULL))
            diff--;
         
         else
            return 0;
      }   
   }
   
   return 1;
}

//copies entire list, first we resize so we can copy
//correctly, then we loop one by one and copy each
//node onto the dst list, if list fail for either then
//we exit failure
int LlistCpy(Llist *src, Llist *dst, void *datacp)
{
   if(LlistFail(src)|| LlistFail(dst))   
      return 0;
   
   if(LlistCpySize(src,dst))
   {
      LlistNode *srcC = NULL;
      LlistNode *dstC = NULL;
      LlistNode *next = NULL;
      srcC = src->head;
      dstC = dst->head;
      do
      {
         next = dstC->next;
         LlistNodeCpy(srcC, dstC, datacp);
         srcC = srcC->next;
         dstC = next;
      } while(srcC != src->head);
   
      return 1;
   }

   return 0;
}

//if the node is null, if the node is a root
//or if the node does not have a next or prev
//pointer we fail the test so we return true we 
//failed the test otherwise return false
int LlistNodeFail(LlistNode* node)
{
   if(node == NULL)
      return 1;
   else if(node->root = 1)
      return 1;
   else if(node->prev == NULL || node->next == NULL)
      return 1;
   else
      return 0;

}

//if our list is null, if the head is null, if the
//head does not have the root flag set or if 
//either the prev or next pointers are NULL for the head
//then failed so we return true, otherwise return falise
int LlistFail(Llist *list)
{
   if(list == NULL)
      return 1;
   else if(list->head == NULL)
      return 1;
   else if(list->length == 0)
      return 1;
   else if(list->head->root != 1)
      return 1;
   else if(list->head->next == NULL ||
           list->head->prev == NULL)
      return 1;
   else
      return 0;
}

