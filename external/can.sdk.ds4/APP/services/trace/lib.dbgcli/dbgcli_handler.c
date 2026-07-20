/* dbgcli.c - small debug command line interpreter */

#include "os.h"
#include "dbgcli_private.h"
#include <string.h>
#include <stdio.h>

//#ifdef PLATFORM_CM800
#include "fw_private.h"
#include "cmd.h"
//#endif

UInt8 dbg_rx_buf[DBGCLI_BUF_SIZE];
UInt8 dbg_tx_buf[DBGCLI_BUF_SIZE];
UInt8 dbg_rx_nr=0; 
UInt8 dbg_rx_nw=0;
UInt8 dbg_tx_ofs=0;
UInt8 dbg_dest = 0; 

void dbgcli_cmd(void);

//dummy for dbg_text
void dbg_tx_release( void )
{

}

void dbg_tx_request( void )
{

}

char blanks(char **parg)
{
char c;

  for(;;)
  {
    c = **parg;
    if(c==0 || c>' ')
      break;
    (*parg)++;
  }
  return c;
}

int atodigit(char c)
{
    if(c>='0' && c<='9')
      return c - '0';
    return -1;
}

int atonib(char c)
{
    if(c>='0' && c<='9')
      c -= '0';
    else
    {
      c |= 0x20;
      if(c>='a' && c<='f')
        c -= 'a'-10;
      else
        return -1;
    }
    return c;
}

int gethex(char **parg,unsigned int *puns)
{
int c;

  if(atonib(blanks(parg)) < 0)
    return -1;

  *puns = 0;
  for(;;)
  {
    c = atonib(**parg);
    if(c<0)
      break;
    *puns = (*puns << 4) | c;
    (*parg)++;
  }
  return 0;
}

// decimal
int getuns(char **parg,unsigned int *puns)
{
int c;

  if(atodigit(blanks(parg)) < 0)
    return -1;

  *puns = 0;
  for(;;)
  {
    c = atodigit(**parg);
    if(c<0)
      break;
    *puns = (*puns * 10) + c;
    (*parg)++;
  }
  return 0;
}

void aalign(unsigned wordsize,unsigned int *puns)
{
  *puns &= ~(wordsize-1);
}

unsigned int mread(unsigned wordsize,unsigned int addr)
{
unsigned int w;

  if(wordsize < 2)
    w = *((unsigned char *)addr);
  else if(wordsize > 2)
    w = *((unsigned int *)addr);
  else
    w = *((unsigned short *)addr);
  return w;
}

void mwrite(unsigned wordsize,unsigned int addr,unsigned int w)
{

  if(wordsize < 2)
    *((unsigned char *)addr) = (unsigned char)w;
  else if(wordsize > 2)
    *((unsigned int *)addr) = w;
  else
    *((unsigned short *)addr) = (unsigned short)w;
}

void ebadarg(void)
{
  printf(" bad arg\n");
}

unsigned int tbuf[4]; // force alignment
char *tptr = (char *)tbuf;
unsigned char tfirst,tlast;
void disp_tbuf(void)
{
unsigned char i;
char c;
    
    i = tfirst;
    printf(" | ");
    for(;;)
    {
        c = tptr[i];
        if((c & 0x7F)<' ' || (c & 0x7F)==0x7F)
            c = '.';
        printf("%c",c);
        if(i==tlast)
            break;
        i++;
    }
    printf("\n");
}


