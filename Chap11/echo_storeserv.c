#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char *message);
void read_childproc(int sig);

int main(int argc, char *argv[])
{
    int serv_sock;
    int clnt_sock;
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;

    char buf[BUF_SIZE];
    int str_len;
    int fds[2];

    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
	    exit(1);
    }

    pid_t pid;
    struct sigaction act;
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    sigaction(SIGCHLD, &act, 0);    // 子进程终止产生SIGCHLD信号

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

    pipe(fds);
    pid = fork();
    if (pid == 0) {
        FILE *fp = fopen("echomsg.txt", "wt");    // 打开文件以文本格式写入
        int i, len;
        for (int i = 0; i < 6; ++i) {
            len = read(fds[0], buf, BUF_SIZE);
            fwrite((void*)buf, 1, len, fp);
        }
        fclose(fp);
        return 0;
    }

    int i = 0;
    while (1) {
        clnt_addr_size = sizeof(clnt_addr);
        clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_addr, &clnt_addr_size);  // 在此等待连接
        if (clnt_sock == -1)
            continue;
        else
            printf("Connected client %d \n", ++i);

        pid = fork();
        if (pid == 0) {
            close(serv_sock);    // 关闭套接字文件描述符，并非关闭套接字
            printf("Begin exchanging data with client %d \n", i);
            while ((str_len=read(clnt_sock, buf, BUF_SIZE)) != 0) {
                write(clnt_sock, buf, str_len);
                write(fds[1], buf, str_len);
            }

            close(clnt_sock);
            puts("Client disconnected...");
            return 0;
        } else {
            close(clnt_sock);
        }
    }
    close(serv_sock);    // 只有两个文件描述符都销毁后，套接字才被销毁
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

void read_childproc(int sig)
{
    int statloc;
    pid_t id;
    while ((id = waitpid(-1, &statloc, WNOHANG)) > 0) {
        if (WIFEXITED(statloc)) {
            printf("Remove proc id: %d \n", id);
            printf("Child send: %d \n", WEXITSTATUS(statloc));
        }
    }
}
