

#include "dbgcli_private.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>



void strout(char *str)
{
	char c;
	while((c=*str++)!=0)
	{
		store_char(c);
	}
}



//static BYTE oradix = 10;

void digitout(UInt8 digit)
{
  digit+='0';
  if(digit>'9')
    digit += 'A'-('0'+10);
  store_char(digit);
}

void dig2out(UInt8 x)
{
UInt8 n;
  n = x/10;
  digitout(n);
  digitout(x - n*10);
}

void byteout(UInt8 b)
{
  digitout(b>>4);
  digitout(b & 15);
}



void halfout(UInt16 h)
{
  byteout(h>>8);
  byteout(h);
}


unsigned char oradix = 10;
static char numbuf[10];
void longout(long int n)
{
long int m;
UInt8 d=0;
	
	if(n<0L && oradix==10)
  {
    store_char('-');
		n=-n;
  }

  while(((UInt32)n)>=oradix)
  {
    m = ((UInt32)n)/oradix;
		numbuf[d++] = ((UInt32)n) - oradix*((UInt32)m);
		n = m;
  }
  digitout(n);
  if(n==0)
    return;
  while(d)
    digitout(numbuf[--d]);  

}


void intout(int n)
{
  oradix = 10;
  longout((long)n);
}



int dbgprintf(const char *fmt,...)
{
va_list vl;
unsigned long x;
char c;
char *ptr;
unsigned char flags,width,lim,cy,i,j;
Dbg_Q_Msg* dbg_q_msg ; 
#define FLAG_ZERO (1<<0)	
	
	
	
	dbg_q_msg = (Dbg_Q_Msg *)q_remove_c( dbg_vars.q_free ) ; 

	if( (!dbg_q_msg | !fmt) ) 
		return FALSE ;

	va_start(vl,fmt);

	os_res_request(dbg_str_res, 0);

	for(;;)
	{
		c = *fmt++;
		if(c==0)
			break;
		if(c!='%')
		{
			store_char(c);
			continue;
		}
		// '%' seen
		flags = 0;
		width = 0;
		c = *fmt++;

		if(c == '0')
		{
			flags |= FLAG_ZERO;
			c = *fmt++;
		}
		while(c>='0' && c<='9')
		{
			width = 10*width + c-'0';
			c = *fmt++;
		}
		x = va_arg(vl,unsigned long);
		switch(c)
		{
		case 'c':
				store_char(x);
				break;
		case 's':
			  ptr = (char *)x;
		    while(*ptr)
					store_char(*ptr++);
				break;
		case '%':
				store_char('%');
				break;
		case 'p':
			  width=8;
			  flags |= FLAG_ZERO;
		// fall through
		default:
			  if(c=='d')
				{
					lim = 10;
					if((signed)x < 0)
					{
						store_char('-');
						x = -x;
					}
				}
				else if(c=='u')
					lim = 10;
				else
					lim = 16;
				
				memset(numbuf,0,sizeof(numbuf));
				for(i=0;i<32;i++)
				{
					// double the number
					cy=0;
					for(j=sizeof(numbuf);j--;)
					{
						cy += (numbuf[j] << 1);
						if(cy >= lim)
						{
							cy -= lim;
							numbuf[j] = cy;
							cy = 1;
						}
						else
						{
							numbuf[j] = cy;
							cy = 0;
						}
					}
					if((signed)x < 0)
						numbuf[sizeof(numbuf)-1]++;

					x<<=1;					
				}
				// find leading non-zero digit
				for(j=0;j<sizeof(numbuf);j++)
				{
					if(numbuf[j])
						break;
				}
				if(j>=sizeof(numbuf))
					j = sizeof(numbuf)-1;
				
				if(width < sizeof(numbuf)-j)
					width = sizeof(numbuf)-j;
				for(j=sizeof(numbuf)-width;j<sizeof(numbuf);j++)
					digitout(numbuf[j]);
				break;			
		}
		
	}
	os_res_release(dbg_str_res);

	va_end(vl);

  q_insert( dbg_vars.q_pending , (Q_Msg *)dbg_q_msg );
	
	return 0;
}

