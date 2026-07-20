/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_system_sense_init.c 20540 2013-04-22 11:49:22Z florin.olariu $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_system_private.h"
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

void dei_system_sense_init( void )
{		
    static Boolean do_one_time = FALSE;

    if(do_one_time==FALSE)
    {
        ATOMIC(do_one_time = TRUE);

        status_system_init();

	    set_hood_close_type( (dei_feature_get( DEI_FEAT_HOOD_TRIGGER_TYPE ) == 2) ? HOOD_NORMALLY_CLOSE : HOOD_NORMALLY_OPEN );

	    set_door_close_type( (dei_feature_get( DEI_FEAT_DOOR_SWITCH_TYPE ) == 2) ? DOOR_NORMALLY_CLOSE : DOOR_NORMALLY_OPEN );

	    set_trunk_close_type( (dei_feature_get( DEI_FEAT_TRUNK_SWITCH_TYPE ) == 2) ? TRUNK_NORMALLY_CLOSE : TRUNK_NORMALLY_OPEN );
			
			

	    set_door_source     ((SOURCE )(dei_feature_get(DEI_FEAT_SENSE_DOORS    ) == 1) ? SOURCE_DATA : SOURCE_NONE);
	    set_hood_source     ((SOURCE )(dei_feature_get(DEI_FEAT_SENSE_HOOD     ) == 1) ? SOURCE_DATA : SOURCE_NONE);
	    set_trunk_source    ((SOURCE )(dei_feature_get(DEI_FEAT_SENSE_TRUNK    ) == 1) ? SOURCE_DATA : SOURCE_NONE);
	    set_ign_source      ((SOURCE )(dei_feature_get(DEI_FEAT_SENSE_IGN      ) == 1) ? SOURCE_DATA : SOURCE_NONE);
	    set_rpm_source      ((SOURCE )(dei_feature_get(DEI_FEAT_SENSE_TACHO  	 ) == 1) ? SOURCE_DATA : SOURCE_NONE);
	    set_brake_source    ((SOURCE )(dei_feature_get(DEI_FEAT_SENSE_BRAKE    ) == 1) ? SOURCE_DATA : SOURCE_NONE);
	    set_handbrake_source((SOURCE )(dei_feature_get(DEI_FEAT_SENSE_HANDBRAKE) == 1) ? SOURCE_DATA : SOURCE_NONE);
	    set_speed_source    ((SOURCE )(dei_feature_get(DEI_FEAT_ENGINE_SENSING ) == 1) ? SOURCE_DATA : SOURCE_NONE);
			set_headlight_source( SOURCE_NONE);
/*
			set_hood_source     ((SOURCE )dei_feature_get(DEI_FEAT_SENSE_HOOD      ));
	    set_trunk_source    ((SOURCE )dei_feature_get(DEI_FEAT_SENSE_TRUNK     ));
	    set_ign_source      ((SOURCE )dei_feature_get(DEI_FEAT_SENSE_IGN       ));
	    set_rpm_source      ((SOURCE )dei_feature_get(DEI_FEAT_SENSE_TACHO     ));
    	set_brake_source    ((SOURCE )dei_feature_get(DEI_FEAT_SENSE_BRAKE     ));
	    set_handbrake_source((SOURCE )dei_feature_get(DEI_FEAT_SENSE_HANDBRAKE ));
			set_speed_source		((SOURCE )dei_feature_get(DEI_FEAT_ENGINE_SENSING));
*/    
			
	    set_rke_lock_use    ((RKE_USE)dei_feature_get(DEI_FEAT_SENSE_OEM_LOCK  ));
	    set_rke_unlock_use  ((RKE_USE)dei_feature_get(DEI_FEAT_SENSE_OEM_UNLOCK));
	    set_rke_trunk_use   ((RKE_USE)dei_feature_get(DEI_FEAT_SENSE_OEM_TRUNK ));
			
	    
        //initialize the sense rs
        {
            Boolean turbo_enabled;
            
            static Sense_Rs_Input_Config input_config ; 

            input_config.inputs[SENSE_RS_IN_NEG1]=dei_feature_get(DEI_FEAT_IN_NEG1);
            //input_config.inputs[SENSE_RS_IN_NEG2]=dei_feature_get(DEI_FEAT_IN_NEG2);
            input_config.inputs[SENSE_RS_IN_NEG3]=dei_feature_get(DEI_FEAT_IN_NEG3);
            input_config.inputs[SENSE_RS_IN_NEG4]=dei_feature_get(DEI_FEAT_IN_NEG4);
            input_config.inputs[SENSE_RS_IN_NEG5]=dei_feature_get(DEI_FEAT_IN_NEG5);
            //input_config.inputs[SENSE_RS_IN_NEG6]=dei_feature_get(DEI_FEAT_IN_NEG6);
            input_config.inputs[SENSE_RS_IN_NEG7]=dei_feature_get(DEI_FEAT_IN_NEG7);

            input_config.inputs[SENSE_RS_IN_POS1]=dei_feature_get(DEI_FEAT_IN_POS1);
            input_config.inputs[SENSE_RS_IN_POS2]=dei_feature_get(DEI_FEAT_IN_POS2);
            input_config.inputs[SENSE_RS_IN_POS3]=dei_feature_get(DEI_FEAT_IN_POS3);

            input_config.inputs[SENSE_RS_IN_NP  ]=dei_feature_get(DEI_FEAT_IN_NEG_AND_POS1);

            turbo_enabled =  dei_feature_get( DEI_FEAT_TURBO_MODE_RUNTIME );
                        
            input_config.start_input_config = turbo_enabled?SENSE_RS_START_INPUT_MAPPED_TO_TURBO:SENSE_RS_START_INPUT_MAPPED_TO_START;

        
            sense_rs_init(&input_config);
        }  	
    }
}

//--------------------------------------------------------------------------//



