//author: Alfonso Puga
//purpose:
//implementing a linked list with unknown datatype structure
//the list is dynamic, has a dummy head node, inserts before
//the head each time by default, contains callback functions
//to satisfy whatever the user needs   
//node for the list, int root is 0 unless you are at the head
//contains a previous and next pointer for linked list behavior
//the void* stores whatever data the user needs
//headnode is not used as a storage place 
#pragma once
#include<stdlib.h>
#include<stdio.h>   
//LlistNode contains a root variavle that determines if you are
//the dummy headnode or not, a next pointer for pointing to the next
//node in the linked list, a prev pointer to point to the previous node
//and finally a data void pointer for the user data
typedef struct _LlistNode
{
  int root;
  struct _LlistNode * next;
  struct _LlistNode * prev;
  void * data;
 
}LlistNode;

//list contains a length, useful for user defined callback functions
//keeps track of head only, rest is available by coursing through 
//the linked list
typedef struct _Llist
{
   LlistNode *head;
   int length;
   int used;   
}Llist;

   
//returns an allocated node, you pass the data
LlistNode * LlistNodeAlloc(void*);
//uses your allocator function to create a void pointer
//to your datatype 
LlistNode * LlistNodeSelfAlloc(void*);
//initializes nodes to defaults
void LlistNodeInit (LlistNode *);
//deletes a node
int LlistNodeDel(LlistNode *);
//list copy
int LlistNodeCpy(LlistNode *, LlistNode *, void *);
//check fo rfailure
int LlistNodeFail(LlistNode*);
//default printing function
void LlistNodePrint(LlistNode*, void*);
//user defined print function for printing nodes
typedef void(*LlistNodePrintData)(void*);
//-----------------------------------------------//
//this function is necessary for the copy function 
//ince we dont know how the structure is set up we
//let the user define his/her own and make it work
typedef void(*LlistNodeDataCpy)(void*,void*);
/*
example of LlistNodeDataCpy callback function:
void LlistNodeDataCpy(void* src, void* dst)
{
  foo * fooSrc = (foo) src;
  foo * fooDst = (foo) dst;
  fooDst->member1 = fooDst->member1;
  fooDst->member2 = fooDst->member2;
  fooDst->member3 = fooDst->member3;
  ...
  fooDst->memberX = fooSrc->memberX;
  return
}*/
//-----------------------------------------------------//
//function that returns a casted void pointer
//that originally pointed to a structure of your datatype
typedef void*(*LlistDataAlloc)(); 
/*
Example
typedef struct _garbage
{
   int x;
   char name[20];
   ...
   ...
}garbage

void * MyDataAllocator()
{
   return malloc(sizeof(garbage));
}

*/
//-------------------------------------------------//
//search function to be implemented by user
//since we dont know how his/her data works, its
//up to them to get it to work, the function
//returns a LlistNode pointer so it can be used
//in conjunction with LlistDelNode(LlistNode, Llist)
//-------------------------------------------------//
//allocates a set amount of nodes for a list
//returns the list with that many nodes, minimum
//number of nodes is 1
Llist * LlistAlloc(int, void*);
//populates list, have to populate the entire linked 
//list's length, so all usable nodes are filled
//each member of the data array must be dynamically
//allocated in order for the code to be consistent
int LlistPopulate(Llist *, void**, int);
//initializes list
int LlistInit(Llist *);
//deletes the entire list
int LlistDel(Llist *);
//insert node into the list
int LlistInsNode(Llist *, void *);
//inserts data into an available node
int LlistInsData(Llist*, void*);
//Deleting node
int LlistDelNode(Llist*, void*,void*,void*, int);
//Deleting target node
int LlistDelNodeTarget(LlistNode *,Llist*);
//nullying all values of all data within the list
int LlistDelDAta(Llist*, void*);
//copy size
int LlistCpySize(Llist *, Llist *, void*);
//copies entire list
int LlistCpy(Llist *, Llist *, void *,void*);
//checks for list failure
int LlistFail(Llist *);
//search for a node
LlistNode * LlistSearchNode(Llist*,void*, void*, int);
//check if the list is in use
int LlistIsEmpty(Llist*);
//returns first data member as a void*
void * LlistRetFirst(Llist*);
//print all members of the list
void LlistPrint(Llist *, void*);
//function for user to make a sorted insert
//implementation is up to the user
typedef void(*LlistSortIns)(Llist*);
//--------------------------------------------//
//much like the sorted insert, the user needs to
//define their sorting method for the list
typedef void(*LlistSort)(Llist*);
//if they want to search for the node with their own function
typedef LlistNode*(*LlistSearch)(Llist*, void*);
//if they want to delete the data in a special way this will be used
typedef int(*LlistUsrDataDel)(void*);

