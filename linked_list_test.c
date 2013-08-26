#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"linked_list.h"

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
   printf("Id entered is: %d\n",per->id); 
   printf("Age entered is: %d\n",per->age); 
   printf("Name entered is: %s\n",per->name); 
   printf("Last name entered is: %s\n",per->lname); 
   return;
}

void mycopy(void* src, void* dst)
{
   printf("\ninside of function mycopy\n");
   person* srcStc = (person*)src;
   person* dstStc = (person*)dst;
   dstStc->age = srcStc->age;
   dstStc->id = srcStc->id;
   strncpy(dstStc->name, srcStc->name,20);
   strncpy(dstStc->lname, srcStc->name,20);
   return; 
}

int main()
{
   LlistNode *testNode = NULL;
   person *test0 = NULL;
   void* info = NULL;

   printf("\n\n\nTesting LlistNodeAlloc\n");
   test0 = malloc(sizeof(person));
   printf("Please enter the id of the person\n");
   scanf("%d",&(test0->id));
   printf("Id entered is: %d\n",test0->id); 
   printf("Please enter the age of the person\n");
   scanf("%d",&(test0->age));
   printf("Age entered is: %d\n",test0->age); 
   printf("Please enter the name of the person\n");
   scanf("%s",test0->name);
   printf("Name entered is: %s\n",test0->name); 
   printf("Please enter the last name of the person\n");
   scanf("%s",test0->lname);
   printf("Last name entered is: %s\n",test0->lname); 
   info = (void*)test0;
   testNode = LlistNodeAlloc(info);
   

   printf("\n\n\ngetting back the info from the void pointer\n");
   person* retPerson = (person*)testNode->data;
   printPerson(retPerson);

   LlistNode* cpNode = NULL;
   person *test1 = malloc(sizeof(person));
   cpNode = LlistNodeAlloc((void*)test1);
   LlistNodeCpy(testNode,cpNode,mycopy); 
   printf("\n\n\nprinting copied person\n"); 
   printPerson((person *)(cpNode->data));
   printf("Deleting the node\n\n\n"); 
   
   LlistNodeDel(testNode);
   if(testNode->data == NULL)
      printf("Successfully deleted\n");
   else
      printf("Failed to delete\n");
   
   testNode = NULL;    
   
   Llist* group1 = NULL;
   int len = 0;
   printf("How long do you want the list?\n");
   scanf("%d",&len);
   group1 = LlistAlloc(len);
   if(group1 == NULL)
   {
      printf("The list did not get allocated\n\n");
      return 0;
   }

   printf("The list should have been alocated\n");
   printf("The number of allocated nodes is %d",group1->length);   
  
   printf("\n\npopulating the list with 3 premade people\n");
    
   void* input[3];
   person* personGrp = malloc(3*sizeof(person));
   personGrp[0].id = 1;
   personGrp[0].age = 12;
   strncpy(personGrp[0].name,"joan",4);
   strncpy(personGrp[0].lname,"floam",5);
   printPerson(&personGrp[0]);
   printf("\n\n");
    
   personGrp[1].id = 2;
   personGrp[1].age = 22;
   strncpy(personGrp[1].name,"dave",4);
   strncpy(personGrp[1].lname,"gave",4);
   printPerson(&personGrp[1]);
   printf("\n\n");
   
   personGrp[2].id = 1;
   personGrp[2].age = 12;
   strncpy(personGrp[2].name,"cup",3);
   strncpy(personGrp[2].lname,"pup",3);
   printPerson(&personGrp[2]);
   printf("\n\n");
   
   input[0] = (void*) &personGrp[0];
   input[1] = (void*) &personGrp[1];
   input[2] = (void*) &personGrp[2];
   
   printf("\n\nattempting to populate\n");
   if(LlistPopulate(group1, input, 3) == 0)
      printf("Failed to populate\n");
   
   

   return 0;
}


