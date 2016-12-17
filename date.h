#ifndef CTEMPLATE_DATE_H
#define CTEMPLATE_DATE_H

#include <time.h>

char date_compare(struct tm *time1, struct tm *time2);
void date_print(struct tm *time);

#endif

