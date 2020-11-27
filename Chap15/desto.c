#include <stdio.h>
#include <fcntl.h>

int main()
{
    FILE *fp;
    int fd = open("news.txt", O_WRONLY|O_CREAT|O_TRUNC);
    if (fd == -1) {
        fputs("File open error", stdout);
        return -1;
    }

    fp = fdopen(fd, "w");
    fputs("Network C programming \n", fp);
    fclose(fp);    // 无需再调用系统函数关闭
    return 0;
}
