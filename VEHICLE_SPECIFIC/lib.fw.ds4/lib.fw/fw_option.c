/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: 15282 2012-06-12 13:29:20Z louis-philippe.rispoli $
/*==========================================================================*/


/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "fw.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void fw_options_set( void );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
static Func_Cfg func_cfg ;        
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

/*--------------------------------------------------------------------------*/

void fw_init( void )
{	
	if( detect_car_search() )
    {
        if( detect_car.bits.can0_detected ) func_cfg.can_dev_id = CAN_DEV_0 ;
        else   
        if( detect_car.bits.can1_detected ) func_cfg.can_dev_id = CAN_DEV_1 ;    
    
        func_init(&func_cfg);

        prog_set_exit_callback((Prog_Func)fw_options_set);

        fw_options_set();
    }
}

/*--------------------------------------------------------------------------*/

void fw_options_set( void )
{
	cfg.confort = prog_retrieve_option(FEAT_COMFORT_CLOSURE)-1;        
    
    switch(prog_retrieve_option(FEAT_CUSTOM1))
    {
        case 1  : tsk_rs_update_cfg(TSK_RS_TAKEOVER_DISABLE);                        break;
        case 2  : tsk_rs_update_cfg(TSK_RS_TAKEOVER_OEM);                            break;         
        case 3  : tsk_rs_update_cfg(TSK_RS_TAKEOVER_AFTERMARKET|TSK_RS_TAKEOVER_OEM);break;         

        default : break;
    }
        
    cfg.aux1    = prog_retrieve_option(FEAT_CUSTOM2);   
    cfg.aux2    = prog_retrieve_option(FEAT_CUSTOM3);
    cfg.aux3    = prog_retrieve_option(FEAT_CUSTOM4);    
}	

/*--------------------------------------------------------------------------*/



