#include<pcap.h>
#include<net/ethernet.h>
#include<netinet/ip.h>
#include<netinet/in.h>
#include<netinet/tcp.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include"hashTableLinkedList.h"
#define P_BUFF 100
#define THREAD_ARRAY_SIZE 10
#define THREAD_DEFAULT_TIMEOUT 1000
#define INPUT_NAME_SIZE 20
#define PAYLOAD_BUF_SIZE 1024
#define SENSOR_ARRAY_PACKET_LENGTH 20
#define etherLen   sizeof(struct ether_header)
#define ipLen  sizeof(struct ip)
#define tcpLen  sizeof(struct tcphdr)
#define fngPntLen 13

//starting from here, is all necessary materials and tools for ---------------------
//packet capture without reasembly
//need to typedef it knows its a type
typedef struct _sensorRdyPckt
{
   unsigned int SRD_pld_len;			//payload length
   unsigned int SRD_hdr_len;			//header length
   uint16_t SRD_id;				//I am probably going to place the parsed packets in a hash table, so I am placing an ID just in case
   uint32_t SRD_sqnsnum;		//sequence number
   uint32_t SRD_src_addr;		//source address	
   uint32_t SRD_dst_addr;		//destination address
   uint16_t SRD_src_prt;		//source port
   uint16_t SRD_dst_prt;  		//destination port
   uint16_t SRD_total_len;		//total length
   uint16_t SRD_total_frag_offset;	//fragment offset
   unsigned char *SRD_httpHdr;		//http header
   unsigned char *SRD_pld;		//c does not have a default byte primitive, I am going to be using a character array to simulate it
					//since it is actually a single byte anyway
   int *fngPnt;				//fingerprint

/*FINGER PRINT EXPLANATION:
 * array of integers, each slot contains a specified number (integer) that represents the character count 
 *INDEX 0               HTTP command    GET = 1         POST = 2        HEAD = 4        OTHER = 8
 *INDEX 1               HTTP PROTOCOL   0.9 = 1         1.0 = 1         1.1 = 4         OTHER = 8       
 *INDEX 2               LENGTH          # OF CHARS              
 *INDEX 3               VARIABLES       # OF INPUT
 *INDEX 4               PERCENT         # OF %
 *INDEX 5               APOS            # OF ' 
 *INDEX 6               PLUS            # OF +          
 *INDEX 7               CDOT            # OF .. 
 *INDEX 8               BACKSLASH       # OF \
 *INDEX 9               OPAREN          # OF (
 *INDEX 10              CPAREN          # OF )
 *INDEX 11              FORWARD         # OF //
 *INDEX 12              LT              # OF <
 *INDEX 13              GT              # OF >
 */

}sensorRdyPckt;



typedef struct _rsmvPcktThrd
{
   int rsmv_id;
   int rsmv_timeout;
   sensorRdyPckt* rsmv_pckt;
   sensorRdyPckt* next_rsmv_pckt;
   pthread_t* rsmv_thrd;
}rsmvPcktThrd;

//function for printing out only packet header and payload
void SRDPcktHdrPldPrint(sensorRdyPckt *);
//packet printer, only relevant information
void SRDPcktPrint(sensorRdyPckt*);
//callback function
void packet_handler(u_char *,const struct pcap_pkthdr*, const u_char*);
//defaulter function
void packet_default(sensorRdyPckt*);
//header to file
void SRDPcktPldToFile(sensorRdyPckt*);
//payload to file
void SRDPcktHdrToFile(sensorRdyPckt*);
//create the firnger print
int *SRDPcktFngrPnt(unsigned char *, int);
//print fingerprint
void fngPntPrint(sensorRdyPckt*);
//trim function
unsigned char* SRDTrim(unsigned char *, int);
//trim header
void SRDTrimHdr(sensorRdyPckt*);
//trim payload
void SRDTrimPld(sensorRdyPckt*);
//formater for the fingerprint
void formatFngPntPrint(int*);
void* pcktAlloc();
int myhashfunc(LH_hashTable*, void *);
void SRDPcktCpy(sensorRdyPckt *, sensorRdyPckt *);
//end of packet capture without reasembly--------------------------------------


