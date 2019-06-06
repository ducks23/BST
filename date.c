#include <stdio.h>
#include "date.h"
#include "string.h"
#include <stdlib.h>



typedef struct _date{
	char *date_s;
	int month;
	int year;
	int day;
}_Date;


/*
 * duplicate() creates a duplicate of `d'
 * returns pointer to new Date structure if successful,
 *         NULL if not (memory allocation failure)
 */
static const Date *duplicateDate(const Date *d){
	const Date *newDate = (Date *)malloc(sizeof(Date));
	newDate = d;	
	if(newDate != NULL)
	    return newDate;
	else
	    return NULL;
}

/*
 * compare() compares two dates, returning <0, 0, >0 if
 * date1<date2, date1==date2, date1>date2, respectively
 */
static int compareDate(const Date *date1, const Date *date2){
	_Date *d1 = (_Date *)date1->self;
	_Date *d2 = (_Date *)date2->self;
	
	if(d1->year < d2->year){
		return -1;
	}
	else if(d1->year > d2->year){
		return 1;
	}
	if(d1->year == d2->year)
	{
		if(d1->month > d2->month)
		    return 1;
		else if(d1->month < d2->month)
		    return -1;
		else if(d1->day > d2->day)
		    return 1;
		else if(d1->day < d2->day)
		    return -1;
		else
		    return 0;
	}
	return 0;
}

/*
 * destroy() returins any storage associated with `d' to the system
 */
static void destroyDate(const Date *d){
	_Date *date = (_Date *)d->self;
	free(date);
	free((void *)d);
}

static Date template = {
	NULL, duplicateDate, compareDate, destroyDate

};

/*
 * Date_create() creates a Date structure from `datestr`
 * `datestr' is expected to be of the form "dd/mm/yyyy"
 * returns pointer to Date structure if successful,
 *         NULL if not (syntax error)
 */
const Date *Date_create(char *datestr){
	Date *d = (Date *)malloc(sizeof(Date));
	_Date *date = (_Date *)malloc(sizeof(_Date));
	int dd, mm, yyyy;
	char day_str[2], month_str[2], year_str[4];
	if(d != NULL)
	{
		strncpy(month_str, datestr, 2);	
		strncpy(day_str, datestr+3, 2);	
		strncpy(year_str, datestr+6, 4);
		month_str[2] = '\0';
		day_str[2] = '\0';
		year_str[4] = '\0';
		dd = atoi(day_str);
		mm = atoi(month_str);
		yyyy = atoi(year_str);
		date->day = dd;
		date->month = mm;
		date->year = yyyy;
//		printf("day %s, month %s, year %s", day_str, month_str, year_str);	
		
		*d = template;
		d->self = date;
		return d;
	}
	else
	{
		free(d);
		return NULL;
	}
}
/*
int main(){
	printf("hello world");

	Date *a, *b;
	a = Date_create("03/06/1995");
	b = Date_create("03/06/1995");

	int x;
	x = compareDate(a,b);
	printf("date: %d\n ", x);
	
	a = Date_create("03/06/1994");
	b = Date_create("03/06/1995");

	x = compareDate(a,b);
	printf("date: %d\n ", x);
	
 	
}
*/
