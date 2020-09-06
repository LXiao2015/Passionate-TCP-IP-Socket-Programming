#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>

void sigint(int sig)
{
    char input;
    if (sig == SIGINT) {
        puts("Pressed CTRL+C. Quit? Y/N");
        input = getchar();
        if (input == 'Y') {
            exit(0);
        }
        fgetc(stdin);
    }
}

int main()
{
    struct sigaction act;
    act.sa_handler = sigint;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    sigaction(SIGINT, &act, 0);

    while (1) {
        puts("Waiting");
        sleep(1);
    }
    return 0;
}
