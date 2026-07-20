/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: eips_private.h 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

#ifndef __EIPS_PRIVATE_H__
#define __EIPS_PRIVATE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "eips.h"
#include "timeout_f.h"
#include "nvfs_usr.h"
#include "dbg.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/// Define of Magic Number \\\

#define EIPS_MIN                       60

#define EIPS_SEC                       1000

#define EIPS_ENGINE_START_CHECK_TIMEOUT 10            // 10  secondes
#define EIPS_ENGINE_STOP_CHECK_TIMEOUT  30            // 30  secondes

#define EIPS_SHUTDOWN_DEFAULT_TIMEOUT   1  * EIPS_MIN // 3   minutes
#define EIPS_ALERT_DEFAULT_TIMEOUT      15            // 15  Secondes

#define EIPS_START_DEFAULT_TIMEOUT 2000

#define EIPS_TIMEOUT_RES                200

//--------------------------------------------------------------------------//

/// List of Option \\\

typedef struct s_Eips_Option
{
  Boolean  enabled        ; // enable or disable

  UInt16   shutdown       ; // time before shutdown   (Secondes)
  UInt16   alert_reminder ; // reminder for the alert (Secondes)
  
}Eips_Option ;

//--------------------------------------------------------------------------//

/// List of Vars \\\

typedef struct s_Eips_Vars
{
  Eips_Option        option           ;

  Timeout            timeout_alert          ;  
  Timeout            timeout_shutdown       ;
  Timeout            timeout_shutdown_check ;
  Timeout            timeout_start        ;

  UInt32             check_timeout        ;

  Boolean            door_status          ;
  Boolean            skd_status           ;
  Boolean            rpm_status           ;
  Boolean            speed_status         ;
  Boolean            gwr_status           ;
  Boolean            fail_status          ;

}Eips_Vars ;



/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

Boolean eips_init_nvfs                   (void) ;
void    eips_shutdown_timeout_stop       (void) ;
void    eips_alert_timeout_start         (void) ;
void    eips_alert_timeout_stop          (void) ;
void    eips_shutdown_check_timeout_start(void) ;
void    eips_shutdown_check_timeout_stop (void) ;

void    eips_alert_func                  (void * arg) ;
void    eips_shutdown_func               (void * arg) ;
void    eips_shutdown_check_func         (void * arg) ;
                                         
void    eips_door_set                    (void * arg) ;
void    eips_door_clr                    (void * arg) ;
void    eips_skd_set                     (void * arg) ;
void    eips_skd_clr                     (void * arg) ;
void    eips_rpm_set                     (void * arg) ;
void    eips_rpm_clr                     (void * arg) ;
void    eips_speed_set                   (void * arg) ;
void    eips_speed_clr                   (void * arg) ;
void    eips_gwr_on                      (void * arg) ;
void    eips_gwr_off                     (void * arg) ;
void    eips_shutdown_timeout_start      (void * arg) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

extern       Eips_Vars       eips_vars       ;

extern const NVFS_Entry_Name nvfs_eips_option ;
extern const NVFS_Entry_Name nvfs_eips_error  ;

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

#endif

