//author: Alfonso Puga
//purpose:
//implementing a linked list with unknown datatype structure
//the list is dynamic, has a dummy head node, inserts before
//the head each time by default, contains callback functions
//to satisfy whatever the user needs
   
//node for the list, int root is 0 unless youa re at the head
//contains a previous and next pointer for linked list behavior
//the void* stores whatever data the user needs
#pragma once
#include<stdlib.h>
   
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
   
}Llist;

   
//returns an allocated node
LlistNode * LlistNodeAlloc(void*);
//initializes nodes to defaults
void LlistNodeInit (LlistNode *);
//deletes a node
void LlistNodeDel(LlistNode *);
//list copy
int LlistNodeCpy(LlistNode *, LlistNode *, void *);
//check fo rfailure
int LlistNodeFail(LlistNode*);
//default printing function
void LlistNodePrint(LlistNode*, void*);
//user defined print function for printing nodes
typedef void*(*LlistNodePrintData)(void*);
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
//-------------------------------------------------//
//search function to be implemented by user
//since we dont know how his/her data works, its
//up to them to get it to work, the function
//returns a LlistNode pointer so it can be used
//in conjunction with LlistDelNode(LlistNode, Llist)
typedef LlistNode*(*LlistSearch)(Llist *list);


//allocates a set amount of nodes for a list
//returns the list with that many nodes, minimum
//number of nodes is 1
Llist * LlistAlloc(int);
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
//Deleting node
int LlistDelNode(LlistNode *,Llist*);
//copy size
int LlistCpySize(Llist *, Llist *);
//copies entire list
int LlistCpy(Llist *, Llist *, void *);
//checks for list failure
int LlistFail(Llist *);
//print all members of the list
void LlistPrint(Llist *, void*);
//function for user to make a sorted insert
//implementation is up to the user
typedef void(*LlistSortIns)(Llist*);
//--------------------------------------------//
//much like the sorted insert, the user needs to
//define their sorting method for the list
typedef void(*LlistSort)(Llist*);

