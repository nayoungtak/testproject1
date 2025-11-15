#ifndef BOOK_H
#define BOOK_H

#include "utils.h"

#define MAX_BOOKS 200

typedef struct {
    char bookId[MAX_ID];
    char title[MAX_TITLE];
    char author[MAX_AUTHOR];
    char editor[MAX_AUTHOR];
    char publisher[MAX_PUBLISHER];
    int borrowStatus;
} Book;

extern Book books[MAX_BOOKS];
extern int bookCount;

void loadBooks();
void saveBooks();
int findBookIndex(const char* bookId);

#endif