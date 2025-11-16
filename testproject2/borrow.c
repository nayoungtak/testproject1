// borrow : 대출/반납/연장 기능
#define _CRT_SECURE_NO_WARNINGS
#include "borrow.h"
#include "user.h"
#include "book.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Borrow borrows[MAX_BORROWS];
int borrowCount = 0;

void loadBorrows() {
    FILE* fp = fopen("borrow.txt", "r");
    if (fp == NULL) {
        return;
    }

    borrowCount = 0;
    while (fscanf(fp, "%s %s %s %d %d",
        borrows[borrowCount].bookId,
        borrows[borrowCount].userId,
        borrows[borrowCount].borrowDate,
        &borrows[borrowCount].overdueStatus,
        &borrows[borrowCount].extensionStatus) == 5) {
        borrowCount++;
        if (borrowCount >= MAX_BORROWS) break;
    }

    fclose(fp);
}

void saveBorrows() {
    FILE* fp = fopen("borrow.txt", "w");
    if (fp == NULL) {
        printf("borrow.txt 파일을 열 수 없습니다.\n");
        return;
    }

    for (int i = 0; i < borrowCount; i++) {
        fprintf(fp, "%s %s %s %d %d\n",
            borrows[i].bookId,
            borrows[i].userId,
            borrows[i].borrowDate,
            borrows[i].overdueStatus,
            borrows[i].extensionStatus);
    }

    fclose(fp);
}

void borrowBook() {
    system("cls");
    printf("\n========================================\n");
    printf("           도서 대출\n");
    printf("========================================\n");

    int userIdx = findUserIndex(currentUserId);

    if (users[userIdx].overdueStatus == 1) {
        printf("연체 도서가 있어 대출이 불가능합니다.\n");
        return;
    }

    if (users[userIdx].borrowCount >= 3) {
        printf("최대 대출 권수(3권)를 초과했습니다.\n");
        return;
    }

    // 검색 방법 선택 추가
    printf("\n대출 방법을 선택하세요:\n");
    printf("1. 도서 번호로 대출\n");
    printf("2. 도서 이름으로 검색 후 대출\n");
    printf("선택: ");

    int searchType;
    scanf("%d", &searchType);
    clearInputBuffer();

    char bookId[MAX_ID] = "";

    if (searchType == 1) {
        // 기존 방식: 전체 목록 보여주기
        printf("\n대출 가능한 도서 목록:\n");
        printf("----------------------------------------\n");
        int availableCount = 0;
        for (int i = 0; i < bookCount; i++) {
            if (books[i].borrowStatus == 0) {
                printf("[%s] %s\n", books[i].bookId, books[i].title);
                printf("    저자: %s | 출판사: %s\n", books[i].author, books[i].publisher);
                printf("----------------------------------------\n");
                availableCount++;
            }
        }

        if (availableCount == 0) {
            printf("대출 가능한 도서가 없습니다.\n");
            return;
        }

        printf("\n대출할 도서 번호: ");
        scanf("%s", bookId);
        clearInputBuffer();
    }
    else if (searchType == 2) {
        char keyword[MAX_TITLE];
        printf("\n검색할 도서 이름 (일부만 입력 가능): ");
        fgets(keyword, MAX_TITLE, stdin);
        keyword[strcspn(keyword, "\n")] = 0;

        printf("\n검색 결과 (대출 가능한 도서):\n");
        printf("----------------------------------------\n");

        int foundCount = 0;
        for (int i = 0; i < bookCount; i++) {
            if (books[i].borrowStatus == 0 && strstr(books[i].title, keyword) != NULL) {
                printf("[%s] %s\n", books[i].bookId, books[i].title);
                printf("    저자: %s | 출판사: %s\n", books[i].author, books[i].publisher);
                printf("----------------------------------------\n");
                foundCount++;
            }
        }

        if (foundCount == 0) {
            printf("'%s'(으)로 검색된 대출 가능한 도서가 없습니다.\n", keyword);
            return;
        }

        printf("\n대출할 도서 번호: ");
        scanf("%s", bookId);
        clearInputBuffer();
    }
    else {
        printf("\n잘못된 선택입니다.\n");
        return;
    }

    int bookIdx = findBookIndex(bookId);
    if (bookIdx == -1) {
        printf("\n해당 도서를 찾을 수 없습니다.\n");
        return;
    }

    if (books[bookIdx].borrowStatus == 1) {
        printf("\n이미 대출중인 도서입니다.\n");
        return;
    }

    Borrow newBorrow;
    strcpy(newBorrow.bookId, bookId);
    strcpy(newBorrow.userId, currentUserId);
    getCurrentDate(newBorrow.borrowDate);
    newBorrow.overdueStatus = 0;
    newBorrow.extensionStatus = 0;

    borrows[borrowCount++] = newBorrow;
    books[bookIdx].borrowStatus = 1;
    users[userIdx].borrowCount++;

    saveBooks();
    saveBorrows();
    saveUsers();

    printf("\n대출이 완료되었습니다!\n");
    printf("도서명: %s\n", books[bookIdx].title);
    printf("대출일: %s\n", newBorrow.borrowDate);
    printf("반납 예정일: %s (14일 이내)\n", newBorrow.borrowDate);
}

