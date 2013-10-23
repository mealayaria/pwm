#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define SERVPORT 3333
#define MAXDATASIZE 1024
#define SERV_IP "192.168.12.10"
int main(int argc, const char *argv[])
{
    int sockfd,len;
    char buf[MAXDATASIZE];
    struct sockaddr_in serv_addr;
    int addr_len = sizeof(struct sockaddr_in);
    /*if (argc < 2) {
        fprintf(stderr, "Please enter the server name.\n");
        exit(1);
    }*/
	
    //创建socket
    if ((sockfd = socket(AF_INET,SOCK_DGRAM,0)) == -1 ) {
        perror("socket");
        exit(1);
    }
    //设置socket相关参数
    serv_addr.sin_port = htons(SERVPORT);
    serv_addr.sin_addr.s_addr = inet_addr(SERV_IP);
    bzero(&(serv_addr.sin_zero),8);

    while (1) {
        memset(buf,'0',MAXDATASIZE);
        read(STDIN_FILENO,buf,MAXDATASIZE);
        sendto(sockfd,buf,sizeof(buf),0,(struct sockaddr *)&serv_addr,addr_len);
        recvfrom(sockfd,buf,sizeof(buf),0,(struct sockaddr *)&serv_addr,&addr_len);
        printf("receive:%s\n",buf);
    }
    close(sockfd);

    
    return 0;
}
