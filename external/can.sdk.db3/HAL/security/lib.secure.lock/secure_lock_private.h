/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: secure_lock_private.h 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

#ifndef _SECURE_LOCK_PRIVATE_H_
#define _SECURE_LOCK_PRIVATE_H_

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "secure_lock.h"
#include "timeout_f.h"
#include "dbg.h"

#if DBG_SECURE_LOCK == 1
  #warning "DBG_SECURE_LOCK Enable"
#endif

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define SECURE_REARM_RUN_TIMEOUT	 250
#define SECURE_REARM_TIMEOUT_NORMAL	3000
#define SECURE_REARM_TIMEOUT_SMART	8000
#define SECURE_LOCK_STARTER_TIMEOUT 5000

typedef struct s_Secure_Lock_Vars
{
  Boolean gwr               ;
  Boolean need_arm          ;
  Boolean door_open         ;
  Timeout timeout           ;
  Timeout starter_timeout   ;
  
} Secure_Lock_Vars;


/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/


extern Secure_Lock_Vars secure_lock ;

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void secure_lock_lock				(void * arg); 
void secure_lock_gwr_on            	(void * arg);
void secure_lock_gwr_off           	(void * arg);
void secure_lock_starter_on         (void * arg);
void secure_lock_starter_off        (void * arg);
void secure_lock_rearm				(void * arg);
void secure_lock_starter_timeout    (void * arg);



#endif


