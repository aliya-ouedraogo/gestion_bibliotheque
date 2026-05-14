#ifndef DATE_UTILS_H
#define DATE_UTILS_H

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct date
{
    int jour;
    int mois;
    int annee;
} date;

time_t create_date(int day, int month, int year);
int days_between(time_t date1, time_t date2);
time_t add_days(time_t date, int days);
int anniversaire(date a, date n);
date difference(date a, date n);

/* Wrappers and helpers for time conversions */
struct tm *to_gmtime(const time_t *t);
char *format_asctime(const struct tm *tblock);
char *format_ctime(const time_t *t);
double seconds_diff(time_t time2, time_t time1);
time_t create_date_time(int day, int month, int year, int hour, int min, int sec);

#endif
