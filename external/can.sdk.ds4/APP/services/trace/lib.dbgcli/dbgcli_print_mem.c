/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dbgcli_print_mem.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "dbgcli_private.h"

#include <stdio.h>
#include <ctype.h>
#include <setjmp.h>



/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static void    print_line      (UInt8 ** addr , jmp_buf throw_vec) ;
static void    print_line_hex  (UInt8 *  addr , UInt8 col_skip , jmp_buf throw_vec) ;
static void    print_line_char (UInt8 *  addr , UInt8 col_skip , jmp_buf throw_vec) ;
static Boolean print_more      (void) ;
static UInt8   rd_mem          (UInt8 * addr , jmp_buf throw_vec) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

static const UInt8 MAX_LINES = 16 ; // Lines to print
static const UInt8 LINE_SIZE = 16 ; // Bytes per line


/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void dbg_print_mem (UInt8 * addr)
{
  jmp_buf throw_vec ;

  if (setjmp (throw_vec) == 0)
    {
      for (;;)
        {
          UInt8 line ;

          for (line = 0 ; line < MAX_LINES ; line++)
            {
              print_line (&addr,throw_vec) ;
            }
    
          if (!print_more ())
            {
              break ;
            }
        }
    }
  else
    {
      printf (" <DATA-ABORT>") ;
    }

  printf ("\r\n") ;
}

//--------------------------------------------------------------------------//

static void print_line (UInt8 ** addr , jmp_buf throw_vec)
{
  UInt8 col_skip = (UInt32) *addr % LINE_SIZE ;

  printf ("%08X: ",(UInt32) *addr) ;

  print_line_hex  (*addr,col_skip,throw_vec) ; printf (" ")    ;

  print_line_char (*addr,col_skip,throw_vec) ; printf ("\r\n") ;

  *addr += LINE_SIZE - col_skip ;
}

//--------------------------------------------------------------------------//

static void print_line_hex (UInt8 * addr , UInt8 col_skip , jmp_buf throw_vec)
{
  UInt8 hex_count = LINE_SIZE - col_skip ;

  for (; col_skip ; col_skip--)
    {
      printf ("-- ") ;
    }

  for (; hex_count ; hex_count--)
    {
      printf ("%02X",(UInt32) rd_mem(addr++,throw_vec)) ;

      if (hex_count != 1)
        {
          printf (" ") ;
        }
    }
}

//--------------------------------------------------------------------------//

static void print_line_char (UInt8 * addr , UInt8 col_skip , jmp_buf throw_vec)
{
  UInt8 chr_count = LINE_SIZE - col_skip ;

  for (; col_skip ; col_skip--)
    {
      printf (" ") ;
    }

  for (; chr_count ; chr_count-- , addr++)
    {
      UInt8 data = rd_mem (addr,throw_vec) ;

      printf ("%c",isprint (data) ? data : '.') ;
    }
}

//--------------------------------------------------------------------------//

static Boolean print_more (void)
{
  printf (":") ;

  for (;;)
    {
      Boolean status ;
  
      switch (dbg_rx())
        {
          case ' '  :
          case 0x0D : status = TRUE  ; break ;
  
          case 'q'  :
          case 'Q'  :
          case 'x'  :
          case 'X'  :
          case 0x1B : status = FALSE ; break ;
  
          default   : continue ;
        }
  
      printf ("\r\n") ;
  
      return status ;
    }
}

//--------------------------------------------------------------------------//

static UInt8 rd_mem (UInt8 * addr,jmp_buf throw_vec)
{
  UInt8 data = *addr ;
  extern volatile Boolean data_abort_flag;

  if (data_abort_flag)
    {
      data_abort_flag = FALSE ;

      longjmp (throw_vec,__LINE__) ;
    }

  return data ;
}
