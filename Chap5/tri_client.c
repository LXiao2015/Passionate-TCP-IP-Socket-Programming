#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define OP_SIZE 4

void error_handling(char *message);

int main(int argc, char* argv[])
{
    int sock;
    struct sockaddr_in serv_addr;
    char msg[BUF_SIZE];
    int msg_len, recv_len, recv_cnt, i=0;

    if (argc != 3) {
        printf("Usage: %s <IP> <port>\n", argv[0]);
	    exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);  // 创建套接字，此时还不能区分服务端或客户端
    if (sock == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)  // 发送连接请求，确定是客户端
        error_handling("connect() error");
    else
        puts("Connected");

    while (i < 3) {
        memset(msg, 0, BUF_SIZE);
        fputs("Input message: ", stdout);
        fgets(msg+OP_SIZE, BUF_SIZE-OP_SIZE, stdin);
        int len = strlen(msg+OP_SIZE);
        memcpy(msg, &len, OP_SIZE);
        write(sock, msg, OP_SIZE+len);

        recv_len = 0;
        while (recv_len == 0 || recv_len < msg_len + OP_SIZE) {
            recv_cnt = read(sock, &msg, BUF_SIZE);
            recv_len += recv_cnt;
            if (recv_len >= OP_SIZE) {
                memcpy(&msg_len, msg, OP_SIZE);
            }
        }
        msg[recv_len] = '\0';

        printf("Message from server: %s", msg+OP_SIZE);
        i++;
    }

    close(sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
