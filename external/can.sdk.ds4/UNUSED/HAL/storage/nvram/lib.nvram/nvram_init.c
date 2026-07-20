/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: nvram_init.c 33268 2015-08-19 15:40:18Z suchita.yadav $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#ifdef DEV_LPC_236x
	#include "iap.h"
#endif
#include "nvram_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#ifndef  NVRAM_SERVER_STK
#define  NVRAM_SERVER_STK 0x290
#warning NVRAM Server stack size defaulted to 0x290 !
#endif

//--------------------------------------------------------------------------//

#ifndef  NVRAM_SERVER_PRIORITY
#define  NVRAM_SERVER_PRIORITY OS_PRI_MAX
#warning NVRAM Server priority defaulted to OS maximum !
#endif

//--------------------------------------------------------------------------//

typedef struct s_Nvram_Op_Args_Init // : public Nvram_Op_Args
{
  Nvram_Op_Args _base_ ;            // !!! Must Be First Field !!!

  Boolean do_format ;

} Nvram_Op_Args_Init ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static void nvram_server_executed_init (Nvram_Op_Args_Init * op_args_init) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//
//! \desc   Initializes the NVRAM library for use by applications.
//!         
//!         If a format is requested, the RAM cache image and both FLASH 
//!         images are initialized to a virgin state of all hex 'FF' values.
//!
//!         If not, then an attempt to read the most recent FLASH image into
//!         the RAM cache is performed.
//!
//!         If only one FLASH image is valid, the invalid one will be overwritten
//!         by the valid one and a different success status will be returned.

//! \assume \li Will be called before any other 'nvram_xxx' function calls.
//!         \li Linker scatter file has correctly located cache, iap_wr, and flash variables.

//! \notes -none-

//! \param do_format \b TRUE  = Explicitly format FLASH (both pages).\n
//!                  \b FALSE = Do not format, only check integrity and load most recent image into cache.

//! \return \li \b NVRAM_OK           : Initiallization was completed correctly.
//!         \li \b NVRAM_OK_RECOVERED : Only one FLASH page was valid, the invalid one was forced to mirror the valid one.
//!         \li \b NVRAM_ERR_WR_FAIL  : Unable to update FLASH.
//!         \li \b NVRAM_ERR_INTERNAL : Unexpected internal error occured.

//! \throws -none-
//--------------------------------------------------------------------------//

Nvram_Status nvram_init (Boolean do_format)
{
  Nvram_Op_Args_Init op_args_init ;

  /*------------------------------------------------------------------------------------------------*\
  | If OS is used, create an actual server task.  If not, the callers thread will act as the server. |
  \*------------------------------------------------------------------------------------------------*/

  #if NVRAM_USE_OS == 1

  if (nvram_vars.tsk_server == 0)
    {
      nvram_vars.tsk_server = os_tsk_create (nvram_tsk_server , NVRAM_SERVER_PRIORITY , NVRAM_SERVER_STK , 0 , "nvram_server") ;
    
      os_tsk_start (nvram_vars.tsk_server , 0) ;
    }

  #endif

  /*--------------------------------------------------------------------------------------*\
  | Setup arguments for call to server.  Operation will execute in servers thread & stack. |
  \*--------------------------------------------------------------------------------------*/

  NVRAM_OP_ARGS_CONTRUCT (op_args_init._base_ , nvram_server_executed_init) ;

  op_args_init.do_format = do_format ;

  nvram_server_call ((Nvram_Op_Args *) & op_args_init) ;

  return op_args_init._base_.status ;
}

//--------------------------------------------------------------------------//

static void nvram_server_executed_init (Nvram_Op_Args_Init * op_args_init)
{
  NvramTry
    {
      /*-------------------------------------------------------------*\
      | Check for errors that can occur due to incorrect scatter file |
      \*-------------------------------------------------------------*/
    
      #define CHECK_0 (iap_sector_map_addr (nvram_image_flash_0) >= 0)
      #define CHECK_1 (iap_sector_map_addr (nvram_image_flash_1) >= 0)
    
      #define CHECK_2 ((UInt32) nvram_image_flash_0 % NVRAM_IMAGE_ALIGN       == 0)
      #define CHECK_3 ((UInt32) nvram_image_flash_1 % NVRAM_IMAGE_ALIGN       == 0)
    
      #define CHECK_4 ((UInt32) nvram_iap_wr_buff   % NVRAM_IAP_WR_BUFF_ALIGN == 0)
    
      if (!CHECK_0 || !CHECK_1 || !CHECK_2 || !CHECK_3 || !CHECK_4)
        {
          NvramThrow NVRAM_ERR_INTERNAL ;
        }
    
      #undef  CHECK_4
      #undef  CHECK_3
    
      #undef  CHECK_2
      #undef  CHECK_1
    
      #undef  CHECK_0

      /*-------------------------------------------------*\
      | Call appropriate function based on format request |
      \*-------------------------------------------------*/

      op_args_init->_base_.status = (op_args_init->do_format ? nvram_init_format : nvram_init_existing) () ;

      nvram_vars.is_initialized    = TRUE  ;
      nvram_vars.is_cache_modified = FALSE ;
    }

  NvramCatch (op_args_init->_base_.status)
    {
    }
}

