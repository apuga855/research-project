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

//parameters:	void pointer "data" to allocated data of your type
//returns: 	LlistNode pointer
//================================================================
//This function takes your data and sets the void pointer 
//to be what you passed, the next pointer and prev pointer
//are set to null, the node created by default will not be a
//head node
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

//parameters:	void pointer "dataalloc" to your allocator function
//returns: 	LlistNode pointer
//--------------------------------------------------------------------
//LlistDataAlloc is a function with no parameters that returns a void*
//               to the user defined structure
//======================================================================
//This function takes in a pointer to a declared function that allocates
//your data type, this function makes a node whose next pointer is null
//previous pointer is also null, by default not the head node and the data
//pointer points to whatever your function returned
LlistNode * LlistNodeSelfAlloc(void* dataalloc)
{
   LlistNode * nNode = NULL;
   void* data = NULL;
   nNode = malloc(sizeof(LlistNode));
   if(nNode == NULL)
      return NULL;
   LlistNodeInit(nNode);
   LlistDataAlloc func = dataalloc;
   data = func();
   nNode->data = data;
   return nNode;
}

//parameters: LlistNode* "node" some allocated node
//returns:    nothing
//======================================================
//initializes allocated node to their respective 0 value
//default behavior is to initialize the node to not
//be a root, aka not the head
void LlistNodeInit (LlistNode *node)
{
   if(node == NULL)
   {
      printf("\nThe node passed was not allocated properly\n\n");
      return;
   }

   node->root = 0;
   node->next = NULL;
   node->prev = NULL;
   node->data = NULL;
   return;
}

//parameters: LlistNode* "node"
//returns     1 if successful, 0 otherwise
//================================================================
//deletes an allocated node, assumes that the data was dynamically
//allocated, if not there will be an error
int LlistNodeDel(LlistNode *node)
{
   if(node == NULL)
   {
      printf("\nThe node passed was not allocated properly\n\n");
      return 0;
   }

   if(node->data != NULL)
      free(node->data);
   LlistNodeInit(node);
   free(node);
   node = NULL;
   return 1;
}

//parameters: LlistNode* "src" pointing to data you want to copy
//			       to another node
//	      LlistNode* "dst" pointing to an allocated node with
//			       garbage information you dont mind losing
//	      void* "datacp"   pointing to your data copy function
//returns:    1 if successful, 0 otherwise
//--------------------------------------------------------------------
//LlistNodeDataCpy void function that takes in 2 void pointers and then
//will cast and copy the correct values to the user defined structure
//=====================================================================
//copies src into dst using datacap callback function
//the callback function passes two void pointers,
//the src data is copied where dst pointer used to point to
//if the src or dst are NULL we fail, if the source is not
//part of a list we fail
//please note that the src and dst must have their respective
//data pointers pointing to something dynamically allocated,
//otherwise you need to handle it
int LlistNodeCpy(LlistNode *src, LlistNode *dst, void *datacp)
{
   if(src == NULL || dst == NULL || datacp == NULL)
      return 0;
   
   if(src->data == NULL || dst->data == NULL)
   {
      printf("\none of the nodes did not have their data properly allocated\n");
      return 0;
   }

   LlistNodeDataCpy func = datacp;
   func(src->data,dst->data);
   return 1; 
}

//parameters: 	LlistNode* "node" pointing to allocated node
//		void* "dataprint" pointing to your print function
//returns:      nothing
//---------------------------------------------------------------
//LlistNodePrintData is a void function that prints all printable
//members of the user defined structure
//=============================================================
//default printing function, you provide how to print your data
//via the void* dataprint function pointer that gets casted to a
//LlistNodePrintData
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
   
   if(node->data == NULL)
      printf("\nThis node is empty\n"); 
    
   LlistNodePrintData func = dataprint;
   func(node->data);
   return;
}