// do memory command for various size words
void do_mem(unsigned mtarget,char *arg)
{
unsigned int a,a2;
unsigned int w;
unsigned wordsize = mtarget & 7;
char *aptr;
char fmt[6];
    
  // force wordsize 1,2,4 only
  if(wordsize < 2)
    wordsize = 1;
  else if(wordsize > 2)
    wordsize = 4;

  if(gethex(&arg,&a))
  {
    ebadarg();
    return;
  }
  aalign(wordsize,&a);

  if(*arg!=':')
    a2 = a;
  else
  {
    arg++;
    if(gethex(&arg,&a2) || a2 < a)
    {
      ebadarg();
      return;
    }
    aalign(wordsize,&a2);
  }

  if(atonib(blanks(&arg)) < 0)
  {// display
    // build format string
    memcpy(fmt," %02X",6); // copy the trailing 0 also
    fmt[3] = '0' + (wordsize<<1); // finish format string

    if(a == a2)
        {// display single value only
      printf(fmt,mread(wordsize,a));
            return;
        }
        
        tfirst = (a & 0xF);
        tlast = 0xf;
        
        // multi value display
    printf("\n%p:",(void *)a);
    for(;;)
    {
      while(dbgclitx_room() < 160)  //2 lines worth
      {
        os_tsk_wait(30);
      }
        
      w = mread(wordsize,a);
      
      // copy data to tbuf
      if(wordsize < 2)
        tptr[a & 0xF] = (unsigned char)w;
      else if(wordsize > 2)
          *((unsigned int *)(tptr + (a & 0xF))) = w;
      else
          *((unsigned short *)(tptr + (a & 0xF))) = (unsigned short)w;
                
      printf(fmt,w);
      if(a==a2)
      {
                tlast = (a + wordsize - 1) & 0xF;
                disp_tbuf();
        break;
      }

      a += wordsize;

      if((a & 0xF)==0)
            {
                disp_tbuf();  //ends with a newline
        printf("%p:",(void *)a);
            }      
    }
    return;
  }
  // set
  aptr = arg; // remember original argument pointer
  for(;;)
  {
    if(gethex(&arg,&w))
    {
      arg = aptr; // restore original argument pointer
      continue;   // start pattern again
    }
    mwrite(wordsize,a,w);
    if(a==a2)
      break;
    a+=wordsize;
  }
}

#if 1   // LPC17xx
#define FAST_GPIO_BASE 0x2009C000
#define PIN_CONNECT_BASE 0x4002C000
#endif

#define PIN_CONNECT_STEP 4
#define PINSEL0_m (PIN_CONNECT_BASE + 0x000)
#define PINMODE0_m (PIN_CONNECT_BASE + 0x040)

#define FAST_GPIO_STEP 0x20
#define FIO0DIR_m (FAST_GPIO_BASE + 0x000)
#define FIO0MASK_m (FAST_GPIO_BASE + 0x010)
#define FIO0PIN_m (FAST_GPIO_BASE + 0x014)
#define FIO0SET_m (FAST_GPIO_BASE + 0x018)
#define FIO0CLR_m (FAST_GPIO_BASE + 0x01C)

// coincidentally by design, even "what" is 1-bit per pin, odd "what" is 2-bits per pin 
#define PIN_DATA 0
#define PIN_MODE 1
#define PIN_DIR  2
#define PIN_FUNC 3

const unsigned pin0_table[4]=
{
    FIO0PIN_m,
    PINMODE0_m,
    FIO0DIR_m,
    PINSEL0_m
};


// get the register address for this port,pin
unsigned pin_addr(unsigned what,unsigned port,unsigned pin)
{
unsigned addr;
    if(what & 1)
      addr = PIN_CONNECT_STEP * ((port<<1) + (pin>>4)); // 2 bits per pin
    else
        addr = FAST_GPIO_STEP * port;
    
    return pin0_table[what] + addr;
}

// write a portpin field
void pin_write(unsigned what,unsigned port,unsigned pin,unsigned x)
{
unsigned addr;
unsigned shift;
    
  addr = pin_addr(what,port,pin);
    if(what & 1) // 2-bit general case
    {
        shift = (pin<<1) & 0x1F;
        *((volatile unsigned *)addr) = 
           ( *((volatile unsigned *)addr)
            & ~(3<<shift)
           )  | ((x & 3)<<shift);
        return;
    }

    shift = pin;
    if(what == PIN_DATA)  // special case
    {
        addr += ((x & 1) ? FIO0SET_m : FIO0CLR_m) - FIO0PIN_m; // use specific set or clr register
        *((volatile unsigned *)addr) = (1<<shift);
        return;
    }
    
    // single bit general case
    
    *((volatile unsigned *)addr) = 
         ( *((volatile unsigned *)addr)
                & ~(1<<shift)
         )  | ((x & 1)<<shift);
}

// read a portpin field
unsigned pin_read(unsigned what,unsigned port,unsigned pin)
{
unsigned addr;
unsigned x;
    
  addr = pin_addr(what,port,pin);
  x = *((volatile unsigned *)addr); // read the full register
    if(what & 1)
        return (x >> ((pin<<1) & 0x1F)) & 3; // double bit
    
    return (x >> pin) & 1; // single bit
}


