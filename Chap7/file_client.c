#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char *message);

int main(int argc, char* argv[])
{
    int sock;
    struct sockaddr_in serv_addr;
    char buf[BUF_SIZE];
    FILE *fp;
    int read_cnt;

    if (argc != 3) {
        printf("Usage: %s <IP> <port>\n", argv[0]);
	    exit(1);
    }

    fp = fopen("receive.dat", "wb");
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

    while ((read_cnt = read(sock, buf, BUF_SIZE)) != 0) {
        fwrite(buf, 1, read_cnt, fp);
    }

    write(sock, "Thanks you", 10);
    fclose(fp);
    close(sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