//parameters:	 int "size" 	   how many nodes do you want
//	       	 void* "dataalloc" pointing to a function
//				   that allocates your datatype
//returns:	  Llist*	   allocated using your parameters
//--------------------------------------------------------------------
//LlistDataAlloc is a function with no parameters that returns a void*
//               to the user defined structure
//==============================================================
//allocates a size amount of nodes, minimum number is 0
//the list allocates "size" nodes pointing to data allocated
//with your dataalloc function, returns a pointer to the list
//you do not have to pass the dataalloc function, but you have
//to remember the node will just have a null pointer for data
Llist * LlistAlloc(int size, void* dataalloc)
{
   //printf("the size is %d\n",size);
   if(size < 0)
   {
      printf("\nsize was less than 0\n\n");
      return NULL;
   }
   int i = 0;
   Llist* nList = malloc(sizeof(Llist));
   nList->head = malloc(sizeof(LlistNode));
   
   //this was added 
   LlistInit(nList);

   /*
   if(LlistInit(nList))
      printf("\nList was correctly initialized\n");
   else
      printf("\nList failed to be correctly initialized\n");   
   */

   while(i < size)
   {  
      LlistDataAlloc func = dataalloc;
      void* data = NULL;
      if(func != NULL)
         data = func();
      if(LlistInsNode(nList, data))
      {
         //printf("\nList added a node correctly\n\n");
         i++;
      }
      else
      {
         printf("\nList FAILED to add a node correctly\n\n");
         return NULL;
      }
   }
   
   nList->length = size;
   nList->used = 0;
   //printf("\nThe value of nlist->length is %d\n",nList->length);
   return nList;
}

//parameters:	Llist* "list" pointing to an allocated list
//		void** "dataArr" array of void pointers pointing
//				 to data you want in your Llist
//		int "arrLen"  length of your array
//returns: 	1 if successfull, 0 if failed
//=============================================================
//populates list sequentially by placing members
//of the void array in the list, the entire
//data array has to used malloc to create the data for each 
//individual datatype it points to
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
   printf("the array length at populate is %d the list length is %d\n\n",arrLen, list->length); 
   int i = 0;
   LlistNode* target = list->head->next;   
   while(i < arrLen)
   {
      target->data = dataArr[i];
      i++;
      target = target->next;
   }
   
   list->used = arrLen;  
   return 1; 
}

//parameters:	Llist* "list" pointing to an allocated list
//returns:	1 if successfull, 0 if failed
//==========================================================
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
   list->used = 0;
   return 1; 
}

//parameters: Llist* "list"  pointing to an allocated list
//returns: 1 if successfull, 0 if failed
//==============================================================
//deleting entire list, first checks for faillist, if it passess
//we delete sequentially starting with deleting head's next until
//we reach the head again, where we delete the head, delete the
//list and exit successfully
int LlistDel(Llist *list)
{
   //printf("\nInside of LlistDel\n");
   if(LlistFail(list))
      return 0;
   LlistNode * start = list->head;
   LlistNode * current = list->head->next;
   while(start != current)
   {
      //printf("\ndeleting a node\n");
      start->next = current->next;
      if(LlistNodeDel(current))
         current = start->next;
      else
      {
         printf("\nThere was an error deleting\n");
         return 0;
      }
   }
   
   if(LlistNodeDel(start))
   {
      list->head = NULL;
      free(list);
      list = NULL;
      return 1;
   }
    
   else
      return 0;
}

//parameters:	Llist* "list" pointing to allocated list
//		void* "nData" pointing to allocated data to add
//returns: 1 if successfull, 0 if failed
//===================================================================
//inserting node into list by handing a void pointer of data
//if LlistFail is true then we exit because the list was malformend
//the payload will automatically be added to a node which is now 
//the very next node to the head
int LlistInsNode(Llist *list, void * nData)
{
   if(LlistFail(list)) 
   {
      printf("the list failed test at insert node\n\n");
      return 0;
   }

   LlistNode * cur = list->head;
   int i = 0;
   while(i < list->used)
   {
      cur = cur->next;
      i++;
   }
   
   if(cur->next == list->head)
   {
      printf("Maing a new node ==============================\n");
      LlistNode * nNode = malloc(sizeof(LlistNode));
      if(nNode == NULL)
      {
         printf("node allocation fail at insert node\n\n");
         return 0;
      }

      LlistNodeInit(nNode);
      nNode->data = nData;
      nNode->next = cur->next;
      nNode->prev = cur;
      nNode->next->prev = nNode;
      cur->next = nNode;
      list->length++;
   }
   
   else
      cur->next->data = nData;

   list->used++;
   return 1;
}