void do_pin(unsigned what,char *arg)
{
unsigned p,a,a2;
unsigned i,x,bits;
char fmt[6];
    
  if(getuns(&arg,&p) || p>4)
  {
    ebadarg();
    return;
  }

  if(*arg!='.')
  {// all pins
    a = 0;
    a2 = 31;
  }
  else
  {// pin
    arg++;
    if(getuns(&arg,&a) || a > 31)
    {
      ebadarg();
      return;
    }
    if(*arg != ':')
    {
      a2 = a;
    }
    else
    {
      arg++;
      if(getuns(&arg,&a2) || a2 > 31 || a2 < a)
      {
        ebadarg();
        return;
      }
    }
  }
    if(what & 1) // 2-bit wide field?
    {
        if(a2 - a > 15)
            a2 = a + 15;
    }

  if(atonib(blanks(&arg)) < 0)
  {// display
    x = 0;
    bits = 0;
    while((int)a2>=(int)a)
    {
      if(what & 1)
      {
          x <<= 2;
          bits+=2;
      }
      else
      {
          x <<= 1;
          bits++;
      }
      x |= pin_read(what,p,a2);
      if(a2==0)
          break;
      a2--;
    }


    // build format string
    memcpy(fmt," %01X",6); // copy the trailing 0 also
    fmt[3] = '0' + ((bits+3)>>2);  // compute number of hexdigits needed

    printf(fmt,x);

    return;
  }
  // set
  if(gethex(&arg,&x))
  {
    ebadarg();
    return;
  }

  while(a <= a2)
  {
    if(what & 1)
    {
        pin_write(what,p,a,(x & 3)); // 2-bit values
        x>>=2;
    }
    else
    {
        pin_write(what,p,a,(x & 1)); // single bit values
        x>>=1;
    }
    a++;
  }
}


void do_pindata(char *arg)
{
  do_pin(PIN_DATA,arg);
}
void do_pinfunc(char *arg)
{
  do_pin(PIN_FUNC,arg);
}
void do_pinmode(char *arg)
{
  do_pin(PIN_MODE,arg);
}
void do_pindir(char *arg)
{
  do_pin(PIN_DIR,arg);
}


void dbginput(char *linebuf,unsigned len)
{
char key;
unsigned cnt;

  cnt = 0;
  for(;;)
  {
//    wake_leave () ;
    key = dbg_rx () ;  //read char from different sources
//    wake_enter () ;

    if(key < ' ')
    {
      if(key=='\r' || key=='\n')
      {
        linebuf[cnt] = 0;
        printf("\n");
        break;
      }
      if(key=='\b')
      {
        if(cnt)
        {
          cnt--;
          printf("\b \b");
        }
      }
    }
    else if(key < 0x7F)
    {
      if(cnt < len-1)
      {
        if(dbg_dest == 0)
          printf("%c",key);
        
        linebuf[cnt++] = key;
      }
    }
  }

}



void do_memw(char *arg)
{
  do_mem(4,arg);
}
void do_memh(char *arg)
{
  do_mem(2,arg);
}
void do_memb(char *arg)
{
  if(*arg)
    do_mem(1,arg);
  //else
  //  dbg_print_mem(RAM_MEM); // traditional dball 'm' command
}

void do_ps(char *arg)
{
  if(*arg==0)
  {
      dbg_print_status();
  }
}

void do_time(char *arg)
{
  //print_current_time (sio_dbg);   
}

#if 0
///////// nvdir


#include "nvfs_private.h"
#include "nvram_private.h"


void *nv_addr(UInt16 ofs)
{
#ifdef PLATFORM_CM800
  return (void *)(IMAGE_DATA (nvram_image_cache) + ofs );
    #else
    #ifdef DEV_LPC_176x
      return (void *)(LPC_AHBRAM0_BASE + ofs);              /// NVRAM_FLASH_BUFF memory location
    #else
      return (void *)(0x7FD00000 + ofs);                /// NVRAM_FLASH_BUFF memory location
    #endif
#endif
}

void nv_rd (UInt16 offset , void * buf , UInt16 size)
{
  memcpy (buf , nv_addr(offset) , size) ;
}


// don't check crc
void nv_rd_header (NVFS_Header * const hdr)
{
  nv_rd (nvfs_vars.nvram.beg , hdr , sizeof(NVFS_Header));
}



// don't check range or crc
void nv_rd_entry (NVFS_Entry * const nvfs_entry , UInt16 ofs)
{
  nv_rd (ofs , nvfs_entry , sizeof(NVFS_Entry));
}

