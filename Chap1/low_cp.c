#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define BUF_SIZE 100
void error_handling(char* message);

int main(void)
{
    int fd1, fd2;
    int n;
    char buf[BUF_SIZE];

    fd1 = open("data.txt", O_RDONLY);  // 只读
    if (fd1 == -1)
        error_handling("fd1 open() error!");

    n = read(fd1, buf, sizeof(buf));
    if (n == -1)
        error_handling("read() error!");

    fd2 = open("data_copy.txt", O_CREAT|O_WRONLY|O_TRUNC);  // 创建文件，只写，若已存在清空原内容
    if (fd2 == -1)
        error_handling("fd2 open() error!");

    if (write(fd2, buf, n) == -1)
        error_handling("write() error!");

    close(fd1);
    close(fd2);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
