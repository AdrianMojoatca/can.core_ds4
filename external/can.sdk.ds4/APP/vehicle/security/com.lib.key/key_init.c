
/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: key_init.c 24495 2014-02-04 19:21:12Z marcio.matsunaga $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "key_private.h"
#include "dst.h"
#include "dst_bus.h"

#include "trace.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static Boolean init_isk        (void) ;
                                 
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

int key_init(void)
{
   TRACE("\r\nKEY INIT\r\n");
   if(key_init_nvfs() && dst_init(&key_memory))
    {
      dst_log_init() ;
      dst_bus_init() ;

      if(dst_log_is_done() && init_isk())
      {
        key_vars.is_prog = TRUE ;
        return key_vars.is_prog ;
      }
    
     return TRUE ;      
	 
    }

  return - 1 ;
}
//--------------------------------------------------------------------------//
static Boolean init_isk (void)
{
  if(dstcrypt_init_nvfs())
    {
      Dst_Log log[DST_LOG_SIZE] ;
      
      dst_log_read(&log[0]) ;

      dstcrypt(&log[0].challenge[0] , (key_memory.config.bit.enc_80? DST_80 : DST_40)) ;

      if(!memcmp(&log[0].challenge[0] , &log[0].response[0] , sizeof_array(log[0].response) ))
        {
          key_write_serial(&log[0].uid[0])   ;    
          key_save         () ;
		  return TRUE ;
        }

      key_memory.config.bit.enc_80 ^= 1 ;
      
      dst_log_read(&log[0]) ;

      dstcrypt(&log[0].challenge[0] , (key_memory.config.bit.enc_80? DST_80 : DST_40)) ;

      if(!memcmp(&log[0].challenge[0] , &log[0].response[0] , sizeof_array(log[0].response) ))
        {
          key_write_serial(&log[0].uid[0])   ;    
          key_save         () ;
          return TRUE ;
        }
      
      dstcrypt_delete_isk() ;
    }

  return FALSE ;
}
