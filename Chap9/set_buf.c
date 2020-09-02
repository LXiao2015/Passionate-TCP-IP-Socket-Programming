#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
void error_handling(char *message);

int main(int argc, char *argv[])
{
    int sock;
    socklen_t len;
    int state, snd_buf=1024*32, rcv_buf=1024*32;

    sock = socket(PF_INET, SOCK_STREAM, 0);

    len = sizeof(snd_buf);
    state = setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)&snd_buf, len);
    if (state)
        error_handling("getsockopt() error!");

    len = sizeof(rcv_buf);
    state = setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void*)&rcv_buf, len);
    if (state)
        error_handling("getsockopt() error!");

    len = sizeof(snd_buf);
    state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)&snd_buf, &len);
    if (state)
        error_handling("getsockopt() error!");

    len = sizeof(rcv_buf);
    state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void*)&rcv_buf, &len);
    if (state)
        error_handling("getsockopt() error!");

    printf("Input buffer size: %d \n", rcv_buf);
    printf("Output buffer size: %d \n", snd_buf);

    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
