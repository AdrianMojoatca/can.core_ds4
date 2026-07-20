/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: trace_tx.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dbg.h"
#include "trace_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/
#if USE_TRACE == 1
/*--------------------------------------------------------------------------*/

/***  some new printf - header ******/
#define PRINTF_LONG_SUPPORT
typedef void (*putcf) (void*,char);
static putcf stdout_putf;
static void* stdout_putp;
/***  some new printf - end header ******/

/*  send */
void putc1 ( void* p, char c)
{
	int data;
	data = (int)c;
	sio_tx( trace_vars.sio , data ); 	
}

void trace_tx( int data )
{
	sio_tx( trace_vars.sio , data ); 	
}

/*--------------------------------------------------------------------------*/

/*int fputc (int ch, FILE *f)
{
  trace_tx( ch );

  return 0 ;
}*/

/*--------------------------------------------------------------------------*/

/*void t_printf( const char *fmt , ... )
{
	
	;
}*/

/***  some new printf - implementation ******/
#ifdef PRINTF_LONG_SUPPORT

static void uli2a(unsigned long int num, unsigned int base, int uc,char * bf)
	{
	int n=0;
	unsigned int d=1;
	while (num/d >= base)
		d*=base;		 
	while (d!=0) {
		int dgt = num / d;
		num%=d;
		d/=base;
		if (n || dgt>0|| d==0) {
			*bf++ = dgt+(dgt<10 ? '0' : (uc ? 'A' : 'a')-10);
			++n;
			}
		}
	*bf=0;
	}

static void li2a (long num, char * bf)
	{
	if (num<0) {
		num=-num;
		*bf++ = '-';
		}
	uli2a(num,10,0,bf);
	}

#endif

static void ui2a(unsigned int num, unsigned int base, int uc,char * bf)
	{
	int n=0;
	unsigned int d=1;
	while (num/d >= base)
		d*=base;		
	while (d!=0) {
		int dgt = num / d;
		num%= d;
		d/=base;
		if (n || dgt>0 || d==0) {
			*bf++ = dgt+(dgt<10 ? '0' : (uc ? 'A' : 'a')-10);
			++n;
			}
		}
	*bf=0;
	}

static void i2a (int num, char * bf)
	{
	if (num<0) {
		num=-num;
		*bf++ = '-';
		}
	ui2a(num,10,0,bf);
	}

static int a2d(char ch)
	{
	if (ch>='0' && ch<='9') 
		return ch-'0';
	else if (ch>='a' && ch<='f')
		return ch-'a'+10;
	else if (ch>='A' && ch<='F')
		return ch-'A'+10;
	else return -1;
	}

static char a2i(char ch,  char** src,int base,int* nump)
	{
	char* p= *src;
	int num=0;
	int digit;
	while ((digit=a2d(ch))>=0) {
		if (digit>base) break;
		num=num*base+digit;
		ch=*p++;
		}
	*src=p;
	*nump=num;
	return ch;
	}

static void putchw(void* putp,putcf putf,int n, char z, char* bf)
	{
	char fc=z? '0' : ' ';
	char ch;
	char* p=bf;
	while (*p++ && n > 0)
		n--;
	while (n-- > 0) 
		putf(putp,fc);
	while ((ch= *bf++))
		putf(putp,ch);
	}

void tfp_format(void* putp,putcf putf,const char *fmt, va_list va)
	{
	char bf[12];
    
	char ch;


	while ((ch=*(fmt++))) {
		if (ch!='%') 
			putf(putp,ch);
		else {
			char lz=0;
#ifdef 	PRINTF_LONG_SUPPORT
			char lng=0;
#endif
			int w=0;
			ch=*(fmt++);
			if (ch=='0') {
				ch=*(fmt++);
				lz=1;
				}
			if (ch>='0' && ch<='9') {
				ch=a2i(ch,(char**)&fmt,10,&w);
				}
#ifdef 	PRINTF_LONG_SUPPORT
			if (ch=='l') {
				ch=*(fmt++);
				lng=1;
			}
#endif
			switch (ch) {
				case 0: 
					goto abort;
				case 'u' : {
#ifdef 	PRINTF_LONG_SUPPORT
					if (lng)
						uli2a(va_arg(va, unsigned long int),10,0,bf);
					else
#endif
					ui2a(va_arg(va, unsigned int),10,0,bf);
					putchw(putp,putf,w,lz,bf);
					break;
					}
				case 'd' :  {
#ifdef 	PRINTF_LONG_SUPPORT
					if (lng)
						li2a(va_arg(va, unsigned long int),bf);
					else
#endif
					i2a(va_arg(va, int),bf);
					putchw(putp,putf,w,lz,bf);
					break;
					}
				case 'x': case 'X' : 
#ifdef 	PRINTF_LONG_SUPPORT
					if (lng)
						uli2a(va_arg(va, unsigned long int),16,(ch=='X'),bf);
					else
#endif
					ui2a(va_arg(va, unsigned int),16,(ch=='X'),bf);
					putchw(putp,putf,w,lz,bf);
					break;
				case 'c' : 
					putf(putp,(char)(va_arg(va, int)));
					break;
				case 's' : 
					putchw(putp,putf,w,0,va_arg(va, char*));
					break;
				case '%' :
					putf(putp,ch);
				default:
					break;
				}
			}
		}
	abort:;
	}


void init_printf(void* putp,void (*putf) (void*,char))
	{
	stdout_putf=putf;
	stdout_putp=putp;
	}

void tfp_printf( const char *fmt, ...)
	{
	va_list va;
	va_start(va,fmt);
	tfp_format(stdout_putp,stdout_putf,fmt,va);
	va_end(va);
	}

static void putcp(void* p,char c)
	{
	*(*((char**)p))++ = c;
	}



void tfp_sprintf(const char* s,char *fmt, ...)
	{
	va_list va;
	va_start(va,fmt);
	tfp_format(&s,putcp,fmt,va);
	putcp(&s,0);
	va_end(va);
	}
/***  some new printf - end implementation ******/

void dbg_tx_text(StrConst * text)
{
	TRACE(text);
}

void dbg_tx_u(UInt32 data, UInt8 digits)
{
	TRACE("%d",data);
}

void dbg_tx(int data)
{
	TRACE("%d",data);
}

void dbg_tx_u16(UInt16 data)
{
	TRACE("%d",data);
}

void dbg_tx_u32(UInt32 data)
{
	TRACE("%d",data);
}

void dbg_tx_b8(UInt8 data)
{
	TRACE("%d",data);
}

void dbg_tx_h4(UInt8  data)
{
	TRACE("%01X",data);
}

void dbg_tx_h8(UInt8  data)
{
	TRACE("%02X",data);
}

void dbg_tx_h16(UInt16 data)
{
	TRACE("%04X",data);
}

void dbg_tx_h32(UInt32 data)
{
	TRACE("%08X",data);
}

void dbg_tx_crlf( void )
{
	TRACE("\n\r");
}

void dbg_tx_space( void )
{
	TRACE(" ");
}

void dbg_rx( void )
{
	TRACE_RX();
}

void dbg_print_mem(UInt8 * mem_addr)
{

}

void dbg_print_status( void )
{

}

void dbg_tx_release( void )
{

}

void dbg_tx_request( void )
{

}

#endif
