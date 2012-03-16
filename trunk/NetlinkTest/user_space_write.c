#include <sys/types.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#define NETLINK_KEVIN 27
#define MAX_PAYLOAD 2048

void printUsage() {
    printf("user_space_write pid\n");
}

int main(int argc, char* argv[]) {
    pid_t destPID = 0; // to kernel by default
    if (argc < 2) {
        printUsage();
        return(EXIT_FAILURE);
    } else {
        destPID = atoi(argv[1]);
        printf("dest pid is [%d]\n", destPID);
    }

    struct sockaddr_nl s_nladdr, d_nladdr;
    struct msghdr msg ;
    struct nlmsghdr *nlh = NULL ;
    struct iovec iov;
    int fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_GENERIC);
    //int fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_KEVIN);

    /* source address */
    memset(&s_nladdr, 0, sizeof(s_nladdr));
    s_nladdr.nl_family = AF_NETLINK ;
    s_nladdr.nl_pad = 0;
    pid_t srcPID = getpid();
    printf("src pid is [%d]\n", srcPID);
    s_nladdr.nl_pid = srcPID;
    s_nladdr.nl_groups = 0;
    bind(fd, (struct sockaddr*)&s_nladdr, sizeof(s_nladdr));

    /* destination address */
    memset(&d_nladdr, 0, sizeof(d_nladdr));
    d_nladdr.nl_family = AF_NETLINK ;
    d_nladdr.nl_pad = 0;
    d_nladdr.nl_pid = destPID; /* destined to kernel */
    d_nladdr.nl_groups = 0;

    /* Fill the netlink message header */
    nlh = (struct nlmsghdr *)malloc(100);
    memset(nlh, 0, 100);
    strcpy(NLMSG_DATA(nlh), "Mr. Kernel, Are you ready ?" );
    nlh->nlmsg_len = 100;
    nlh->nlmsg_pid = srcPID;
    nlh->nlmsg_flags = 1;
    nlh->nlmsg_type = 0;

    /*iov structure */

    iov.iov_base = (void *)nlh;
    iov.iov_len = nlh->nlmsg_len;

    /* msg */
    memset(&msg,0,sizeof(msg));
    msg.msg_name = (void *) &d_nladdr ;
    msg.msg_namelen=sizeof(d_nladdr);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    sendmsg(fd, &msg, 0);

    free(nlh);
    close(fd);
    return(EXIT_SUCCESS);
}

