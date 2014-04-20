/*
overlord thread launches
overlord launches all other threads
overlord resumes caller thread so all threads are ready to continue

COMMUNICATION BETWEEN THREADS
---------------------------------
THREAD |to|  THREAD  |  SIGNAL  |
---------------------------------
OVERLORD->  MAIN     =  SIGUSR1 |SUCCESS
OVERLORD->  MAIN     =  SIGUSR2 |FAILURE
MAIN    ->  OVERLORD  = SIGABRT |
MAIN    ->  PACKING  =  SIGUSR1 |
PACKING ->  OVERLORD =  SIGUSR1 |
CLEANUP ->  OVERLORD =  SIGUSR2 |
WORKER	->  CLEANUP  =  SIGUSR1 |      
OVERLORD->  WORKER   =  SIGUSR1 |
---------------------------------


*/
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
#define WORKER_NUM 10
int semid;			//id for semaphores
int semnum = 1;			//number of semaphores
char pathname[200];		//pathname for generating ipckey
key_t ipckey;			//ipckey

pid_t mainID;			//main thread id
pid_t overlordID;		//overlord thread id
pid_t cleanupID;		//cleanup thread id
pid_t packingID;		//packing thread id

int mainfd;			//main log
int overlordfd;			//overlord log
int cleanupfd;			//cleanup log
int packingfd;			//packing log
int success;			//trigger for success or failure			
int MAINFD;			//global for main log

void * overlordThread(void*);
void successHandle(int);
void failureHandle(int);
void overlordHandle(int);
void * packingThread(void*);
void * cleanupThread(void*);
void* workerThread(void*);
void packingHandle(int);
char handlerbuf[100];			//main buffer
sigset_t fullmask;		//all signals
sigset_t emptymask;		//no signals

union semun {
    int val;               /* used for SETVAL only */
    struct semid_ds *buf;  /* used for IPC_STAT and IPC_SET */
    ushort *array;         /* used for GETALL and SETALL */
};

char mainbuf[100];			//main buffer
struct sigaction successAction;	//success action when recieving signal from overlord
struct sigaction failureAction;	//failure action when receiving signal from overlord

int main()
{
   pthread_t overlord;
   mainID = getpid();		//main pid
   //int check = 0;
   int S_F = 0; 			
   void* status;
   sigset_t mainMask;
   sigset_t successMask;
   sigset_t failureMask;
//================file setup block=====================================
   mainfd = open("main.log", O_CREAT | O_WRONLY | O_TRUNC, 0600);//opening up logfile
   if(mainfd < 0)			//checking for success
   {
      perror("opening outfile for overlord\n");
      exit(EXIT_FAILURE);
   }
   MAINFD = mainfd;
   strcpy(mainbuf,"Starting the multithreading simulation\n\n");//simulation starting
   write(mainfd, mainbuf, strlen(mainbuf));   
//================file  setup block=====================================
//================signal mask setup block=====================================
   sigfillset(&fullmask);
   sigemptyset(&emptymask);
   sigfillset(&mainMask); 			  
   sigdelset(&mainMask, SIGUSR1); 
   sigdelset(&mainMask, SIGUSR2); 
   sigfillset(&successMask); 			  
   sigdelset(&mainMask, SIGUSR1); 
   sigfillset(&failureMask); 			  
   sigdelset(&mainMask, SIGUSR2); 
//================signal mask setup block=====================================
//================sigaction setup block=====================================
   strcpy(mainbuf,"Creating the sigactions\n");
   write(mainfd, mainbuf, strlen(mainbuf));
   strcpy(mainbuf,"Success Setup...");
   write(mainfd, mainbuf, strlen(mainbuf));
   successAction.sa_handler = successHandle;
   successAction.sa_flags = SA_RESTART;
   successAction.sa_mask = successMask;
   strcpy(mainbuf,"finished\n");
   write(mainfd, mainbuf, strlen(mainbuf));
   strcpy(mainbuf,"Failure Setup...");
   write(mainfd, mainbuf, strlen(mainbuf));
   failureAction.sa_handler = failureHandle;
   failureAction.sa_flags = SA_RESTART;
   failureAction.sa_mask = failureMask;
   strcpy(mainbuf,"finished\n");
   write(mainfd, mainbuf, strlen(mainbuf));
//================sigaction setup block=====================================
//================sigaction Ready block=====================================
   if(sigaction(SIGUSR1, &successAction, NULL) == -1)
   {
      strcpy(mainbuf,"SUCCESS WAS CAUGHT IN ACTION!\n");
      write(mainfd, mainbuf, strlen(mainbuf));
      success = 1;
   }

   if(sigaction(SIGUSR2, &failureAction, NULL) == -1)
   {
      strcpy(mainbuf,"FAILURE WAS CAUGHT IN ACTION!\n");
      write(mainfd, mainbuf, strlen(mainbuf));
      success = 0;
      exit(EXIT_FAILURE);
   }
//================sigaction Ready block=====================================
   if(sigprocmask(SIG_BLOCK,&fullmask, NULL ) < 0)
   {
      strcpy(mainbuf,"***ERROR setting up sigprocmask for main***\n");
      write(mainfd, mainbuf, strlen(mainbuf));
      success = 0;
      exit(EXIT_FAILURE);
   }

   else
   {
      strcpy(mainbuf,"Mask was successfully set up!\n");
      write(mainfd, mainbuf, strlen(mainbuf));
   }
 
   strcpy(mainbuf,"Launching overlord thead...\n");
   write(mainfd, mainbuf, strlen(mainbuf));   
//================overlord thread block=====================================
   strcpy(mainbuf,"Starting Overlord Thread...\n");	//printing message
   write(mainfd, mainbuf, strlen(mainbuf));
   if(pthread_create(&overlord, NULL, overlordThread, NULL))	//launching packing thread
   {
      strcpy(mainbuf,"***ERROR LAUNCHING OVERLORD THREAD***\n");//printing message
      write(mainfd, mainbuf, strlen(mainbuf));
   }
   strcpy(mainbuf,"SUCCESSFUL AT LAUNCHING OVERLORD THREAD\n");	//printing message
   write(mainfd, mainbuf, strlen(mainbuf));
//================end packing thread block=====================================
   strcpy(mainbuf,"Suspending until all threads are initialized\n");	//printing message
   write(mainfd, mainbuf, strlen(mainbuf));
   sigsuspend(&mainMask);
   S_F = pthread_join(overlord, &status);

   if(S_F)
   {
      strcpy(mainbuf,"ERROR RETURNING OUT OF THREAD\n");	//printing message
      write(mainfd, mainbuf, strlen(mainbuf));   
   }
   
   if(success)
   {
      strcpy(mainbuf,"Everything worked\n");	//printing message
      write(mainfd, mainbuf, strlen(mainbuf));   
   }
   
   else
   {
      strcpy(mainbuf,"There were errors\n");	//printing message
      write(mainfd, mainbuf, strlen(mainbuf));   
   }
   
   
      
   if(close(mainfd))
   {
      printf("Failed to properly close the file\n");
      exit(EXIT_FAILURE);
   }
   
   else
   {
      printf("File closed properly\n");
      exit(EXIT_SUCCESS);    
   }
}

