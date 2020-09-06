#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{
    int statloc;
    pid_t pid = fork();

    if (pid == 0) {
        return 3;
    }
    else {
        printf("Child PID: %d \n", pid);
        pid = fork();

        if (pid == 0) exit(7);
        else {
            printf("Child PID: %d \n", pid);
            wait(&statloc);
            if (WIFEXITED(statloc))
                printf("Child send 1: %d \n", WEXITSTATUS(statloc));

            wait(&statloc);
            if (WIFEXITED(statloc))
                printf("Child send 1: %d \n", WEXITSTATUS(statloc));

            sleep(30);
        }
    }
    return 0;
}