//reasembly variable section and tools---------------------------------------//
sensorRdyPckt* pcktBuffer = NULL;
//rsmvPcktThrd *rsmvPcktThrdArr = NULL;
sensorRdyPckt *rsmvPcktArr = NULL;

int h = 0;							//counter
int thrdCnt = 0;

//this structure is going to hold all information
//required for the reasembly

//structure defaulter
void rsmvPcktThrd_default(rsmvPcktThrd*);
//setter function
void rsmvPcktThrd_set(rsmvPcktThrd*,int, int, sensorRdyPckt*, pthread_t*);

//sensorRdyPckt *SRD_pckt = NULL;//this is the sensor ready packet info
//rsmvPcktThrd* rsmvPcktThrdArr = NULL;
//sensorRdyPckt* rsmvPcktArr = NULL;
pthread_t* rsmvThrd = NULL; 
LH_hashTable* sharedMemSeg = NULL;
//rsmvPcktThrd *rsmvPcktThrdArr = malloc(sizeof(rsmvPcktThrd) * THREAD_ARRAY_SIZE);
//sensorRdyPckt *rsmvPcktArr = malloc(sizeof(sensorRdyPckt) * THREAD_ARRAY_SIZE);
//for(; thrdCnt < THREAD_ARRAY_SIZE; thrdCnt++)
//   packet_default(rsmvPcktArr[thrdCnt]);

//int thrdCnt = 0;
//rsmvThrd = malloc(sizeof(pthread_t) * THREAD_ARRAY_SIZE);

