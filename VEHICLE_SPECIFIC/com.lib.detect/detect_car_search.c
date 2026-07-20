/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: detect_car_search.c 4270 2012-11-07 06:19:27Z florin.olariu $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "detect_private.h"

#include "sense.h"

#include "func_private.h"
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

//--------------------------------------------------------------------------//

Boolean detect_car_search( void )
{
    Boolean can0_ok = TRUE ; 
    Boolean can1_ok = TRUE ; 

    if( detect_init_nvfs()   ) 
	{
		can_usr_init  (CAN0_ENABLE | CAN1_ENABLE) ;
		can_per_init (3);    
    	can_sync_init(CAN_DEV_0 , 3 );
		sense_init(CAN_DEV_0);
		return TRUE;         
	}
    
    led_detect_started();

	detect_evt = os_evt_create();
    
    can_usr_init  (CAN0_ENABLE | CAN1_ENABLE) ;
	detect_init();
	TRACE("\r\nDETECT: detect_init completed before detect_can_search\r\n");
	
	
    //can_sw_filter_disable(CAN_DEV_0);
    //can_sw_filter_disable(CAN_DEV_1);
    
    
	detect_evt = os_evt_create();
	 
    detect_can_search();        /// CAN network searching...

	TRACE("\rAFTER DETECT_CAN_SEARCH");

    
	if( !car.bits.can0_detected )
    {
		led_detect_err_can0();

        os_tsk_wait( 3000 ) ;

        return FALSE;
    }
    

	TRACE("\rBEFORE DETECT_IGN_SEARCH");
	   
    if( !detect_ign_search() )  /// Ignition searching...
    {                
        TRACE("\r\n I'M IN IGNTION SEARCH\r\n");
		
		led_detect_err_invalid_ignition();

        os_tsk_wait( 3000 ) ;

        return FALSE;
    }
	

	led_detect_in_progress();
    

	/*else
	{
		// ADI - 02.02.2017
		TRACE("\r\n DETECT IGNITION DONE \r\n");
		led_detect_done();
		os_tsk_wait(3000);
		//return TRUE;
		// end ADI - 02.02.2017
	}*/

	/*
    if( !detect_vin_search() )  /// VIN searching ...
    {
        led_detect_err_invalid_vin();
        
        os_tsk_wait(3000);
        
        return FALSE ;
    } 

    if( !detect_pgm() )  /// Other car programming...
    {
        led_detect_err_invalid_bcm();
        
        os_tsk_wait(3000);
        
        return FALSE ;
    } 
	*/
          
    TRACE( "\n\r[ DETECT : IGNITION_FOUND]\n\r") ;

    TRACE( "\n\r[ DETECT : CAN0_%s]\n\r" ,  car.bits.can0_detected ? "FOUND" : "NOT FOUND" ) ;
    TRACE( "\n\r[ DETECT : CAN1_%s]\n\r" ,  car.bits.can1_detected ? "FOUND" : "NOT FOUND" ) ;

   end_detect_phase:
   {
        nvfs_wr( nvfs_fw_data , &car  );
        nvram_usr_commit();
              
        os_tsk_wait( 3000 );

        wdog_v_force_reset( WDOG_V_RESET_FIRMWARE , (char*)__FUNCTION__ , __LINE__ ) ;     
   }

   return TRUE;// --- never called.keep compiler happy
}


//--------------------------------------------------------------------------//

