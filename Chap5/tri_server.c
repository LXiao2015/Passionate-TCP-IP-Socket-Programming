#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define OP_SIZE 4

void error_handling(char *message);

int main(int argc, char *argv[])
{
    int serv_sock;
    int clnt_sock;
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;

    char msg[BUF_SIZE];
    int msg_len, recv_len, recv_cnt, i;

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

    for (i = 0; i < 5; ++i) {
        clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_addr, &clnt_addr_size);  // 在此等待连接
        if (clnt_sock == -1)
            error_handling("accept() error");
        else
            printf("Connected client %d \n", i+1);

        while (1) {
            int over = 0;
            recv_len = 0;
            while (recv_len == 0 || recv_len < msg_len + OP_SIZE) {
                recv_cnt = read(clnt_sock, &msg[recv_len], BUF_SIZE);
                if (recv_cnt == -1) {
                    error_handling("read() error!");
                }
                if (recv_cnt == 0) {
                    over = 1;
                    break;
                }
                recv_len += recv_cnt;
                if (recv_len >= OP_SIZE) {
                    memcpy(&msg_len, msg, OP_SIZE);
                }
            }
            msg[recv_len] = '\0';

            if (over == 1) {
                break;
            }

            printf("Message from client: %s", msg+OP_SIZE);
            fputs("Input message: ", stdout);
            fgets(msg+OP_SIZE, BUF_SIZE-OP_SIZE, stdin);
            int len = strlen(msg+OP_SIZE);
            memcpy(msg, &len, OP_SIZE);
            write(clnt_sock, msg, OP_SIZE+len);
        }
        close(clnt_sock);
    }
    close(serv_sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