/*
//only strict signals
int intToSigAddStrict(int sig, sigset_t * mask, overlordPayload* pl, int trigger)
{
   switch(sig)
   {
      case 1:
         sigdelset(mask, SIGHUP);
         if(trigger)
         {
            pl->sigS = 1;
            sigdelset()
         }
         break;
      case 2:
         sigdelset(mask, SIGINT);
         payloadsig = 2;
         break;
      case 3:
         sigdelset(mask, SIGQUIT);
         payloadsig = 3;
         break;
      case 4:
         sigdelset(mask, SIGILL);
         payloadsig = 4;
         break;
      case 6:
         sigdelset(mask, SIGABRT);
         payloadsig = 6;
         break;
      case 8:
         sigdelset(mask, SIGFPE);
         payloadsig = 6;
         break;
      case 11:
         sigdelset(mask, SIGSEGV);
         payloadsig = 11;
         break;
      case 13:
         sigdelset(mask, SIGPIPE);
         payloadsig = 13;
         break;
      case 14:
         sigdelset(mask, SIGALRM);
         payloadsig = 14;
         break;
      case 15:
         sigdelset(mask, SIGTERM);
         payloadsig = 15;
         break;
      default:
         return 0;
   }
   return 1;
}
*/

void successHandle(int signum)
{
   strcpy(handlerbuf,"handler: SUCCESSFUL SETUP OF THREADS!\n\n");//simulation starting
   write(MAINFD, handlerbuf, strlen(handlerbuf));
   success = 1;  
}

