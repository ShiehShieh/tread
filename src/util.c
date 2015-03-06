#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <ncurses.h>


void clear_win_char(WINDOW* wp) {
    wclear(wp);
    box(wp, 0, 0);
    wrefresh(wp);

    return;
}


void notify(char* info, char* type, char* name) {
    move(LINES-1, 0);
    clrtoeol();

    mvaddstr(LINES-1, 0, info);
    attron(A_REVERSE);
    mvaddstr(LINES-1, COLS/2, type);
    attroff(A_REVERSE);
    mvaddstr(LINES-1, COLS/2+strlen(type)+2, basename(name));

    refresh();

    return;
}


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
    notify("", "", "");
    mvaddstr(LINES-1, 0, message);
    refresh();

    return;
}