//start of functions for reasembly
void *reasembly(void*);
//end of functions for reasembly
//end of reasembly variables and tools
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
      packet_default(&pcktBuffer[h]);

   pthread_t* rsmvThrd = NULL; 
   sharedMemSeg = LH_HashTableAllocSetBuff(300, pcktAlloc);
   //rsmvPcktThrd *rsmvPcktThrdArr = malloc(sizeof(rsmvPcktThrd) * THREAD_ARRAY_SIZE);
  // sensorRdyPckt *rsmvPcktArr = malloc(sizeof(sensorRdyPckt) * THREAD_ARRAY_SIZE);
   //for(; thrdCnt < THREAD_ARRAY_SIZE; thrdCnt++)
     // packet_default(rsmvPcktArr[thrdCnt]);
   
   thrdCnt = 0;
   rsmvThrd = malloc(sizeof(pthread_t) * THREAD_ARRAY_SIZE);



   //loop that creates each thread, we pass the address of specified slots in or thread array,
   //we specify the function vody of the thread in this case reasembly and finally we cast
   //our structure slot as a void pointer and we pass it to the thread
   /*for(; thrdCnt < THREAD_ARRAY_SIZE; thrdCnt++)
   {
      if(pthread_create(&rsmvThrd[thrdCnt], NULL, reasembly, (void*)rsmvPcktArr[thrdCnt]) != 0)
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
    int fragFlag = 0;
    //holy crap this gets crazy really quickly
    //this is a constant structure pointer of type ether_header nicknamed ethernetHeader
    //this structure actually comes predefined, take a look at it for all the crazy crap 
    //you can do with it
    char contnue = '\n';
    const struct ether_header* ethernetHeader;
    //constant structure pointer of type ip nicknamed ipHeader also predefined for us
    const struct ip* ipHeader;
    //constant structure pointer of type tcphdr nicknamed tcpHeader also predefined for us
    const struct tcphdr* tcpHeader;
    u_char* data;	//data segment pointer of unsigned characters
    int dataLength = 0;	//data length in bytes from the packet
    u_char* dataStr = NULL;	//unsigned char pointer that we are going to use 
    int iteration = 0;		//one of my counters
    int pldC = 0;		//payload counter
    int httpHdrC = 0;	 	//header counter
    //int unprint = 0;
    int unprintCount = 0;
    int slot = 0;
    sensorRdyPckt *SRD_pckt = NULL;//this is the sensor ready packet info
    //pcktArray = malloc(SENSOR_ARRAY_PACKET_LENGTH * sizeof(sensorRdyPckt));
    SRD_pckt = malloc(sizeof(sensorRdyPckt));	//allocating a SRD_pckt
    packet_default(SRD_pckt);//initializing the new packet that was allocated    
    //for(; i < SENSOR_ARRAY_PACKET_LENGTH; i++)
    //   packet_default(&SRD_pckt);       
    //i = 0;
    ethernetHeader = (struct ether_header*)pckt;		//attempting to cast as an ether_header structure, which contains the fields of the packet
    //there are a lot of defines inside of the ethernet header, we are checking here if the ETHERTYPE is IP 
    //if it is, then we can start the parsing of the packet
    //the ntohs function changes the the byte orders to the current hosts byte ordering scheme, then we go in
    //and compare the ether type unsigned short char to the ETHERTYPE_IP define 

    if (ntohs(ethernetHeader->ether_type) == ETHERTYPE_IP) 	
    {  

       u_char moreFrag;			//checking for more fragments
       moreFrag = *(pckt + etherLen + 6);		//getting the more fragment bit
       //moreFrag = *(pckt + etherLen + 50);		//getting the more fragment bit
       //this is assigning the address located at the target* address of the packet plust the size of the
       //structure that contained the ethernet header, since the packet is basivally just a gigantic
       //array of bytes, then all we have to do is parse through it like we would an array, which is
       //by offsets, theregore what we get, is the ipheader
       u_char mask = 0x20;//get rid of all bits, except for the wanted one, aka MF bit 0x40 = 0100 0000 which is 1 byte which is the size of a unsigned character
       moreFrag = moreFrag & mask; 
       ipHeader = (struct ip*)(pckt + etherLen);
       if(moreFrag > 0 || ipHeader->ip_off != 0)//checking for fragmentation
       {
          fragFlag = 1;
          printf("The flag was caught, the value for MF is %d The offset is %d",
                (int) fragFlag, (int)ipHeader->ip_off);
       }

       SRD_pckt->SRD_src_addr = ntohl(ipHeader->ip_src.s_addr);//getting the source address in net to host endian mode, storing it into the SRD packet
       SRD_pckt->SRD_dst_addr = ntohl(ipHeader->ip_dst.s_addr);//getting the destination address in net to host endian mode, storing it into the SRD packet
       SRD_pckt->SRD_total_len = ntohl(ipHeader->ip_len);	       //total length
       SRD_pckt->SRD_id = ipHeader->ip_id;
        
       if(ipHeader->ip_p == IPPROTO_TCP)		      //checking if it has an tcp header
       {
          tcpHeader = (struct tcphdr*)(pckt + etherLen+ ipLen);   //casting the tcp header start from the packet into a tcphdr pointer so we can actually get members out
          SRD_pckt->SRD_src_prt = ntohs(tcpHeader->source);	  //casting the source port in net to host endian mode, storing it into the SRD packet
          SRD_pckt->SRD_dst_prt = ntohs(tcpHeader->dest);          //casting the destination port int net to host endian mode, storing it into the SRd packet
          SRD_pckt->SRD_sqnsnum = ntohl(tcpHeader->seq);           //storing the sequence number in the SRD packet in net to host endian mode
          data = (u_char*)(pckt + etherLen + ipLen + tcpLen);     //get the data segment start from the packet 
          dataLength = pcktHeader->len - (etherLen + ipLen + tcpLen);//the data length in bytes from the pcktHeader structure,
                                                                     // we subtract the ether length and ip length and tcp length to know the 
	  							     //data segment length, its all in bytes
          dataStr = malloc(dataLength * sizeof(u_char));             //allocate enough space to hold the data segment in our dataStr pointer
          for(; iteration < dataLength; iteration++) 		     //parsing loop
          {  
	    if((data[iteration] >= 32 && data[iteration] <= 126) || data[iteration] == 10 || data[iteration] == 11 || data[iteration] == 13) //printable characters are copied   
	     {
		 dataStr[iteration - unprintCount] = (u_char)data[iteration];
		 //unprint = 1;
	     }

            else 
	          unprintCount++;

	     if(!SRD_pckt->SRD_httpHdr && data[iteration] == '\r' &&
                 data[iteration + 1] == '\n' &&
                 data[iteration + 2] == '\r' &&
                 data[iteration + 3] == '\n')//this checks for when the http header ends
             {
                 slot = 0;
		 iteration = iteration + 3;
                 httpHdrC += unprintCount;
		 SRD_pckt->SRD_httpHdr = malloc((iteration) * sizeof(u_char));//set the SRD packet header into the correct size
		 for(; httpHdrC < iteration; httpHdrC++, slot++) //copy all characters from the packet header into the SRD packet header section
		    SRD_pckt->SRD_httpHdr[slot] = (u_char)dataStr[slot];
		 SRD_pckt->SRD_hdr_len = httpHdrC - unprintCount;					//setting packet header length
	     } 
          }

	  if(httpHdrC == dataLength)//if the packet length is the same as the packet header length then this does not have a payload
	     SRD_pckt->SRD_pld = NULL;
          else
          {
             SRD_pckt->SRD_pld = malloc((dataLength - httpHdrC) * sizeof(u_char)); //otherwise allocate the payload size and get the characters remaining	 
	     pldC = httpHdrC + 1;							  //start after the http header 
             slot = 0;
             for(; pldC < iteration; pldC++, slot++)					  //passing copying  
 	        SRD_pckt->SRD_pld[slot] = dataStr[slot];			
	     SRD_pckt->SRD_pld_len = pldC - httpHdrC;//setting payload length
          }
       } 	  
     }

     if (dataLength > 0)					//check if our packet was not empty, it could be, maybe 
     {
        if(LH_hashFunc(sharedMemSeg,(void*)SRD_pckt, myhashfunc,SRDPcktCpy, pcktAlloc))
           printf("Successfully hashed\n");
        else
           printf("Problem hashing\n");
      
        SRDTrimHdr(SRD_pckt);
	SRDTrimPld(SRD_pckt);
        printf("Printing Results\n\n");				//printing the results
        SRDPcktPrint(SRD_pckt);
        SRDPcktHdrPldPrint(SRD_pckt);
        printf("Writting files\n\n");
        SRDPcktPldToFile(SRD_pckt);
        SRDPcktHdrToFile(SRD_pckt);
        fngPntPrint(SRD_pckt);
     }

    //printf("Finished with the first packet from file, would you like to continue? (y/n)\n");
    //scanf("%c", &contnue);

    //if(contnue == 'n'){exit(0);}

}



//printing function

void SRDPcktPrint(sensorRdyPckt *curPckt)
{
   printf("Printing packet #%u\nSource Address: %d\nDestination Address: %d\nSource Port: %d\nDestination Port: %d\n" 
	  ,curPckt->SRD_id, curPckt->SRD_src_addr, curPckt->SRD_dst_addr, curPckt->SRD_src_prt, curPckt->SRD_dst_prt);
}



//payload and header printer
void SRDPcktHdrPldPrint(sensorRdyPckt *curPckt)
{
   if(curPckt->SRD_httpHdr == NULL)
      printf("Http header was empty, not printing it\n");
   else
      printf("\nHttp Header\n%s\n",curPckt->SRD_httpHdr);

   if(curPckt->SRD_pld == NULL)
      printf("Payload was empty, not printing it\n");
   else
      printf("\nFull Payload:\n%s\n", curPckt->SRD_pld);
}



//blanking function
void packet_default(sensorRdyPckt *curPckt)
{
   curPckt->SRD_total_frag_offset = 0;
   curPckt->SRD_total_len = 0;
   curPckt->SRD_pld_len = 0;
   curPckt->SRD_hdr_len = 0;
   curPckt->SRD_id = 0;
   curPckt->SRD_sqnsnum = 0;
   curPckt->SRD_src_addr = 0;	
   curPckt->SRD_dst_addr = 0;
   curPckt->SRD_src_prt = 0 ;
   curPckt->SRD_dst_prt = 0;
   curPckt->SRD_httpHdr = NULL;
   curPckt->SRD_pld = NULL;
   curPckt->fngPnt = NULL;
}


void SRDPcktCpy(sensorRdyPckt *src, sensorRdyPckt *dst)
{
   dst->SRD_total_frag_offset = src->SRD_total_frag_offset;
   dst->SRD_total_len = src->SRD_total_len;
   dst->SRD_pld_len = src->SRD_pld_len;
   dst->SRD_hdr_len = src->SRD_hdr_len;
   dst->SRD_id = src->SRD_id;
   dst->SRD_sqnsnum = src->SRD_src_addr;	
   dst->SRD_dst_addr = src->SRD_dst_addr;
   dst->SRD_src_prt = src->SRD_src_prt;
   dst->SRD_dst_prt = src->SRD_dst_prt;
   dst->SRD_httpHdr = src->SRD_httpHdr;
   dst->SRD_pld = src->SRD_pld;
   dst->fngPnt = src->fngPnt;
}

//payload to file

void SRDPcktPldToFile(sensorRdyPckt* curPckt)
{
  if(curPckt->SRD_pld == NULL)
  {
      printf("\npayload is empty, not writting file\n");
      return;
  }

  FILE *pldFile = fopen("payload", "w");
  if(pldFile == NULL)
  {
     printf("\n*****Failed to open up the payload file*****\n");
     return;
  }

  fprintf(pldFile, "%s", curPckt->SRD_pld);  
  fclose(pldFile);  
}



//header to file

void SRDPcktHdrToFile(sensorRdyPckt* curPckt)
{
  if(curPckt->SRD_httpHdr == NULL)
  {
      printf("\nhttp header is empty, not writting file\n");
      return;
  }

  FILE * hdrFile; 
  hdrFile = fopen("httpHdr", "w");
  if(hdrFile == NULL) 
  {
     printf("\n*****Failed to open up the payload file*****\n");
     return;
  }

  fprintf(hdrFile, "%s", curPckt->SRD_httpHdr);  
  fclose(hdrFile);    
}



//fingerprints and calls the printing function for the finger print
void fngPntPrint(sensorRdyPckt *curPckt)
{
   if(curPckt->SRD_httpHdr == NULL || curPckt->SRD_hdr_len == 0)
   {
      printf("\nThere was no packet header, skipping printing fingerprint\n");
      return;
   }

   curPckt->fngPnt = SRDPcktFngrPnt(curPckt->SRD_httpHdr, curPckt->SRD_hdr_len);
   if(curPckt->fngPnt[0] == 8 && curPckt->fngPnt[1] == 8)
   {
      printf("\nCurrent packet was not structured properly \n");
      return;
   }

   formatFngPntPrint(curPckt->fngPnt);
}

//fingerprint print function
void formatFngPntPrint(int *fngP)
{
   printf("Printing the finger print\nHTTP COMMAND: %d\nHTTP PROTOCOL: %d\n"
	   ,fngP[0], fngP[1]);
   printf("LENGTH: %d\nVARIABLES: %d\nPERCENT#: %d\nAPOS#: %d\nPLUS#: %d\n"
	   ,fngP[2],fngP[3],fngP[4], fngP[5],fngP[6]);
   printf("CDOT#: %d\nBACKSLASH#: %d\nOPAREN#: %d\nCPAREN#: %d\nFORWARD#: %d\nLT#: %d\nGT#: %d\n"
	   ,fngP[7],fngP[8],fngP[9],fngP[10],fngP[11],fngP[12],fngP[13]);
}

//fingerprinting function
int * SRDPcktFngrPnt(unsigned char* curPcktData, int dataLen)
{
   if(curPcktData == NULL || dataLen == 0 )
   {
      printf("There was not header to parse\n");
      return NULL;
   }

   int i = 0;
   int cmd = 8;
   int cmdSet = 0;
   int proto = 8;
   int protoSet = 0;
   int len = dataLen;	//calculated during parsing
   int var = 0;
   int pcnt = 0;
   int apos = 0;
   int plus = 0;
   int cdot = 0;
   int bckslsh = 0;
   int oparen = 0;
   int cparen = 0;
   int fwrd = 0;
   int lt = 0;
   int gt = 0;
   int qstnmrk = 0;
   unsigned char *target = curPcktData;
   int *fngPnt = malloc(sizeof(int) * fngPntLen);

   for(;*target != '\n' && i < len; i++)
   {
      if(protoSet == 0 && (*target == 48 || *target == 49))		//proto
      {
         target++;
	 if(*target == 46)				//http version 
         {
            target++;
	    if(*target == 48)
            {
		proto = 2;
                protoSet = 1;
            }
	    else if(*target == 57)
            {
		proto = 1;
                protoSet = 1;
            }
	    else if(*target == 49)
            {
		proto = 4;
                protoSet = 1;
            }
            else 
	    {
		target--;
		target--;
	    }
	 }

	 else
	    target--;
      }

      if(cmdSet == 0 && *target == 71)			//cmd get
      {
	 target++;
         if(*target == 69)
	 {
	    target++;
	    if(*target == 84)
            {
	       cmd = 1;
	       cmdSet = 1;	
	    }
	    else

	       target--;
	 }
	 else
	    target--;
      }

   if(*target == 72 && cmdSet == 0)                  //cmd head
      {
         target++;
         if(*target == 69)
         {
            target++;
            if(*target == 65)
            {
               target++;
               if(*target == 68)
               {
                  cmd = 4;
                  cmdSet = 1;
               }
               else
                  target--;
            }   
            else
               target--;
         }
         else
            target--;
      }

    if(*target == 80 && cmdSet == 0)                  //cmd post
      {
         target++;
         if(*target == 79)
         {  
            target++;
            if(*target == 83)
            {
               target++;
               if(*target == 84)
               {
                  cmd = 2;
                  cmdSet = 1;
               }
               else
                  target--;
            }   
            else
               target--;
         }
         else
            target--;
      }

   if(*target == 46)			//.. counter
      {
         target++;
         if(*target == 46)
	    cdot++;
         else
    	    target--;	     
      }

       if(*target == 47){			// // counter
         target++;
         if(*target == 47)
	    fwrd++;
         else
    	    target--;	     
      }

      if(*target == 63){			//conditional for variables
	 qstnmrk = 1;
	 continue;
      }

      if(*target == 37){			//percent counter
         pcnt++;
	 continue;
      }

     if (qstnmrk == 1 && *target == 38){	//variable counter
         var++; 
         continue;
      }

      if(*target == 39){			//apostrophe counter
         apos++;
	 continue;
      }

      if(*target == 43){			//addition counter
         plus++;
	 continue;
      }

      if(*target == 40){			//open parentheses counter
         oparen++;
	 continue;
      }

      if(*target == 41){			//close parentheses counter
	 cparen++;
	 continue;
      }

      if(*target == 60){			//less than counter
         lt++;
	 continue;
      }

      if(*target == 62){			//greater than counter
         gt++;
	 continue;
      } 

      if(*target == 92){			//backslash counter
         bckslsh++;
	 continue;
      }

      target++;
   }



/*FINGER PRINT EXPLANATION:
 * array of integers, each slot contains a specified number (integer) that represents the character count 

 *INDEX 0		HTTP command 	GET = 1		POST = 2	HEAD = 4	OTHER = 8
 *INDEX 1		HTTP PROTOCOL	0.9 = 1		1.0 = 1		1.1 = 4		OTHER = 8	
 *INDEX 2		LENGTH		# OF CHARS		
 *INDEX 3		VARIABLES	# OF INPUT
 *INDEX 4		PERCENT		# OF %
 *INDEX 5		APOS		# OF ' 
 *INDEX 6		PLUS		# OF +		
 *INDEX 7		CDOT		# OF ..	
 *INDEX 8		BACKSLASH	# OF \
 *INDEX 9		OPAREN		# OF (
 *INDEX 10		CPAREN		# OF )
 *INDEX 11		FORWARD		# OF //
 *INDEX 12		LT		# OF <
 *INDEX 13		GT		# OF >

*/

   fngPnt[0] = cmd;
   fngPnt[1] = proto;
   fngPnt[2] = len;
   fngPnt[3] = var;
   fngPnt[4] = pcnt;
   fngPnt[5] = apos;
   fngPnt[6] = plus;
   fngPnt[7] = cdot;
   fngPnt[8] = bckslsh;
   fngPnt[9] = oparen;
   fngPnt[10] = cparen;
   fngPnt[11] = fwrd;
   fngPnt[12] = lt;
   fngPnt[13] = gt;

   return fngPnt;
}