//parameters: LlistNode* "node" to an allocated node to delete
//parameters:	Llist* "list" pointing to allocated list
//		void* "nData" pointing to allocated data to add
//returns: 1 if successfull, 0 if failed
//===================================================================
//inserting node into list by handing a void pointer of data
//if LlistFail is true then we exit because the list was malformend
//the payload will automatically be added to a node which is now 
//the very next node to the head
int LlistInsNodeTarget(Llist *list, void * nData, int target)
{
   if(LlistFail(list)) 
   {
      printf("the list failed test at insert node\n\n");
      return 0;
   }
   
   LlistNode * current = list->head->next;
   LlistNode * nNode = malloc(sizeof(LlistNode));
   if(nNode == NULL)
   {
      printf("node allocation fail at insert node\n\n");
      return 0;
   }

   LlistNodeInit(nNode);
   nNode->data = nData;
   int i = 0;
   while(i < target)
   {
      if(i > list->used)
         return 0;
      current = current->next;
      i++; 
   }

   nNode->prev = current;
   nNode->next = current->next;
   nNode->next->prev = nNode;
   current->next = nNode;
   list->length++;
   list->used++;
   return 1;
}

//parameters: LlistNode* "node" to an allocated node to delete
//	      Llist* "list" pointing to an allocated list
//returns: 1 if successfull 0 otherwise
//============================================================
//deleting node from list, first we check if LlistFail,
//if so we exit failure, if the node that we are trying
//to delete fails the node test, then we exit failure
//otherwise the node is deleted and the chain is fixed
//accordingly
int LlistDelNodeTarget(Llist *list, int target)
{
   if(LlistFail(list))
      return 0;
   if(list->used < target) 
      return 0;
   
   LlistNode* current = list->head;
   int i = 0;
   while(i < target)
      current = current->next;
   
   current->prev->next = current->next;
   current->next->prev = current->prev;
   free(current->data);
   LlistNodeInit(current);
   free(current);
   current = NULL;
   list->used--;
   list->length--;
   return 1;
}

//parameters: Llist* "list"
//	      void* "data"
//returns: 1 if successfull 0 otherwise
//parameters: LlistNode* "node" to an allocated node to delete
//	      Llist* "list" pointing to an allocated list
//returns: 1 if successfull 0 otherwise
//============================================================
//deleting node from list, first we check if LlistFail,
//if so we exit failure, if the node that we are trying
//to delete fails the node test, then we exit failure
//otherwise the node is deleted and the chain is fixed
//accordingly
int LlistDelNodeTargetN(LlistNode *node,Llist *list)
{
   if(LlistFail(list))
      return 0;
   if(LlistNodeFail(node)) 
      return 0;
   node->prev->next = node->next;
   node->next->prev = node->prev;
   if(LlistNodeDel(node))
   {
      list->length--;
      list->used--;
      return 1; 
   }
   
   else
      return 0;
}

//parameters: Llist* "list"
//	      void* "data"
//returns: 1 if successfull 0 otherwise
//============================================================
//inserts data into the list using the "used" variable
int LlistInsData(Llist* list, void* data)
{
   if(LlistFail(list))
      return 0;
   if(list->used == list->length)//>=
   {
      LlistInsNode(list,data);
      return 1;
   }
   
   int i = 0;
   LlistNode* ptr = list->head->next;
   while(i < list->used)
   {
      ptr = ptr->next;
      i++;
   }

   ptr->data = data;
   list->used++;
   return 1;
}

