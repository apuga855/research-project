#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"linked_list.h"
/*
List of functions					works		doesnt work

LlistNode related functions

LlistNode* LlistNodeAlloc(void*)			X		0	
LlistNode* LlistNodeSelfAlloc(void*)			X		0	
void LlistNodeInit (LlistNode *);			X		0
int LlistNodeDel(LlistNode *);			        X		0	
int LlistNodeCpy(LlistNode *, LlistNode *, void *);	X		0
int LlistNodeFail(LlistNode*);				X		0	
void LlistNodePrint(LlistNode*, void*);			X		0
typedef void*(*LlistNodePrintData)(void*);		X		0
typedef void(*LlistNodeDataCpy)(void*, void*);		X		0
typedef LlistNode*(*LlistSearch)(Llist list);		X		0
typedef void*(LlistDataAlloc);				X		0

Llist related functions
Llist * LlistAlloc(int, void*);				X		0
int LlistPopulate(Llist *, void**, int);		X		0
int LlistInit(Llist *);					X		0
int LlistDel(Llist *);					X		0
int LlistInsNode(Llist *, void *);			X		0
int LlistDelNode(Llist*, void*, void*, int);		X		0	
int LlistDelNodeTarget(LlistNode *,Llist*);		X		0
int LlistCpySize(Llist *, Llist *, void*);		X		0
int LlistCpy(Llist *, Llist *, void *, void*);		X		0
int LlistFail(Llist *);					X		0
LlistNode* LlistSearchNode(Llist*, void*);
void LlistPrint(Llist*, void*);				X		0
typedef void(*LlistSortIns)(Llist*);			
typedef void(*LlistSort)(Llist*);			
typedef LlistNode*(*LlistSearch)(Llist*, void*);	
*/
//This program attempts to test the basic functionality 
//of the linked list header file
typedef struct _person
{
   int age;
   int id;
   char name[20];
   char lname[20];
}person;

void printPerson(person* per)
{
   if(per == NULL) return; 
   printf("\nId entered is: %d",per->id); 
   printf("\nAge entered is: %d",per->age); 
   printf("\nName entered is: %s",per->name); 
   printf("\nLast name entered is: %s\n",per->lname); 
   return;
}

void mycopy(void* src, void* dst)
{
   printf("\ninside of function mycopy\n");
   printf("\nsrc address is %p\n",src);
   printf("\ndst address is %p\n",dst);
   person* srcStc = (person*)src;
   person* dstStc = (person*)dst;
   printPerson(srcStc);
   dstStc->age = srcStc->age;
   dstStc->id = srcStc->id;
   strncpy(dstStc->name, srcStc->name,20);
   strncpy(dstStc->lname, srcStc->lname,20);
   printf("\nfinished function mycopy\n");
   printPerson(dstStc);
   return; 
}

void* mydataalloc()
{
   person* data =  malloc(sizeof(person));
   return (void*) data;
}

void* MyPayloadCreate()
{
   person* payload = NULL;
   payload = malloc(sizeof(person));
   printf("Enter the name of who you are looking for\n");
   scanf("%s",payload->name);
   printf("Enter the last name of who you are looking for\n");
   scanf("%s",payload->lname);
   return (void*) payload;
}


LlistNode* MySearch(Llist* list, void* payload)
{
   person* currentPerson = NULL;
   person* target = (person)payload;
   LlistNode* current = NULL;
   current = list->head->next;
   while(current != list->head)
   {
      currentPerson = (person)current->data;
      if(strncmp(currentPerson->name,target->name,20)   &&
         strncmp(currentPerson->lname,target->lname,20) == 0)
        {
           int i = 0;
           printf("Found:\n")
           printPerson(currentPerson);
           printf("\nIs this the correct person or would you like to keep looking?\n"
                   "Press 0 to quit and 1 to keep looking\n");
           scanf("%d",&i);
           if(i == 1)
              continue;
           else
              return currentPerson;           
        } 
   }
   
   printf("No hits with the given payload\n\n");
   return NULL;
}