void failureHandle(int signum)
{
   strcpy(handlerbuf,"***ERROR STARTING THREADS***\n\n");//simulation starting
   write(MAINFD, handlerbuf, strlen(handlerbuf));
   success = 0;   
}
/*
===================|
Overlord Thread    |
===================|
-Overlord thread is passed a pointer to am array with two slots
 	slot 0 = id of calling process
  	slot 1 = signal to send to calling process/thread as an integer
		 will be processed to the correct signal in the thread

-Caller process or thread suspends self waits for signal passed to overlord thread
-Overlord launches workers, cleanup, packing thread and shared memory segment
-After all threads are ready, the overlord sends the signal back to the 
 main thread or process and notifies everything is ready

COMMUNICATION BETWEEN THREADS
---------------------------------
THREAD |to|  THREAD  |  SIGNAL  |
---------------------------------
PACKING ->  OVERLORD =  SIGUSR1 |
WORKER	->  CLEANUP  =  SIGUSR1 |      
CLEANUP ->  OVERLORD =  SIGUSR2 |
OVERLORD->  WORKER   =  SIGUSR1 |
OVERLORD->  MAIN     =  SIGUSR2 |
MAIN    -> OVERLORD  = "userdef"|
---------------------------------
*/
void* overlordThread(void* msg)
{
  
   int i = 0;
   overlordID = getpid();		//getting pid into the global pid variable 
   char overlordbuf[100];		//creating a bugger
   pthread_t packing;   		//packing thread
   //pthread_t cleanup;			//cleanup thread
   //pthread_t workers[WORKER_NUM];	//worker threads
   //struct sigaction terminateAction;	//terminates program
   sigset_t overlordMask;
   
   overlordfd = open("overlord.log", O_CREAT | O_WRONLY | O_TRUNC, 0600);//opening file
   if(overlordfd < 0)			//if file failed ot open
   {
      perror("Failed opening outfile for overlord\n");
      success = 0;
      kill(mainID, SIGUSR2);
      exit(0);
   }
   
   if(sigprocmask(SIG_BLOCK,&fullmask, NULL ) < 0)
   {
      strcpy(mainbuf,"***ERROR setting up sigprocmask for main***\n");
      write(mainfd, mainbuf, strlen(mainbuf));
      success = 0;
      exit(EXIT_FAILURE);
   }
   
   sigfillset(&overlordMask);
   sigdelset(&overlordMask, SIGABRT);
   //terminateAction.sa_handler = overlordHandle;
   //terminateAction.sa_mask = overlordMask;
   //terminateAction.sa_flags = SA_RESTART; 
    
   
   strcpy(overlordbuf,"Overlord is running...\n\n");	//printing out message		
   write(overlordfd, overlordbuf, strlen(overlordbuf));
   
   if(sigaction(SIGUSR1, &successAction, NULL) == -1)
   {
      strcpy(mainbuf,"SUCCESS WAS CAUGHT IN ACTION!\n");
      write(overlordfd, overlordbuf, strlen(overlordbuf));
      success = 1;
   }
/* 
   getcwd(pathname,200);		//creating a key for the semaphores
   strcat(pathname, "foo/");
   ipckey = ftok(pathname,42);
   semid = semget(ipckey, semnum, 0666 | IPC_CREAT);
 
   if(semid < 0)			//checking if the semaphore id was created
   {
      strcpy(overlordbuf,"Failed creating the semaphore\n");
      write(overlordfd, overlordbuf, strlen(overlordbuf));
      kill(mainID, SIGUSR2);
      _exit(1);
   }   
*/
   strcpy(overlordbuf,"Semaphores were correctly created\nLaunching other threads:\n");//printing message
   write(overlordfd, overlordbuf, strlen(overlordbuf));

//================packing thread block=====================================
   strcpy(overlordbuf,"Launching Packing Thread...\n");	//printing message
   write(overlordfd, overlordbuf, strlen(overlordbuf));
   if(pthread_create(&packing, NULL, packingThread, NULL))	//launching packing thread
   {
      strcpy(overlordbuf,"***ERROR LAUNCHING PACKING THREAD***\n");//printing message
      write(overlordfd, overlordbuf, strlen(overlordbuf));
      exit(EXIT_FAILURE);
   }
   strcpy(overlordbuf,"SUCCESSFUL AT LAUNCHING PACKING THREAD\n");	//printing message
   write(overlordfd, overlordbuf, strlen(overlordbuf));
//================end packing thread block=====================================

//================cleanup thread block=====================================
   strcpy(overlordbuf,"Launching Cleanup Thread...\n");	//printing message
   write(overlordfd, overlordbuf, strlen(overlordbuf));
   if(pthread_create(&packing, NULL, cleanupThread, NULL))	//launching cleanup
   {
      strcpy(overlordbuf,"***ERROR LAUNCHING CLEANUP THREAD***\n");//printing message
      write(overlordfd, overlordbuf, strlen(overlordbuf));
      exit(EXIT_FAILURE);
   }
   strcpy(overlordbuf,"SUCCESSFUL AT LAUNCHING CLEANUP THREAD\n");	//printing message
   write(overlordfd, overlordbuf, strlen(overlordbuf));
//================end cleanup thread block=====================================

//================worker thread block=====================================
   strcpy(overlordbuf,"Launching ALL Worker Threads...\n");//printing message
   write(overlordfd, overlordbuf, strlen(overlordbuf));	
   for(i = 0; i < WORKER_NUM; i++)			//launching worker thread
   {
      if(pthread_create(&packing, NULL, workerThread, NULL))
      {
         strcpy(overlordbuf,"***ERROR LAUNCHING ONE OF THE PACKING THREAD***\n");//printing message
         write(overlordfd, overlordbuf, strlen(overlordbuf));
         exit(EXIT_FAILURE);
      } 
   }
   strcpy(overlordbuf,"SUCCESSFUL AT LAUNCHING CLEANUP THREAD\n");	//printing message
   write(overlordfd, overlordbuf, strlen(overlordbuf));
//================end worker thread block=====================================
   
   strcpy(overlordbuf,"All threads launched successfully\nSignaling caller\n");	//printing message
   write(overlordfd, overlordbuf, strlen(overlordbuf));
   success = 1;
   kill(mainID, SIGUSR1);
   strcpy(overlordbuf,"Overlord is exiting\n");	//printing message
   write(overlordfd, overlordbuf, strlen(overlordbuf));
   pthread_exit(0);
//   kill(overlordID,SIGSTOP);	//stop self
/*
   strcpy(overlordbuf,"Overlord woke up!\n\n");
   write(overlordfd, overlordbuf, strlen(overlordbuf)); 

   sigset_t overlordmask;
   sigfillset(&overlordmask);
   sigdelset(&overlordmask,SIGUSR1);   
   
   struct sigaction overlordaction;
   overlordaction.sa_handler = overlordNewPacketHandler;
   overlordaction.sa_flags = SA_RESTART;
   overlordaction.sa_mask = overlordmask;
*/   
}


