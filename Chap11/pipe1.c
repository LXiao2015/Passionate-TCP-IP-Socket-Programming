#include <stdio.h>
#include <unistd.h>
#define BUF_SIZE 30

int main(int argc, char *argv[])
{
    int fds[2];
    pid_t pid;

    pipe(fds);
    pid = fork();
    if (pid == 0) {
        char str[] = "Who are you?";
        write(fds[1], str, sizeof(str));
    } else {
        char buf[BUF_SIZE];
        read(fds[0], buf, BUF_SIZE);
        puts(buf);
    }
    return 0;
}
