#pragma once
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
#define P_BUFF 100		//buffer size for packets
#define THREAD_ARRAY_SIZE 10	//array for worker threads
#define THREAD_DEFAULT_TIMEOUT 1000	//thread timeout
#define INPUT_NAME_SIZE 20		//size for name 
#define PAYLOAD_BUF_SIZE 1024		//payload buffer
#define SENSOR_ARRAY_PACKET_LENGTH 20
#define etherLen   sizeof(struct ether_header)
#define ipLen  sizeof(struct ip)
#define tcpLen  sizeof(struct tcphdr)

//callback function
void packet_handler(u_char *,const struct pcap_pkthdr*, const u_char*);
