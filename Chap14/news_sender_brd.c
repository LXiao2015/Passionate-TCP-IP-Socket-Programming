#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define TTL 64
#define BUF_SIZE 30
void error_handling(char *message);

int main(int argc, char* argv[])
{
    int sock;
    struct sockaddr_in brd_addr;
    FILE *fp;
    char buf[BUF_SIZE];
    int so_brd = 1;

    if (argc != 3) {
        printf("Usage: %s <BroadcastIP> <port>\n", argv[0]);
	    exit(1);
    }

    sock = socket(PF_INET, SOCK_DGRAM, 0);  // 创建套接字，此时还不能区分服务端或客户端
    if (sock == -1)
        error_handling("socket() error");

    memset(&brd_addr, 0, sizeof(brd_addr));
    brd_addr.sin_family = AF_INET;
    brd_addr.sin_addr.s_addr = inet_addr(argv[1]);
    brd_addr.sin_port = htons(atoi(argv[2]));

    setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (void*)&so_brd, sizeof(so_brd));
    if ((fp = fopen("news.txt", "r")) == NULL)
        error_handling("fopen() error");

    while (fgets(buf, BUF_SIZE, fp)) {
        sendto(sock, buf, strlen(buf), 0, (struct sockaddr*)&brd_addr, sizeof(brd_addr));
        sleep(2);
    }
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
