#ifndef TR_UI
#define TR_UI

#include <ncurses.h>
#include "bookcase.h"


int update_bk(WINDOW* wp, int fp, long index[], int *now);

int update_bks(WINDOW* wp, bookcase_t* bcp, int highlight);

int update_bcs(WINDOW* wp, manifest_t* mp, int highlight);

int switch_to_bk(WINDOW* wp, char* filename);

int switch_to_bks(WINDOW* wp, bookcase_t* bcp);

int display_ui(manifest_t* mp);

#endif