void returnBook() {
    system("cls");
    printf("\n========================================\n");
    printf("           도서 반납\n");
    printf("========================================\n");

    int userIdx = findUserIndex(currentUserId);

    printf("\n내가 대출한 도서:\n");
    printf("----------------------------------------\n");
    int myBorrowCount = 0;
    for (int i = 0; i < borrowCount; i++) {
        if (strcmp(borrows[i].userId, currentUserId) == 0) {
            int bookIdx = findBookIndex(borrows[i].bookId);
            if (bookIdx != -1) {
                printf("[%s] %s\n", borrows[i].bookId, books[bookIdx].title);
                printf("    대출일: %s", borrows[i].borrowDate);
                if (borrows[i].overdueStatus == 1) {
                    printf("    연체중");
                }
                if (borrows[i].extensionStatus == 1) {
                    printf(" (연장함)");
                }
                printf("\n----------------------------------------\n");
                myBorrowCount++;
            }
        }
    }

    if (myBorrowCount == 0) {
        printf("대출한 도서가 없습니다.\n");
        return;
    }

    char bookId[MAX_ID];
    printf("\n반납할 도서 번호: ");
    scanf("%s", bookId);
    clearInputBuffer();

    int borrowIdx = -1;
    for (int i = 0; i < borrowCount; i++) {
        if (strcmp(borrows[i].bookId, bookId) == 0 &&
            strcmp(borrows[i].userId, currentUserId) == 0) {
            borrowIdx = i;
            break;
        }
    }

    if (borrowIdx == -1) {
        printf("\n해당 도서의 대출 기록을 찾을 수 없습니다.\n");
        return;
    }

    int bookIdx = findBookIndex(bookId);

    books[bookIdx].borrowStatus = 0;
    users[userIdx].borrowCount--;

    for (int i = borrowIdx; i < borrowCount - 1; i++) {
        borrows[i] = borrows[i + 1];
    }
    borrowCount--;

    int hasOverdue = 0;
    for (int i = 0; i < borrowCount; i++) {
        if (strcmp(borrows[i].userId, currentUserId) == 0 &&
            borrows[i].overdueStatus == 1) {
            hasOverdue = 1;
            break;
        }
    }
    users[userIdx].overdueStatus = hasOverdue;

    saveBooks();
    saveBorrows();
    saveUsers();

    printf("\n반납이 완료되었습니다!\n");
    printf("도서명: %s\n", books[bookIdx].title);
}

