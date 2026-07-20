

#include "device.h"
#include "timestamp.h"

static UInt32 boot_delta;
static UInt32 local_delta;



// read seconds counter from rtc
static time_t rtc_seconds(void)
{
UInt32 c0,c1,c2;
time_t dsec,y,tday;
	
  for(;;)
  {
    c0 = LPC_RTC->CTIME0;  // wday, h, m, s   (all 0 based)
    c1 = LPC_RTC->CTIME1;  // year, mon, mday (month and mday start at 1)
    c2 = LPC_RTC->CTIME2;  // yday            (starts at 1)
    if(LPC_RTC->CTIME0 == c0)
      break;
  }

  dsec = (((c0>>16) & 0x1F) * 60 + ((c0>>8) & 0x3f)) * 60 + ((c0>>0) & 0x3f);
  y = ((c1>>16) & 0xFFF) + 3; // add 3 to shift the leap year as the last of a group of 4 years
  tday = (y>>2)*(4*365+1) + (y & 3)*365 + (c2 & 0x1FF) - 1;

  return tday*86400 + dsec; // seconds.
}


// seconds since last boot (ie power on)
time_t time_boot(void)
{
  return rtc_seconds() + boot_delta;
} 






void timestamp_init(void)
{
time_t now;

  local_delta = 0; // give raw time
  now = rtc_seconds();
  boot_delta = (time_t)(-now);

}




// unix day 0 is Jan 1, 1970
// our internal day 0 is Mar 1, 2000

unsigned short cumday[13]=
{
  0, 31, 61, 92,
  122, 153, 184, 214,
  244, 275, 305, 336,
  (unsigned short)(-1)
};

// convert a struct tm to a time_t
time_t mktime(struct tm *tm)
{
unsigned char m;
unsigned y;

  y = tm->tm_year - 2000;
  m = tm->tm_mon - 3; // 0th month is March, for us 
  if((signed)m < 0) // before march?
  {
    m += 12; // make it positive
    y--; // .. of the previous year
  }

  return tm->tm_sec
    + 60 * ( tm->tm_min
           + 60 * ( tm->tm_hour
                  + 24 * (  tm->tm_mday - 1 // start at 0
                         + cumday[m]
                         + (4*365+1) * (y>>2)
                         + 365 * (y & 3)
                         )
                  )
           )
    ;

}


struct tm *localtime_r(time_t t,struct tm *tm)
{
time_t d,q;
	
  d = t/86400;
  t -= 86400 * d; // seconds within day

  tm->tm_hour = t/3600;
	t -= 3600*tm->tm_hour;
	tm->tm_min = t/60;
	t -= 60*tm->tm_min;
	tm->tm_sec = t;

  q = d/(4*365+1); // number of quadyears
  d -= (4*356+1)*q; // remove quadyear days from days

  tm->tm_year = 2000 + 4*q;
  q = d/365; // number of years
  tm->tm_year += q;
  d -= 365*q;  // yearday


  // start from March
  for(tm->tm_mon = 0; d >= cumday[tm->tm_mon]; tm->tm_mon++)
    ;
  tm->tm_mon += (3-1); // convert to starting from January
  if(tm->tm_mon > 12) // beyond december?
  {
    tm->tm_mon -= 12; // correct it 
    tm->tm_year++;       // and bump year
  }
  return tm;
}


// convert a time_t to a struct tm
struct tm *localtime(time_t t)
{
static struct tm tms;
	return localtime_r(t,&tms);
}




time_t time(time_t *p)
{
time_t t;
	
  t = local_delta + rtc_seconds();
	if(p)
		*p = t;
	return t; // returns localtime as a time_t
}


// set current calendar time RIGHT NOW
void time_set(time_t t)
{
  // this is the time we want NOW to be...
  local_delta = t - rtc_seconds(); // therefore localtime is rtc_seconds() + local_delta
}

// set current calender time
void mktime_set(struct tm *tm)
{
	time_set(mktime(tm));
}

 







