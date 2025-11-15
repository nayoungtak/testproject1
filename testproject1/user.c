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

    printf("전화번호: ");
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