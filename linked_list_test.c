#include<stdlib.h>
#include<stdio.h>
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
   printf("Id entered is: %d\n",per->id); 
   printf("Age entered is: %d\n",per->age); 
   printf("Name entered is: %s\n",per->name); 
   printf("Last name entered is: %s\n",per->lname); 
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
   person* retPerson = (struct person*)testNode->data;
   printPerson(retPerson);
   LlistNode* cpNode = NULL;
   
   printf("Deleting the node\n"); 
   LlistNodeDel(testNode);
   if(testNode->data == NULL)
      printf("Successfully deleted\n");
   else
      printf("Failed to delete\n");
   testNode = NULL;    
   
   
   
   return 0;
}


