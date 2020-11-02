#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>
#define BUF_SIZE 30

int main(int argc, char *argv[])
{
    fd_set reads, temps;
    int result, str_len;
    char buf[BUF_SIZE];
    struct timeval timeout;

    FD_ZERO(&reads);    // 所有位初始化为0
    FD_SET(0, &reads);    // index为0的fd注册进reads, 0是控制台标准输入

    while (1)
    {
        temps = reads;
        timeout.tv_sec = 5;    // 每次调用select前初始化超时时间，因为select会将这个值替换为剩余时间
        result = select(1, &temps, 0, 0, &timeout);
        if (result == -1)
        {
            puts("select() error!");
            break;
        }
        else if (result == 0)
        {
            puts("Time out!");
        }
        else
        {
            if (FD_ISSET(0, &temps))    // 判断是否包含index为0的fd
            {
                str_len = read(0, buf, BUF_SIZE);
                buf[str_len] = 0;
                printf("Message feom console: %s", buf);
            }
        }
    }
    return 0;
}
