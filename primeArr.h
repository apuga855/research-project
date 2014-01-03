#pragma once
#include<stdio.h>
#include<stdlib.h>
typedef struct _kprime
{
   unsigned int maxnum;
   unsigned int capslot;
   unsigned int hash2; 
}kprime;

//allocator function
kprime* kprimeAlloc();
//returns the cap prime 
int kprimeCap(kprime*);
//returns the hash2 prime
int kprimehash2(kprime*);
//initializes the structure
void kprimeInit(kprime*);
//finds the hash2 prime, have to have a cap prime to find
void kprimeFindhash2(kprime*);
//finds the cap prime
int kprimeFindCap(int, kprime*);
