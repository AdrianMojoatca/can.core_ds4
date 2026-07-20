
#ifndef TIMESTAMP_H
#define TIMESTAMP_H

typedef UInt32 time_t; // seconds

struct tm
{
 UInt8 tm_sec;
 UInt8 tm_min;
 UInt8 tm_hour;
 UInt8 tm_mday; // starts at 1
 UInt8 tm_mon;  // starts at 1
 UInt16 tm_year;
};


void timestamp_init(void);
time_t time(time_t *);
time_t mktime(struct tm *tm);

#endif
