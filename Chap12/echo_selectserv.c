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

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);  // �����׽���
    if (serv_sock == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)  // ����IP�Ͷ˿�
        error_handling("bind() error");

    if (listen(serv_sock, 5) == -1)  // ������״̬
        error_handling("listen() error");

    FD_ZERO(&reads);    // ����λ��ʼ��Ϊ0
    FD_SET(serv_sock, &reads);    // �������׽���ע���reads
    fd_max = serv_sock;

    while (1)
    {
        temps = reads;
        timeout.tv_sec = 5;    // ÿ�ε���selectǰ��ʼ����ʱʱ�䣬��Ϊselect�Ὣ���ֵ�滻Ϊʣ��ʱ��
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
                if (i == serv_sock)    // ��������
                {
                    clnt_addr_size = sizeof(clnt_addr);
                    clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_addr, &clnt_addr_size);
                    FD_SET(clnt_sock, &reads);    // ��ӿͻ����׽���
                    if (fd_max < clnt_sock)
                    {
                        fd_max = clnt_sock;
                    }
                    printf("Connected client: %d \n", clnt_sock);
                }
                else    // ������
                {
                    str_len = read(i, buf, BUF_SIZE);
                    if (str_len == 0)    // �ر�����
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
