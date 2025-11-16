// borrow : 대출/반납/연장 기능
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "user.h"
#include "book.h"
#include "borrow.h"
#include "utils.h"

void mainMenu();

int main() {
    int choice;
    int isLoggedIn = 0;

    loadUsers();
    loadBooks();
    loadBorrows();

    while (1) {
        system("cls");
        printf("\n========================================\n");
        printf("      도서관 대출 관리 시스템\n");
        printf("========================================\n");

        if (!isLoggedIn) {
            printf("1. 회원가입\n");
            printf("2. 로그인\n");
            printf("0. 종료\n");
            printf("========================================\n");
            printf("선택: ");
            scanf("%d", &choice);
            clearInputBuffer();

            switch (choice) {
            case 1:
                signUp();
                break;
            case 2:
                if (login()) {
                    isLoggedIn = 1;
                }
                break;
            case 0:
                printf("\n프로그램을 종료합니다.\n");
                return 0;
            default:
                printf("\n잘못된 선택입니다.\n");
            }
        }
        else {
            mainMenu();
            isLoggedIn = 0;
        }

        if (!isLoggedIn) {
            printf("\n계속하려면 Enter를 누르세요...");
            getchar();
        }
    }

    return 0;
}

void mainMenu() {
    int choice;

    while (1) {
        system("cls");
        updateOverdueStatus();

        int userIdx = findUserIndex(currentUserId);
        printf("\n========================================\n");
        printf("       도서관 대출 관리 시스템\n");
        printf("========================================\n");
        printf("사용자: %s님\n", users[userIdx].userName);
        printf("현재 대출 도서: %d권\n", users[userIdx].borrowCount);
        if (users[userIdx].overdueStatus == 1) {
            printf("    연체 도서가 있습니다!\n");
        }
        printf("========================================\n");
        printf("1. 도서 대출\n");
        printf("2. 도서 반납\n");
        printf("3. 대출 연장 신청\n");
        printf("4. 대출 현황 출력\n");
        printf("5. 회원 탈퇴\n");
        printf("6. 로그아웃\n");
        printf("0. 종료\n");
        printf("========================================\n");
        printf("선택: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {
        case 1:
            borrowBook();
            break;
        case 2:
            returnBook();
            break;
        case 3:
            extendBorrow();
            break;
        case 4:
            showBorrowStatus();
            break;
        case 5:
            deleteAccount();
            if (currentUserId[0] == '\0') {
                return;
            }
            break;
        case 6:
            currentUserId[0] = '\0';
            printf("\n로그아웃 되었습니다.\n");
            return;
        case 0:
            printf("\n프로그램을 종료합니다.\n");
            exit(0);
        default:
            printf("\n잘못된 선택입니다.\n");
        }

        printf("\n계속하려면 Enter를 누르세요...");
        getchar();
    }
}