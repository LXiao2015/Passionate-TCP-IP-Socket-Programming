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

    FD_ZERO(&reads);    // ����λ��ʼ��Ϊ0
    FD_SET(0, &reads);    // indexΪ0��fdע���reads, 0�ǿ���̨��׼����

    while (1)
    {
        temps = reads;
        timeout.tv_sec = 5;    // ÿ�ε���selectǰ��ʼ����ʱʱ�䣬��Ϊselect�Ὣ���ֵ�滻Ϊʣ��ʱ��
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
            if (FD_ISSET(0, &temps))    // �ж��Ƿ����indexΪ0��fd
            {
                str_len = read(0, buf, BUF_SIZE);
                buf[str_len] = 0;
                printf("Message feom console: %s", buf);
            }
        }
    }
    return 0;
}
