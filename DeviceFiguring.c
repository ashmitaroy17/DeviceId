
#include<netinet/in.h>
#include<errno.h>
#include<netdb.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<netinet/ip_icmp.h>
#include<netinet/udp.h>
#include<netinet/tcp.h>
#include<netinet/ip.h>
#include<netinet/if_ether.h>
#include<net/ethernet.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/ioctl.h>
#include<sys/time.h>
#include<sys/types.h>
#include<unistd.h>
void ProcessPacket(unsigned char* , int);
void print_ip_header(unsigned char* , int);
void print_tcp_packet(unsigned char* , int);
void print_udp_packet(unsigned char* , int);
void print_icmp_packet(unsigned char* , int);
void PrintData(unsigned char* , int);FILE*logfile;
struct sockaddr_in source,dest;
int tcp= 0, udp= 0, icmp= 0, others= 0, igmp= 0, total= 0, i, j;
int main() {
int saddr_size, data_size;
struct sockaddr saddr;
unsigned char*buffer= (unsigned char*) malloc(65536); 
logfile= fopen("logfile.txt","w");
if(logfile== NULL) {
  printf("Unable to create the logfile.txt file.");

    }   
   printf("Starting...\n");

  int sock_raw= socket( AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

if(sock_raw< 0) 
  {
  //Printsthe error withaproper message.
  perror("Socket Error");
   return 1;
  }
  while(1) {
  saddr_size= sizeof saddr;//Receive a packet
   data_size= recvfrom(sock_raw, buffer, 65536, 0, &saddr, (socklen_t*)&saddr_size);
   if(data_size< 0) {
    printf("Recvfrom error, failed to receive packets\n");
    return 1;
    }//Now process the packet
    ProcessPacket(buffer, data_size);
    }
    close(sock_raw);
    printf("Finished");
    return 0;
    }
    void ProcessPacket(unsigned char* buffer, int Size) {//Get the IP Header part of this packet , excluding the ethernet header
    struct iphdr*iph= (struct iphdr*)(buffer+ sizeof(struct ethhdr));
    ++total;
    switch(iph->protocol) {//Check the Protocol and do accordingly...
    case1://ICMP Protocol
    ++icmp;
    print_icmp_packet( buffer, Size);
    break;
    case2://IGMP Protocol
    ++igmp;
    break;
    case6://TCP Protocol
    ++tcp;
    print_tcp_packet(buffer, Size);
    break;
    case17://UDP Protocol
    ++udp;
    print_udp_packet(buffer, Size);
    break;
    default://Some Other Protocol like ARP etc.
    ++others;
        break;}

    }

    int getType(int val1, int val2, int val3) {
    char mac[7];
    snprintf(mac, sizeof(mac), "%.2X%.2X%.2X", val1, val2, val3);// return 1 for mobile, 0 for desktop
    if(strncmp(mac, "F81654", 3)==0) {
    return 0;

    }    else if(strncmp(mac, "00065B", 3)==0) {//Dell Laptop
    return 0;
    }
    else if(strncmp(mac, "00061B", 3)==0) {//Lenovo Notebook
    return 0;

    }
    else if(strncmp(mac, "0012FE", 3)==0) {//Lenovo Mobile
        return 1;
        }
        else if(strncmp(mac, "001321", 3)==0) {//HP Laptops
        return 0;
        }
        else if(strncmp(mac, "001320", 3)==0) {//Intel Corporate
        return 0;
        }
        else if(strncmp(mac, "009EC8", 3)==0) {//Xiaomi Comm.
        return 1;
        }
        else if(strncmp(mac, "0000F0", 3)==0) {//Samsung Mobile
        return 1;
        }
        else if(strncmp(mac, "FCFC48", 3)==0) {//Apple Mobile
        return 1;
        }
        else if(strncmp(mac, "FCE557", 3)==0) {//Nokia Mobile
        return 1;
        }
        else if(strncmp(mac, "000039", 3)==0) {//Toshiba Laptop
        return 0;
        }
        else if(strncmp(mac, "000255", 3)==0) {//IBM PC
            return 0;
        }
        else{return(rand()%2);
        }

    }
    void print_ethernet_header(unsigned char* Buffer, int Size) {struct ethhdr*eth= (struct ethhdr*)Buffer; 

        fprintf(logfile, "\n");

        fprintf(logfile, "Ethernet Header\n");

        fprintf(logfile, " |-Destination Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n", eth->h_dest[0], eth->h_dest[1], eth->h_dest[2], eth->h_dest[3], eth->h_dest[4], eth->h_dest[5]);

        fprintf(logfile, " |-Source Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n", eth->h_source[0], eth->h_source[1], eth->h_source[2], eth->h_source[3], eth->h_source[4], eth->h_source[5]);

        fprintf(logfile, " |-Protocol : %u\n", (unsigned short)eth->h_proto);

        printf("\n------------------------------------------------------------------\n\n");

        printf("Destination Mac Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n", eth->h_dest[0], eth->h_dest[1], eth->h_dest[2], eth->h_dest[3], eth->h_dest[4], eth->h_dest[5]);

        printf("Source Mac Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n", eth->h_source[0], eth->h_source[1], eth->h_source[2], eth->h_source[3], eth->h_source[4], eth->h_source[5]);

        int typeDest= getType(eth->h_dest[0], eth->h_dest[1], eth->h_dest[2]);int typeSrc= getType(eth->h_source[0], eth->h_source[1], eth->h_source[2]);printf("\n");

        if(typeDest== 1) {

            printf("Destination Device is : %s\n", "Mobile");

        }

        else{

            printf("Destination Device is : %s\n", "Desktop");

        }

        if(typeSrc== 1) {

            printf("Source Device is : %s\n", "Mobile");

        }

        else{printf("Source Device is : %s\n", "Desktop");

        printf("\n------------------------------------------------------------------\n");

    }

}

void print_ip_header(unsigned char* Buffer, int Size) {

    print_ethernet_header(Buffer, Size);

    unsigned short iphdrlen;

    struct iphdr*iph= (struct iphdr*)(Buffer+ sizeof(struct ethhdr));

    iphdrlen =iph->ihl*4;

    memset(&source, 0, sizeof(source));

    source.sin_addr.s_addr= iph->saddr;

    memset(&dest, 0, sizeof(dest));

    dest.sin_addr.s_addr= iph->daddr;

    fprintf(logfile, "\n");

        fprintf(logfile, "IP Header\n");

        fprintf(logfile, " |-IP Version : %d\n",(unsigned int)iph->version);

        fprintf(logfile, "|-IP Header Length : %dDWORDS or %dBytes\n",(unsigned int)iph->ihl,((unsigned int)(iph->ihl))*4);

        fprintf(logfile, " |-Type Of Service : %d\n",(unsigned int)iph->tos);

        fprintf(logfile, " |-IP Total Length : %dBytes(Size of Packet)\n",ntohs(iph->tot_len));

        fprintf(logfile, " |-Identification : %d\n",ntohs(iph->id));

        fprintf(logfile, " |-TTL : %d\n",(unsigned int)iph->ttl);

        fprintf(logfile, " |-Protocol : %d\n",(unsigned int)iph->protocol);

        fprintf(logfile, " |-Checksum : %d\n",ntohs(iph->check));

        fprintf(logfile, " |-Source IP : %s\n",inet_ntoa(source.sin_addr));

        fprintf(logfile, " |-Destination IP : %s\n",inet_ntoa(dest.sin_addr));

    }

    void print_tcp_packet(unsigned char* Buffer, int Size) {unsigned short iphdrlen;

        struct iphdr*iph= (struct iphdr*)( Buffer+ sizeof(struct ethhdr));

        iphdrlen= iph->ihl*4;struct tcphdr*tcph=(struct tcphdr*)(Buffer+ iphdrlen+ sizeof(struct ethhdr));

        int header_size= sizeof(struct ethhdr) + iphdrlen+ tcph->doff*4;

        fprintf(logfile, "\n\n***********************TCP Packet*************************\n");

        print_ip_header(Buffer,Size);

        fprintf(logfile, "\n");

        fprintf(logfile, "TCP Header\n");

        fprintf(logfile, " |-Source Port : %u\n",ntohs(tcph->source));

        fprintf(logfile, " |-Destination Port: %u\n",ntohs(tcph->dest));

        fprintf(logfile, " |-Sequence Number : %u\n",ntohl(tcph->seq));

        fprintf(logfile, " |-Acknowledge Number : %u\n",ntohl(tcph->ack_seq));

        fprintf(logfile, " |-Header Length : %dDWORDS or %dBYTES\n",(unsigned int)tcph->doff,(unsigned int)tcph->doff*4);

        fprintf(logfile, " |-Urgent Flag : %d\n",(unsigned int)tcph->urg);

        fprintf(logfile, " |-Acknowledgement Flag : %d\n",(unsigned int)tcph->ack);

        fprintf(logfile, " |-Push Flag : %d\n",(unsigned int)tcph->psh);

        fprintf(logfile, " |-Reset Flag : %d\n",(unsigned int)tcph->rst);

        fprintf(logfile, " |-Synchronise Flag : %d\n",(unsigned int)tcph->syn);

        fprintf(logfile, " |-Finish Flag : %d\n",(unsigned int)tcph->fin);

        fprintf(logfile, " |-Window : %d\n",ntohs(tcph->window));

        fprintf(logfile, " |-Checksum : %d\n",ntohs(tcph->check));

        fprintf(logfile, " |-Urgent Pointer : %d\n",tcph->urg_ptr);

        fprintf(logfile, "\n");fprintf(logfile, " DATA Dump ");

        fprintf(logfile, "\n");fprintf(logfile, "IP Header\n");

        PrintData(Buffer, iphdrlen);fprintf(logfile, "TCP Header\n");

        PrintData(Buffer+iphdrlen, tcph->doff*4);fprintf(logfile, "Data Payload\n");

        PrintData(Buffer+ header_size, Size-header_size);

        fprintf(logfile, "\n###########################################################");

    }

    void print_udp_packet(unsigned char*Buffer, int Size) {

        unsigned short iphdrlen;

        struct iphdr*iph= (struct iphdr*)(Buffer+ sizeof(struct ethhdr));

        iphdrlen= iph->ihl*4;struct udphdr*udph= (struct udphdr*)(Buffer+ iphdrlen+ sizeof(struct ethhdr));

        int header_size= sizeof(struct ethhdr) + iphdrlen+ sizeof udph;

        fprintf(logfile, "\n\n***********************UDP Packet*************************\n");

        print_ip_header(Buffer, Size);fprintf(logfile, "\nUDP Header\n");

        fprintf(logfile, " |-Source Port : %d\n", ntohs(udph->source));

        fprintf(logfile, " |-Destination Port : %d\n", ntohs(udph->dest));

        fprintf(logfile, " |-UDP Length : %d\n",ntohs(udph->len));

        fprintf(logfile, " |-UDP Checksum : %d\n", ntohs(udph->check));

        fprintf(logfile, "\n");

        fprintf(logfile, "IP Header\n");

        PrintData(Buffer, iphdrlen);

        fprintf(logfile, "UDP Header\n");

        PrintData(Buffer+iphdrlen,sizeof udph);

        fprintf(logfile, "Data Payload\n");//Move the pointer ahead and reduce the size of string

        PrintData(Buffer+ header_size, Size-header_size);

        fprintf(logfile, "\n###########################################################");

    }

    void print_icmp_packet(unsigned char* Buffer, int Size) {

        unsigned short iphdrlen;

        struct iphdr*iph= (struct iphdr*)(Buffer+ sizeof(struct ethhdr));

        iphdrlen= iph->ihl* 4;

        struct icmphdr*icmph= (struct icmphdr*)(Buffer+ iphdrlen+ sizeof(struct ethhdr));

        fprintf(logfile, "\n\n***********************ICMP Packet*************************\n");

        print_ip_header(Buffer, Size);

        fprintf(logfile, "\n");

        fprintf(logfile, "ICMP Header\n");

        fprintf(logfile, " |-Type : %d",(unsigned int)(icmph->type));

        if((unsigned int)(icmph->type) == 11) {

            fprintf(logfile, " (TTL Expired)\n");

        }

        else if((unsigned int)(icmph->type) == ICMP_ECHOREPLY) {

            fprintf(logfile, " (ICMP Echo Reply)\n");

        }

        fprintf(logfile, " |-Code : %d\n",(unsigned int)(icmph->code));

        fprintf(logfile, " |-Checksum : %d\n",ntohs(icmph->checksum));

        fprintf(logfile, "\n");fprintf(logfile, "IP Header\n");

        int header_size= sizeof(struct ethhdr) + iphdrlen+ sizeof icmph;

        PrintData(Buffer,iphdrlen);fprintf(logfile, "UDP Header\n");

        PrintData(Buffer+ iphdrlen, sizeof icmph);

        fprintf(logfile, "Data Payload\n");//Move the pointer ahead and reduce the size of string

        PrintData(Buffer+ header_size, (Size-header_size) );

        fprintf(logfile, "\n###########################################################");

        }

        void PrintData(unsigned char* data, int Size) {int i, j;

        for(i=0; i< Size; i++) {if( i!=0&& i%16==0) {//if one line of hex printing is complete...

        fprintf(logfile, " ");

        for(j=i-16; j<i; j++) {

        if(data[j]>=32&& data[j]<=128) {

        fprintf(logfile, "%c",(unsigned char)data[j]);//if it’s a number oran alphabet

        }

        else{fprintf(logfile, ".");//otherwise print a dot

    }

}

fprintf(logfile, "\n");

}

if(i%16==0) fprintf(logfile, " "); 

{

  fprintf(logfile, " %02X",(unsigned int)data[i]);

}

if( i==Size-1) {//print the last spaces

for(j=0;j<15-i%16;j++) {

fprintf(logfile, " ");//extra spaces

  }

fprintf(logfile, " ");

  for(j=i-i%16; j<=i; j++) {

    if(data[j]>=32&& data[j]<=128) {

        fprintf(logfile, "%c",(unsigned char)data[j]);

    }

    else{fprintf(logfile, ".");

}

}fprintf(logfile, "\n");

}

}

}
