#ifndef BOOKCASE
#define BOOKCASE

#include <stddef.h>

struct tmp_book;
struct tmp_bookcase;
struct tmp_manifest;

typedef struct tmp_book{
    char* name;
    char* path;
    struct tmp_book* next;
	struct tmp_bookcase* belongto;
} book_t;

typedef struct tmp_bookcase {
	char* name;
	char* path;
	book_t* booklist;
    struct tmp_bookcase* next;
    struct tmp_manifest* belongto;
    size_t bksize;
} bookcase_t;

typedef struct tmp_manifest{
    char* name;
    char* path;
	bookcase_t* bclist;
    size_t bcsize;;
} manifest_t;

void print_manifest(manifest_t* mp);

void print_bookcase(bookcase_t* bcp);

int chcase(book_t* book, bookcase_t* newbc);

char* path_join(char* a, char* b);

int build_book(char* path, book_t* bkp);

int insert_bklist(bookcase_t* bcp, book_t* item);

int dele_from_booklist(bookcase_t* bcp, book_t* item);

int build_bookcase(char* directory, bookcase_t* bc);

int insert_bclist(manifest_t* mp, bookcase_t* item);

int dele_from_bclist(manifest_t* mp, bookcase_t* item);

int build_manifest(char *directory, manifest_t* mp);

#endif

