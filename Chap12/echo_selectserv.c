#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>
#define BUF_SIZE 100

void error_handling(char *message);

int main(int argc, char *argv[])
{
    int serv_sock;
    int clnt_sock;
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;

    fd_set reads, temps;
    int result, str_len;

    int fd_max, fd_num, i;
    char buf[BUF_SIZE];
    struct timeval timeout;
    if (argc != 2)
    {
        printf("Usage : %s <port>\n", argv[0]);
        exit(0);
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

    FD_ZERO(&reads);    // 所有位初始化为0
    FD_SET(serv_sock, &reads);    // 服务器套接字注册进reads
    fd_max = serv_sock;

    while (1)
    {
        temps = reads;
        timeout.tv_sec = 5;    // 每次调用select前初始化超时时间，因为select会将这个值替换为剩余时间
        result = select(fd_max+1, &temps, 0, 0, &timeout);
        if (result == -1)
        {
            puts("select() error!");
            break;
        }
        if (result == 0)
        {
            puts("Time out!");
            continue;
        }
        for (i = 0; i < fd_max + 1; ++i)
        {
            if (FD_ISSET(i, &temps))
            {
                if (i == serv_sock)    // 连接请求
                {
                    clnt_addr_size = sizeof(clnt_addr);
                    clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_addr, &clnt_addr_size);
                    FD_SET(clnt_sock, &reads);    // 添加客户端套接字
                    if (fd_max < clnt_sock)
                    {
                        fd_max = clnt_sock;
                    }
                    printf("Connected client: %d \n", clnt_sock);
                }
                else    // 读数据
                {
                    str_len = read(i, buf, BUF_SIZE);
                    if (str_len == 0)    // 关闭连接
                    {
                        FD_CLR(i, &reads);
                        close(i);
                        printf("Closed client: %d \n", i);
                    }
                    else
                    {
                        write(i, buf, str_len);
                    }
                }
            }
        }
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
