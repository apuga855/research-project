


//starting from here, is all necessary materials and tools for ---------------------
//packet capture without reasembly
//need to typedef it knows its a type
typedef struct _sensorRdyPckt
{
   unsigned int SRD_pld_len;            //payload length
   unsigned int SRD_hdr_len;            //header length
   int SRD_id;                          //I am probably going to place the parsed packets in a hash table, so I am placing an ID just in case
   uint32_t SRD_sqnsnum;                //sequence number
   uint32_t SRD_src_addr;               //source address        
   uint32_t SRD_dst_addr;               //destination address
   uint16_t SRD_src_prt;                //source port
   uint16_t SRD_dst_prt;                //destination port
   uint16_t SRD_total_len;              //total length
   uint16_t SRD_total_frag_offset;      //fragment offset
   unsigned char *SRD_httpHdr;          //http header
   unsigned char *SRD_pld;              //c does not have a default byte primitive, I am going to be using a character array to simulate it
                                        //since it is actually a single byte anyway
   int *fngPnt;                         //fingerprint
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

//function for printing out only packet header and payload
void SRD_pcktHdrPldPrint(sensorRdyPckt *);
//packet printer, only relevant information
void SRD_pcktPrint(sensorRdyPckt*);
//defaulter function
void SRD_pcktDefault(sensorRdyPckt*);
//header to file
void SRD_pcktPldToFile(sensorRdyPckt*);
//payload to file
void SRD_pcktHdrToFile(sensorRdyPckt*);
//create the firnger print
int* SRD_pcktFngrPnt(unsigned char *, int);
//print fingerprint
void SRD_fngPntPrint(sensorRdyPckt*);
//trim function
unsigned char* SRD_trim(unsigned char *, int);
//trim header
void SRD_trimHdr(sensorRdyPckt*);
//trim payload
void SRD_trimPld(sensorRdyPckt*);
//formater for the fingerprint
void SRD_frmtFngPntPrint(int*);

