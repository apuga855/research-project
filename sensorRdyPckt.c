#include"sensorRdyPckt.h"

//function for printing out only packet header and payload
void SRD_pcktHdrPldPrint(sensorRdyPckt* curPckt)
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

//packet printer, only relevant information
void SRD_pcktPrint(sensorRdyPckt* curPckt)
{
   printf("Printing packet #%d\nSource Address: %d\nDestination Address: %d\nSource Port: %d\nDestination Port: %d\n"
          ,curPckt->SRD_id, curPckt->SRD_src_addr, curPckt->SRD_dst_addr, curPckt->SRD_src_prt, curPckt->SRD_dst_prt);

}

//defaulter function
void SRD_pcktDefault(sensorRdyPckt* curPckt);
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

//header to file
void SRD_pcktPldToFile(sensorRdyPckt*)
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

//payload to file
void SRD_pcktHdrToFile(sensorRdyPckt*)
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

//create the firnger print
int* SRD_pcktFngrPnt(unsigned char *, int)
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
   int len = dataLen;   //calculated during parsing
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
      if(protoSet == 0 && (*target == 48 || *target == 49))             //proto
      {
         target++;
         if(*target == 46)                              //http version 
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

      if(cmdSet == 0 && *target == 71)                  //cmd get
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

   if(*target == 46)                    //.. counter
   {
      target++;
      if(*target == 46)
         cdot++;
      else
         target--;
   }

   if(*target == 47)
   {                       // // counter
      target++;
      if(*target == 47)
         fwrd++;
      else
         target--;
   }

      if(*target == 63){                        //conditional for variables
         qstnmrk = 1;
         continue;
      }

      if(*target == 37){                        //percent counter
         pcnt++;
         continue;
      }

     if (qstnmrk == 1 && *target == 38){        //variable counter
         var++;
         continue;
      }

      if(*target == 39){                        //apostrophe counter
         apos++;
         continue;
      }

      if(*target == 43){                        //addition counter
         plus++;
         continue;
      }

      if(*target == 40){                        //open parentheses counter
         oparen++;
         continue;
      }

      if(*target == 41){                        //close parentheses counter
         cparen++;
         continue;
      }

      if(*target == 60){                        //less than counter
         lt++;
         continue;
      }

      if(*target == 62){                        //greater than counter
         gt++;
         continue;
      }

      if(*target == 92){                        //backslash counter
         bckslsh++;
         continue;
      }

      target++;
   }

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

//print fingerprint
void SRD_fngPntPrint(sensorRdyPckt*)
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

//trim function
unsigned char* SRD_trim(unsigned char *, int)
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

//trim header
void SRD_trimHdr(sensorRdyPckt*)
{
   curPckt->SRD_httpHdr = SRDTrim(curPckt->SRD_httpHdr, curPckt->SRD_hdr_len);
}

//trim payload
void SRD_trimPld(sensorRdyPckt*)
{
   curPckt->SRD_pld = SRDTrim(curPckt->SRD_pld, curPckt->SRD_pld_len);
}

//formater for the fingerprint
void SRD_formatFngPntPrint(int*)
{
   printf("Printing the finger print\nHTTP COMMAND: %d\nHTTP PROTOCOL: %d\n"
           ,fngP[0], fngP[1]);
   printf("LENGTH: %d\nVARIABLES: %d\nPERCENT#: %d\nAPOS#: %d\nPLUS#: %d\n"
           ,fngP[2],fngP[3],fngP[4], fngP[5],fngP[6]);
   printf("CDOT#: %d\nBACKSLASH#: %d\nOPAREN#: %d\nCPAREN#: %d\nFORWARD#: %d\nLT#: %d\nGT#: %d\n"
           ,fngP[7],fngP[8],fngP[9],fngP[10],fngP[11],fngP[12],fngP[13]);
}

