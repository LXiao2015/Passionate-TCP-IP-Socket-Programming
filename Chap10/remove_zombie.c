#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>

void read_childproc(int sig)
{
    int statloc;
    pid_t id;
    while ((id = waitpid(-1, &statloc, WNOHANG)) != -1) {
        printf("Enter signal handler...");
        if (WIFEXITED(statloc)) {
            printf("Remove proc id: %d \n", id);
            printf("Child send: %d \n", WEXITSTATUS(statloc));
        }
    }
}

int main()
{
    pid_t pid;
    struct sigaction act;
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    sigaction(SIGCHLD, &act, 0);    // 子进程终止产生SIGCHLD信号

    pid = fork();
    if (pid == 0) {
        puts("I am child process");
        sleep(10);
        return 12;
    } else {
        printf("Child proc id: %d \n", pid);
        pid = fork();
        if (pid == 0) {
            puts("I am child process");
            sleep(10);
            exit(24);
        } else {
            int i;
            printf("Child proc id: %d \n", pid);
            for (i = 0; i < 5; ++i) {
                puts("Wait...");
                sleep(5);
            }
        }
    }
    return 0;
}
