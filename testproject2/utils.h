#ifndef UTILS_H
#define UTILS_H

#define MAX_ID 20
#define MAX_PW 20
#define MAX_NAME 50
#define MAX_PHONE 15
#define MAX_TITLE 100
#define MAX_AUTHOR 50
#define MAX_PUBLISHER 50
#define OVERDUE_DAYS 14

void clearInputBuffer();
void getCurrentDate(char* date);
int calculateDaysDiff(const char* date1, const char* date2);

#endif