void extendBorrow() {
    system("cls");
    printf("\n========================================\n");
    printf("         대출 연장 신청\n");
    printf("========================================\n");

    int userIdx = findUserIndex(currentUserId);

    if (users[userIdx].overdueStatus == 1) {
        printf("연체 도서가 있어 연장이 불가능합니다.\n");
        return;
    }

    printf("\n연장 가능한 도서:\n");
    printf("----------------------------------------\n");
    int extendableCount = 0;
    for (int i = 0; i < borrowCount; i++) {
        if (strcmp(borrows[i].userId, currentUserId) == 0 &&
            borrows[i].extensionStatus == 0) {
            int bookIdx = findBookIndex(borrows[i].bookId);
            if (bookIdx != -1) {
                printf("[%s] %s\n", borrows[i].bookId, books[bookIdx].title);
                printf("    대출일: %s\n", borrows[i].borrowDate);
                printf("----------------------------------------\n");
                extendableCount++;
            }
        }
    }

    if (extendableCount == 0) {
        printf("연장 가능한 도서가 없습니다.\n");
        return;
    }

    char bookId[MAX_ID];
    printf("\n연장할 도서 번호: ");
    scanf("%s", bookId);
    clearInputBuffer();

    int borrowIdx = -1;
    for (int i = 0; i < borrowCount; i++) {
        if (strcmp(borrows[i].bookId, bookId) == 0 &&
            strcmp(borrows[i].userId, currentUserId) == 0) {
            borrowIdx = i;
            break;
        }
    }

    if (borrowIdx == -1) {
        printf("\n해당 도서의 대출 기록을 찾을 수 없습니다.\n");
        return;
    }

    if (borrows[borrowIdx].extensionStatus == 1) {
        printf("\n이미 연장한 도서입니다. (1회만 가능)\n");
        return;
    }

    borrows[borrowIdx].extensionStatus = 1;
    saveBorrows();

    printf("\n대출 연장이 완료되었습니다!\n");
    printf("추가 대출 기간: 14일\n");
}

void showBorrowStatus() {
    system("cls");
    printf("\n========================================\n");
    printf("          대출 현황\n");
    printf("========================================\n");

    int userIdx = findUserIndex(currentUserId);
    printf("사용자: %s\n", users[userIdx].userName);
    printf("총 대출 권수: %d권\n", users[userIdx].borrowCount);
    printf("========================================\n\n");

    if (users[userIdx].borrowCount == 0) {
        printf("대출한 도서가 없습니다.\n");
        return;
    }

    char today[11];
    getCurrentDate(today);

    for (int i = 0; i < borrowCount; i++) {
        if (strcmp(borrows[i].userId, currentUserId) == 0) {
            int bookIdx = findBookIndex(borrows[i].bookId);
            if (bookIdx != -1) {
                printf("도서번호: %s\n", borrows[i].bookId);
                printf("도서명: %s\n", books[bookIdx].title);
                printf("저자: %s\n", books[bookIdx].author);
                printf("대출일: %s\n", borrows[i].borrowDate);

                int days = calculateDaysDiff(borrows[i].borrowDate, today);
                int maxDays = borrows[i].extensionStatus == 1 ? 28 : 14;
                int remainDays = maxDays - days;

                printf("경과일: %d일", days);
                if (borrows[i].extensionStatus == 1) {
                    printf(" (연장함)");
                }
                printf("\n");

                if (remainDays > 0) {
                    printf("반납까지: %d일 남음\n", remainDays);
                }
                else {
                    printf("    연체: %d일 경과\n", -remainDays);
                }

                printf("----------------------------------------\n");
            }
        }
    }
}

void updateOverdueStatus() {
    char today[11];
    getCurrentDate(today);

    for (int i = 0; i < borrowCount; i++) {
        int days = calculateDaysDiff(borrows[i].borrowDate, today);
        int maxDays = borrows[i].extensionStatus == 1 ? 28 : 14;

        if (days > maxDays) {
            borrows[i].overdueStatus = 1;

            int userIdx = findUserIndex(borrows[i].userId);
            if (userIdx != -1) {
                users[userIdx].overdueStatus = 1;
            }
        }
    }

    saveBorrows();
    saveUsers();
}