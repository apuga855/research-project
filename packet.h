#pragma once
#include<netinet/ip.h>
#include<pcap.h>

#define CLASS_UNKNOWN                   0x0
#define CLASS_ETHER                     0x1
#define CLASS_IP                        0x2
#define CLASS_TCP                       0x4
#define CLASS_HTTP                      0x8

#define PAYLOAD_BUF_SIZE                1024
#define SENSOR_ARRAY_PACKET_LENGTH      20
#define ETHERLEN                        14
#define fngPntLen                       13
#define INET4_ADDRSTRLEN                16
#define HTTP_PORT                       80
#define THREAD_ARRAY_SIZE               10
#define THREAD_DEFAULT_TIMEOUT          1000
#define P_BUFF                          100
#define INPUT_NAME_SIZE                 20

typedef struct _etherData {
   uint16_t type;
} etherData;

typedef struct _ipData {
    uint16_t hdrlen;			//header length
    uint16_t pktlen;
    uint16_t id;
    uint8_t morefrag;
    uint16_t fragoffset;	//fragment offset
    uint8_t proto;
    uint32_t src;		//source address	
    uint32_t dst;		//destination address
    u_char *data;
    uint16_t datalen;
} ipData;

typedef struct _tcpData {
   uint16_t srcport;		//source port
   uint16_t dstport;  		//destination port
   uint32_t seq;		//sequence number
   uint16_t offset;
   const u_char *data;
   uint16_t datalen;
} tcpData;

typedef struct _httpData {
   const u_char *headers;
   const u_char *body;
   size_t bodylen;
   size_t hdrlen;
} httpData;

typedef struct _sensorRdyPckt
{
   int *fngPnt;				//fingerprint
   signed char class;
   etherData ether;
   ipData ip;
   tcpData tcp;
   httpData http;

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
void SRDPcktHdrPldPrint(sensorRdyPckt *);
//callback function
void packet_handler(u_char *,const struct pcap_pkthdr*, const u_char*);
//defaulter function
void packet_init(sensorRdyPckt*);
//header to file
void SRDPcktPldToFile(sensorRdyPckt*);
//payload to file
void SRDPcktHdrToFile(sensorRdyPckt*);
//create the firnger print
int *SRDPcktFngrPnt(sensorRdyPckt*);
//print fingerprint
void fngPntPrint(sensorRdyPckt*);
//formater for the fingerprint
void formatFngPntPrint(int*);
void* pcktAlloc();
void SRDPcktCpy(sensorRdyPckt *, sensorRdyPckt *);
char *decodeIP(uint32_t);
void parsePacket(sensorRdyPckt *, const u_char *, size_t);
void printPacket(sensorRdyPckt *);
void *reassembly(void*);
