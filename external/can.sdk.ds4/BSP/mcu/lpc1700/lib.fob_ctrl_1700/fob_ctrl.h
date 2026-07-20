/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: fob_ctrl.h 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

#ifndef __FOB_CTRL_H__
#define __FOB_CTRL_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "device.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*--------------------------------------------------------------------------*/

typedef void ( *Fob_Ctrl_Callback )( void );

/*--------------------------------------------------------------------------*/

typedef enum
{
    
    FOB_CTRL_ENTER  , //<--enter in fob control mode   ( first press of lock , after unlock / timeout )
    FOB_CTRL_LEAVE  , //<--exit from fob control mode       
    FOB_CTRL_RS     
    
}Fob_Ctrl_Actions ; 

/*--------------------------------------------------------------------------*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

EXTERN_C_ENTER

void    fob_ctrl_init      ( void );  
Boolean fob_ctrl_is_enable ( void );

void    fob_ctrl_reset_init( void );
UInt32  fob_ctrl_get_tbp   ( void ); //<--- get timeout between presses value(in ms)

void    fob_ctrl_callback ( Fob_Ctrl_Actions action , Fob_Ctrl_Callback callback_func );
                                                        // used to register custom actions
                                                        // default actions :
                                                        // - FOB_CTRL_ENTER : disable siren chirps for DEI
                                                        // - FOB_CTRL_LEAVE : restore siren chirps for DEI
                                                        // - FOB_CTRL_RS    : insert in cmd queue INPUT_RF_STRAT_STOP_TOGGLE                                

void fob_ctrl_dei_rs_toggle    ( void );   // sends INPUT_RF_START_STOP with fob

EXTERN_C_LEAVE


#endif
