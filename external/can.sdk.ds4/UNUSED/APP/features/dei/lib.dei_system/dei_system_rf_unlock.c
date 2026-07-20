/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_system_rf_unlock.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_system_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
#define UNLOCK_TYPE		(void*)((!dei_system_2nd_unlock ? CMD_UNLOCK_WITH_DISARM : 0)|\
                                ((((Cmd_Rf_Pld*)cmd_get_pld(remote))->bits.id!=0) ? CMD_UNLOCK_USER2 : 0 ))

#define UNLOCK_DOUBLE_PULSE         ((dei_feature_get( DEI_FEAT_DOORLOCK_PULSES)==2)||\
                                     (dei_feature_get( DEI_FEAT_DOORLOCK_PULSES)==4))

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void dei_system_rf_unlock( void *remote )
{			
    Boolean disarm_oem = FALSE ;  


    if( get_ss_panic()   )
	{	  
	    if( get_ss_lock() )
		{
			notify_send( NOTIFY_LOCK , DEI_NOTIFY_LOCK_PLD );
		}
		else
		{	
			notify_send( NOTIFY_RANGE_CHECK );			
		}

		dei_system_panic_stop();

		return ; 
	}

	if( dei_rs_mts_ready_for_activation() )
	{
		cmd_q_rs_insert( INPUT_RF_START_STOP_TOOGLE , 100 , 0 );

		return  ; 
	}

	if( get_ign_status() && !get_rs_gwr() )
	{
		notify_send( NOTIFY_RANGE_CHECK );	
		
		cmd_q_rs_insert( INPUT_UNLOCK_DRIVER1 | INPUT_ON , 100 , UNLOCK_TYPE ) ; 	
        
        if( dei_feature_get( DEI_FEAT_2ND_UNLOCK ) != DEI_2ND_UNLOCK_OFF )
		{
			cmd_q_rs_insert( INPUT_UNLOCK_ALL1 | INPUT_ON , 100 , UNLOCK_TYPE ) ; 
		}

		return ; 
	}

							
//    if( get_ss_lock() )
//	{				
//		dei_system_2nd_unlock = FALSE ;		
//	}
		
	notify_send( NOTIFY_UNLOCK  , DEI_NOTIFY_UNLOCK_PLD );
	 	

    if( get_ss_lock() )
	{		
		dei_system_ch3_link_disarm();					
		dei_system_ch4_link_disarm();					
		dei_system_ch5_link_disarm();					
		dei_system_ch6_link_disarm();					
	}
	
	
	if( !dei_system_2nd_unlock )
	{
		if( dei_feature_get(DEI_FEAT_OEM_ALARM_DISARM_OUTPUT)!= DEI_OEM_DISARM_RS_ONLY )
		{					
			cmd_q_rs_insert( INPUT_DISARM     , 100 ) ; 
		}

        if( dei_feature_get(DEI_FEAT_OEM_DISARM)==DEI_OEM_DISARM_ON )
        {
            if( get_rs_gwr() == FALSE )
            {
                set_rs_gwr(TRUE);
                cmd_q_rs_insert(INPUT_GWR_ON , 100 , 0 );
                
                os_tsk_wait(100);

                set_rs_acc(TRUE);
                cmd_q_rs_insert(INPUT_RS_ACCESSORY_ON , 100 , 0 );
                
                os_tsk_wait(100);

                set_rs_ign(TRUE);
                cmd_q_rs_insert(INPUT_RS_IGNITION_ON , 100 , 0 );
                
                os_tsk_wait(800);

                disarm_oem = TRUE ;
            }
        }
			
		if( UNLOCK_DOUBLE_PULSE )
        {
            cmd_q_rs_insert( INPUT_UNLOCK_ALL1 | INPUT_ON , 100 , UNLOCK_TYPE ) ; 	
        }
        else
        {
            cmd_q_rs_insert( INPUT_UNLOCK_DRIVER1 | INPUT_ON , 100 , UNLOCK_TYPE ) ; 	
        }

	  
		if( get_ss_sensor_npc(1) )
		{
			dei_prk_flash( DEI_PRK_UNLOCK_NPC_ON ) ; 
		}
		else
		{		  
			if( get_ss_door_trigger() || get_ss_hood_trigger() || get_ss_trunk_trigger() || get_ss_ign_trigger() || get_ss_sensor_trigger(1) )
			{				
				dei_prk_flash( DEI_PRK_UNLOCK_TRIGGER_ON ) ;
			}
			else
			{
				if( dei_feature_get(DEI_FEAT_PARKING_LIGHT_OUTPUT) != 3 ) //parking light off
                {
                    dei_prk_flash( get_rs_gwr() ? DEI_PRK_UNLOCK_NO_EVENT_RS : DEI_PRK_UNLOCK_NO_EVENT ) ; 
                }
                else
                {
                    dei_prk_flash( DEI_PRK_UNLOCK_NO_EVENT ) ; 
                }                    

			}
		}

		if( dei_feature_get( DEI_FEAT_2ND_UNLOCK ) != DEI_2ND_UNLOCK_OFF )
		{		
			dei_system_2nd_unlock = TRUE ; 
			vrtc_stop(vrtc_system_2nd_unlock);
			vrtc_update(vrtc_system_2nd_unlock, SYSTEM_2ND_UNLOCK_WINDOW);
			vrtc_start(vrtc_system_2nd_unlock);
		}
	}
	else
	{					
		cmd_q_rs_insert( INPUT_UNLOCK_ALL1 | INPUT_ON , 100 , UNLOCK_TYPE ) ; 

		dei_system_2nd_unlock = FALSE ; 
		vrtc_stop(vrtc_system_2nd_unlock);
	}	
	
							
    if( disarm_oem == TRUE )
    {        
        if( get_rs_ign() == TRUE )
        {
            set_rs_ign(FALSE);
            cmd_q_rs_insert(INPUT_RS_IGNITION_OFF , 100 , 0 );               
        }
        
        os_tsk_wait(200);
        
        if( get_rs_acc() == TRUE )
        {
            set_rs_acc(FALSE);
            cmd_q_rs_insert(INPUT_RS_ACCESSORY_OFF , 100 , 0 );               
        }
        
        os_tsk_wait(200);

        if( get_rs_gwr() == TRUE )
        {            
            set_rs_gwr(FALSE);
            cmd_q_rs_insert(INPUT_GWR_OFF , 100 , 0 );   
        }        
    }
    set_ss_lock(FALSE);
}

//-------------------------------------------------------------------------------------------------------------//
void dei_system_2nd_unlock_reset    (void)
{
	if((dei_feature_get( DEI_FEAT_IGN_CONTROLLED_LOCKS ) == 1) 
		|| (dei_feature_get( DEI_FEAT_IGN_CONTROLLED_LOCKS ) == 3))
	  dei_system_2nd_unlock = FALSE;
}


//-------------------------------------------------------------------------------------------------------------//

#undef UNLOCK_TYPE
#undef UNLOCK_DOUBLE_PULSE
