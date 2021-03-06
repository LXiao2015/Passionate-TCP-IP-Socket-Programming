#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 30

void error_handling(char *message);

int main(int argc, char *argv[])
{
    int recv_sock;
    struct sockaddr_in adr;
    struct ip_mreq join_adr;

    char buf[BUF_SIZE];
    int str_len;

    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
	    exit(1);
    }

    recv_sock = socket(PF_INET, SOCK_DGRAM, 0);  // 创建套接字
    if (recv_sock == -1)
        error_handling("socket() error");

    memset(&adr, 0, sizeof(adr));
    adr.sin_family = AF_INET;
    adr.sin_addr.s_addr = htonl(INADDR_ANY);
    adr.sin_port = htons(atoi(argv[1]));

    if (bind(recv_sock, (struct sockaddr*) &adr, sizeof(adr)) == -1)  // 分配IP和端口
        error_handling("bind() error");

    while (1) {
        str_len = recvfrom(recv_sock, buf, BUF_SIZE-1, 0, NULL, 0);
        if (str_len < 0)
            break;
        buf[str_len] = 0;
        fputs(buf, stdout);
    }
    close(recv_sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
