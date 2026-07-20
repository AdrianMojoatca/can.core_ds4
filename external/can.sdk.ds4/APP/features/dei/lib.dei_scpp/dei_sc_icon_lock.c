/*==========================================================================*/
// $Id: dei_sc_icon_lock.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_sc_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
#define DEI_SC_LOCK_ZONE_ACTIVE  (NOTIFY_PLD_ZONE_DOOR|NOTIFY_PLD_ZONE_HOOD|NOTIFY_PLD_ZONE_TRUNK|NOTIFY_PLD_ZONE_IGN)
#define DEI_SC_LOCK_NPC_ACTIVE   (NOTIFY_PLD_ZONE_S1_NPC|NOTIFY_PLD_ZONE_S2_NPC|NOTIFY_PLD_ZONE_S3_NPC)
/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static Boolean dei_sc_icon_lock_pagging        ( void ) ;
static Boolean dei_sc_icon_lock_sync           ( Boolean valet_status ) ;

static Boolean dei_sc_icon_lock_pagging_silent ( void ) ;
static Boolean dei_sc_icon_lock_sync_silent    ( Boolean valet_status ) ;
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

Boolean dei_sc_icon_lock( Notify_Pld_Zone_Report zone )
{    
    Boolean ret      = TRUE ;
	Boolean is_valet = FALSE ;	
	
	switch( dei_sc_info_get( DEI_SC_INFO_SECURITY ) )
	{
		case DEI_SECURITY_ARMED_VALET_ON   :
		case DEI_SECURITY_DISARMED_VALET_ON:
		{
			dei_sc_info_set( DEI_SC_INFO_SECURITY  , DEI_SECURITY_ARMED_VALET_ON );    

			is_valet = TRUE ; 
		}
		break;

		case DEI_SECURITY_ARMED_RS_VALET_ON   :
		case DEI_SECURITY_DISARMED_RS_VALET_ON:
		{
			dei_sc_info_set( DEI_SC_INFO_SECURITY  , DEI_SECURITY_ARMED_RS_VALET_ON );    

			is_valet = TRUE ; 
		}
		break;

		case DEI_SECURITY_ARMED_COMBO_VALET_ON   :
		case DEI_SECURITY_DISARMED_COMBO_VALET_ON:
		{
			dei_sc_info_set( DEI_SC_INFO_SECURITY  , DEI_SECURITY_ARMED_COMBO_VALET_ON );    

			is_valet = TRUE ; 
		}
		break;
		
		case DEI_SECURITY_ARMED   :
		case DEI_SECURITY_DISARMED:
		{
			dei_sc_info_set( DEI_SC_INFO_SECURITY  , DEI_SECURITY_ARMED );  
		}
		break;

		default :  
		{
			dei_sc_info_set( DEI_SC_INFO_SECURITY  , DEI_SECURITY_ARMED ); 
		}
		break;
	}				  	
	         
    dei_sc_info_set( DEI_SC_INFO_DOOR_STATUS    , !!(zone & NOTIFY_PLD_ZONE_DOOR    ) );
    dei_sc_info_set( DEI_SC_INFO_TRUNK_STATUS   , !!(zone & NOTIFY_PLD_ZONE_TRUNK   ) );
    dei_sc_info_set( DEI_SC_INFO_HOOD_STATUS    , !!(zone & NOTIFY_PLD_ZONE_HOOD    ) );
	dei_sc_info_set( DEI_SC_INFO_IGNITION_STATUS, !!(zone & NOTIFY_PLD_ZONE_IGN     ) );
	dei_sc_info_set( DEI_SC_INFO_SENSOR1_NPC    , !!(zone & NOTIFY_PLD_ZONE_S1_NPC  ) );
	dei_sc_info_set( DEI_SC_INFO_SENSOR2_NPC    , !!(zone & NOTIFY_PLD_ZONE_S2_NPC  ) );
	dei_sc_info_set( DEI_SC_INFO_SENSOR3_NPC    , !!(zone & NOTIFY_PLD_ZONE_S3_NPC  ) );
    
        
    
    if( DEI_SC_PAGGING() ) 
    {
        if( dei_sc_is_silent )
        {
            ret &= dei_sc_icon_lock_pagging_silent() ; 
            
            if( zone & (DEI_SC_LOCK_ZONE_ACTIVE | DEI_SC_LOCK_NPC_ACTIVE) )
            {
                ret &= dei_sc_icon_lock_active_zone_report() ; 
            }
        }
        else
        {
            ret &= dei_sc_icon_lock_pagging() ;         
            
            if( zone & (DEI_SC_LOCK_ZONE_ACTIVE | DEI_SC_LOCK_NPC_ACTIVE) )
            {
                ret &= dei_sc_icon_lock_active_zone_report() ; 
            }
        }
    }
    else
    {
        if( dei_sc_is_silent )
        {
            ret &= dei_sc_icon_lock_sync_silent(is_valet) ; 
            
            if( zone & (DEI_SC_LOCK_ZONE_ACTIVE | DEI_SC_LOCK_NPC_ACTIVE)  )
            {
               ret &= dei_sc_icon_lock_active_zone_report() ;                         
            }
        }
        else
        {
            ret &= dei_sc_icon_lock_sync(is_valet) ;             
            
            if( zone & (DEI_SC_LOCK_ZONE_ACTIVE | DEI_SC_LOCK_NPC_ACTIVE) )
            {
                ret &= dei_sc_icon_lock_active_zone_report() ;           
            }
        }

		timeout_update ( dei_sc_timeout_lock , DEI_SC_TIMEOUT_LOCK );						
		timeout_start  ( dei_sc_timeout_lock );
					
    }
    
    return ret ;
}

//--------------------------------------------------------------------------//

Boolean dei_sc_icon_lock_pagging( void )
{    
   return dei_sc_tx( DEI_SC_ACK_LOCK , DEI_SC_PAGGING_TX , 100 ) ; 
}

//--------------------------------------------------------------------------//

Boolean dei_sc_icon_lock_pagging_silent( void )
{    
   return  dei_sc_tx( DEI_SC_ACK_UPDATE_PAGE , DEI_SC_PAGGING_TX , 100 ) ;     
}    

//--------------------------------------------------------------------------//

Boolean dei_sc_icon_lock_sync( Boolean valet_status )
{    
    if( valet_status )
    {
        return  dei_sc_tx( DEI_SC_ACK_LOCKED_VALET_ON , DEI_SC_NORMAL_TX , 100 ) ; 
    }

    return dei_sc_tx( DEI_SC_ACK_LOCK , DEI_SC_NORMAL_TX , 100 ) ; 
}

//--------------------------------------------------------------------------//

Boolean dei_sc_icon_lock_sync_silent( Boolean valet_status )
{
   if( valet_status )
   {
        return  dei_sc_tx( DEI_SC_ACK_LOCKED_VALET_ON , DEI_SC_NORMAL_TX , 100 ) ; 
   }

   return dei_sc_tx( DEI_SC_ACK_SILENT_LOCKED , DEI_SC_NORMAL_TX , 100 ) ; }

//--------------------------------------------------------------------------//

#undef DEI_SC_LOCK_ZONE_ACTIVE
#undef DEI_SC_LOCK_NPC_ACTIVE
