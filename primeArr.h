#pragma once
#include<stdio.h>
#include<stdlib.h>
typedef struct _kprime
{
   unsigned int maxnum;
   unsigned int capslot;
   unsigned int hash2; 
}kprime;


kprime* kprimeAlloc();
int kprimeCap(kprime*);
int kprimehash2(kprime*);
void kprimeInit(kprime*);
void kprimeFindhash2(kprime*);
int kprimeFindCap(int, kprime*);
