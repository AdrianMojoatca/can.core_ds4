/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: fw_init.c 15282 2012-06-12 13:29:20Z louis-philippe.rispoli $
/*==========================================================================*/


/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "fw.h"
//#include "prog_private.h"
//#include "key.h"
#include "detect_private.h"
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
//extern Frd_Car      frd_car                     ;

//extern Detect_Vars     detect_vars  ;      
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

/*--------------------------------------------------------------------------*/

void fw_init( void )
{	
	TRACE("\r FW INIT!");
	//return; // WE ARE FULL!!! - STOP HERE

	//detect_vars.prog_state = 0 ; // 0 = init, 1 = log, 2 = bypass, 3 = done

	/*
	key_init();
    while(!detect_is_done ()) ;
	*/	

	if( detect_car_search() )
    {
        TRACE("\r\n CAR DETECTED \r\n");

		car.bits.can0_detected = TRUE;
        
        if( car.bits.can0_detected ) func_cfg.can_dev_id = CAN_DEV_0 ;

		func_init();

        prog_set_exit_callback((Prog_Func)fw_options_set);

        fw_options_set();
    }

}

/*--------------------------------------------------------------------------*/

void fw_options_set( void )
{
	func_cfg.comfort = prog_retrieve_option(FEAT_COMFORT_CLOSURE);        
    
    /*switch(prog_retrieve_option(FEAT_CUSTOM1))
    {
        case 1  : tsk_rs_update_cfg(TSK_RS_TAKEOVER_DISABLE);                        break;
        case 2  : tsk_rs_update_cfg(TSK_RS_TAKEOVER_OEM);                            break;         
        case 3  : tsk_rs_update_cfg(TSK_RS_TAKEOVER_AFTERMARKET|TSK_RS_TAKEOVER_OEM);break;    
        case 4  : tsk_rs_update_cfg(TSK_RS_SKIP_DOOR_SHUTDOWN);                      break;

        default : break;
    } */
	//tsk_rs_update_cfg(TSK_RS_TAKEOVER_SKD|TSK_RS_TAKEOVER_AFTERMARKET|TSK_RS_TAKEOVER_OEM); // ADI                       
	tsk_rs_update_cfg(TSK_RS_TAKEOVER_DISABLE); // ADI - 14.11.2016                      
        
    func_cfg.aux1    = prog_retrieve_option(FEAT_CUSTOM2);   
    func_cfg.aux2    = prog_retrieve_option(FEAT_CUSTOM3);
    func_cfg.aux3    = prog_retrieve_option(FEAT_CUSTOM4); 
}	

/*--------------------------------------------------------------------------*/



