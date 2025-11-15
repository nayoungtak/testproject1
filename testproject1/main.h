#pragma once
#define MAX_USERS 100
#define MAX_BOOKS 200
#define MAX_BORROWS 500
#define MAX_ID 20
#define MAX_PW 20
#define MAX_NAME 50
#define MAX_PHONE 15
#define MAX_TITLE 100
#define MAX_AUTHOR 50
#define MAX_PUBLISHER 50
#define OVERDUE_DAYS 14

// 구조체 정의
typedef struct {
    char userId[MAX_ID];
    char password[MAX_PW];
    char userName[MAX_NAME];
    char phone[MAX_PHONE];
    int borrowCount;
    int overdueStatus;  // 0: 정상, 1: 연체중
} User;

typedef struct {
    char bookId[MAX_ID];
    char title[MAX_TITLE];
    char author[MAX_AUTHOR];
    char editor[MAX_AUTHOR];
    char publisher[MAX_PUBLISHER];
    int borrowStatus;  // 0: 대출가능, 1: 대출중
} Book;

typedef struct {
    char bookId[MAX_ID];
    char userId[MAX_ID];
    char borrowDate[11];  // YYYY-MM-DD
    int overdueStatus;  // 0: 정상, 1: 연체
    int extensionStatus;  // 0: 연장안함, 1: 연장함
} Borrow;

// 전역 변수
User users[MAX_USERS];
Book books[MAX_BOOKS];
Borrow borrows[MAX_BORROWS];
int userCount = 0;
int bookCount = 0;
int borrowCount = 0;
char currentUserId[MAX_ID] = "";

// 함수 선언
void loadUsers();
void loadBooks();
void loadBorrows();
void saveUsers();
void saveBorrows();
void signUp();
int login();
void mainMenu();
void borrowBook();
void returnBook();
void extendBorrow();
void showBorrowStatus();
void clearInputBuffer();
int isUserExists(const char* userId);
int findBookIndex(const char* bookId);
int findUserIndex(const char* userId);
int calculateDaysDiff(const char* date1, const char* date2);
void getCurrentDate(char* date);
void updateOverdueStatus();