#include <sys/types.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#define NETLINK_NITRO 27
#define MAX_PAYLOAD 2048

int main() {
    struct sockaddr_nl s_nladdr, d_nladdr;
    struct msghdr msg;
    struct nlmsghdr *nlh = NULL;
    struct iovec iov;
    int fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_NITRO);

    /* source address */
    memset(&s_nladdr, 0 ,sizeof(s_nladdr));
    s_nladdr.nl_family = AF_NETLINK ;
    s_nladdr.nl_pad = 0;
    pid_t srcPID = getpid();
    printf("pid is [%d]\n", srcPID);
    s_nladdr.nl_pid = srcPID;
    s_nladdr.nl_groups = 0;
    bind(fd, (struct sockaddr*)&s_nladdr, sizeof(s_nladdr));

    /* destination address */
    memset(&d_nladdr, 0 ,sizeof(d_nladdr));

    /*iov structure */
    iov.iov_base = (void *)nlh;
    iov.iov_len = 100;
    /* msg */
    memset(&msg,0,sizeof(msg));
    msg.msg_name = (void *) &d_nladdr ;
    msg.msg_namelen=sizeof(d_nladdr);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    do {
        sleep(2);
        printf("no msg, wait ... \n");
    } while (recvmsg(fd, &msg, 0) <= 0);

    printf("receive [%s]\n", (char*)NLMSG_DATA(nlh));

    close(fd);
    return (EXIT_SUCCESS);
}

