#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define RES_SIZE 4
#define OP_SIZE 4

void error_handling(char *message);

int main(int argc, char* argv[])
{
    int sock;
    struct sockaddr_in serv_addr;
    char opmsg[BUF_SIZE];
    int op_cnt, result, i=0;

    if (argc != 3) {
        printf("Usage: %s <IP> <port>\n", argv[0]);
	    exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);  // 创建套接字，此时还不能区分服务端或客户端
    if (sock == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0 ,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)  // 发送连接请求，确定是客户端
        error_handling("connect() error");
    else
        puts("Connected");

    fputs("Input operand count: ", stdout);
    scanf("%d", &op_cnt);
    if (op_cnt <= 0)
        return -1;
    opmsg[0] = (char)op_cnt;

    while (i < op_cnt) {
        fputs("Input operand: ", stdout);
        scanf("%d", (int*)&opmsg[i*OP_SIZE+1]);
        i++;
    }

    fgetc(stdin);
    fputs("Input operator: ", stdout);
    scanf("%c", &opmsg[op_cnt*OP_SIZE+1]);
    write(sock, opmsg, op_cnt*OP_SIZE+2);
    read(sock, &result, RES_SIZE);

    printf("Operation result: %d\n", result);
    close(sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
