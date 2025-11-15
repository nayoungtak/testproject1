#pragma once
#ifndef BORROW_H
#define BORROW_H

#include "utils.h"

#define MAX_BORROWS 500

typedef struct {
    char bookId[MAX_ID];
    char userId[MAX_ID];
    char borrowDate[11];
    int overdueStatus;
    int extensionStatus;
} Borrow;

extern Borrow borrows[MAX_BORROWS];
extern int borrowCount;

void loadBorrows();
void saveBorrows();
void borrowBook();
void returnBook();
void extendBorrow();
void showBorrowStatus();
void updateOverdueStatus();

#endif