#if 0
  nvfs_range_check (entry_addr                         ) ;
  nvfs_range_check (entry_addr + sizeof *nvfs_entry - 1) ;

  nvfs_crc_test_entry (nvfs_entry) ;

  nvfs_crc_test_entry_link (& nvfs_entry->link.name) ; 
  nvfs_crc_test_entry_link (& nvfs_entry->link.data) ; 
#endif


//--------------------------------------------------------------------------//

static UInt16 nv_find_entry_fetch_list_root (void)
{
  NVFS_Header hdr ;
  nv_rd_header (& hdr) ;

  return hdr.mem.list.root ;
}


void do_nvdir(char *arg)
{
NVFS_Entry ent;
UInt16 ofs;
char name[64];

  (void)arg;

  ofs = nv_find_entry_fetch_list_root(); 
  while (ofs)
    {
      nv_rd_entry (& ent , ofs) ;

      nv_rd(ent.link.name.addr, name, ent.link.name.size);
            name[ent.link.name.size] = 0;  //terminate string
      printf("@%08X attr=%04X dsize=%04X @%08X %s\r\n",nv_addr(ofs),
                                                ent.link.attributes,
                                                ent.link.data.size,
                                                nv_addr(ent.link.data.addr),
                                                name);

      ofs      = ent.link.next ;
    }
}


#endif


//#ifdef PLATFORM_CM800

void do_cmd(char *arg)
{
    UInt8 cmd ;
    cmd = *(arg + 1);
    
  switch( cmd )
    {
//        case '1' :/* nothing to do in analogical firmware */break;            
        case 'k' : cmd_q_rs_insert (INPUT_KEY2GO_LOG_DONE       , COMMAND_INSERT_TIMEOUT); break ; 
      
        case 'l' : cmd_q_rs_insert (INPUT_OEM_LOCK              , COMMAND_INSERT_TIMEOUT); break ; 
		    case 'i' : cmd_q_rs_insert (INPUT_OEM_ARM               , COMMAND_INSERT_TIMEOUT); break ; 
        case 'u' : cmd_q_rs_insert (INPUT_OEM_UNLOCK            , COMMAND_INSERT_TIMEOUT); break ; 
        case 't' : cmd_q_rs_insert (INPUT_OEM_TRUNK             , COMMAND_INSERT_TIMEOUT); break ; 
        case '1' : cmd_q_rs_insert (INPUT_RF_AUX1              , COMMAND_INSERT_TIMEOUT); break ; 
        case '2' : cmd_q_rs_insert (INPUT_RF_AUX2              , COMMAND_INSERT_TIMEOUT); break ; 
        case '3' : cmd_q_rs_insert (INPUT_RF_AUX3              , COMMAND_INSERT_TIMEOUT); break ; 
        case '4' : cmd_q_rs_insert (INPUT_RF_AUX4              , COMMAND_INSERT_TIMEOUT); break ; 
        case '5' : cmd_q_rs_insert (INPUT_RF_AUX5              , COMMAND_INSERT_TIMEOUT); break ; 
        case '6' : cmd_q_rs_insert (INPUT_RF_AUX6              , COMMAND_INSERT_TIMEOUT); break ; 
        case '7' : cmd_q_rs_insert (INPUT_RF_AUX7              , COMMAND_INSERT_TIMEOUT); break ; 
        case '8' : cmd_q_rs_insert (INPUT_RF_AUX8              , COMMAND_INSERT_TIMEOUT); break ; 

        case 's' : cmd_q_rs_insert (INPUT_RF_START_STOP_TOOGLE , COMMAND_INSERT_TIMEOUT); break ; 

        case 'p' : 
          {
            static Boolean state = FALSE ;

            state ^= 1 ;

            cmd_q_rs_insert(state?INPUT_PARKING_ON:INPUT_PARKING_OFF ,COMMAND_INSERT_TIMEOUT);
          }
          break;
        case '?' :
            {
                dbg_print_status();
            } break;

       default  : 
         break ; 
    }    

}

//#endif

///////////////////



typedef void (*DBGF)(char *);

static struct
{
  DBGF func;
  const char *name;
} cmd[]=
{

  {do_help,"help"},
  {do_memb,"m"},
  {do_memh,"mh"},
  {do_memw,"mw"},

//  {do_nvdir, "nvdir"},

  {do_pindata,"p"},
  {do_pindir,"p.dir"},
  {do_pinfunc,"p.func"},
  {do_pinmode,"p.mode"},

  {do_ps,"ps"},
  {do_time,"time"},
  {do_help,"?"},

//#ifdef PLATFORM_CM800
    {do_cmd,"cmd"},
//#endif


  {NULL,NULL}
};

