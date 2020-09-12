#include <stdio.h>
#include <unistd.h>
#define BUF_SIZE 30

int main(int argc, char *argv[])
{
    int fds[2];
    pid_t pid;
    char buf[BUF_SIZE];

    pipe(fds);
    pid = fork();
    if (pid == 0) {
        char str[] = "Who are you?";
        write(fds[1], str, sizeof(str));
        read(fds[0], buf, BUF_SIZE);
        printf("Child proc output: %s\n", buf);
    } else {
        sleep(2);
        read(fds[0], buf, BUF_SIZE);
        printf("Parent proc output: %s\n", buf);
        char str[] = "Thanks you for your message.";
        write(fds[1], str, sizeof(str));
        sleep(3);
    }
    return 0;
}