//calls the trimmer for the header
void SRDTrimHdr(sensorRdyPckt* curPckt)
{
   curPckt->SRD_httpHdr = SRDTrim(curPckt->SRD_httpHdr, curPckt->SRD_hdr_len);   
}

//call the trimmer for the payload
void SRDTrimPld(sensorRdyPckt* curPckt)
{
   curPckt->SRD_pld = SRDTrim(curPckt->SRD_pld, curPckt->SRD_pld_len);
}

//The hash function
int myhashfunc(LH_hashTable* table, void * data)
{
   int slot = 0;
   int rslot = 0;
   int i = 1;
   sensorRdyPckt* p = (sensorRdyPckt*) data;
   slot = (p->SRD_id) % (kprimeCap(table->LH_primenums));

   printf("\nAttempting to hash %u\n",p->SRD_id);

   if(LlistIsEmpty(table->LH_table[slot].LHN_list))
      return slot;

   else
   {
      rslot = (p->SRD_id) % (kprimehash2(table->LH_primenums));
      do
      {
         slot = (slot + (i * rslot)) % (table->LH_capacity);
         i++;
      }while(!LlistIsEmpty(table->LH_table[slot].LHN_list));
   }
   return slot;
}

void* pcktAlloc()
{
   sensorRdyPckt* tmp = malloc(sizeof(sensorRdyPckt));
   return (void*) tmp;
}

