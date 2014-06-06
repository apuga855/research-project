#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<arpa/inet.h>
#include<net/ethernet.h>
#include<unistd.h>
#include<pcap.h>
#include"packet.h"

char *decodeIP(uint32_t ip) {
    char *ipstr = malloc(INET4_ADDRSTRLEN);
    struct in_addr addr;
    addr.s_addr = ip;
    inet_ntop(AF_INET, &addr, ipstr, INET4_ADDRSTRLEN);
    return ipstr;
}

void parsePacket(sensorRdyPckt *parsed, const u_char *raw, size_t raw_len) {
    const u_char* raw_begin = raw;
    parsed->class |= CLASS_ETHER;
    parsed->ether.type = ntohs(*(uint16_t*)(raw+12));
    raw += 14;

    if (parsed->ether.type != ETHERTYPE_IP) 	
        return;

    parsed->class |= CLASS_IP;
    parsed->ip.hdrlen = (*raw & 0x0f) * 4;
    parsed->ip.pktlen = ntohs(*(uint16_t*)(raw+2));
    parsed->ip.id = ntohs(*(uint16_t*)(raw+4));
    parsed->ip.morefrag = (*(raw+6) & 0x20) > 0;
    parsed->ip.fragoffset = ntohs((*(uint16_t*)(raw+6)) & 0x1fff) << 3;
    parsed->ip.proto = *(uint8_t*)(raw+9);
    parsed->ip.src = *(uint32_t*)(raw+12);
    parsed->ip.dst = *(uint32_t*)(raw+16);
    parsed->ip.datalen = parsed->ip.pktlen - parsed->ip.hdrlen;
    parsed->ip.data = malloc(parsed->ip.datalen);
    if (!parsed->ip.data) {
        fprintf(stderr, "Failed to parse data");
        return;
    }
    raw += parsed->ip.hdrlen; //FIXME: validate length
    memcpy(parsed->ip.data, raw, parsed->ip.datalen);

    if(parsed->ip.proto != IPPROTO_TCP) //checking if it has an tcp header
        return;

    parsed->class |= CLASS_TCP;
    parsed->tcp.srcport = ntohs(*((uint16_t*)raw));
    parsed->tcp.dstport = ntohs(*((uint16_t*)(raw+2)));
    parsed->tcp.seq = ntohl(*((uint32_t*)(raw+4)));
    parsed->tcp.offset = (*(raw+12)>>4)*4;
    parsed->tcp.datalen = parsed->ip.pktlen - parsed->ip.hdrlen - parsed->tcp.offset;
    raw += parsed->tcp.offset; //get the data segment start from the packet 
    parsed->tcp.data = raw;

    if (!(parsed->tcp.srcport == HTTP_PORT || parsed->tcp.dstport == HTTP_PORT))
        return;

    const unsigned char *http_body;
    http_body = (u_char *)strstr((const char*)raw, "\r\n\r\n");
    if (!http_body) {
        /* FIXME: This does not work well because the TCP stream needs to be
         * serialized before processing it. TCP data does not necessarily
         * arrive all at once. */
        return;
    }
    parsed->class |= CLASS_HTTP;
    parsed->http.headers = parsed->tcp.data;
    parsed->http.hdrlen = (size_t)http_body - (size_t)parsed->http.headers;
    http_body += 4;
    parsed->http.body = http_body;
    parsed->http.bodylen = (size_t)(raw_begin+raw_len) - (size_t)http_body;
}

void printPacket(sensorRdyPckt *pckt) {

    if (!(pckt->class & CLASS_ETHER))
        return;

    puts("");
    puts("|------ New Packet------|");

    if (!(pckt->class & CLASS_IP))
        return;

    char *src_ip = decodeIP(pckt->ip.src);
    char *dst_ip = decodeIP(pckt->ip.dst);
    printf("IP Header Length: 0x%04x\n", pckt->ip.hdrlen);
    printf("IP Packet Length: %hu\n", pckt->ip.pktlen);
    printf("IP Identifier: 0x%04x (%u)\n", pckt->ip.id, pckt->ip.id);
    printf("IP Fragmentation Bit: %s\n", pckt->ip.morefrag ? "set" : "not set");
    printf("IP Frag Offset: 0x%04x\n", pckt->ip.fragoffset);
    printf("IP Protocol: 0x%02x\n", pckt->ip.proto);
    printf("IP Source Address: %s\n", src_ip);
    printf("IP Destination Address: %s\n", dst_ip);
    printf("IP Data Len: %hu\n", pckt->ip.datalen);
    free(src_ip);
    free(dst_ip);

    if (!(pckt->class & CLASS_TCP))
        return;

    printf("TCP Source Port: %hu\n", pckt->tcp.srcport);
    printf("TCP Destination Port: %hu\n", pckt->tcp.dstport);
    printf("TCP Sequence Number: %u\n", pckt->tcp.seq);
    printf("TCP Data Offset: %u\n", pckt->tcp.offset);
    printf("TCP Data Len: %u\n", pckt->tcp.datalen);

    if (pckt->class & CLASS_HTTP) {

        if (pckt->http.hdrlen > 0) {
            puts("*** HTTP Headers ***");
            fwrite(pckt->http.headers, 1, pckt->http.hdrlen, stdout);
            puts("");
        }

        if (pckt->http.bodylen > 0) {
            puts("*** HTTP Body ***");
            fwrite(pckt->http.body, 1, pckt->http.bodylen, stdout);
            puts("");
        }
    }
    else {
        puts("*** TCP Data ***");
        fwrite(pckt->tcp.data, 1, pckt->tcp.datalen, stdout);
        puts("");
    }
    
    SRDPcktFngrPnt(pckt);
    formatFngPntPrint(pckt->fngPnt);
      
}

