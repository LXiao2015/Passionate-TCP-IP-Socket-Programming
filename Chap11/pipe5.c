#include <stdio.h>
#include <unistd.h>
#define BUF_SIZE 30

int main(int argc, char *argv[])
{
    int fds1[2], fds2[2];
    pid_t pid;
    char buf[BUF_SIZE];

    pipe(fds1), pipe(fds2);
    pid = fork();
    if (pid == 0) {
        char *str[] = {
            "Who are you?",
            "Hello",
            "Bye-bye"
        };
        for (int i = 0; i < 3; ++i) {
            write(fds1[1], str[i], /*sizeof(str[i])*/BUF_SIZE);
            read(fds2[0], buf, BUF_SIZE);
            printf("Child proc output: %s\n", buf);
        }
    } else {
        char *str[] = {
            "Thanks you for your message.",
            "Hi",
            "See you"
        };
        for (int i = 0; i < 3; ++i) {
            read(fds1[0], buf, BUF_SIZE);
            printf("Parent proc output: %s\n", buf);
            write(fds2[1], str[i], /*sizeof(str[i])*/BUF_SIZE);
        }
    }
    return 0;
}
