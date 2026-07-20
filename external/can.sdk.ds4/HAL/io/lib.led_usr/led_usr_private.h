/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: led_usr_private.h 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

#ifndef __LED_USR_PRIVATE_H__
#define __LED_USR_PRIVATE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "device.h"
#include "led_usr.h"
#include "timeout.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

typedef struct
{
  Boolean is_init ;
  Boolean gwr     ;
  Boolean ign     ;

  Timeout timeout ; 
  
}Led_Usr_Vars ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

extern Led_Usr_Vars led_usr_vars ;

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

void led_usr_gwr_on             (void * arg) ;  
void led_usr_gwr_off            (void * arg) ;
void led_usr_lock               (void * arg) ;
void led_usr_unlock             (void * arg) ;
void led_usr_trunk              (void * arg) ;
void led_usr_aux1               (void * arg) ;
void led_usr_aux2               (void * arg) ;
void led_usr_aux3               (void * arg) ;
void led_usr_timeout            (void * arg) ;
void led_usr_rs_ign_on          (void * arg) ;
void led_usr_rs_ign_off         (void * arg) ;
void led_usr_rs_start_on        (void * arg) ;
void led_usr_rs_start_off       (void * arg) ;
void led_usr_key2go_led_done    (void * arg) ;

#endif




