#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUF_SIZE 30
void error_handling(char *message);
void urg_handler(int sig);


int main(int argc, char *argv[])
{
    int serv_sock;
    int clnt_sock;
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;

    char buf[BUF_SIZE];
    int str_len, state;

    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
	    exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);  // 创建套接字
    if (serv_sock == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)  // 分配IP和端口
        error_handling("bind() error");

    if (listen(serv_sock, 5) == -1)  // 可连接状态
        error_handling("listen() error");

    clnt_addr_size = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_addr, &clnt_addr_size);  // 在此等待连接

    while (1) {
        str_len = recv(clnt_sock, buf, sizeof(buf)-1, MSG_PEEK|MSG_DONTWAIT);
        if (str_len > 0) break;
    }
    buf[str_len] = 0;
    printf("Buffering %d bytes: %s \n", str_len, buf);

    str_len = recv(clnt_sock, buf, sizeof(buf)-1, 0);
    buf[str_len] = 0;
    printf("Reading %d bytes: %s \n", str_len, buf);
    close(clnt_sock);
    close(serv_sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}