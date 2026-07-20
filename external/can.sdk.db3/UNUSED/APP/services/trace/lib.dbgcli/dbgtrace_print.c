/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dbgtrace_print.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "dbgcli_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
UInt8 dbg_tx_max_wait=0;
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

static void _store_char(char ch)
{
//ATOMIC
// (
  if(dbgtx_nw == dbgtx_nr)
    dbg_tx_max_wait = 3;
  
	dbgtx_fifo[dbgtx_nw & (DBGTX_FIFO_SIZE-1)] = ch;
	dbgtx_nw++;
//  );
}

void store_char(char ch)
{
   if(ch == '\n')
	   _store_char('\r');
   
   _store_char(ch);
}

	
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
	
	

static void putchw(int n, char z, char* bf)
	{
	char fc=z? '0' : ' ';
	char ch;
	char* p=bf;
	while (*p++ && n > 0)
		n--;
	while (n-- > 0) 
		{
			store_char(fc);
		}
	while ((ch= *bf++) != 0)
		{
			store_char(ch);
		}
	}

	
	
void tfp_format( const char *fmt, va_list va)
{
		char bf[12];
    
	char ch;

	os_res_request(dbg_str_res,0);

	while ((ch=*(fmt++)) != 0) {
		if (ch!='%') 
		{
			store_char(ch);
		}
		else 
			{
					char lz=0;
					int w=0;
					ch=*(fmt++);
					if (ch=='0') {
						ch=*(fmt++);
						lz=1;
						}
					if (ch>='0' && ch<='9') {
						ch=a2i(ch,(char**)&fmt,10,&w);
						}
					switch (ch) {
						case 0: 
							goto abort;
						case 'u' : {
							ui2a(va_arg(va, unsigned int),10,0,bf);
							putchw(w,lz,bf);
							break;
							}
						case 'd' :  {
							i2a(va_arg(va, int),bf);
							putchw(w,lz,bf);
							break;
							}
						case 'x': case 'X' : 
							ui2a(va_arg(va, unsigned int),16,(ch=='X'),bf);
							putchw(w,lz,bf);
							break;
						case 'c' :
						{
							store_char((char)(va_arg(va, int)));
						}							
							break;
						case 's' : 
							putchw(w,0,va_arg(va, char*));
							break;
						case '%' :
						{
							store_char(ch);
						}
						default:
							break;
				}
			}
		}
	abort:;	
		os_res_release(dbg_str_res);
}
	


void d_store( const char *fmt, ...)
	{
	va_list va;
	va_start(va,fmt);
		{
			tfp_format(fmt,va);
		}
	va_end(va);
	}


Boolean dbg_print( const char *str, ...)
{
	
    Dbg_Q_Msg* dbg_q_msg ; 
	
    va_list arg ; 
    if(dbgclitx_room() < 160 )  //2 lines worth
      return FALSE;
  
    dbg_q_msg = (Dbg_Q_Msg *)q_remove_c( dbg_vars.q_free ) ; 

    if( (!dbg_q_msg | !str) ) 
			return FALSE ;

    dbg_q_msg->str = (void *)str ; 

    va_start( arg , str );
    {		
				#if DBG_ARG_SIZE == 1
					dbg_q_msg->arg[0] = (void *)va_arg( arg , VARG );
				#endif

				#if DBG_ARG_SIZE == 2
					dbg_q_msg->arg[0] = (void *)va_arg( arg , VARG );
					dbg_q_msg->arg[1] = (void *)va_arg( arg , VARG );
				#endif

				#if DBG_ARG_SIZE == 3
					dbg_q_msg->arg[0] = (void *)va_arg( arg , VARG );
					dbg_q_msg->arg[1] = (void *)va_arg( arg , VARG );
					dbg_q_msg->arg[2] = (void *)va_arg( arg , VARG );
				#endif

				#if DBG_ARG_SIZE == 4
					dbg_q_msg->arg[0] = (void *)va_arg( arg , VARG );
					dbg_q_msg->arg[1] = (void *)va_arg( arg , VARG );
					dbg_q_msg->arg[2] = (void *)va_arg( arg , VARG );
					dbg_q_msg->arg[3] = (void *)va_arg( arg , VARG );
				#endif						

		}
    va_end( arg );

    #define ARG(x)  dbg_q_msg->arg[x]

				if( dbg_q_msg->str )
            {
            	#if DBG_ARG_SIZE == 1
								d_store( dbg_q_msg->str , ARG(0)) ;                
							#endif
						
							#if DBG_ARG_SIZE == 2
								d_store( dbg_q_msg->str , ARG(0) , ARG(1) ) ;                
							#endif			
							
							#if DBG_ARG_SIZE == 3
								d_store( dbg_q_msg->str , ARG(0) , ARG(1), ARG(2) ) ;                
							#endif	
							
							#if DBG_ARG_SIZE == 4
								d_store( dbg_q_msg->str , ARG(0) , ARG(1), ARG(2), ARG(3) ) ;                
							#endif	
            }					    
    #undef ARG
						

    q_insert( dbg_vars.q_pending , (Q_Msg *)dbg_q_msg );
    
    return TRUE ;
						
}

