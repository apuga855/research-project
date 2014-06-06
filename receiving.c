#include<stdlib.h>
#include<stdio.h>
#include<fcntl.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdint.h>
#include"packet.h"

int main()
{
   int check = 1;
   int fd;
   char * connectPipe = "/tmp/connectPipe";
   sensorRdyPckt* pckt;
   printf("This program simulates the genetic algorithm\n");
   mkfifo(connectPipe,066);
   fd = open(connectPipe, O_RDONLY);
   

   
   while(check)
   { 
      read(fd,pckt,sizeof(pckt));
      if(pckt == NULL)
      {
         check = 0;
         continue;
      }
      
      printf("Printing packet\n");
      //SRD_pcktPrint(pckt);
      printf("Printing header\n");
      //SRD_pcktHdrPldPrint(pckt);
      printf("Printing fingerprint\n");
      //SRD_formatFngPntPrint(pckt->fngPnt);
   }

   printf("Finished Simulation\n");
}
