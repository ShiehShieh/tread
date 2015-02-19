#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

void err_quit(char* message) {
    fprintf(stderr, "%s\n", message);

    exit(0);
}

void err_sys(char* message) {
    fprintf(stderr, "%s\n", message);

    exit(0);
}

void err_msg(char* message) {
    fprintf(stderr, "%s\n", message);

    exit(0);
}