void do_help(char *arg)
{
unsigned i;
    
    for(i=0;cmd[i].name;i++)
    {
        printf(" %s",cmd[i].name);
    }
  printf("\n");
}


char dbgline[DBGCLI_BUF_SIZE];
unsigned dbgcli_ofs=0;
Boolean dbgcli_char_state = 0;
UInt8 dbg_rx_buf_rd=0;

static int dbgcli_char(int c)
{
int status = 0;
    
        if(c < ' ')
        {// control character
            if(c == '\n' || c=='\r')
            {
							  dbgline[dbgcli_ofs++] = '\n'; // we always end with a \n for printing  
                if(dbg_dest==0)
                    printf("\n");
                status = 1;
            }
            else if(c == '\b')
            {
                if(dbgcli_ofs)
                {
                    dbgcli_ofs--;
                    if(dbg_dest==0)
                        printf("\b \b");
                }
            }
        }
        else if(c < 0x7F && dbgcli_ofs < sizeof(dbgline)-3)
        {
            // stuff char into buffer
            dbgline[dbgcli_ofs++] = c;
            if(dbg_dest==0)
                putchar(c);
        }
        dbgline[dbgcli_ofs] = 0;
        return status;
}


/**
* @brief  UInt8 dbgcli_chkabort(void)
* @param
* @return
*/
UInt8 dbgcli_chkabort(void)
{
    return 0;  //TODO
}

/**
* @brief  void dbgcli_rx_pkt(void) 
*         call by intl_port 
* @param  
* @return
*/
void dbgcli_rx_pkt(void)
{
#if 0  
uint8_t c;
    if(dbgcli_char_state == 0)
    {
        while((dbg_rx_buf_rd & DBGCLI_BUF_SIZE) < dbg_rx_ofs)
        {
					  c = dbg_rx_buf[dbg_rx_buf_rd];
            dbgcli_char_state = dbgcli_char(c);
					  dbg_rx_buf_rd++;
            if(dbgcli_char_state)
            {                   
                dbgcli_cmd();   // command is ready
                return;
            }
        }
    }
    else // don't remove chars from the fifo, wait until the command is done
        dbgcli_char_state = 2;
#endif    
}

/**
* @brief  int do_dbgcli_cmd(char *buf)
* @param
* @return
*/
int do_dbgcli_cmd(char *buf)
{
char *ptr;
unsigned i, len;
    len = 0;
    
    ptr = buf;

    if(blanks(&ptr) == 0)
      return 0;

    for(i=0;cmd[i].name;i++)
    {
      len = strlen(cmd[i].name);
      if(memcmp(ptr,cmd[i].name,len)==0 && ptr[len]<=' ')
        break;
    }
    if(cmd[i].name)
    {
      cmd[i].func(ptr+len);
      return 0;
    }
    else
    {
        printf("Eh?");
        return -1;
    }
}

void dbgcli_cmd(void)
{
//static unsigned count;
    if(dbgcli_char_state == 0)
        return; // still editing?

    if(dbgline[1]!=0) // non-blank line?
    {
        do_dbgcli_cmd(dbgline);
        
        if(dbgcli_chkabort()) // stopped because of abort?
        {   
            printf("^stopped");
        }
        printf("\n");
    }       
    printf("\r\nngmm>"); // show prompt

    // reset input machine
    dbgcli_ofs = 0; // start new input on new line
    dbgline[0] = 0; // clear previous data
    dbgcli_char_state = 0;
    
    if(dbgcli_chkabort())
      dbgcli_rx_pkt(); // won't cause action until after I return

}


#define CMDBUF_SIZE 128
static char cmdbuf[CMDBUF_SIZE];
// never returns
void dbgcli_handler(void)
{
char *ptr;
unsigned i,len;
    len = 0;
  
  printf("\n");
  for(;;)
  {
    printf("dbg>");
    dbginput(cmdbuf,CMDBUF_SIZE);
    ptr = cmdbuf;

    if(blanks(&ptr)<' ')
      continue;

    for(i=0;cmd[i].name;i++)
    {
      len = strlen(cmd[i].name);
      if(memcmp(ptr,cmd[i].name,len)==0 && ptr[len]<=' ')
        break;
    }
    if(cmd[i].name)
    {
      cmd[i].func(ptr+len);
      printf("\n");
    }
    else if(*ptr > ' ')
      printf("Eh?\n");

  }
}

