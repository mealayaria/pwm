
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "pwm.h"

#define SERVPORT 3333
#define MAX_CONNECT_NO 10
#define MAXDATASIZE 1024
int ctrl(char *ctrl)
{
    switch(*ctrl){
        //左转
        case '4':return 0;

        //右转
        case '6':return 0;

        //前进
        case '8':return 0;

        //后退
        case '2':return 0;

        //停止
        case '5':printf("success!\n");
                return 0;
        case '0':pwm_ctrl(800,80,1);
                return 0;
        case '1':pwm_ctrl(600,30,1);
                return 0;
        case 'q':pwm_ctrl(200,50,0);//总之没办法了又加上flag位复位，但还是无效。
                return -1;

        default:return 1;
    }
}
int main(void)
{
    struct sockaddr_in addr;
    int sockfd;
    int addr_len = sizeof(struct sockaddr_in);
    char buf[MAXDATASIZE],ctrls[8];
    char err[] = "Wrong control word!";

    if((sockfd = socket(AF_INET,SOCK_DGRAM,0)) == -1) {
        perror("socket");
        exit(1);
    }
    printf("socket success!sockfd=%d\n",sockfd);
    
    bzero(&addr,sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(SERVPORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sockfd,(struct sockaddr *)&addr,sizeof(struct sockaddr)) == -1) {
        perror("bind");
        exit(1);
    }
    printf("bind success!\n");

    pwm_ctrl(400,50,1);
    while (1) {
        memset(buf,'0',MAXDATASIZE);
        recvfrom(sockfd,buf,sizeof(buf),0,(struct sockaddr *)&addr,(socklen_t *)&addr_len);
        strcpy(ctrls,buf);
        if (ctrl(ctrls) > 0) {
            sendto(sockfd,err,sizeof(err),0,(struct sockaddr *)&addr,addr_len);
        }
        else if (ctrl(ctrls) == 0){
            sendto(sockfd,"successful!",12,0,(struct sockaddr *)&addr,addr_len);
        }
        else {
            break;
        }
        //len = recvfrom(sockfd,buf,sizeof(buf),0,(struct sockaddr *)&addr,&addr_len);
        //printf("receive from %s\n",inet_ntoa(addr.sin_addr));
        //sendto(sockfd,buf,len,0,(struct sockaddr *)&addr,addr_len);
    } 
    close(sockfd);

    return 0;
}
