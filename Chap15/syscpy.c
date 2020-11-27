#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#define BUF_SIZE 3    // 最短数组长度

int main()
{
    int fd1, fd2;
    int len;
    char buf[BUF_SIZE];

    fd1 = open("news.txt", O_RDONLY);    // 使用尽量大的文件
    fd2 = open("cpy.txt", O_WRONLY|O_CREAT|O_TRUNC);

    while ((len = read(fd1, buf, sizeof(buf))) > 0)
        write(fd2, buf, len);

    close(fd1);
    close(fd2);
    return 0;
}
