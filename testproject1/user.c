// user : 사용자 관리 (회원가입, 로그인)
#define _CRT_SECURE_NO_WARNINGS
#include "user.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

User users[MAX_USERS];
int userCount = 0;
char currentUserId[MAX_ID] = "";

void loadUsers() {
    FILE* fp = fopen("user.txt", "r");
    if (fp == NULL) {
        return;
    }

    userCount = 0;
    while (fscanf(fp, "%s %s %s %s %d %d",
        users[userCount].userId,
        users[userCount].password,
        users[userCount].userName,
        users[userCount].phone,
        &users[userCount].borrowCount,
        &users[userCount].overdueStatus) == 6) {
        userCount++;
        if (userCount >= MAX_USERS) break;
    }

    fclose(fp);
}

void saveUsers() {
    FILE* fp = fopen("user.txt", "w");
    if (fp == NULL) {
        printf("user.txt 파일을 열 수 없습니다.\n");
        return;
    }

    for (int i = 0; i < userCount; i++) {
        fprintf(fp, "%s %s %s %s %d %d\n",
            users[i].userId,
            users[i].password,
            users[i].userName,
            users[i].phone,
            users[i].borrowCount,
            users[i].overdueStatus);
    }

    fclose(fp);
}

void signUp() {
    system("cls");
    printf("\n========================================\n");
    printf("           회원가입\n");
    printf("========================================\n");

    if (userCount >= MAX_USERS) {
        printf("더 이상 회원가입을 할 수 없습니다.\n");
        return;
    }

    User newUser;

    printf("아이디: ");
    scanf("%s", newUser.userId);
    clearInputBuffer();

    if (isUserExists(newUser.userId)) {
        printf("\n이미 존재하는 아이디입니다.\n");
        return;
    }

    printf("비밀번호: ");
    scanf("%s", newUser.password);
    clearInputBuffer();

    printf("이름: ");
    scanf("%s", newUser.userName);
    clearInputBuffer();

    printf("전화번호(예시:010-0000-0000): ");
    scanf("%s", newUser.phone);
    clearInputBuffer();

    newUser.borrowCount = 0;
    newUser.overdueStatus = 0;

    users[userCount++] = newUser;
    saveUsers();

    printf("\n회원가입이 완료되었습니다!\n");
}

int login() {
    system("cls");
    printf("\n========================================\n");
    printf("            로그인\n");
    printf("========================================\n");

    char userId[MAX_ID];
    char password[MAX_PW];

    printf("아이디: ");
    scanf("%s", userId);
    clearInputBuffer();

    printf("비밀번호: ");
    scanf("%s", password);
    clearInputBuffer();

    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].userId, userId) == 0 &&
            strcmp(users[i].password, password) == 0) {
            strcpy(currentUserId, userId);
            printf("\n로그인 성공! 환영합니다, %s님!\n", users[i].userName);
            printf("\n계속하려면 Enter를 누르세요...");
            getchar();
            return 1;
        }
    }

    printf("\n아이디 또는 비밀번호가 일치하지 않습니다.\n");
    return 0;
}

void deleteAccount() {
    system("cls");
    printf("\n========================================\n");
    printf("           회원 탈퇴\n");
    printf("========================================\n");

    int userIdx = findUserIndex(currentUserId);

    if (users[userIdx].borrowCount > 0) {
        printf("대출 중인 도서가 있어 회원 탈퇴가 불가능합니다.\n");
        printf("모든 도서를 반납한 후 다시 시도해주세요.\n");
        return;
    }

    printf("\n정말로 회원 탈퇴하시겠습니까?\n");
    printf("탈퇴 후 모든 정보가 삭제됩니다.\n");
    printf("계속하시려면 비밀번호를 입력하세요: ");

    char password[MAX_PW];
    scanf("%s", password);
    clearInputBuffer();

    if (strcmp(users[userIdx].password, password) != 0) {
        printf("\n비밀번호가 일치하지 않습니다.\n");
        return;
    }

    printf("\n최종 확인: 정말 탈퇴하시겠습니까? (y/n): ");
    char confirm;
    scanf("%c", &confirm);
    clearInputBuffer();

    if (confirm != 'y' && confirm != 'Y') {
        printf("\n회원 탈퇴가 취소되었습니다.\n");
        return;
    }

    // 사용자 정보 삭제 (배열에서 제거)
    for (int i = userIdx; i < userCount - 1; i++) {
        users[i] = users[i + 1];
    }
    userCount--;

    saveUsers();

    printf("\n회원 탈퇴가 완료되었습니다.\n");
    printf("그동안 이용해주셔서 감사합니다.\n");

    currentUserId[0] = '\0';  // 로그아웃 처리
}

int isUserExists(const char* userId) {
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].userId, userId) == 0) {
            return 1;
        }
    }
    return 0;
}

int findUserIndex(const char* userId) {
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].userId, userId) == 0) {
            return i;
        }
    }
    return -1;
}