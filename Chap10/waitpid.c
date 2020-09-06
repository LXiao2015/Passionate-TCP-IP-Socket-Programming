#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    int statloc;
    pid_t pid = fork();

    if (pid == 0) {
        sleep(15);
        return 24;
    } else {
        while (!waitpid(-1, &statloc, WNOHANG)) {
            sleep(3);
            puts("Do sth else.");
        }

        if (WIFEXITED(statloc))
            printf("Child send: %d \n", WEXITSTATUS(statloc));
    }
    return 0;
}