void overlordHandle(int signum)
{
   char buf[100];
   strcpy(buf,"***ERROR STARTING THREADS***\n\n");//simulation starting
   write(overlordfd, buf, strlen(buf));
    
}

/*
---------------------------------
THREAD |to|  THREAD  |  SIGNAL  |
---------------------------------
OVERLORD->  MAIN     =  SIGUSR1 |SUCCESS
OVERLORD->  MAIN     =  SIGUSR2 |FAILURE
MAIN    ->  PACKING  =  SIGUSR1 |
MAIN    ->  OVERLORD  = SIGABRT |
PACKING ->  OVERLORD =  SIGUSR1 |
CLEANUP ->  OVERLORD =  SIGUSR2 |
WORKER	->  CLEANUP  =  SIGUSR1 |      
OVERLORD->  WORKER   =  SIGUSR1 |
---------------------------------
*/

void * packingThread(void* msg)
{   
   packingID = getpid();
   kill(packingID,SIGSTOP);
   char packingbuf[100];		//creating a bugger
   struct sigaction packingAction;
   sigset_t packingMask;
   sigfillset(&packingMask);
   sigdelset(&packingMask, SIGUSR1);
 
   /*
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
   */
   
   packingfd = open("packing.log", O_CREAT | O_WRONLY | O_TRUNC, 0600);
   if(packingfd < 0)
   {
      perror("opening outfile for packing\n");
      kill(mainID, SIGUSR2);
      exit(0);
   }
   
   if(sigprocmask(SIG_BLOCK,&fullmask, NULL ) < 0)
   {
      strcpy(packingbuf,"***ERROR setting up sigprocmask for packing***\n");
      write(packingfd, packingbuf, strlen(packingbuf));
      kill(mainID, SIGUSR2);
   }
   
   
   strcpy(packingbuf,"Packing thread is running...\n");//simulation starting
   write(packingfd, packingbuf, strlen(packingbuf));
   
   packingAction.sa_handler = packingHandle;
   packingAction.sa_flags = SA_RESTART;
   packingAction.sa_mask = packingMask;
   
//================sigaction Ready block=====================================
   if(sigaction(SIGUSR1, &packingAction, NULL) == -1)
   {
      strcpy(packingbuf,"FAILURE WAS CAUGHT IN ACTION AT PACKING!\n");
      write(packingfd, packingbuf, strlen(packingbuf));
      success = 0;
   }
//================sigaction Ready block=====================================
   
   /* TODO: return something? */
   return NULL;
}

void packingHandle(int sig)
{
   //char handlebuf[100];
}

void* workerThread(void* msg)
{
    //int id = (int)(msg);
    /* TODO: WTF */
    return NULL;
}

void * cleanupThread(void* msg)
{
   cleanupID = getpid();
   kill(cleanupID,SIGSTOP);
   
   /*
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
   */
   
   
   cleanupfd = open("cleanup.log", O_CREAT | O_WRONLY | O_TRUNC, 0600);
   if(cleanupfd < 0)
   {
      perror("opening outfile for packing\n");
      exit(0);
   }  

   return NULL; /* TODO: or something else? */
}