//parameters: Llist* "list" to a list
//            void* function called nullify
//returns:    1 if successful, 0 otherwise
//--------------------------------------------------------------------
//LlistUsrDataDel is a function with a void * as parameter, retuns 1 
//if successful, 0 otherwise. The function nullifies all values in the
//struct
//==============================================================
//nulls all data using the user function 
int LlistDelData(Llist* list, void* nullify)
{
   LlistNode* ptr = list->head->next;
   LlistUsrDataDel func = nullify;
   while(ptr != list->head)
   {
      if(func(ptr->data))
         continue;
      else 
      {
         printf("\nerror nullying data\n");
         return 0;
      }
   }
   
   list->used = 0;
   return 1;
}

//parameters: Llist* "src" to allocated list we do not want to change
//	      Llist* "dst" to allocated list we will change
//	      void* "dataalloc" to function that allocates a datatype
//returns: 1 if successfull, 0 otherwise
//===================================================================
//making two lists have same size, size use is src
//diff is used to properly add or remove nodes,
//it is assumed dst's data members are no longer
//relevant so they are deleted without question
//if either fail the list test then we exit failure
int LlistCpySize(Llist *src, Llist *dst, void* dataalloc)
{
   printf("starting to copy size\n");
   if(LlistFail(src)|| LlistFail(dst))   
      return 0;
   
   printf("src length = %d dst length = %d\n",src->length, dst->length);
   if(src->length < dst->length)
   {
      printf("dst is bigger\n");
      while(src->length < dst->length)
      {
         if(LlistDelNodeTargetN(dst->head->prev, dst))
            continue;
         else
         {
            printf("error deleting the node\n");
            return 0;
         }
      }
   }

   else if(src->length > dst->length)
   {
      printf("src is bigger\n");
      while(src->length > dst->length)
      { 
         LlistDataAlloc func = dataalloc;
         void * data = NULL;
         data = func();
         if(data == NULL)
	 {
	    printf("data incorrectly initialized\n");
	    return 0;
	 }
        
	 LlistInsNode(dst,data);         
         printf("diff is now %d\n",(src->length - dst->length));
	 
      }   
   }
   printf("finished copying size\n\n");
   
   return 1;
}

//parameters: Llist* "src" source list
//            Llist* "dst" destination list
//            void* "datacp" pointer to a function
//            void* "dataalloc" pointer to a function
//returns: 1 if successful, 0 otherwise
//====================================================
//copies entire list, first we resize so we can copy
//correctly, then we loop one by one and copy each
//node onto the dst list, if list fail for either then
//we exit failure
int LlistCpy(Llist *src, Llist *dst, void *datacp, void *dataalloc)
{
   printf("starting LlistCpy\n");
   if(LlistFail(src)|| LlistFail(dst))   
      return 0;
   
   if(LlistCpySize(src,dst,dataalloc))
   {
      LlistNode *srcC = NULL;
      LlistNode *dstC = NULL;
      srcC = src->head->next;
      dstC = dst->head->next;
      while(srcC != src->head)
      {
         printf("src pointer is %p dst pointer is %p",srcC,dstC);
         LlistNodeCpy(srcC, dstC, datacp);
         srcC = srcC->next;
         dstC = dstC->next;
      }
      
      dst->length = src->length;
      dst->used = src->used;
      printf("finishing LlistCpy\n");
     
      return 1;
   }

   return 0;
}

//parameters: LlistNode* "node"
//returns 1 if node failed, 0 if it did not fail
//==============================================
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

//parameters: Llist* "list"
//return: 1 if list failed, 0 if it did not fail
//=======================================================
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
      //printf("The List passed the test\n\n");
      return 0;
   }
}

