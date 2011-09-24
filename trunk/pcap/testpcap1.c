/***************************************************
* file: testpcap1.c
* Date: Thu Mar 08 17:14:36 MST 2001
* Author: Martin Casado
* Location: LAX Airport (hehe)
*
* Simple single packet capture program
*****************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <pcap.h> /* if this gives you an error try pcap/pcap.h */
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h> /* includes net/ethernet.h */
#include <netinet/ip.h>
#include <time.h>
#include <unistd.h>

int handle_packet(const struct pcap_pkthdr* pkthdr, const u_char* packet);
int handle_ip(const struct pcap_pkthdr* pkthdr, const u_char* packet);

int handle_packet(const struct pcap_pkthdr* pkthdr, const u_char* packet) {
    int i;
    struct ether_header *eptr; /* net/ethernet.h */
    u_char *ptr; /* printing out hardware header info */
    
    /*
    struct pcap_pkthdr {
        struct timeval ts;
        time stamp
        bpf_u_int32 caplen; length of portion present
        bpf_u_int32;
        lebgth this packet (off wire)
    }
    */
    printf("Grabbed packet of length %d\n", pkthdr->len);
    printf("Recieved at ..... %s\n", ctime((const time_t*)&pkthdr->ts.tv_sec));
    printf("Ethernet address length is %d\n", ETHER_HDR_LEN);
    /* lets start with the ether header... */
    eptr = (struct ether_header*)packet;
    ptr = eptr->ether_dhost;
    i = ETHER_ADDR_LEN;
    printf(" Destination Address: ");
    do {
        printf("%s%x",(i == ETHER_ADDR_LEN) ? " " : ":",*ptr++);
    } while(--i > 0);
    printf("\n");
    ptr = eptr->ether_shost;
    i = ETHER_ADDR_LEN;
    printf(" Source Address: ");
    do {
        printf("%s%x",(i == ETHER_ADDR_LEN) ? " " : ":",*ptr++);
    } while(--i > 0);
    printf("\n");

    /* Do a couple of checks to see what packet type we have..*/
    if (ntohs (eptr->ether_type) == ETHERTYPE_IP) {
        printf("Ethernet type hex:%x dec:%d is an IP packet\n",
        ntohs(eptr->ether_type),
        ntohs(eptr->ether_type));
        
        handle_ip(pkthdr, packet);
    } else if (ntohs (eptr->ether_type) == ETHERTYPE_ARP) {
        printf("Ethernet type hex:%x dec:%d is an ARP packet\n",
        ntohs(eptr->ether_type),
        ntohs(eptr->ether_type));
    }else {
        printf("Ethernet type %x not IP", ntohs(eptr->ether_type));
        exit(1);
    }

    return 0;
}

int handle_ip(const struct pcap_pkthdr* pkthdr, const u_char* packet) {
    int len;
    struct ip *ip;
    u_int length = pkthdr->len;
    u_int hlen, off, version;

    ip = (struct ip*)(packet + sizeof(struct ether_header));
    length -= sizeof(struct ether_header);
    
    if (length < sizeof(struct ip)) {
        printf("truncated ip %d\n", length);
        return 1;
    }
    
    len = ntohs(ip->ip_len);
    hlen = ip->ip_hl;
    version = ip->ip_v;
    
    if (version != 4) {
        printf("Unknown version %d\n", version);
        return 1;
    }
    
    if (hlen < 5) {
        printf("bad ip header length %d\n", hlen);
    }
    
    if (length < len) {
        printf("truncated ip %d bytes missing\n", len - length);
    }
    
    off = ntohs(ip->ip_off);
    if ((off & 0x1fff) == 0) {
        printf("src ip: [%s], dst ip: [%s]\n", 
            inet_ntoa(ip->ip_src), inet_ntoa(ip->ip_dst));
        printf("header length: %d, version %d\n", hlen, version);
        printf("ip packet length: %d, offset: %d\n", len, off);
    }         
            
    return 0;
}

int main(int argc, char **argv) {
    char *dev;
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* handle;
    const u_char *packet;
    struct pcap_pkthdr hdr;
    /* pcap.h */
    /* grab a device to peak into... */
    dev = pcap_lookupdev(errbuf);
    if(dev == NULL) {
        printf("%s\n",errbuf);
        exit(1);
    }
    printf("DEV: %s\n",dev);

    handle = pcap_open_live(dev, BUFSIZ, 0, 0, errbuf);
    if(handle == NULL) {
        printf("pcap_open_live(): %s\n", errbuf);
        exit(1);
    }

    while (1) {
        packet = pcap_next(handle, &hdr);
        if (packet == NULL) {
            /* dinna work *sob* */
            printf("Didn't grab packet\n");
            printf("err: %s\n", pcap_geterr(handle));
            sleep(1);
        } else {
            handle_packet(&hdr, packet);
        }
    }
        
    pcap_close(handle);
    
    return 0;
}

