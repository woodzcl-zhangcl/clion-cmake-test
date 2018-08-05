#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>
#include <unistd.h>

bool bExit = false;

// sigint signal
void handler(int sig) {
    bExit = true;
    printf("will be exiting process.\n");
}

int main(int argc, char *argv[]) {
    printf("Hello, World!\n");

    // capture sigint
    void (*f)(int);
    f = signal(SIGINT, (void (*)(int)) handler);
    if (SIG_ERR == f) {
        exit(1);
    }

    for (; !bExit;) {
        int i = 0;
        i++;
    }

    pause(); // will be awake by Ctr+C

    return 0;
}
