/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: wdog_v_kick_idle.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
 
#include "wdog_v_private.h"

#ifdef  WDOG_V_IDLE_MODE_ACTIVE

#include "vrtc.h"
#include "wdog.h"
#include "wake.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#ifndef WDOG_V_KICK_PERIOD_IDLE
#define WDOG_V_KICK_PERIOD_IDLE     VRTC_1SEC*30
#endif

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void wdog_v_wake_service( Wake_Status state );
static void wdog_v_idle_service( void );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
static Vrtc    wdog_v_vrtc    ;
static Boolean wdog_v_sleeping;
/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*--------------------------------------------------------------------------*/

void wdog_v_kick_idle_init( void )
{
    wake_register( (Wake_Func)wdog_v_wake_service );             
    
    wdog_v_sleeping = FALSE; 
    	
	wdog_v_vrtc = vrtc_create((Vrtc_Func)wdog_v_idle_service );	        
}

/*--------------------------------------------------------------------------*/

void wdog_v_wake_service( Wake_Status state )
{    
    if( state == s_SLEEP )
    {
        vrtc_stop  ( wdog_v_vrtc ); 
        
        wdog_kick();

        wdog_v_sleeping = TRUE;
               
        vrtc_update( wdog_v_vrtc  , WDOG_V_KICK_PERIOD_IDLE );	    
        vrtc_start ( wdog_v_vrtc );		
    }else{
        wdog_v_sleeping = FALSE;    
    }      
}

/*--------------------------------------------------------------------------*/

void wdog_v_idle_service( void )
{        
    if( wdog_v_sleeping )
    {
        wdog_kick();
    
        vrtc_update( wdog_v_vrtc  , WDOG_V_KICK_PERIOD_IDLE );	    
        vrtc_start ( wdog_v_vrtc );		
    }        					        
}

/*--------------------------------------------------------------------------*/

#endif
