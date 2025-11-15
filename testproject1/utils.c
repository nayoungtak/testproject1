// utils : 공통 유틸리티 함수 (날짜, 입력 버퍼)
#define _CRT_SECURE_NO_WARNINGS
#include "utils.h"
#include <stdio.h>
#include <time.h>

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void getCurrentDate(char* date) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(date, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
}

int calculateDaysDiff(const char* date1, const char* date2) {
    struct tm tm1 = { 0 }, tm2 = { 0 };

    sscanf(date1, "%d-%d-%d", &tm1.tm_year, &tm1.tm_mon, &tm1.tm_mday);
    sscanf(date2, "%d-%d-%d", &tm2.tm_year, &tm2.tm_mon, &tm2.tm_mday);

    tm1.tm_year -= 1900;
    tm1.tm_mon -= 1;
    tm2.tm_year -= 1900;
    tm2.tm_mon -= 1;

    time_t time1 = mktime(&tm1);
    time_t time2 = mktime(&tm2);

    return (int)difftime(time2, time1) / (60 * 60 * 24);
}