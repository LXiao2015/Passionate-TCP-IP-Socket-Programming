#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define OP_SIZE 4
#define FILENAME_LEN 32

void error_handling(char *message);

int main(int argc, char* argv[])
{
    int sock;
    struct sockaddr_in serv_addr;
    char msg[BUF_SIZE];
    char filename[FILENAME_LEN];
    FILE *fp;

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

    memset(msg, 0, BUF_SIZE);
    fputs("Input file name: ", stdout);
    fgets(msg+OP_SIZE, BUF_SIZE-OP_SIZE, stdin);
    int len = strlen(msg+OP_SIZE) - 1;    // extract trailing '\n'
    memcpy(msg, &len, OP_SIZE);
    write(sock, msg, OP_SIZE+len);

    memcpy(filename, msg+OP_SIZE, len);
    memcpy(filename+len, ".new", 4);
    filename[len+4] = '\0';
    if ((fp = fopen(filename, "ab") ) == NULL) {
        close(sock);
        exit(1);
    }

    while (1) {
        int n = read(sock, msg, BUF_SIZE);
        if (n == 0) {
            break;
        }
        fwrite(msg, 1, n, fp);
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
