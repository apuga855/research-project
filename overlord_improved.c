#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>



char pathname[200];		//pathname for generating ipckey
key_t ipckey;			//ipckey

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
void* workerThread(void*);
void packingHandle(int);
void cleanupHandle(int);
sigset_t fullmask;		//all signals
sigset_t emptymask;		//no signals

pthread_t packing;
pthread_t overlord;
pthread_t worker;

struct sigaction successAction;
struct sigaction failureAction;

int init()
{
    void* status;
    sigset_t mainMask;
    sigset_t successMask;
    sigset_t failureMask;

    mainfd = open("main.log", O_CREAT | O_WRONLY | O_TRUNC, 0600);
    if(mainfd < 0)
    {
        perror("opening outfile for overlord\n");
        exit(EXIT_FAILURE);
    }
    MAINFD = mainfd;

    sigfillset(&fullmask);
    sigemptyset(&emptymask);

    sigfillset(&mainMask); 			  
    sigdelset(&mainMask, SIGUSR1); 
    sigdelset(&mainMask, SIGUSR2); 

    sigfillset(&successMask); 			  
    sigdelset(&successMask, SIGUSR1); 

    sigfillset(&failureMask); 			  
    sigdelset(&failureMask, SIGUSR2); 

    successAction.sa_handler = successHandle;
    successAction.sa_flags = SA_RESTART;
    successAction.sa_mask = successMask;

    failureAction.sa_handler = failureHandle;
    failureAction.sa_flags = SA_RESTART;
    failureAction.sa_mask = failureMask;

    if(sigaction(SIGUSR1, &successAction, NULL) == -1)
    {
        success = 1;
    }

    if(sigaction(SIGUSR2, &failureAction, NULL) == -1)
    {
        success = 0;
        exit(EXIT_FAILURE);
    }
        
    if(sigprocmask(SIG_BLOCK, &fullmask, NULL) < 0)
    {
        success = 0;
        exit(EXIT_FAILURE);
    }

    if(pthread_create(&overlord, NULL, overlordThread, NULL)) {
        exit(EXIT_FAILURE);
    }

    sigsuspend(&mainMask);

    if (pthread_join(overlord, &status)) {
        exit(EXIT_FAILURE);
    }

    if(close(mainfd)) {
        printf("Failed to properly close the file\n");
        exit(EXIT_FAILURE);
    } 
    else {
        printf("File closed properly\n");
        exit(EXIT_SUCCESS);
    }
}

void successHandle(int signum)
{
    puts("handler: SUCCESSFUL SETUP OF THREADS!\n");
    success = 1;  
}

void failureHandle(int signum)
{
    puts("***ERROR STARTING THREADS***");
    success = 0;   
}

void* overlordThread(void* msg)
{
    sigset_t overlordMask;

    overlordfd = open("overlord.log", O_CREAT | O_WRONLY | O_TRUNC, 0600);
    if(overlordfd < 0)
    {
        perror("Failed opening outfile for overlord\n");
        success = 0;
        kill(getpid(), SIGUSR2);
        //exit(0);
    }

    if(sigprocmask(SIG_BLOCK,&fullmask, NULL ) < 0) {
        exit(EXIT_FAILURE);
    }

    sigfillset(&overlordMask);
    sigdelset(&overlordMask, SIGABRT);

    puts("Overlord is running...");

    if(sigaction(SIGUSR1, &successAction, NULL) == -1)
    {
        exit(EXIT_FAILURE);
    }

    if(pthread_create(&worker, NULL, workerThread, NULL))
    {
        exit(EXIT_FAILURE);
    } 

    puts("Launching Packing Thread...");
    if (pthread_create(&packing, NULL, packingThread, NULL))
    {
        exit(EXIT_FAILURE);
    }
    success = 1;
    //pthread_kill(overlord, SIGUSR1);
    puts("Overlord is exiting");
    pthread_exit(0);
    
    //   pthread_kill(gettid(),SIGSTOP);	//stop self
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
    puts("***ERROR STARTING THREADS***");
}

void * packingThread(void* msg)
{   
    struct sigaction packingAction;
    sigset_t packingMask;

    packingAction.sa_handler = packingHandle;
    packingAction.sa_flags = SA_RESTART;
    packingAction.sa_mask = packingMask;

    sigfillset(&packingMask);
    sigdelset(&packingMask, SIGUSR1);

    packingfd = open("packing.log", O_CREAT | O_WRONLY | O_TRUNC, 0600);
    if(packingfd < 0)
    {
        perror("opening outfile for packing\n");
        kill(getpid(), SIGUSR2);
        exit(0);
    }

    if(sigprocmask(SIG_BLOCK,&fullmask, NULL ) < 0)
    {
        perror("***ERROR setting up sigprocmask for packing***");
        kill(getpid(), SIGUSR2);
    }

    sigsuspend(packingMask);

    if(sigaction(SIGUSR1, &packingAction, NULL) == -1)
    {
        puts("FAILURE WAS CAUGHT IN ACTION AT PACKING!");
        success = 0;
    }
    return NULL;
}

void packingHandle(int sig)
{

}

void* workerThread(void* msg)
{
    return NULL;
}

void cleanupHandle(int sig) {
{
    cleanupfd = open("cleanup.log", O_CREAT | O_WRONLY | O_TRUNC, 0600);
    if(cleanupfd < 0)
    {
        perror("opening outfile for packing\n");
        exit(0);
    }  

    return NULL;
}

int main(void) {
    init();
}
