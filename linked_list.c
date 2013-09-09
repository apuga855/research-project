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
#include<stdio.h>

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
   if(src == NULL || dst == NULL || datacp == NULL)
      return 0;

   dst->next = src->next;
   dst->prev = src->prev;
   LlistNodeDataCpy func = datacp;
   func(src->data,dst->data);
   return 1; 
}

//default printing function, they provide how to print the data
void LlistNodePrint(LlistNode* node, void* dataprint)
{  
   if(node->root == 1)
      printf("\n\nThe node is the head");
   if(node->root == 0)
      printf("\n\nThe node is not the head");
   printf("\nThe next pointer points to %p",node->next);
   printf("\nThe previous pointer points to %p",node->prev);
   if(node->root == 1)
      return;
   
   LlistNodePrintData func = dataprint;
   func(node->data);
   return;
}

//allocates a size amount of nodes, minimum number
//has to be 1 for the head node
Llist * LlistAlloc(int size)
{
   if(size <= 0)
   {
      printf("size was less than or equal to 0\n\n");
      return NULL;
   }
   int i = 0;
   Llist* nList = malloc(sizeof(Llist));
   nList->head = malloc(sizeof(LlistNode));
   if(LlistInit(nList))
      printf("List was correctly initialized\n");
   else
      printf("List failed to be correctly initialized\n");   

   while(i < size)
   {
      if(LlistInsNode(nList, NULL))
      {
         printf("List added a node correctly\n\n");
         i++;
      }
      else
      {
         printf("List FAILED to add a node correctly\n\n");
         return NULL;
      }
   }
   
   nList->length = size;
   return nList;
}

//populates list sequentially by placing members
//of the void array in the list, the entire
//data array shoudl have used malloc to create
//the data
int LlistPopulate(Llist *list, void** dataArr, int arrLen)
{
   if(LlistFail(list))
   {
      printf("list failed the test\n\n");
      return 0;
   }
   if(arrLen > list->length) 
   {
      printf("The input length was longer than the available nodes\n\n");
      return 0;
   }
 
   int i = 0;
   LlistNode* target = list->head->next;   
   while(i < arrLen)
   {
      target->data = dataArr[i];
      i++;
      target = target->next;
   }
   
   return 1; 
}

//initializes the list, we check for a head node, if this
//was not allocated we fail, otherwise we initialize the head
//node, flag and finally the length
int LlistInit(Llist *list)
{
   if(list->head == NULL)
   {
      printf("failed at list init, head is null\n\n");
      return 0;
   }
   LlistNodeInit(list->head);
   list->head->next = list->head;
   list->head->prev = list->head;
   list->head->root = 1;
   list->length = 0;
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
   {
      printf("the list failed test at insert node\n\n");
      return 0;
   }
   LlistNode * nNode = NULL;
   LlistNode * prev = NULL;
   nNode = malloc(sizeof(LlistNode));
   if(nNode == NULL)
   {
      printf("node allocation fail at insert node\n\n");
      return 0;
   }
   LlistNodeInit(nNode);
   nNode->data = nData;
   prev = list->head->next;
   list->head->next = nNode;
   nNode->next = prev;
   nNode->prev = list->head;
   prev->prev = nNode;
   //prev = list->head->prev;
   //list->head->prev = nNode;
   //nNode->next = list->head;
   //nNode->prev = prev;
   //prev->next = nNode;
   list->length++;
   return 1;
}

//deleting node from list, first we check if LlistFail,
//if so we exit failure, if the node that we are trying
//to delete fails the node test, then we exit failure
//otherwise the node is deleted and the chain is fixed
//accordingly
int LlistDelNodeTarget(LlistNode *node,Llist *list)
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
   
      return 0;
   }

   return 1;
}

