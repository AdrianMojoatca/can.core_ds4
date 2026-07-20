/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: nvfs_private.h 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

#ifndef __NVFS_PRIVATE_H__
#define __NVFS_PRIVATE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include <setjmp.h>

#include "device.h"

//--------------------------------------------------------------------------//

#ifndef   NVFS_USE_OS
  #error !!! NVFS_USE_OS Must be explicitly defined by user !!!
#else

  #if NVFS_USE_OS == 1
    #include "os.h"
  #endif

  #if NVFS_US_OS != 0 && NVFS_USE_OS != 1
    #error !!! NVFS_USE_OS must be either 0 or 1 !!!
  #endif

#endif

//--------------------------------------------------------------------------//

#include "nvram.h"
#include "nvfs.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define NVFS_CRC_INIT 0xFF

//--------------------------------------------------------------------------//

#ifndef NVFS_FORMAT_FILL_VAL
#define NVFS_FORMAT_FILL_VAL 0xFF
#endif

//--------------------------------------------------------------------------//

#define NVFS_EXCEPTION_REPORT(id) nvfs_report ((NVFS_Status)(id) , __FILE__ , __LINE__ , TRUE)

//--------------------------------------------------------------------------//

enum // !!! ORDER IS CRITICAL : DO NOT CHANGE !!!
{
  NVFS_FORMAT_TYPE_1_00 = 0
} ;

//--------------------------------------------------------------------------//

typedef struct
{
  UInt16 addr ;
  UInt16 size ;

  UInt8  crc  ;

} NVFS_Entry_Link ;

//--------------------------------------------------------------------------//

typedef struct
{
  struct
    {
      NVFS_Entry_Link name  ;
      NVFS_Entry_Link data  ;

      UInt16          next  ;

      NVFS_Attributes attributes ;

    } link ;

  UInt8 crc ;

} NVFS_Entry ;

//--------------------------------------------------------------------------//

typedef struct
{
  UInt8 format_type   ; // !!! MUST ALWAYS BE FIRST FIELD : NOT PART OF CRC CALCULATION : MUST BE UINT8 !!!

  struct
    {
      UInt16 free ;     // Where new entries start 

      struct
        {
          UInt16 root ; // 0 = NULL
          UInt16 last ; // 0 = NULL
  
        } list  ;

    } mem ;

  UInt8 crc ;

} NVFS_Header ;

//--------------------------------------------------------------------------//

typedef struct
{
  #if NVFS_USE_OS == 1

    Res    res   ;  // OS resource object

  #else

    UInt8 _void_ ;  // Not used, but need to satisfy compiler

  #endif

} NVFS_Atomic ;

//--------------------------------------------------------------------------//

typedef struct
{
  NVFS_Atomic            atomic ;                   // Atomic access control variables

  NVFS_Exception_Handler nvfs_exception_handler ;   // Called when exceptoin thrown or report generated

  Boolean                is_initialized ;           // TRUE when init was done and ok

  struct
    {
      UInt16 beg ;     // NVRAM begining for fs
      UInt16 end ;     // NVRAM end      for fs

    } nvram ;

  struct
    {
      NVFS_Status arg       ;

      int         caught    ;

      jmp_buf   * throw_vec ;

    } exception ;

  #if NVFS_USE_OS == 1

  Tsk tsk_server ;

  #endif

} NVFS_Vars ;

//--------------------------------------------------------------------------//

typedef struct s_Nvfs_Op_Args
{
  NVFS_Status status ; // !!! MUST BE FIRST FIELD FOR INIT !!!

  void (* op_func)(struct s_Nvfs_Op_Args * const) ;

} Nvfs_Op_Args ;

//--------------------------------------------------------------------------//

typedef void (* Nvfs_Op_Func) (Nvfs_Op_Args * const) ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

void    nvfs_throw               (NVFS_Status nvfs_status , const char * const file_name , const UInt32 file_line                    ) ;
void    nvfs_report              (NVFS_Status nvfs_status , const char * const file_name , const UInt32 file_line , Boolean ignore_ok) ; 

UInt8   nvfs_rd_nvram            (UInt16 addr) ;
void    nvfs_wr_nvram            (UInt16 addr , UInt8 data) ;

void    nvfs_rd_header           (NVFS_Header * const nvfs_header) ;
void    nvfs_wr_header           (NVFS_Header * const nvfs_header) ;

void    nvfs_rd_entry            (NVFS_Entry  * const nvfs_entry , UInt16 entry_addr) ;
void    nvfs_wr_entry            (NVFS_Entry  * const nvfs_entry , UInt16 entry_addr) ;

UInt8   nvfs_crc_calc            (void * data , UInt16 size) ;

void    nvfs_crc_calc_header     (NVFS_Header * const nvfs_header) ;
void    nvfs_crc_test_header     (NVFS_Header * const nvfs_header) ;

void    nvfs_crc_test_entry      (NVFS_Entry  * const nvfs_entry) ;
void    nvfs_crc_calc_entry      (NVFS_Entry  * const nvfs_entry) ;

void    nvfs_crc_test_entry_link (NVFS_Entry_Link * const nvfs_entry_link) ;
void    nvfs_crc_calc_entry_link (NVFS_Entry_Link * const nvfs_entry_link) ;

void    nvfs_range_check         (UInt16 nvram_addr) ;

void    nvfs_init_format         (void) ;
void    nvfs_init_existing       (void) ;

Boolean nvfs_find_entry          (NVFS_Entry_Name const entry_name , UInt16 * const entry_addr , UInt16 * const entry_addr_prev) ;

void    nvfs_atomic_init         (void) ;

void    nvfs_server_call         (Nvfs_Op_Args * const op_args , Nvfs_Op_Func const op_func) ;

#if NVFS_USE_OS == 1

void    nvfs_tsk_server          (void * _void_) ;

#endif

void    nvfs_server_iter_next    (NVFS_Iterator * const iter) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

extern NVFS_Vars nvfs_vars ;

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

                        /// Exception Handling \\\

/// NOTE: All exception handling *must* be done in atomic regions - not thread safe \\\

/// NOTE: Body of 'try' block *must* enter/exit - do not use break, goto, continue in body \\\
                               w
/// NOTE: Body of 'catch' block is ok to use flow modifiers (break, goto , continue \\\

/// NOTE: OK to throw from within initial 'try' block, will be caught as expected.

/// NOTE: OK to re-throw from 'catch' block, will be caught by nested ancestor

#define NvfsTry \
{                                                                                                               \
  jmp_buf   throw_vec_curr ;                                                                                    \
  jmp_buf * throw_vec_prev = nvfs_vars.exception.throw_vec ; nvfs_vars.exception.throw_vec = & throw_vec_curr ; \
                                                                                                                \
  if ((nvfs_vars.exception.caught = setjmp (throw_vec_curr)) == 0)

//--------------------------------------------------------------------------//

#define NvfsCatch(_arg_) \
                                                   \
  else _arg_ = nvfs_vars.exception.arg ;           \
                                                   \
  nvfs_vars.exception.throw_vec = throw_vec_prev ; \
}                                                  \
                                                   \
if (nvfs_vars.exception.caught)

//--------------------------------------------------------------------------//

#define NvfsThrow for (;; nvfs_throw (nvfs_vars.exception.arg , __FILE__ , __LINE__)) nvfs_vars.exception.arg = 


//--------------------------------------------------------------------------//

#define NVFS_SERVER_CALL(_args_ , _func_) nvfs_server_call ((Nvfs_Op_Args *) & (_args_) , (Nvfs_Op_Func)(_func_))

/*==========================================================================*/

#endif