//trimmer
unsigned char * SRDTrim(unsigned char *array, int arraySize)
{
   if(array == NULL || arraySize <= 0)
      return NULL;

   int NLCounter = 0;
   int slot = 0;
   unsigned char * tmp = array;

   while(*tmp == '\n' || *tmp =='\r' || *tmp == ' ')
   {
      tmp++;
      NLCounter++;
   }

   tmp = NULL;
   tmp = malloc((arraySize - NLCounter)* sizeof(unsigned char));

   for(; slot < arraySize - NLCounter; slot++)
      tmp[slot] = array[slot + NLCounter];

   free(array);
   array = NULL;
   return tmp;
}



void *reasembly(void* pckt)
{
   printf("New Thread just got launched this message should be repeated a total of %d times", THREAD_ARRAY_SIZE);
   //sensorRdyPckt *rsmvPckt = (sensorRdyPCkt)pckt;
   return NULL;   
}


//reasembly defaulter
void rsmvPcktThrd_default(rsmvPcktThrd* curPcktThrd)
{
   curPcktThrd->rsmv_id = 0;
   curPcktThrd->rsmv_timeout = 0;
   curPcktThrd->rsmv_pckt = NULL;
   curPcktThrd->rsmv_thrd = NULL;     
   curPcktThrd->next_rsmv_pckt = NULL;
}

//setter function
void rsmvPcktThrd_set(rsmvPcktThrd* rsmvData, int setId, int setTimeout, sensorRdyPckt* setPckt, pthread_t* setThrd)
{
   rsmvData->rsmv_id = setId;
   rsmvData->rsmv_pckt = setPckt;
   rsmvData->rsmv_timeout = setTimeout;
   rsmvData->rsmv_thrd = setThrd;   
}

