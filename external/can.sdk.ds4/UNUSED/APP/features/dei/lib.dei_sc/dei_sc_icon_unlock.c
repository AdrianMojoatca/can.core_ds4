/*==========================================================================*/
// $Id: dei_sc_icon_unlock.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_sc_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define DEI_SC_UNLOCK_NPC_EVENT      (NOTIFY_PLD_TRIG_S1_NPC | NOTIFY_PLD_TRIG_S2_NPC | NOTIFY_PLD_TRIG_S3_NPC)

#define DEI_SC_UNLOCK_TRIGGER_EVENT  (NOTIFY_PLD_TRIG_S1|NOTIFY_PLD_TRIG_S2|NOTIFY_PLD_TRIG_S3|\
					                  NOTIFY_PLD_TRIG_DOOR|NOTIFY_PLD_TRIG_HOOD|NOTIFY_PLD_TRIG_TRUNK|NOTIFY_PLD_TRIG_IGN)
/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static Boolean dei_sc_icon_unlock_pagging        ( void ) ;
static Boolean dei_sc_icon_unlock_sync           ( Boolean valet_status ) ;
static Boolean dei_sc_icon_unlock_sync_silent    ( Boolean valet_status ) ;
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

Boolean dei_sc_icon_unlock( Notify_Pld_Trigger trig )
{    
    Boolean ret      = TRUE ;
	Boolean is_valet = FALSE ;	
	
	switch( dei_sc_info_get( DEI_SC_INFO_SECURITY ) )
	{
		case DEI_SECURITY_ARMED_VALET_ON   :
		case DEI_SECURITY_DISARMED_VALET_ON:
		{
			dei_sc_info_set( DEI_SC_INFO_SECURITY  , DEI_SECURITY_DISARMED_VALET_ON );    

			is_valet = TRUE ; 
		}
		break;
		
		case DEI_SECURITY_ARMED   :
		case DEI_SECURITY_DISARMED:
		{
			dei_sc_info_set( DEI_SC_INFO_SECURITY  , DEI_SECURITY_DISARMED );  
		}
		break;

		default :  
		{
			dei_sc_info_set( DEI_SC_INFO_SECURITY  , DEI_SECURITY_DISARMED ); 
		}
		break;
	}				  	
	
    
    
    
    dei_sc_info_set( DEI_SC_INFO_DOOR_TRIGGERED        , !!(trig&NOTIFY_PLD_TRIG_DOOR   )) ; 
    dei_sc_info_set( DEI_SC_INFO_HOOD_TRIGGERED        , !!(trig&NOTIFY_PLD_TRIG_HOOD   )) ; 
    dei_sc_info_set( DEI_SC_INFO_TRUNK_TRIGGERED       , !!(trig&NOTIFY_PLD_TRIG_TRUNK  )) ; 
    dei_sc_info_set( DEI_SC_INFO_IGNITION_TRIGGERED    , !!(trig&NOTIFY_PLD_TRIG_IGN    )) ; 
    dei_sc_info_set( DEI_SC_INFO_SENSOR1_TRIGGERED     , !!(trig&NOTIFY_PLD_TRIG_S1     )) ; 
    dei_sc_info_set( DEI_SC_INFO_SENSOR2_TRIGGERED     , !!(trig&NOTIFY_PLD_TRIG_S2     )) ; 
    dei_sc_info_set( DEI_SC_INFO_SENSOR3_TRIGGERED     , !!(trig&NOTIFY_PLD_TRIG_S3     )) ;
    dei_sc_info_set( DEI_SC_INFO_SENSOR1_NPC           , !!(trig&NOTIFY_PLD_TRIG_S1_NPC )) ; 
    dei_sc_info_set( DEI_SC_INFO_SENSOR2_NPC           , !!(trig&NOTIFY_PLD_TRIG_S1_NPC )) ; 
    dei_sc_info_set( DEI_SC_INFO_SENSOR3_NPC           , !!(trig&NOTIFY_PLD_TRIG_S1_NPC )) ; 
    

    if( !!(trig & DEI_SC_UNLOCK_NPC_EVENT) )
    {
        dei_sc_info_set( DEI_SC_INFO_TRIGGER , DEI_ICON_TRIGGER_NPC_ON ) ; 
    }
    else
    {
        if( !!(trig & DEI_SC_UNLOCK_TRIGGER_EVENT) )
        {
            dei_sc_info_set( DEI_SC_INFO_TRIGGER , DEI_ICON_TRIGGER_ACTIVE ) ;             
        }
        else
        {
            dei_sc_info_set( DEI_SC_INFO_TRIGGER , DEI_ICON_TRIGGER_NO_TRIGGER ) ;             
        }
    }            
    
    
    

    if( DEI_SC_PAGGING() ) 
    {
         ret &= dei_sc_icon_unlock_pagging() ; 
         
         if( (trig & (DEI_SC_UNLOCK_NPC_EVENT | DEI_SC_UNLOCK_TRIGGER_EVENT )) )
         {
            ret &= dei_sc_tx( DEI_SC_ACK_TRIGGER_ZONE_REPORT , DEI_SC_PAGGING_TX , 100 );        
         }
    }
    else
    {
        if( dei_sc_is_silent )
        {
            ret &= dei_sc_icon_unlock_sync_silent(is_valet) ; 
            
            if( trig & (DEI_SC_UNLOCK_NPC_EVENT | DEI_SC_UNLOCK_TRIGGER_EVENT ) )
            {
                ret &= dei_sc_tx( DEI_SC_ACK_TRIGGER_ZONE_REPORT , DEI_SC_NORMAL_TX , 100 );        
            }
        }
        else
        {
            ret &= dei_sc_icon_unlock_sync(is_valet) ;             
            
            if( trig & (DEI_SC_UNLOCK_NPC_EVENT | DEI_SC_UNLOCK_TRIGGER_EVENT) )
            {
                ret &= dei_sc_tx( DEI_SC_ACK_TRIGGER_ZONE_REPORT , DEI_SC_NORMAL_TX , 100 );        
            }
        }
    }
        
    return ret ;        
}
//--------------------------------------------------------------------------//

Boolean dei_sc_icon_unlock_pagging( void )
{    
   return dei_sc_tx( DEI_SC_ACK_UPDATE_PAGE , DEI_SC_PAGGING_TX , 100 ) ; 
}

//--------------------------------------------------------------------------//

Boolean dei_sc_icon_unlock_sync( Boolean valet_status )
{     
    if( valet_status)
    {
        return  dei_sc_tx( DEI_SC_ACK_UNLOCKED_VALET_ON , DEI_SC_NORMAL_TX , 100 ) ; 
    }

    return dei_sc_tx( DEI_SC_ACK_UNLOCK , DEI_SC_NORMAL_TX , 100 ) ; 
}

//--------------------------------------------------------------------------//

Boolean dei_sc_icon_unlock_sync_silent( Boolean valet_status )
{
     
    if( valet_status )
    {
        return  dei_sc_tx( DEI_SC_ACK_UNLOCKED_VALET_ON , DEI_SC_NORMAL_TX , 100 ) ; 
    }

    return dei_sc_tx( DEI_SC_ACK_SILENT_UNLOCKED , DEI_SC_NORMAL_TX , 100 ) ; 
}

//--------------------------------------------------------------------------//

#undef DEI_SC_UNLOCK_NPC_EVENT
#undef DEI_SC_UNLOCK_TRIGGER_EVENT