void* pcktAlloc()
{
   sensorRdyPckt* tmp = malloc(sizeof(sensorRdyPckt));
   return (void*) tmp;
}

//fingerprint print function
void formatFngPntPrint(int *fngP)
{
   if(fngP == NULL)
      return;
   printf("Printing the finger print\nHTTP COMMAND: %d\nHTTP PROTOCOL: %d\n"
	   ,fngP[0], fngP[1]);
   printf("LENGTH: %d\nVARIABLES: %d\nPERCENT#: %d\nAPOS#: %d\nPLUS#: %d\n"
	   ,fngP[2],fngP[3],fngP[4], fngP[5],fngP[6]);
   printf("CDOT#: %d\nBACKSLASH#: %d\nOPAREN#: %d\nCPAREN#: %d\nFORWARD#: %d\nLT#: %d\nGT#: %d\n"
	   ,fngP[7],fngP[8],fngP[9],fngP[10],fngP[11],fngP[12],fngP[13]);
}

//fingerprinting function
//int * SRDPcktFngrPnt(const u_char * curPcktData, int dataLen)
void SRDPcktFngrPnt(sensorRdyPckt* pckt)
{
    
    //if(pckt == NULL || pckt->http.hdrlen == 0 || pckt->class == CLASS_HTTP)
    if(pckt == NULL || pckt->http.hdrlen == 0 || pckt->http.headers == NULL)
    {
        printf("There was not header to parse\n");
        return;
    }

    int i = 0;
    int cmd = 8;
    int cmdSet = 0;
    int proto = 8;
    int protoSet = 0;
    int len = pckt->http.hdrlen;	//calculated during parsing
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
    const u_char *target = pckt->http.headers;
    int * fngPnt = malloc(fngPntLen * sizeof(int));
    printf("Fingerprint:\n");
    for(;/**target != '\n' && */i < len; i++)
    {
   
        //printf("loop %c\n",*target);

        if(protoSet == 0 && (*target == '0' || *target == '1'))		//proto
        {
            target++;
            if(*target == '.')				//http version 
            {
                target++;
                if(*target == '1')
                {
                    proto = 2;
                    protoSet = 1;
                }
                else if(*target == '9')
                {
                    proto = 1;
                    protoSet = 1;
                }
                else if(*target == '1')
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



        if(cmdSet == 0 && !strncmp("GET", (const char*)(target),3))
        {
           cmd = 1;
           cmdSet = 1;
           target = target + 3;
        }

/*
        if(protoSet == 0 && cmdSet == 0 && *target == 'G')			//cmd get
        {
            target++;
            if(*target == 'E')
            {
                target++;
                if(*target == 'T')
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
*/
        if(cmdSet == 0 && !strncmp("HEAD",(const char*)(target),4))
        {
           cmd = 4;
           cmdSet = 1;
           target = target + 4;
        }
/*
        if(protoSet == 0 &&*target == 'H' && cmdSet == 0)                  //cmd head
        {
            target++;
            if(*target == 'E')
            {
                target++;
                if(*target == 'A')
                {
                    target++;
                    if(*target == 'D')
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
*/
        if(cmdSet == 0 && !strncmp("POST", (const char*)(target),3))
        {
           cmd = 2;
           cmdSet = 1;
           target = target + 4;
        }
/*
        if(*target == 'P' && cmdSet == 0)                  //cmd post
        {
            target++;
            if(*target == 'O')
            {  
                target++;
                if(*target == 'S')
                {
                    target++;
                    if(*target == 'T')
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
*/

        if(!strncmp("..", (const char *)(target),2))
        {
           cdot++;
           target = target + 1; 
           continue;
        }
/*
        if(*target == '.')			//.. counter
        {
            target++;
            if(*target == '.')
            {
                cdot++;
                continue;
            }
            else
                target--;	     
        }
*/


        if(!strncmp("//", (const char*)(target),2))
        {
           fwrd++;
           target = target + 1;
           continue;
        }
/*
        if(*target == '/'){			// // counter
            target++;
            if(*target == '/')
            {
                fwrd++;
                continue;
            }
            else
                target--;	     
        }
*/


        if(*target == '?'){			//conditional for variables
            target++;
            qstnmrk = 1;
            continue;
        }

        if(*target == '%'){			//percent counter
            target++;
            pcnt++;
            continue;
        }

        if (qstnmrk == 1 && *target == '&'){	//variable counter
            target++;
            var++; 
            continue;
        }

        if(*target == 39){			//apostrophe counter
            target++;
            apos++;
            continue;
        }

        if(*target == '+'){			//addition counter
            target++;
            plus++;
            continue;
        }

        if(*target == ')'){			//open parentheses counter
            target++;
            oparen++;
            continue;
        }

        if(*target == '('){			//close parentheses counter
            target++;
            cparen++;
            continue;
        }

        if(*target == '<'){			//less than counter
            target++;
            lt++;
            continue;
        }

        if(*target == '>'){			//greater than counter
            target++;
            gt++;
            continue;
        } 

        if(*target == 92){			//backslash counter
            target++;
            bckslsh++;
            continue;
        }
        target++;

    }

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

   pckt->fngPnt = fngPnt;
   return;
}

//header to file

void SRDPcktHdrToFile(sensorRdyPckt* curPckt)
{
  if(curPckt->http.headers == NULL)
  {
      printf("\nhttp header is empty, not writting file\n");
      return;
  }

  FILE * hdrFile; 
  hdrFile = fopen("http_headers", "w");
  if(hdrFile == NULL) 
  {
     printf("\n*****Failed to open up the payload file*****\n");
     return;
  }

  fprintf(hdrFile, "%s", curPckt->http.headers);  
  fclose(hdrFile);    
}

//blanking function
void packet_init(sensorRdyPckt *pckt)
{
   pckt->ip.fragoffset = 0;
   pckt->ip.pktlen = 0;
   pckt->ip.hdrlen = 0;
   pckt->ip.id = 0;
   pckt->ip.src = 0;	
   pckt->ip.dst = 0;
   pckt->ip.data = NULL;
   pckt->ip.datalen = 0;
   pckt->tcp.seq = 0;
   pckt->tcp.srcport = 0 ;
   pckt->tcp.dstport = 0;
   pckt->tcp.data = NULL;
   pckt->tcp.datalen = 0;
   pckt->http.headers = NULL;
   pckt->http.hdrlen = 0;
   pckt->http.body = NULL;
   pckt->http.bodylen = 0;
   pckt->fngPnt = NULL;
   pckt->class = CLASS_UNKNOWN;
}


void packet_copy(sensorRdyPckt *src, sensorRdyPckt *dst)
{
   dst->ip.fragoffset = src->ip.fragoffset;
   dst->ip.pktlen = src->ip.pktlen;
   dst->ip.hdrlen = src->ip.hdrlen;
   dst->ip.id = src->ip.id;
   dst->ip.src = src->ip.src;
   dst->ip.dst = src->ip.dst;
   dst->ip.data = src->ip.data;
   dst->ip.datalen = src->ip.datalen;
   dst->tcp.seq = src->tcp.seq;
   dst->tcp.srcport = src->tcp.srcport;
   dst->tcp.dstport = src->tcp.dstport;
   dst->tcp.data = src->tcp.data;
   dst->tcp.datalen = src->tcp.datalen;
   dst->http.headers = src->http.headers;
   dst->http.hdrlen = src->http.hdrlen;
   dst->http.body = src->http.body; 
   dst->http.bodylen = src->http.bodylen; 
   dst->fngPnt = src->fngPnt; 
   dst->class = src->class;
}

void SRDPcktPldToFile(sensorRdyPckt* curPckt)
{
  if(curPckt->http.body== NULL)
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

  fprintf(pldFile, "%s", curPckt->http.body);  
  fclose(pldFile);  
}



//fingerprints and calls the printing function for the finger print
/*void fngPntPrint(sensorRdyPckt *curPckt)
{
   if(curPckt->http.headers == NULL || curPckt->http.hdrlen== 0)
   {
      printf("\nThere was no packet header, skipping printing fingerprint\n");
      return;
   }

   curPckt->fngPnt = SRDPcktFngrPnt(curPckt->http.headers, curPckt->http.hdrlen);
   if(curPckt->fngPnt[0] == 8 && curPckt->fngPnt[1] == 8)
   {
      printf("\nCurrent packet was not structured properly \n");
      return;
   }

   formatFngPntPrint(curPckt->fngPnt);
}
*/
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

void *reassembly(void* pckt)
{
   printf("New Thread just got launched this message should be repeated a total of %d times", THREAD_ARRAY_SIZE);
   //sensorRdyPckt *rsmvPckt = (sensorRdyPCkt)pckt;
   return NULL;   
}

