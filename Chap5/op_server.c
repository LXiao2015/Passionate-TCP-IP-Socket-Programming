#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define OP_SIZE 4

void error_handling(char *message);

int(*op)(int, int[]);

int add(int op_cnt, int num[]);
int minus(int op_cnt, int num[]);
int multiply(int op_cnt, int num[]);

int main(int argc, char *argv[])
{
    int serv_sock;
    int clnt_sock;
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;

    char ops[BUF_SIZE];
    int op_cnt, recv_len, recv_cnt, i;

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
        op_cnt = 0;
        clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_addr, &clnt_addr_size);  // 在此等待连接
        if (clnt_sock == -1)
            error_handling("accept() error");
        else
            printf("Connected client %d \n", i+1);

        read(clnt_sock, &op_cnt, 1);
        op_cnt = (int)op_cnt;

        recv_len = 0;
        while (recv_len < op_cnt * OP_SIZE + 1) {
            recv_cnt = read(clnt_sock, &ops[recv_len], BUF_SIZE);
            recv_len += recv_cnt;
        }

        switch (ops[recv_len-1]) {
            case '+': op = add; break;
            case '-': op = minus; break;
            case '*': op = multiply; break;
            default: fputs("Wrong operator!", stdout); break;
        }

        int result = op(op_cnt, (int*)ops);

        write(clnt_sock, (char*)&result, 4);
        close(clnt_sock);
    }
    close(serv_sock);
    return 0;
}

int add(int op_cnt, int num[]) {
    int res = num[0];
    for (int i = 1; i < op_cnt; ++i) {
        res += num[i];
    }
    return res;
}

int minus(int op_cnt, int num[]) {
    int res = num[0];
    for (int i = 1; i < op_cnt; ++i) {
        res -= num[i];
    }
    return res;
}

int multiply(int op_cnt, int num[]) {
    int res = num[0];
    for (int i = 1; i < op_cnt; ++i) {
        res *= num[i];
    }
    return res;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
