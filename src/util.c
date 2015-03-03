#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

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

void err_ui(char* message) {
    mvaddstr(LINES-1, 0, message);
    refresh();

    return;
}

