#include <ncurses.h>
#include "bookcase.h"


#define QUIT -2


int update_content(WINDOW* wp, int fp) {

    return 0;
}

int update_bks(WINDOW* wp, bookcase_t* bcp, int highlight) {
    int ox = 20, x = 20, y = 1, counter = 0;
    book_t *bkp = NULL;

    for (bkp = bcp->booklist; bkp != NULL; bkp = bkp->next)
    {
        if (counter == highlight)
        {
            wattron(wp, A_REVERSE);
            mvwaddstr(wp, y, x, bkp->name);
            wattroff(wp, A_REVERSE);
        } else {
            mvwaddstr(wp, y, x, bkp->name);
        }

        x = ox;
        ++y;
        ++counter;
    }

    wrefresh(wp);

    return 0;
}

int update_bcs(WINDOW* wp, manifest_t* mp, int highlight) {
    int ox = 1, x = 1, y = 1, counter = 0;
    bookcase_t *bcp = NULL;

    for (bcp = mp->bclist; bcp != NULL; bcp = bcp->next)
    {
        if (counter == highlight)
        {
            wattron(wp, A_REVERSE);
            mvwaddstr(wp, y, x, bcp->name);
            wattroff(wp, A_REVERSE);
        } else {
            mvwaddstr(wp, y, x, bcp->name);
        }

        x = ox;
        ++y;
        ++counter;
    }

    wrefresh(wp);

    return 0;
}


int switch_to_bks(WINDOW* wp, WINDOW* last, bookcase_t* bcp) {
    int counter = 0, c = 0, highlight = 0, choice = 0, n_choices = bcp->bksize-1;
    book_t *bkp = NULL;

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

            default:
                break;
        }
        update_bks(wp, bcp, highlight);
        if(choice != -1) {
            counter = 0;
            for (bkp = bcp->booklist; bkp != NULL; bkp = bkp->next)
            {
            }
            if (choice == QUIT)
            {
                return 0;
            }
        }
    }

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

    WINDOW *wp_bcs = newwin(LINES, 20, 0, 0);
    WINDOW *wp_bks = newwin(LINES, COLS-20, 0, 20);
    keypad(wp_bcs, TRUE);
    keypad(wp_bks, TRUE);
    getch();
    box(wp_bcs, 0, 0);
    box(wp_bks, 0, 0);
    wrefresh(wp_bcs);
    wrefresh(wp_bks);

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

            default:
                break;
        }
        update_bcs(wp_bcs, mp, highlight);
        if(choice != -1) {
            counter = 0;
            for (bcp = mp->bclist; bcp != NULL; bcp = bcp->next)
            {
                if (counter++ == choice) {
                    switch_to_bks(wp_bks, wp_bcs, bcp);
                }
            }
            if (choice == QUIT)
            {
                break;
            }
        }
    }

    getch();

    delwin(wp_bcs);
    delwin(wp_bks);
    endwin();

    return 0;
}

