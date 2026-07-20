/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: evt_private.h 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

#ifndef __EVT_PRIVATE_H__
#define __EVT_PRIVATE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "evt.h"
#include "status_system.h"
#include "cmd.h"
#include "timeout_f.h"
#include "trace.h"


#if DBG_EVT == 1
	#warning "DBG_EVT is Enable"
    #define EVT_TRACE_ZONE( x )    do { x; }while (0);
    #define EVT_TRACE_INIT(sio)    trace_init(sio)    
	#define EVT_TRACE_RX()		   trace_rx()
    #define EVT_TRACE_RX_T(t)	   trace_rx_t(t)
	#define EVT_TRACE(args...)     trace_print(  args )        
#else
    #define EVT_TRACE_INIT(sio)      
	#define EVT_TRACE_RX()                  (1)
    #define EVT_TRACE_RX_T(t)               (1)
	#define EVT_TRACE_ZONE(x)               ;
	#define EVT_TRACE(args...)      
#endif

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*--------------------------------------------------------------------------*/

#define EVENT_Q_TIMEOUT	100

/*--------------------------------------------------------------------------*/

#ifndef EVT_TIMEOUT_UPDATE_IGN
    #define EVT_TIMEOUT_UPDATE_IGN      1000        //<-- 1000ms
#endif

/*--------------------------------------------------------------------------*/

typedef UInt32  Evt_Guard_R ;

/*--------------------------------------------------------------------------*/

typedef Evt_Guard_R ( *Evt_Guard_F)( void );

/*--------------------------------------------------------------------------*/

typedef Boolean Evt_Set_A ;

/*--------------------------------------------------------------------------*/

typedef void  (*Evt_Set_F)( Evt_Set_A );

/*--------------------------------------------------------------------------*/

struct s_Evt_Entry
{
  SInt16 evt     ;
  SInt16 cmd_msg ;

  Evt_Guard_F  guard_func  ;
  Evt_Guard_R  guard_ret   ;

  Evt_Set_F    set_func ;
  Evt_Set_A    set_arg  ;

};

/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

void        evt_init( void );

Evt_Entry*  evt_search                      ( UInt8 event );

Boolean     evt_guard_driver_door_open      ( void );
Boolean     evt_guard_driver_door_close     ( void );
Boolean     evt_guard_other_door_open       ( void );
Boolean     evt_guard_other_door_close      ( void );

Boolean     evt_guard_hood_open             ( void );
Boolean     evt_guard_hood_close            ( void );
Boolean     evt_guard_trunk_open            ( void );
Boolean     evt_guard_trunk_close           ( void );
Boolean     evt_guard_ign_on                ( void );
Boolean     evt_guard_ign_off               ( void );
Boolean     evt_guard_brake_on              ( void );
Boolean     evt_guard_brake_off             ( void );
Boolean     evt_guard_handbrake_on          ( void );
Boolean     evt_guard_handbrake_off         ( void );
Boolean     evt_guard_glowplug_off_active   ( void );
Boolean     evt_guard_glowplug_off_inactive ( void );
        
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

extern const Evt_Entry evt_table[] ;
extern const UInt8 EVT_TABLE_SIZE ;

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif

