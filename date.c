#include <langinfo.h>
#include <stdio.h>
#include "date.h"

char date_compare(struct tm *time1, struct tm *time2) {

	if ( time1->tm_year < time2->tm_year ) {
		return -1;
	}
	if ( time1->tm_year > time2->tm_year ) {
		return 1;
	}

	if ( time1->tm_mon < time2->tm_mon ) {
		return -2;
	}
	if ( time1->tm_mon > time2->tm_mon ) {
		return 2;
	}

	if ( time1->tm_mday < time2->tm_mday ) {
		return -3;
	}
	if ( time1->tm_mday > time2->tm_mday ) {
		return 3;
	}

	if ( time1->tm_hour < time2->tm_hour ) {
		return -4;
	}
	if ( time1->tm_hour > time2->tm_hour ) {
		return 4;
	}

	if ( time1->tm_min < time2->tm_min ) {
		return -5;
	}
	if ( time1->tm_min > time2->tm_min ) {
		return 5;
	}

	if ( time1->tm_sec < time2->tm_sec ) {
		return -6;
	}
	if ( time1->tm_sec > time2->tm_sec ) {
		return 6;
	}

	return 0;
}

void date_print(struct tm *time) {
	char datestring[256];
	strftime(datestring, sizeof(datestring), nl_langinfo(D_T_FMT), time);

	printf("%s\n", datestring);

}