//parameters: Llist* "list"
//            void* "printNode"
//return: Nothing
//=======================================================
//prints the list using the printNode function pointer, uses
//it in every single node in the list
void LlistPrint(Llist* list, void* printNode)
{
   if(LlistFail(list))
   {
      printf("list failed the test at print\n");
      return;
   }   
   
   printf("\nUsing print function to print each node\n");
   printf("This list has %d length and a total of %d used", list->length, list->used);
    
   int i = 0;
   LlistNode* traverse = NULL;
   traverse = list->head;
   printf("\nstarting at the head\n");
   LlistNodePrint(traverse, printNode);
   while(i < list->length && list->used != 0 && list->used > 0 && list->used > i)
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

//parameters: Llist* "list"
//            void* "searchFunc"
//            void* "payload"
//	      int "override"
//return: LlistNode* of list wanted, NULL otherwise
//=======================================================
//searches the list using a set of predefined patterns or overrides going directly
//to the search method. Override 0 shows the searches, override 1 searches based on 
//an entered number, override 2 searches using a function that you created 
LlistNode* LlistSearchNode(Llist* list, void* searchFunc, void* payload, int override)
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
      scanf("%d",&i);
   }
  
   if(i == 1)
   {
      LlistNode* current = list->head;
      int loc;
      printf("Please enter node# (aka node 1, node 2, etc, there is no node 0)\n");
      scanf("%d",&loc);
      if(loc > list->length)
      {
         printf("The location entered is beyond the bounds of the list\n");
         return NULL;
      }
      while(loc != 0 && current != list->head)
      {
         current = current->next;
         if(current == list->head)
         {
            printf("The number entered made us go past or to the head node, returning NULL\n");
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
      return func(list,payload);      
   }

   else
   {
      printf("You picked an unsupported option\n");
      return NULL;
   }
}

//parameters: Llist* "list"
//            void* "func"
//            void* "printfunc"
//	      void* "payload"
//            int "override"
//return: 1 if list failed, 0 if it did not fail
//=======================================================
//This function deletes members of the list, override 0 shows menu, override 1
//uses an integer entered to delete a node, override 2 uses a defined search function,
//override 3 traverses the list one by one, deletes node on demand, 0 is to exit;
int LlistDelNode(Llist* list, void* func,void* printfunc, void* payload, int override)
{
   int i = override;
   if(override == 0)
   {
      printf("====================NodeDeletionMenu=================\n"
             "1. Use an iteger to delete a node\n"
             "2. Use a user defined search function to delete a node\n"
             "3. Traverse the linked list, delete if wanted\n"
             "0. Exit\n\n\n"
            );
      scanf("%d",&i);
   }
   
   switch(i)
   {
      case 1:
      {
             return LlistDelNodeTargetN(LlistSearchNode(list,NULL,NULL,1),list);
             break;
      }
      case 2:
      {
             if(func == NULL)
             {
                printf("You did not pass a function to use\n");
                return 0;
             }
             return LlistDelNodeTargetN(LlistSearchNode(list,func,payload,2), list);
             break;
      }
      case 3:
      {
             if(printfunc == NULL)
             {
                printf("No print function available\n");
                return 0;
             }
             LlistNode* target = NULL;
             target = list->head->next;
             char c = 'n';
             if(target == list->head)
             {
                printf("The list has no nodes to delete\n");
                return 0;
             }

             while(target != list->head)
             {
                LlistNodePrint(target,printfunc);
                printf("Would you like to delete this node? y/n\n");
                scanf("%c",&c);
                if(c == 'y')
                {
                   LlistDelNodeTargetN(target, list);
                   return 1;
                }
             }
             printf("Back at the head, since you didn't delete anything, failed to delete\n");
             return 0;
             break;
      }
      default:
      {
             printf("nothing selected, quitting\n\n");
             return 0;
             break;
      }
   }
}

//parameters: Llist* "list"
//return: 1 if true, 0 if false
//=======================================================
//checks if list is empty
int LlistIsEmpty(Llist* list)
{
   if(list->used > 0)
      return 0;

   else
      return 1;
}

//parameters: Llist* "list"
//return: void*
//=======================================================
//returns the first element in the list, for rehashing purposes
void* LlistRetFirst(Llist* list)
{
   if(list->head->next == list->head)
      return NULL;

   else
      return list->head->next->data;
}
