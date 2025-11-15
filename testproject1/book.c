#define _CRT_SECURE_NO_WARNINGS
#include "book.h"
#include <stdio.h>
#include <string.h>

Book books[MAX_BOOKS];
int bookCount = 0;

void loadBooks() {
    FILE* fp = fopen("book.txt", "r");
    if (fp == NULL) {
        return;
    }

    bookCount = 0;
    char line[500];

    while (fscanf(fp, "%s", books[bookCount].bookId) == 1) {
        fgetc(fp);
        fgets(books[bookCount].title, MAX_TITLE, fp);
        books[bookCount].title[strcspn(books[bookCount].title, "\n")] = 0;

        fgets(books[bookCount].author, MAX_AUTHOR, fp);
        books[bookCount].author[strcspn(books[bookCount].author, "\n")] = 0;

        fgets(books[bookCount].editor, MAX_AUTHOR, fp);
        books[bookCount].editor[strcspn(books[bookCount].editor, "\n")] = 0;

        fgets(books[bookCount].publisher, MAX_PUBLISHER, fp);
        books[bookCount].publisher[strcspn(books[bookCount].publisher, "\n")] = 0;

        fscanf(fp, "%d", &books[bookCount].borrowStatus);
        fgetc(fp);

        bookCount++;
        if (bookCount >= MAX_BOOKS) break;
    }

    fclose(fp);
}

void saveBooks() {
    FILE* fp = fopen("book.txt", "w");
    if (fp == NULL) {
        printf("book.txt 파일을 열 수 없습니다.\n");
        return;
    }

    for (int i = 0; i < bookCount; i++) {
        fprintf(fp, "%s\n%s\n%s\n%s\n%s\n%d\n",
            books[i].bookId,
            books[i].title,
            books[i].author,
            books[i].editor,
            books[i].publisher,
            books[i].borrowStatus);
    }

    fclose(fp);
}

int findBookIndex(const char* bookId) {
    for (int i = 0; i < bookCount; i++) {
        if (strcmp(books[i].bookId, bookId) == 0) {
            return i;
        }
    }
    return -1;
}