//if the node is null, if the node is a root
//or if the node does not have a next or prev
//pointer we fail the test so we return true we 
//failed the test otherwise return false
int LlistNodeFail(LlistNode* node)
{
   if(node == NULL)
      return 1;
   else if(node->root == 1)
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
   {
      printf("list was null\n\n");
      return 1;
   }
   else if(list->head == NULL)
   {
      printf("list head was null\n\n");
      return 1;
   }
   else if(list->length < 0)
   {
      printf("list length was less than 0\n\n");
      return 1;
   }
   else if(list->head->root != 1)
   {
      printf("list head root was not 1\n\n");
      return 1;
   }
   else if(list->head->next == NULL ||
           list->head->prev == NULL)
   {
      printf("list head did not point to anything else\n\n");
      return 1;
   }
   else
   {
      printf("The List passed the test\n\n");
      return 0;
   }
}
//printing all members of the list
void LlistPrint(Llist* list, void* printNode)
{
   if(LlistFail(list))
   {
      printf("list failed the test at print\n");
      return;
   }   
   
   printf("\nUsing print function to print each node\n");
   
   int i = 0;
   LlistNode* traverse = NULL;
   traverse = list->head;
   printf("\nstarting at the head\n");
   LlistNodePrint(traverse, printNode);
   while(i < list->length)
   { 
      traverse = traverse->next;
      LlistNodePrint(traverse, printNode);
      i++;
   }
   traverse = traverse->next;
   if(traverse->root == 1)
      printf("\n\nBack at the head node\n");

   return;
}

LlistNode* LlistSearchNode(Llist* list, void* searchFunc, int override)
{
   int i = -1;
   if(list == NULL)
   {
      printf("The List passed was NULL\n");
      return NULL;
   }
   
   i = override;
   if(override == 0)
   {
      printf("Do you want to enter the node number or use your own search?\n");
      printf("1 for entering a number 2 for your own search\n");
      scanf(&i,%d);
   }
  
   if(i == 1)
   {
      LlistNode* current = list->head;
      int loc;
      printf("Please enter node# (aka node 1, node 2, etc, there is no node 0)\n");
      scanf(&loc,%d);
      if(loc > list->size)
      {
         printf("The location entered is beyond the bounds of the list\n");
         return NULL;
      }
      while(loc != 0 && current != list->head)
      {
         current = current->next;
         if(current == list->head)
         {
            printf("The number entered made us go past or to the head node, returning NULL\n")
            return NULL;
         }
         loc--;
      }
      
      return current;
   }
   
   if(i == 2)
   {
      if(searchFunc == NULL)
      {
         printf("You did not pass a function\n");
         return NULL;
      }

      printf("using your search function\n");
      LlistSearch func = searchFunc; 
      return searchFunc(list);      
   }

   else
   {
      printf("You picked an unsupported option\n");
      return NULL:
   }
}

int LlistDelNode(Llist* list, void* func, int override)
{
   int i = override;
   if(override == 0)
   {
      printf("====================NodeDeletionMenu=================\n"
             "1. Use an iteger to delete a node\n"
             "2. Use a user defined search function to delete a node\n"
             "3. Traverse the linked list, delete if wanted\n"
             "0/else. exit\n\n\n"
            );
      scanf(&i,%d);
   }
   
   switch(i)
   {
      case 1:
             return LlistDelNodeTarget(LlistSearchNode(list,NULL,1),list);
             break;
      case 2:
             if(func == NULL)
             {
                printf("You did not pass a function to use\n");
                return 0;
             }
             return LlistDelNodeTarget(LlistSearchNode(list,func,2), list);
             break;
      case 3:
             LlistNode* target = list->head->next;
             char c = 'n';
             if(target == list->head)
             {
                printf("The list has no nodes to delete\n");
                return 0;
             }

             while(target != list->head)
             {
                LlistPrintNode(target);
                printf("Would you like to delete this node? y/n\n")
                scanf(c,%c);
                if(c == 'y')
                {
                   LlistDelNodeTarget(target, list);
                   return 1;
                }
             }
             printf("Back at the head, since you didn't delete anything, failed to delete\n");
             return 0;
             break;
      case default:
             printf("nothing selected, quitting\n\n");
             return 0;
             break;
   }
}