int main()
{
   LlistNode *testNode = NULL;
   person *orgPer = NULL;
   void* info = NULL;

   printf("\n\n\nTesting LlistNodeAlloc\n");
   orgPer = malloc(sizeof(person));
   printf("Please enter the id of the person\n");
   scanf("%d",&(orgPer->id));
   printf("Id entered is: %d\n",orgPer->id); 
   printf("Please enter the age of the person\n");
   scanf("%d",&(orgPer->age));
   printf("Age entered is: %d\n",orgPer->age); 
   printf("Please enter the name of the person\n");
   scanf("%s",orgPer->name);
   printf("Name entered is: %s\n",orgPer->name); 
   printf("Please enter the last name of the person\n");
   scanf("%s",orgPer->lname);
   printf("Last name entered is: %s\n",orgPer->lname); 
   info = (void*)orgPer;
   testNode = LlistNodeAlloc(info);
   

   printf("\n\n\ngetting back the info from the void pointer\n");
   person* retPerson = (person*)testNode->data;
   printPerson(retPerson);
   printf("\n\n\nusing the node print function on original node\n");
   LlistNodePrint(testNode,printPerson);

   LlistNode* cpNode = NULL;
   person *nullPer = malloc(sizeof(person));
   cpNode = LlistNodeAlloc((void*)nullPer);
   LlistNodeCpy(testNode,cpNode,mycopy); 
   printf("\n\n\nprinting copied person using print function\n"); 
   LlistNodePrint(cpNode,printPerson);
   
   printf("\n\n\nDeleting the node\n"); 
   
   LlistNodeDel(testNode);
   if(testNode->data == NULL)
      printf("Successfully deleted\n");
   else
      printf("Failed to delete\n");
   
   testNode = NULL;    
   


   Llist* group1 = NULL;
   int len = 0;
   printf("\n\n\nHow long do you want the list?\n");
   scanf("%d",&len);
   printf("The value of len is %d\n",len);
   group1 = LlistAlloc(len, mydataalloc);
   if(group1 == NULL)
   {
      printf("The list did not get allocated\n\n");
      return 0;
   }

   printf("The list should have been alocated\n");
   printf("The number of allocated nodes is %d",group1->length);   
  
   printf("\n\npopulating the list with 3 premade people\n");
    
   void* input[3];
   person* personGrp0 = malloc(sizeof(person));
   person* personGrp1 = malloc(sizeof(person));
   person* personGrp2 = malloc(sizeof(person));
   input[0] = personGrp0;
   input[1] = personGrp1;
   input[2] = personGrp2;
   personGrp0->id = 1;
   personGrp0->age = 12;
   strncpy(personGrp0->name,"joan",4);
   strncpy(personGrp0->lname,"floam",5);
   printPerson(personGrp0);
   printf("\n\n");
    
   personGrp1->id = 2;
   personGrp1->age = 22;
   strncpy(personGrp1->name,"dave",4);
   strncpy(personGrp1->lname,"gave",4);
   printPerson(personGrp1);
   printf("\n\n");
   
   personGrp2->id = 1;
   personGrp2->age = 12;
   strncpy(personGrp2->name,"cup",3);
   strncpy(personGrp2->lname,"pup",3);
   printPerson(personGrp2);
   printf("\n\n");
   
   input[0] = (void*) personGrp0;
   input[1] = (void*) personGrp1;
   input[2] = (void*) personGrp2;
   
   printf("\n\nattempting to populate\n");
   if(LlistPopulate(group1, input, 3) == 0)
      printf("Failed to populate\n");
 
   LlistPrint(group1, printPerson);
   
   printf("\n\nNow testing the LlistCpy function with the LlistCpySize\n\n");
   Llist* listCpy = LlistAlloc(0, NULL);
   LlistCpy(group1, listCpy,mycopy,mydataalloc); 
   printf("\n\nPrinting the source\n\n");
   LlistPrint(group1, printPerson);
   printf("\n\nPrinting the destination\n\n");
   LlistPrint(listCpy, printPerson);
        
   //if(LlistDel(group1))
   //   printf("List was deleted correctly\n");
   //else
   //   printf("Failed to print list\n");

   return 0;
}


