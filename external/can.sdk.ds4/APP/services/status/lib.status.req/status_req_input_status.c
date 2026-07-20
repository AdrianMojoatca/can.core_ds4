/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: status_req_input_status.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "status_req_private.h"
#include "prog.h"
#include "rf.h"

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

void status_req_input_status(void * arg)
{
	
      req_input.status.byte = 0 ;

      if((d2d_get_ena(SET_DRIVER_OPEN) && d2d_get_ena(SET_DRIVER_CLOSE)))
        {
   		  if (status_get_door (STATUS_DOOR_FRONT_DRIVER) == 1)
		    {
		  	  req_input.status.bit.driver = 1 ;
			}
        }
      
      if((d2d_get_ena(SET_OTHER_OPEN) && d2d_get_ena(SET_OTHER_CLOSE)))
        {
		  if (status_get_door (STATUS_DOOR_REAR_DRIVER) == 1)
		  	req_input.status.bit.other = 1 ;
		  if (status_get_door (STATUS_DOOR_FRONT_PASSENGER) == 1)
		  	req_input.status.bit.other = 1 ;
		  if (status_get_door (STATUS_DOOR_REAR_PASSENGER) == 1)
		  	req_input.status.bit.other = 1 ;		   
        }

      if((d2d_get_ena(SET_TRUNK_OPEN) && d2d_get_ena(SET_TRUNK_CLOSE)))
        {
          if (status_get_door (STATUS_DOOR_TRUNK) == 1)
		    req_input.status.bit.trunk = 1 ;
        }

      if((d2d_get_ena(SET_HOOD_OPEN) && d2d_get_ena(SET_HOOD_CLOSE)))
        {
          if (status_get_door (STATUS_DOOR_HOOD) == 1)
		    req_input.status.bit.hood = 1 ;
        }

      if((d2d_get_ena(SET_BRAKE_ON) && d2d_get_ena(SET_BRAKE_OFF)))
        {
          if (status_get_brake (STATUS_BRAKE_FOOT) == 1)
		    req_input.status.bit.brake = 1 ;
        }

      if((d2d_get_ena(SET_KEY_IN) && d2d_get_ena(SET_KEY_OUT)))
        {
          if (status_get_key (STATUS_KEY_IN) == 1)
		    req_input.status.bit.key_in = 1 ;
        }

      if((d2d_get_ena(SET_PRK_NTRL_ACTIVE) && d2d_get_ena(SET_PRK_NTRL_NOT_ACTIVE)))
        {
          #if STATUS_TRANS_ENABLE == 1
		  if (status_get_trans (STATUS_TRANS_PARK) == 1)
		    req_input.status.bit.park_neutral = 1 ;
		  if (status_get_trans (STATUS_TRANS_NEUTRAL) == 1)
			req_input.status.bit.park_neutral = 1 ;
		  #endif
        }

      if((d2d_get_ena(SET_HANDBRAKE_ON) && d2d_get_ena(SET_HANDBRAKE_OFF)))
        {
          if (status_get_brake (STATUS_BRAKE_EMERGENCY) == 1)
		    req_input.status.bit.handbrake = 1 ;
        }
    

	if (prog_retrieve_option (FEAT_RF_OUTPUT) != RF_TD)	// NO RFTD, use 0xFE
	{												
		UInt8 data [3] = {SET_STATUS, 0, 0};											
		
		
			data[1] = req_input.support.byte ; 
			data[2] = req_input.status.byte ;
		
	
		d2d_tx_frame (data, sizeof_array(data)) ;
	}
	else
	{
		rf_update () ;
	}
}

