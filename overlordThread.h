/*
Author: Alfonso Puga
purpose: 
This is the body of the overlord thread
*/
#pragma once
#include<stdio.h>
#include<stdlib.h>
#include <pthread.h>
#include<unistd.h>
#include "sensorRdyPckt.h"
#define OVERLORD_SEM 1
#define SLAVE_SEM 2
#define WAITRESS_SEM 3

char buf[100];	//buffer to write to file
int ovrThrdFile = open("overlordThrdInfo",O_CREAT | O_WRONLY | O_TRUNC, 0600);//creating file
volatile sig_atomic_t done = 0;
sigset_t fullMask;			//mask that blocks everything
sigset_t emptyMask;		        //mask for the empty set
sigset_t overlordMask;		//mask for the overlord
sigfillset(&fullMask)		//setting it to block everything
sigfillset(&overlordMask);		//set overlord mask to empty
sigdelset(&overlordMask, SIGUSR1);	//setting mask to not block sigusr1
sigemptyset(&emptyMask);		//set empty mask to empty
sigaddset(&overlordMask, SIGUSR1);   //adding SIGUSR1 to the mask so we know when to start
sigaddset(&overlordMask, SIGUSR2);   //adding SIGUSR2 to the mask so we know when to start

void* overlordThrd(void * )
{
   if(outf < 0)//checking if file exploded
   {
      perror("\nERROR OPENNING THE FILE\n");//print out explosion
      exit(0);				//explosion, so exit
   }   
   
   strcpy(buf, "\nOverlord Thread Start\n");//copy the message into the buffer
   write(outf ,buf ,strlen(buf));	//write it out
   strcpy(buf, "Setting mask blocks and sentinels\n");//copy the message into the buffer
   write(outf ,buf ,strlen(buf));	//write it out
   struct sigaction overlordActionS;	//sigaction for overlord
   struct sigaction overlordActionW;	//sigaction for overlord
   overlordActionS.sa_handler = overlordActionSHandler;//setting action for action handler
   overlordActionW.sa_handler = overlordActionWHandler;//setting action for action handler
   overlordActionS.sa_flags = SA_RESTART;//flag is to restart
   overlordActionW.sa_flags = SA_RESTART;//flag is to restart
   sigaction(SIGUSR1, &overlordActionS, NULL);//setting action to online
   sigaction(SIGUSR2, &overlordActionW, NULL);//setting action to online
   strcpy(buf, "Ready to suspend until SIGUSR1 is received\n");//copy the message into the buffer
   write(outf ,buf ,strlen(buf));	//write it out
   sigprocmask(SIG_BLOCK, &fullmask, NULL);//blocking all signals
   
   do
   {       //endless loop to simulate pause
      sigsuspend(&overlordMask);	//pause on overlord mask, wakes up if SIGUSR1 is caught, this behavior is temporary
   }while(!done);


}

void overlordActionSHandler(int dummy)
{
   sigprocmask(SIG_BLOCK, &fullmask, NULL);//blocking all signals
   strcpy(buf, "Caught a SIGUSR1\n");//copy the message into the buffer
   write(outf ,buf ,strlen(buf));	//write it out
   strcpy(buf, "Slave handler\n");//copy the message into the buffer
   write(outf ,buf ,strlen(buf));	//write it out
   

}

void overlordActionWHandler(int dummy)
{
   sigprocmask(SIG_BLOCK, &fullmask, NULL);//blocking all signals
   strcpy(buf, "Caught a SIGUSR2\n");//copy the message into the buffer
   write(outf ,buf ,strlen(buf));	//write it out
   strcpy(buf, "Waitress handler\n");//copy the message into the buffer
   write(outf ,buf ,strlen(buf));	//write it out

}
