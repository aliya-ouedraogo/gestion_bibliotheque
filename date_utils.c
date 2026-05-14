#include <stdio.h>
#include <time.h>
#include <math.h>
#include "date_utils.h"

time_t create_date(int day, int month, int year) {
    struct tm date = {0};
    date.tm_mday = day;
    date.tm_mon = month - 1;
    date.tm_year = year - 1900;
    date.tm_hour = 12;
    return mktime(&date);
}

int days_between(time_t date1, time_t date2) {
    double seconds = difftime(date2, date1);
    return (int)(seconds / (60 * 60 * 24));
}

time_t add_days(time_t date, int days) {
    return date + (days * 24 * 3600);
}

/* Return GMT (UTC) broken-down time for given time_t */
struct tm *to_gmtime(const time_t *t) {
    return gmtime(t);
}

/* Return an asctime-formatted string for a given struct tm */
char *format_asctime(const struct tm *tblock) {
    return asctime(tblock);
}

/* Return a ctime-formatted string for a given time_t */
char *format_ctime(const time_t *t) {
    return ctime(t);
}

/* Wrapper around difftime */
double seconds_diff(time_t time2, time_t time1) {
    return difftime(time2, time1);
}

/* Create a time_t with explicit hour/min/sec using mktime */
time_t create_date_time(int day, int month, int year, int hour, int min, int sec) {
    struct tm date = {0};
    date.tm_mday = day;
    date.tm_mon = month - 1;
    date.tm_year = year - 1900;
    date.tm_hour = hour;
    date.tm_min = min;
    date.tm_sec = sec;
    return mktime(&date);
}

int anniversaire(date a, date n) {
    int tmp;
    tmp = (a.annee) - (n.annee);
    if (((n.mois) * 31 + (n.jour)) > ((a.mois) * 31 + (a.jour)))
        tmp = tmp - 1;
    return tmp;
}

date difference(date a, date n) {
    date diff;
    diff.mois = fabs(a.mois - n.mois);
    diff.jour = fabs(a.jour - n.jour);
    diff.annee = fabs(a.annee - n.annee);
    return diff;
}