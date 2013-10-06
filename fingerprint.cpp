//callback function for pcap loop
void packet_handler(u_char* usrData,const struct pcap_pkthdr* pcktHeader, const u_char* pckt)
{
   int fragFlag = 0;
   //holy crap this gets crazy really quickly

   //this is a constant structure pointer of type ether_header 
   //nicknamed ethernetHeader this structure actually comes 
   //predefined, take a look at it for all the crazy crap 
   //you can do with it
   char contnue = '\n';
   const struct ether_header* ethernetHeader;
   //constant structure pointer of type ip nicknamed ipHeader
   //also predefined for us
   const struct ip* ipHeader;
   //constant structure pointer of type tcphdr nicknamed
   //tcpHeader also predefined for us
   const struct tcphdr* tcpHeader;
   u_char* data;       //data segment pointer of unsigned characters
   int dataLength = 0; //data length in bytes from the packet
   u_char* dataStr = NULL;//unsigned char pointer that we are going to use 
   int iteration = 0;//one of my counters
   int pldC = 0;//payload counter
   int httpHdrC = 0;//header counter
   //int unprint = 0;
   int unprintCount = 0;//used to throw out initial garbage characters
   int slot = 0;//counter variable
   sensorRdyPckt *SRD_pckt = NULL;//this is the sensor ready packet info
   //pcktArray = malloc(SENSOR_ARRAY_PACKET_LENGTH * sizeof(sensorRdyPckt));
   SRD_pckt = malloc(sizeof(sensorRdyPckt));   //allocating a SRD_pckt
   packet_default(SRD_pckt);//initializing the new packet that was allocated    
   //for(; i < SENSOR_ARRAY_PACKET_LENGTH; i++)
   //   packet_default(&SRD_pckt);       
   //i = 0;
   ethernetHeader = (struct ether_header*)pckt;//attempting to cast as an ether_header structure, which contains the fields of the packet
    //there are a lot of defines inside of the ethernet header, we are checking here if the ETHERTYPE is IP 
    //if it is, then we can start the parsing of the packet
    //the ntohs function changes the the byte orders to the current hosts byte ordering scheme, then we go in
    //and compare the ether type unsigned short char to the ETHERTYPE_IP define 
    if (ntohs(ethernetHeader->ether_type) == ETHERTYPE_IP)
    {
       unsigned char moreFrag;                  //checking for more fragments
       moreFrag = *(pckt + etherLen + 50);              //getting the more fragment bit
       //this is assigning the address located at the target* address of the packet plust the size of the
       //structure that contained the ethernet header, since the packet is basivally just a gigantic
       //array of bytes, then all we have to do is parse through it like we would an array, which is
       //by offsets, theregore what we get, is the ipheader
       ipHeader = (struct ip*)(pckt + etherLen);
       if((int)moreFrag != 0 || (int)ipHeader->ip_off != 0)//checking for fragmentation
       {
          fragFlag = 1;
          printf("The flag was caught, the value for MF is %d The offset is %d",
          (int) moreFrag, (int)ipHeader->ip_off);
       }

       SRD_pckt->SRD_src_addr = ntohl(ipHeader->ip_src.s_addr);//getting the source address in net to host endian mode, storing it into the SRD packet
       SRD_pckt->SRD_dst_addr = ntohl(ipHeader->ip_dst.s_addr);//getting the destination address in net to host endian mode, storing it into the SRD packet
       SRD_pckt->SRD_total_len = ipHeader->ip_len;             //total length 

       if(ipHeader->ip_p == IPPROTO_TCP)                      //checking if it has an tcp header
       {

          tcpHeader = (struct tcphdr*)(pckt + etherLen+ ipLen);   //casting the tcp header start from the packet into a tcphdr pointer so we can actually get members out
          SRD_pckt->SRD_src_prt = ntohs(tcpHeader->source);       //casting the source port in net to host endian mode, storing it into the SRD packet
          SRD_pckt->SRD_dst_prt = ntohs(tcpHeader->dest);          //casting the destination port int net to host endian mode, storing it into the SRd packet
          SRD_pckt->SRD_sqnsnum = ntohl(tcpHeader->seq);           //storing the sequence number in the SRD packet in net to host endian mode
          data = (u_char*)(pckt + etherLen + ipLen + tcpLen);     //get the data segment start from the packet 
          dataLength = pcktHeader->len - (etherLen + ipLen + tcpLen);//the data length in bytes from the pcktHeader structure,
                                                                     // we subtract the ether length and ip length and tcp length to know the 
                                                                     //data segment length, its all in bytes
          dataStr = malloc(dataLength * sizeof(u_char));             //allocate enough space to hold the data segment in our dataStr pointer
          for(; iteration < dataLength; iteration++)                 //parsing loop
          {
             if((data[iteration] >= 32 && data[iteration] <= 126) || 
                data[iteration] == 10 || data[iteration] == 11 || data[iteration] == 13) //printable characters are copied   
                   dataStr[iteration - unprintCount] = (u_char)data[iteration];

             else
                unprintCount++;

             if(!SRD_pckt->SRD_httpHdr && data[iteration] == '\r' && data[iteration + 1] == '\n' &&
                data[iteration + 2] == '\r' && data[iteration + 3] == '\n')//this checks for when the http header ends
             {
                slot = 0;
                iteration = iteration + 3;
                httpHdrC += unprintCount;
                SRD_pckt->SRD_httpHdr = malloc((iteration) * sizeof(u_char));//set the SRD packet header into the correct size
                for(; httpHdrC < iteration; httpHdrC++, slot++)                           //copy all characters from the packet header into the SRD packet header section
                   SRD_pckt->SRD_httpHdr[slot] = (u_char)dataStr[slot];
                SRD_pckt->SRD_hdr_len = httpHdrC - unprintCount;                                       //setting packet header length
             }
          }

          if(httpHdrC == dataLength)//if the packet length is the same as the packet header length then this does not have a payload
             SRD_pckt->SRD_pld = NULL;
          else
          {
             SRD_pckt->SRD_pld = malloc((dataLength - httpHdrC) * sizeof(u_char)); //otherwise allocate the payload size and get the characters remaining        
             pldC = httpHdrC + 1;                                                         //start after the http header 
             slot = 0;
             for(; pldC < iteration; pldC++, slot++)                                      //passing copying  
                SRD_pckt->SRD_pld[slot] = dataStr[slot];
             SRD_pckt->SRD_pld_len = pldC - httpHdrC;//setting payload length
          }
       }  
    }  

    if (dataLength > 0)                                        //check if our packet was not empty, it could be, maybe 
    {
       SRDTrimHdr(SRD_pckt);
       SRDTrimPld(SRD_pckt);
       printf("Printing Results\n\n");                         //printing the results
       SRDPcktPrint(SRD_pckt);
       SRDPcktHdrPldPrint(SRD_pckt);
       printf("Writting files\n\n");
       SRDPcktPldToFile(SRD_pckt);
       SRDPcktHdrToFile(SRD_pckt);
       fngPntPrint(SRD_pckt);
    }

    printf("Finished with the first packet from file, would you like to continue? (y/n)\n");
    scanf("%c", &contnue);
    
    if(contnue == 'n'){exit(0);}

    return;
}

