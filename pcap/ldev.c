#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char **argv) {
    char *dev;
    char *net;
    char *mask;
    int ret;
    char errbuf[PCAP_ERRBUF_SIZE];
    bpf_u_int32 netp;
    bpf_u_int32 maskp;
    struct in_addr addr;
    
    dev = pcap_lookupdev(errbuf);
    if (NULL == dev) {
        printf("%s\n", errbuf);
        exit(1);
    }
    
    printf("dev: %s\n", dev);
    
    ret = pcap_lookupnet(dev, &netp, &maskp, errbuf);
    
    if (-1 == ret) {
        printf("%s\n", errbuf);
        exit(1);
    }
    
    addr.s_addr = netp;
    net = inet_ntoa(addr);
    
    if (NULL == net) {
        perror("inet_ntoa");
        exit(1);
    }
    
    printf("net: %s\n", net);
    
    addr.s_addr = maskp;
    mask = inet_ntoa(addr);
    
    if (NULL == mask) {
        perror("inet_ntoa");
        exit(1);
    }
    
    printf("mask: %s\n", mask);
    
    return 0;
}

