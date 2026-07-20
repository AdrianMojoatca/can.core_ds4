/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: nvram_private.h 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

#ifndef __NVRAM_PRIVATE_H__
#define __NVRAM_PRIVATE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include <setjmp.h>

#include "crc.h"
#include "nvram.h"

//--------------------------------------------------------------------------//

#ifndef   NVRAM_USE_OS
  #error !!! NVRAM_USE_OS Must be explicitly defined by user !!!
#else

  #if NVRAM_USE_OS == 1
    #include "os.h"
  #endif

  #if NVRAM_US_OS != 0 && NVRAM_USE_OS != 1
    #error !!! NVRAM_USE_OS must be either 0 or 1 !!!
  #endif

#endif

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/// Size & Alignment Requirements for FLASH storage area \\\

#define NVRAM_IMAGE_SIZE        4096U
#define NVRAM_IMAGE_ALIGN       4096U

/// Size & Alignment Requirements for IAP Write buffer storage area \\\

#define NVRAM_IAP_WR_BUFF_SIZE   256U
#define NVRAM_IAP_WR_BUFF_ALIGN  256U

/*------------------------------------------------------------------------*\
| This implementation expects specific constants.  Ensure they are correct |
\*------------------------------------------------------------------------*/

#if NVRAM_IMAGE_SIZE != 4096
#error NVRAM !!! Image size *must* be 4096 !!!
#endif

#if NVRAM_IAP_WR_BUFF_SIZE != 256
#error !!! NVRAM IAP write buffer size *must* be 256 !!!
#endif

//--------------------------------------------------------------------------//

typedef enum // !!! Order & Value Critical !!!
{
  IMAGE_FLASH_0 = 0 ,
  IMAGE_FLASH_1 = 1 ,

} Nvram_Image_Flash_ID ;

//--------------------------------------------------------------------------//

typedef struct
{
  UInt32 data_age ;
  UInt16 data_crc ;

} Nvram_Image_Info ;

//--------------------------------------------------------------------------//

typedef struct
{
  Nvram_Image_Info image_info     ;
  UInt16           image_info_crc ;

} Nvram_Header ;

//--------------------------------------------------------------------------//

typedef struct
{
  unsigned             is_initialized     : 1 ;
  unsigned             is_cache_modified  : 1 ;

  Nvram_Image_Flash_ID image_flash_recent : 1 ;

  struct
    {
      Nvram_Status   arg ;

      int            line   ;
      const char   * file   ;
    
      int            caught ;
      jmp_buf      * throw_vec_curr ;

    } exception ;

  #if NVRAM_USE_OS == 1

  Tsk tsk_server ;

  #endif

} Nvram_Vars ;

//--------------------------------------------------------------------------//

typedef struct s_Nvram_Nvram_Op_Args
{
  Nvram_Status status ;

  void (* nvram_op_func)(struct s_Nvram_Nvram_Op_Args * const) ;

  #define NVRAM_OP_ARGS_CONTRUCT(_op_args_ , _func_) (_op_args_).nvram_op_func = (Nvram_Op_Func) (_func_)

} Nvram_Op_Args ;

//--------------------------------------------------------------------------//

typedef void (* Nvram_Op_Func) (Nvram_Op_Args * const) ;

//--------------------------------------------------------------------------//

#define IMAGE_DATA(_nvram_image_)   ((UInt8        *)((_nvram_image_) + sizeof (Nvram_Header)))
#define IMAGE_HEADER(_nvram_image_) ((Nvram_Header *)( _nvram_image_)                         )

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

Nvram_Status nvram_init_format      (void) ;
Nvram_Status nvram_init_existing    (void) ;

void         nvram_image_crc_set    (UInt8       * const nvram_image) ;
Boolean      nvram_image_crc_verify (UInt8 const * const nvram_image) ;

void         nvram_image_cache_rd   (UInt8       * const nvram_image_flash) ;
void         nvram_image_cache_wr   (UInt8       * const nvram_image_flash) ;

UInt16       nvram_calc_crc         (void const  * mem , UInt16 size) ;

void         nvram_check_range      (UInt16 nvram_addr , UInt16 size) ;

void         nvram_cache_age        (void) ;
void         nvram_cache_save       (Boolean do_mirror) ;

void         nvram_server_call      (Nvram_Op_Args * const op_args) ;

#if NVRAM_USE_OS == 1

void         nvram_tsk_server       (void * _void_) ;

#endif

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

extern Nvram_Vars nvram_vars ;

extern UInt8      nvram_image_cache   [NVRAM_IMAGE_SIZE] ;     
                                                       
extern UInt8      nvram_image_flash_0 [NVRAM_IMAGE_SIZE] ;      
extern UInt8      nvram_image_flash_1 [NVRAM_IMAGE_SIZE] ;      
                                                       
extern UInt8      nvram_iap_wr_buff   [NVRAM_IAP_WR_BUFF_SIZE] ;

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

                        /// Exception Handling \\\

/// NOTE: All exception handling *must* be done in atomic regions - not thread safe \\\

/// NOTE: Body of 'try' block *must* enter/exit - do not use break, goto, continue in body \\\

/// NOTE: Body of 'catch' block is ok to use flow modifiers (break, goto , continue \\\

/// NOTE: OK to throw from within initial 'try' block, will be caught as expected.

/// NOTE: OK to re-throw from 'catch' block, will be caught by nested ancestor

#define NvramTry \
{                                                                                                                      \
  jmp_buf   throw_vec ;                                                                                                \
  jmp_buf * throw_vec_prev = nvram_vars.exception.throw_vec_curr ; nvram_vars.exception.throw_vec_curr = & throw_vec ; \
                                                                                                                       \
  if ((nvram_vars.exception.caught = setjmp (throw_vec)) == 0)

//--------------------------------------------------------------------------//

#define NvramCatch(_arg_) \
                                                         \
  else _arg_ = nvram_vars.exception.arg ;                \
                                                         \
  nvram_vars.exception.throw_vec_curr = throw_vec_prev ; \
}                                                        \
                                                         \
  if (nvram_vars.exception.caught)

//--------------------------------------------------------------------------//

#define NvramThrow for (nvram_vars.exception.line = __LINE__ , nvram_vars.exception.file = __FILE__ ;; longjmp (*nvram_vars.exception.throw_vec_curr , 1)) nvram_vars.exception.arg = 

/*==========================================================================*/

#endif

