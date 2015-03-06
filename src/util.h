#ifndef UTIL
#define UTIL

#include <ncurses.h>

void clear_win_char(WINDOW* wp);

void notify(char* info, char* type, char* name);

void err_quit(char* message);

void err_sys(char* message);

void err_msg(char* message);

void err_ui(char* message);

#endif

