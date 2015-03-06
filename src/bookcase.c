#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <libgen.h>
#include <sys/stat.h>

#include "bookcase.h"
#include "util.h"


unsigned int IOBUFF = 2048;


// debug function
void print_manifest(manifest_t* mp) {
    bookcase_t* bcp = NULL;

    for (bcp = mp->bclist; bcp != NULL; bcp = bcp->next) {
        if (write(STDOUT_FILENO, bcp->name, strlen(bcp->name)) != \
                (ssize_t)strlen(bcp->name)) {
            err_sys("write error");
            return;
        }
        printf(": ");
        print_bookcase(bcp);
        printf("\n");
    }

    return;
}


// debug function
void print_bookcase(bookcase_t* bcp) {
    book_t* bkp = NULL;

    for (bkp = bcp->booklist; bkp != NULL; bkp = bkp->next) {
        if (write(STDOUT_FILENO, bkp->name, strlen(bkp->name)) != \
                (ssize_t)strlen(bkp->name)) {
            err_sys("write error");
            return;
        }
        printf("     ");
        if (write(STDOUT_FILENO, bkp->path, strlen(bkp->path)) != \
                (ssize_t)strlen(bkp->path)) {
            err_sys("write error");
            return;
        }
        printf("\n");
    }

    return;
}


int chstack(book_t* book, bookcase_t* newbc) {
    dele_from_booklist(book->belongto, book);
    insert_bklist(newbc, book);

    return 0;
}


char* path_join(char* a, char* b) {
    if (a[strlen(a)-1] != '/') {
        return strcat(strcat(a, "/"), b);
    } else {
        return strcat(a, b);
    }
}


int build_book(char* path, book_t* bkp) {
    char* temp = basename(path);

    if (temp[0] == '.') {
        return -1;
    }

    bkp->name = (char*)malloc(sizeof(char)*(strlen(temp)+1));
    strcpy(bkp->name, temp);
    bkp->path = (char*)malloc(sizeof(char)*(strlen(path)+1));
    strcpy(bkp->path, path);
    bkp->belongto = NULL;
    bkp->next = NULL;

    return 0;
}


int insert_bklist(bookcase_t* bcp, book_t* item) {
    item->next = bcp->booklist;
    bcp->booklist = item;
    item->belongto = bcp;
    ++bcp->bksize;

    return 0;
}


int dele_from_booklist(bookcase_t* bcp, book_t* item) {
    book_t* pre = bcp->booklist;
    book_t* bkp = bcp->booklist;

    for (; bkp != NULL; bkp = bkp->next) {
        if (strcpy(item->path, bkp->path) == 0) {
            if (pre == bkp) {
                bcp->booklist = bkp->next;
            } else {
                pre->next = bkp->next;
            }
        }
    }

    return 0;
}


int build_bookcase(char* directory, bookcase_t* bcp) {
    char* temp = basename(directory);
    DIR* dir = NULL;
    struct dirent* subfile = NULL;
    book_t* bkp = NULL;

    if (temp[0] == '.') {
        return -1;
    }

    if ((dir = opendir(directory)) == NULL) {
        err_msg("bookcase should be a directory");
        return -1;
    }

    bcp->name = (char*)malloc(sizeof(char)*(strlen(temp)+1));
    strcpy(bcp->name, temp);
    bcp->path = (char*)malloc(sizeof(char)*(strlen(directory)+1));
    strcpy(bcp->path, directory);
    bcp->next = NULL;
    bcp->booklist = NULL;
    bcp->belongto = NULL;
    bcp->bksize = 0;

    while((subfile = readdir(dir)) != NULL) {
        char* temp = (char*)malloc(sizeof(char)*(strlen(directory)+strlen(subfile->d_name)+2));
        temp = strcpy(temp, directory);
        bkp = (book_t*)malloc(sizeof(book_t));
        if ((build_book(path_join(temp, subfile->d_name), bkp)) < 0) {
            free(temp);
            free(bkp);
            continue;
        }
        insert_bklist(bcp, bkp);
    }

    return 0;
}


int insert_bclist(manifest_t* mp, bookcase_t* item) {
    item->next = mp->bclist;
    mp->bclist = item;
    item->belongto = mp;
    ++mp->bcsize;

    return 0;
}


int dele_from_bclist(manifest_t* mp, bookcase_t* item) {
    bookcase_t* pre = mp->bclist;
    bookcase_t* bcp = mp->bclist;

    for (; bcp != NULL; bcp = bcp->next) {
        if (strcpy(item->path, bcp->path) == 0) {
            if (pre == bcp) {
                mp->bclist = bcp->next;
            } else {
                pre->next = bcp->next;
            }
        }
    }

    return 0;
}


int build_manifest(char *directory, manifest_t* mp) {
    char* temp = basename(directory);
    DIR* rootdir = NULL;
    struct dirent* subdir = NULL;
    bookcase_t* bcp = NULL;

    if (temp[0] == '.') {
        err_quit("path can not start with \'.\'");
    }

    if ((rootdir = opendir(directory)) == NULL) {
        perror("Can't open directory");
        exit(1);
    }

    mp->name = (char*)malloc(sizeof(char)*(strlen(temp)+1));
    strcpy(mp->name, temp);
    mp->path = (char*)malloc(sizeof(char)*(strlen(directory)+1));
    strcpy(mp->path, directory);
    mp->bclist = NULL;
    mp->bcsize = 0;

    while((subdir = readdir(rootdir)) != NULL) {
        char* temp = (char*)malloc(sizeof(char)*(strlen(directory)+1));
        temp = strcpy(temp, directory);
        bcp = (bookcase_t*)malloc(sizeof(bookcase_t));
        if ((build_bookcase(path_join(temp, subdir->d_name), bcp)) < 0) {
            free(temp);
            free(bcp);
            continue;
        }
        insert_bclist(mp, bcp);
    }

    return 0;
}

