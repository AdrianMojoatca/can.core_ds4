/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: run_safe_private.h 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

#ifndef __RUN_SAFE_PRIVATE_H__
#define __RUN_SAFE_PRIVATE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "run_safe.h"
#include "timeout_f.h"
#include "dbg.h"
#include "status.h"


#ifndef RUN_SAFE_TIMEOUT
  #define RUN_SAFE_TIMEOUT 45000
#else
    #warning "Run safe new Timeout"
#endif

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/


enum
{
  NOT_ACTIVE ,
  ACTIVE  
} ;

/*==========================================================================*/

/*==========================================================================*/

typedef struct s_Run_Safe_Entry
{
  Run_Safe_Func func ;

  Timeout run_safe_timeout ;
  UInt8   run_safe_state   ;
  UInt8   gwr_state        ;
	UInt8   runsafe_door_status	 ;
   
} Run_Safe_Entry ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

extern Run_Safe_Entry run_safe_entry ;

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/
EXTERN_C_ENTER

void run_safe_timeout_elapsed   (void * arg) ;
void run_safe_gwr_on            (void * arg) ;
void run_safe_gwr_off           (void * arg) ;
void run_safe_shutdown          (void);

EXTERN_C_LEAVE
/*==========================================================================*/

#endif

