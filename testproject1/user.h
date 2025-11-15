#ifndef USER_H
#define USER_H

#include "utils.h"

#define MAX_USERS 100

typedef struct {
    char userId[MAX_ID];
    char password[MAX_PW];
    char userName[MAX_NAME];
    char phone[MAX_PHONE];
    int borrowCount;
    int overdueStatus;
} User;

extern User users[MAX_USERS];
extern int userCount;
extern char currentUserId[MAX_ID];

void loadUsers();
void saveUsers();
void signUp();
int login();
int isUserExists(const char* userId);
int findUserIndex(const char* userId);

#endif