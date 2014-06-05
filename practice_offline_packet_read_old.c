#include<pcap.h>
#include<netinet/ip.h>
#include<netinet/in.h>
#include<netinet/tcp.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include<string.h>
#include"hashTableLinkedList.h"
#include"packet.h" 


//Fragmentation notes:
//this is assigning the address located at the target* address of the packet plust the size of the
//structure that contained the ethernet header, since the packet is basivally just a gigantic
//array of bytes, then all we have to do is parse through it like we would an array, which is
//by offsets, theregore what we get, is the ipheader
//get rid of all bits, except for the wanted one, aka MF bit 0x20 = 0010 0000 which is 1 byte which is the size of a unsigned character


//starting from here, is all necessary materials and tools for ---------------------
//packet capture without reassembly
//need to typedef it knows its a type

int myhashfunc(LH_hashTable*, void *);
//end of packet capture without reassembly--------------------------------------


//reassembly variable section and tools---------------------------------------//
sensorRdyPckt* pcktBuffer = NULL;

int h = 0;							//counter
int thrdCnt = 0;

//this structure is going to hold all information
//required for the reassembly

LH_hashTable* sharedMemSeg = NULL;

int main(int argc, char **argv)
{	

   if (argc != 2) {
     fprintf(stderr, "Usage: %s <pcap file>\n", *argv);
     return 1;
   }
   pcap_t *file = NULL;		    //this will be the actual structure we use
   char errbuf[PCAP_ERRBUF_SIZE];   //required for the opening in offline mode, gives us error data
   char *inputName = argv[1]; //get filename from command line argument
   pcktBuffer = malloc(sizeof(sensorRdyPckt) * P_BUFF);
   for(;h < P_BUFF; h++)				//defaulting all the packets
      packet_init(&pcktBuffer[h]);

   sharedMemSeg = LH_HashTableAllocSetBuff(300, pcktAlloc);
   
   thrdCnt = 0;



   //loop that creates each thread, we pass the address of specified slots in or thread array,
   //we specify the function vody of the thread in this case reassembly and finally we cast
   //our structure slot as a void pointer and we pass it to the thread
   /*for(; thrdCnt < THREAD_ARRAY_SIZE; thrdCnt++)
   {
      if(pthread_create(&rsmvThrd[thrdCnt], NULL, reassembly, (void*)rsmvPcktArr[thrdCnt]) != 0)
      {
         printf("There was an error allocating the threads, exiting");
         return 1;
      }
      rsmvPcktThrd_set(&rsmvPcktThrdArr[thrd], thrd, THREAD_DEFAULT_TIMEOUT, &rsmvPcktArr[thrd], &rsmvThrd[thrd]);
   }
*/
   //attempting to open up the file in pcap format with the given name and errorbuf for printing out an error
   file = pcap_open_offline(inputName, errbuf);
   //if we were not able to open up the file, we exit
   if(file == NULL){printf("pcap_open_offline failed %s \n", errbuf); return 1;}
   //start the pcap main loop, note, the callback function is passed to it
   //to be more precise we give the descriptor, 0 as limit of packets we want to analyze, in this case
   //0 is actually no limit, the callback function is called packetHandler, NULL is on the parameter
   //where we could place some user defined data to the callback

   if(pcap_loop(file, 0, packet_handler, NULL))
      {printf("pcap main loop failed %s \n", pcap_geterr(file)); return 1;}
   //this tells us we are done with the main loop
   printf("Done with main loop\n");
   free(pcktBuffer);
   if(LH_HashTableDel(sharedMemSeg))
      printf("Successful delete of everything\n");
   else
      printf("Problem deallocating\n");

   return 0;

}

//packet handling function with the requirements given to us by libcap

void packet_handler(u_char* usrData,const struct pcap_pkthdr* pcktHeader, const u_char* pckt)
{
    //holy crap this gets crazy really quickly
    //this is a constant structure pointer of type ether_header nicknamed ethernetHeader
    //this structure actually comes predefined, take a look at it for all the crazy crap 
    //you can do with it
    //char contnue = '\n';
    //constant structure pointer of type ip nicknamed ipHeader also predefined for us
    //constant structure pointer of type tcphdr nicknamed tcpHeader also predefined for us
    //const u_char* data;	//data segment pointer of unsigned characters
    //int dataLength = 0;	//data length in bytes from the packet
    //int iteration = 0;		//one of my counters
    //pcktArray = malloc(SENSOR_ARRAY_PACKET_LENGTH * sizeof(sensorRdyPckt));
    sensorRdyPckt SRD_pckt;//this is the sensor ready packet info
    packet_init(&SRD_pckt);//initializing the new packet that was allocated    
    //there are a lot of defines inside of the ethernet header, we are checking here if the ETHERTYPE is IP 
    //if it is, then we can start the parsing of the packet
    //the ntohs function changes the the byte orders to the current hosts byte ordering scheme, then we go in
    //and compare the ether type unsigned short char to the ETHERTYPE_IP define 
    
    parsePacket(&SRD_pckt, pckt, pcktHeader->len);
    printPacket(&SRD_pckt);
    // FIXME: validate lengths of things before using them as offsets
    
           /*
    if (dataLength > 0)					//check if our packet was not empty, it could be, maybe 
    {
        if(LH_hashFunc(sharedMemSeg,(void*)SRD_pckt, myhashfunc,SRDPcktCpy, pcktAlloc) != -1)
            printf("Successfully hashed\n");
        else
            printf("Problem hashing\n");

        SRDTrimHdr(SRD_pckt);
        SRDTrimPld(SRD_pckt);
        SRDPcktHdrPldPrint(SRD_pckt);
        printf("Writting files\n\n");
        SRDPcktPldToFile(SRD_pckt);
        SRDPcktHdrToFile(SRD_pckt);
        fngPntPrint(SRD_pckt);
    }

    //printf("Finished with the first packet from file, would you like to continue? (y/n)\n");
    //scanf("%c", &contnue);

    //if(contnue == 'n'){exit(0);}
    */

}

//payload to file

//The hash function
int myhashfunc(LH_hashTable* table, void * data)
{
   int slot = 0;
   int rslot = 0;
   int i = 1;
   sensorRdyPckt* p = (sensorRdyPckt*) data;
   slot = (p->ip.id) % (kprimeCap(table->LH_primenums));

   printf("\nAttempting to hash %u\n",p->ip.id);

   if(LlistIsEmpty(table->LH_table[slot].LHN_list))
      return slot;

   else
   {
      rslot = (p->ip.id) % (kprimehash2(table->LH_primenums));

      if(rslot == 0 && slot == 0)
          rslot = (p->ip.id + 13) % (kprimehash2(table->LH_primenums)); 
      do
      {
         slot = (slot + (i * rslot)) % (table->LH_capacity);
         i++;
      }while(!LlistIsEmpty(table->LH_table[slot].LHN_list));
   }
   return slot;
}

