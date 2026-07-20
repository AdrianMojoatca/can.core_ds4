/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: evt_guard.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "evt_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/



/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

/*--------------------------------------------------------------------------*/

Boolean evt_guard_other_door_open( void )
{
unsigned src = get_door_source();
    return ((src == SOURCE_DATA || src == SOURCE_AUTO))&& get_other_door_status() ;
}

/*--------------------------------------------------------------------------*/

Boolean evt_guard_other_door_close( void )
{
unsigned src = get_door_source();
    return ((src == SOURCE_DATA || src == SOURCE_AUTO))&& !get_other_door_status() ;
}

/*--------------------------------------------------------------------------*/

Boolean evt_guard_driver_door_open( void )
{
unsigned src = get_door_source();
    return ((src == SOURCE_DATA || src == SOURCE_AUTO))&& get_driver_door_status() ;
}

/*--------------------------------------------------------------------------*/

Boolean evt_guard_driver_door_close( void )
{
unsigned src = get_door_source();
    return ((src == SOURCE_DATA || src == SOURCE_AUTO))&& !get_driver_door_status() ;
}

/*--------------------------------------------------------------------------*/

Boolean evt_guard_hood_open( void )
{
unsigned src = get_hood_source();
    return ((src == SOURCE_DATA || src == SOURCE_AUTO))&& get_hood_status() ;
}

/*--------------------------------------------------------------------------*/

Boolean evt_guard_hood_close( void )
{
unsigned src = get_hood_source();
    return ((src == SOURCE_DATA || src == SOURCE_AUTO))&& !get_hood_status() ;
}

/*--------------------------------------------------------------------------*/

Boolean evt_guard_trunk_open( void )
{
unsigned src = get_trunk_source();
    return ((src == SOURCE_DATA || src == SOURCE_AUTO))&& get_trunk_status() ;
}

/*--------------------------------------------------------------------------*/

Boolean evt_guard_trunk_close( void )
{
unsigned src = get_trunk_source();
    return ((src == SOURCE_DATA || src == SOURCE_AUTO))&& !get_trunk_status() ;
}

/*--------------------------------------------------------------------------*/

Boolean evt_guard_ign_on( void )
{
unsigned src = get_ign_source();
    return ((src == SOURCE_DATA || src == SOURCE_AUTO))&& get_ign_status() ;
}

/*--------------------------------------------------------------------------*/

Boolean evt_guard_ign_off( void )
{
unsigned src = get_ign_source();
    return ((src == SOURCE_DATA || src == SOURCE_AUTO))&& !get_ign_status() ;
}

/*--------------------------------------------------------------------------*/

Boolean evt_guard_brake_on( void )
{
unsigned src = get_brake_source();
    return ((src == SOURCE_DATA || src == SOURCE_AUTO))&& get_brake_status() ;
}

/*--------------------------------------------------------------------------*/

Boolean evt_guard_brake_off( void )
{
unsigned src = get_brake_source();
    return ((src == SOURCE_DATA || src == SOURCE_AUTO))&& !get_brake_status() ;
}

/*--------------------------------------------------------------------------*/

Boolean evt_guard_handbrake_on( void )
{
unsigned src = get_handbrake_source();
    return ((src == SOURCE_DATA || src == SOURCE_AUTO))&& get_handbrake_status() ;
}

/*--------------------------------------------------------------------------*/

Boolean evt_guard_handbrake_off( void )
{
unsigned src = get_handbrake_source();
    return ((src == SOURCE_DATA || src == SOURCE_AUTO))&& !get_handbrake_status() ;
}

/*--------------------------------------------------------------------------*/

Boolean evt_guard_glowplug_off_active( void )
{
unsigned src = get_glowplug_source();
    return ((src == SOURCE_DATA || src == SOURCE_AUTO))&& get_glowplug_off_status() ;
}

/*--------------------------------------------------------------------------*/

Boolean evt_guard_glowplug_off_inactive( void )
{
unsigned src = get_glowplug_source();
    return ((src == SOURCE_DATA || src == SOURCE_AUTO))&& !get_glowplug_off_status() ;
}

/*--------------------------------------------------------------------------*/
