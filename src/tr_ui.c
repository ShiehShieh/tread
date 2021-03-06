#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>
#include <ncurses.h>
#include <sys/stat.h>
#include "bookcase.h"
#include "util.h"


#define QUIT -2
extern unsigned int IOBUFF;


int update_content(WINDOW* wp, int fd, int ins, long index[], int *now) {
    int x = 0, y = 0;
    char *content = NULL;
    FILE *fp = NULL;
    struct stat buf;

    if (fstat(fd, &buf) < 0) {
        err_ui("fstat error.");
        return -1;
    }

    if ((ins == 1 && *now == 0)
            || (ins == 2 && *now == 1023)
            || (ins == 2 && index[(*now)+1] == buf.st_size)) {
        return 0;
    }

    if ((fp = fdopen(fd, "r")) == NULL) {
        err_ui("fdopen error.");
        return -1;
    }

    if ((content = (char*)malloc(sizeof(char)*IOBUFF)) == NULL) {
        err_ui("malloc error.");
        return -1;
    }

    clear_win_char(wp);
    wmove(wp, 1, 1);
    getyx(wp, y, x);
    if (ins == 1) {
        fseek(fp, index[--(*now)], SEEK_SET);
    } else if (ins == 2) {
        fseek(fp, index[++(*now)], SEEK_SET);
    }

    while(y < LINES-2) {
        if ((fgets(content, IOBUFF, fp)) == NULL) {
            if (feof(fp)) {
                break;
            } else if (ferror(fp)) {
                err_ui("fgets error.");
                return -1;
            }
        }
        waddstr(wp, content);
        getyx(wp, y, x);
        wmove(wp, y, x+1);
    }
    index[(*now)+1] = ftell(fp);
    wrefresh(wp);

    return 0;
}


int update_bks(WINDOW* wp, bookcase_t* bcp, int highlight) {
    int x = 1, y = 1, counter = 0;
    book_t *bkp = NULL;

    for (bkp = bcp->booklist; bkp != NULL; bkp = bkp->next) {
        if (counter == highlight) {
            wattron(wp, A_REVERSE);
            mvwaddstr(wp, y, x, bkp->name);
            wattroff(wp, A_REVERSE);
        } else {
            mvwaddstr(wp, y, x, bkp->name);
        }
        x = 1;
        ++y;
        ++counter;
    }

    wrefresh(wp);

    return 0;
}


int update_bcs(WINDOW* wp, manifest_t* mp, int highlight) {
    int x = 1, y = 1, counter = 0;
    bookcase_t *bcp = NULL;

    for (bcp = mp->bclist; bcp != NULL; bcp = bcp->next) {
        if (counter == highlight) {
            wattron(wp, A_REVERSE);
            mvwaddstr(wp, y, x, bcp->name);
            wattroff(wp, A_REVERSE);
        } else {
            mvwaddstr(wp, y, x, bcp->name);
        }
        x = 1;
        ++y;
        ++counter;
    }
    wrefresh(wp);

    return 0;
}


int switch_to_bk(WINDOW* wp, char* filename) {
    int c = 0, choice = 0, ins = 0, fd = 0, now = 0;
    long index[1024]; 

    memset(index, 0, sizeof(long)*1024);

    if ((fd = open(filename, O_RDONLY)) < -1) {
        mvaddstr(LINES-1, 0, "Can not open this file.");
        refresh();
        return -1;
    }

    notify("Done", "book", filename);
    update_content(wp, fd, ins, index, &now);
    while(1) {
        ins = 0;
        choice = -1;
        c = getch();
        switch (c) {
            case KEY_UP:
            case 'k':
                ins = 1;
                break;
            case KEY_DOWN:
            case 'j':
                ins = 2;
                break;
            case 'q':
                choice = QUIT;
                break;
        }
        notify("Done", "book", filename);
        update_content(wp, fd, ins, index, &now);
        if(choice != -1) {
            if (choice == QUIT) {
                return 0;
            }
        }
    }

    return 0;
}


