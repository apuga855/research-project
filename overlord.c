#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>



#define SEMAPHORE_NUM 1
int semid;
int semnum = 1;
char pathname[200];
key_t ipckey;

pid_t overlordID;
pid_t cleanupID;
pid_t packingID;

int mainfd;
int overlordfd;
int cleanupfd;
int packingfd;


int main()
{
   pthread_t overlordThrd;
   pthread_t cleanupThrd;
   pthread_t packingThrd;
   char mainbuf[100];
   int check = 0; 
      
   mainfd = open("main.log", O_CREAT | O_WRONLY | O_TRUC, 0600);
   if(mainfd < 0)
   {
      perror("opening outfile for overlord\n");
      exit(0);
   }
   
   strcpy(mainbuf,"Starting the multithreading simulation\n\n");
   write(mainfd, mainbuf, strlen(mainbuf));
   
   strcpy(mainbuf,"Launching threads\n");
   write(mainfd, mainbuf, strlen(mainbuf));
   
   check = pthread_create(&overlordThrd, NULL, overlord, (void*)"Overlord launching");
   if(check)
   {
      strcpy(mainbuf,"ERROR LAUNCHING OVERLORD\n");
      write(mainfd, mainbuf, strlen(mainbuf));
      exit(-1);
   }

   check = pthread_create(&cleanupThrd, NULL, overlord, (void*)"Cleanup launching");
   if(check)
   {
      strcpy(mainbuf,"ERROR LAUNCHING CLEANUP \n");
      write(mainfd, mainbuf, strlen(mainbuf));
      exit(-1);
   }

   check = pthread_create(&packingThrd, NULL, overlord, (void*)"Packing launching");
   if(check)
   {
      strcpy(mainbuf,"ERROR LAUNCHING PACKING\n");
      write(mainfd, mainbuf, strlen(mainbuf));
      exit(-1);
   }
    
   strcpy(mainbuf,"Finished launching threads, waking up overlord\n");
   write(mainfd, mainbuf, strlen(mainbuf));
   kill(overlordID, SIGCONT); 
   strcpy(mainbuf,"Main thread going to sleep so the overlord can set up\n");
   write(mainfd, mainbuf, strlen(mainbuf));
   sleep(3);
    
      
}

void* overlord(void* msg)
{
   overlordID = getpid(); 
   char overlordbuf[100];
   
   overlordfd = open("overlord.log", O_CREAT | O_WRONLY | O_TRUC, 0600);
   if(overlordfd < 0)
   {
      perror("opening outfile for overlord\n");
      exit(0);
   }
   
   strcpy(overlordbuf,"Created overlord\n\n");
   write(overlordfd, overlordbuf, strlen(overlordbuf));
   
   getcwd(pathname,200);
   strcat(pathname, "foo/");
   ipckey = ftok(pathname,42);
   semid = semget(ipckey, semnum, 0666 | IPC_CREAT); 
   if(semid < 0)
   {
      perror("Failed creating the semaphore\n");
      _exit(1);
   }   
   strcpy(overlordbuf,"Semaphores were correctly created\nGoing to sleep\n");
   write(overlordfd, overlordbuf, strlen(overlordbuf));
   
   kill(overlordID,SIGSTOP);
   strcpy(overlordbuf,"Overlord woke up!\n\n");
   write(overlordfd, overlordbuf, strlen(overlordbuf)); 

   sigset_t overlordmask;
   sigfillset(&overlordmask);
   sigdelset(&overlordmask,SIGUSR1);   
   
   struct sigaction overlordaction;
   overlordaction.sa_handler = overlordNewPacketHandler;
   overlordaction.sa_flags = SA_RESTART;
   overlordaction.sa_mask = overlordmask;
   
   
   
}

void overlordNewPacketHandler(int signo)
{
   
}

void * packing(void* msg)
{
   packingID = getpid();
   kill(packingID,SIGSTOP);
   

   struct sembuf rlsPack;
   rlsPack.sem_num = semid;
   rlsPack.sem_op = - 1;
   rlsPack.sem_flg = SEM_UNDO;

   struct sembuf grbPack[2];
   grbPack[0].sem_num = semid;
   grbPack[0].sem_op = 0;
   grbPack[0].sem_flg = SEM_UNDO;
   
   grbPack[1].sem_num = semid;
   grbPack[1].sem_op = + 1;
   grbPack[1].sem_flg = SEM_UNDO;
   
   
   packingfd = open("packing.log", O_CREAT | O_WRONLY | O_TRUC, 0600);
   if(packingfd < 0)
   {
      perror("opening outfile for packing\n");
      exit(0);
   }

   
}

void * cleanup(void* msg)
{
   cleanupID = getpid();
   kill(cleanupID,SIGSTOP);
   

   struct sembuf rlsClean;
   rlsClean.sem_num = semid;
   rlsClean.sem_op = - 1;
   rlsClean.sem_flg = SEM_UNDO;

   struct sembuf grbClean[2];
   grbClean[0].sem_num = semid;
   grbClean[0].sem_op = 0;
   grbClean[0].sem_flg = SEM_UNDO;
   
   grbClean[1].sem_num = semid;
   grbClean[1].sem_op = + 1;
   grbClean[1].sem_flg = SEM_UNDO;
   
   
   
   cleanupfd = open("cleanup.log", O_CREAT | O_WRONLY | O_TRUC, 0600);
   if(cleanupfd < 0)
   {
      perror("opening outfile for packing\n");
      exit(0);
   }

   
}