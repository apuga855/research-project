#include <stdio.h>
#include <pcap.h>

int main(int argc, char *argv[])
{
   pcap_t *handle;	//handles the session
   char *dev;		//this will be device to sniff on
   char errbuf[PCAP_ERRBUFF_SIZE];	//setting up a buffer of the specific size
   struct bpf_program fp;		//compiled filter
   char filter_exp[] = "port 2000"	//setting up my filter to be only on port 2000
   bpf_u_int32 mask;		//netmask
   bpf_u_int32 net;		//our ip
   struct pcap_pkthdr header;	//the header that pcap will give us
   const u_char *packet;	//actual packet

   dev = pcap_lookupdev(errbuf);
   if(dev == NULL){printf("Could not open the given device: %s\n",errbuf); return 1;	//test if we got a device}
   if(pcap_lookupnet(dev, &net, &mask, errbuf) == -1)	//test the mask
   {
      printf("Could not open the given device %s: %s\n",dev, errbuf);
      net = 0;
      mask = 0;
      return 1;
   }
   handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);
   if(handle == NULL){printf("Could not open the given device%s: %s\n", somedev, errbuf); return 1;	//test if we got a device}

   

   return 0;
}