int switch_to_bks(WINDOW* wp, bookcase_t* bcp) {
    int counter = 0, c = 0, highlight = 0, choice = 0, n_choices = bcp->bksize-1;
    book_t *bkp = NULL;

    clear_win_char(wp);
    notify("Done", "bookcase", bcp->name);
    update_bks(wp, bcp, highlight);
    while(1) {
        choice = -1;
        c = getch();
        switch (c) {
            case KEY_UP:
            case 'k':
                if(highlight == 0) {
                    highlight = n_choices;
                }
                else {
                    --highlight;
                }
                break;
            case KEY_DOWN:
            case 'j':
                if(highlight == n_choices) {
                    highlight = 0;
                }
                else {
                    ++highlight;
                }
                break;
            case 'q':
                choice = QUIT;
                break;
            case 10:
                choice = highlight;
                break;
        }
        update_bks(wp, bcp, highlight);
        if(choice != -1) {
            if (choice == QUIT) {
                return 0;
            }

            counter = 0;
            for (bkp = bcp->booklist; bkp != NULL; bkp = bkp->next) {
                if (counter++ == choice) {
                    switch_to_bk(wp, bkp->path);
                    clear_win_char(wp);
                    update_bks(wp, bcp, highlight);

                    break;
                }
            }
        }
    }

    return 0;
}


int display_help(WINDOW* wp) {
    int y = 0;

    clear_win_char(wp);

    mvwaddstr(wp, y, 1, "h: display this menu");
    ++y;
    mvwaddstr(wp, y, 1, "q: quit");
    ++y;
    mvwaddstr(wp, y, 1, "jk: down,up");
    ++y;
    mvwaddstr(wp, y, 1, "h: display this menu");
    ++y;

    return 0;
}


int display_ui(manifest_t* mp) {
    int c = 0, counter = 0, choice = -1, highlight = 0, n_choices = mp->bcsize-1;
    bookcase_t *bcp = NULL;

    initscr();
    clear();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    WINDOW *wp_bcs = newwin(LINES-1, 20, 0, 0);
    WINDOW *wp_bks = newwin(LINES-1, COLS-20, 0, 20);
    keypad(wp_bcs, TRUE);
    keypad(wp_bks, TRUE);
    getch(); // strange enough... the box of the window can't been seen without this line.
    box(wp_bcs, 0, 0);
    box(wp_bks, 0, 0);
    wrefresh(wp_bcs);
    wrefresh(wp_bks);

    notify("Done", "manifest", mp->name);
    update_bcs(wp_bcs, mp, 0);
    while(1) {
        choice = -1;
        c = getch();
        switch (c) {
            case KEY_UP:
            case 'k':
                if(highlight == 0) {
                    highlight = n_choices;
                }
                else {
                    --highlight;
                }
                break;
            case KEY_DOWN:
            case 'j':
                if(highlight == n_choices) {
                    highlight = 0;
                }
                else {
                    ++highlight;
                }
                break;
            case 'q':
                choice = QUIT;
                break;
            case 10:
                choice = highlight;
                break;
        }
        notify("Done", "manifest", mp->name);
        update_bcs(wp_bcs, mp, highlight);
        if(choice != -1) {
            if (choice == QUIT) {
                mvaddstr(LINES-1, 0, "Quit Tread? ([yes]/no):");
                refresh();
                c = getch();
                if (c == 'y') {
                    break;
                } else {
                    notify("Stop quiting", "manifest", mp->name);
                    continue;
                }
            }

            counter = 0;
            for (bcp = mp->bclist; bcp != NULL; bcp = bcp->next) {
                if (counter++ == choice) {
                    switch_to_bks(wp_bks, bcp);
                    break;
                }
            }
        }
    }

    delwin(wp_bcs);
    delwin(wp_bks);
    endwin();

    return 0;
}

