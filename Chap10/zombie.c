#include <stdio.h>
#include <unistd.h>

int main()
{
    pid_t pid = fork();

    if (pid == 0) {
        puts("Hi, I am a child process");
    } else {
        printf("Child Process ID: %d \n", pid);
        sleep(30);
    }
    return 0;
}
