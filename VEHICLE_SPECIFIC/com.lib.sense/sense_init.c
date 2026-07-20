/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: sense_init.c 4280 2012-11-16 10:43:59Z florin.olariu $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "sense_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
extern Boolean hb_on_3B3;
extern Boolean tr_rel_active;



struct S_GEARBOX
{
    UInt8   type;
} gearbox;  
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void sense_init( Can_Dev_Id can_id )
{
    sense_rs_can_init(NULL);

	set_ign_source(SOURCE_DATA);  // THIS IS FUCKIN' 1
	set_door_source(SOURCE_DATA); // THIS IS FUCKIN' 2
	set_trunk_source(SOURCE_DATA); // THIS IS FUCKIN' 3
	set_rke_lock_use(RKE_ENABLE);  // THIS IS FUCKIN' 4
	set_rke_trunk_use(RKE_ENABLE); // THIS IS FUCKIN' 5
	set_handbrake_source(SOURCE_DATA); // THIS IS FUCKIN' 6
	set_brake_source(SOURCE_DATA); // THIS IS FUCKIN' 7
	set_rke_unlock_use(RKE_ENABLE); // THIS IS FUCKIN' 8

	set_speed_source(SOURCE_DATA); // THIS IS FUCKIN' 9
	set_rpm_source(SOURCE_DATA); // THIS IS FUCKIN' 10

	TRACE("\rSENSE INIT");
    
    can_register:
    {
        static const Can_Usr_Array1 can_table[] = 
        {
            { (Reg_Func)sense_doors_3B3             , 0x3B3 },            
            { (Reg_Func)sense_trunk_3B3             , 0x3B3 },
            { (Reg_Func)sense_hood_3B3              , 0x3B3 },            
            { (Reg_Func)sense_ignition_3B3          , 0x3B3 },                    
            { (Reg_Func)sense_rke_331               , 0x331 },
			{ (Reg_Func)sense_rke_3B3               , 0x3B3 }, // ADI - 06.01.2016

			//{ (Reg_Func)detect_frd_sense_ign        , 0x3B3 },                                  
			              
            { (Reg_Func)sense_left_front_window_xxx , 0x006 },     
            { (Reg_Func)sense_right_front_window_xxx, 0x007 },   
            { (Reg_Func)sense_left_rear_window_xxx  , 0x008 },    
            { (Reg_Func)sense_right_rear_window_xxx , 0x009 }, 
			           
        	//{ (Reg_Func)sense_handbrake_3BC         , 0x3BC },   
			{ (Reg_Func)sense_handbrake_213         , 0x213 }, 

            { (Reg_Func)sense_brake_3C3             , 0x3C3 },   
            { (Reg_Func)sense_rpm_204               , 0x204 },                  
            { (Reg_Func)sense_hazard_3B3            , 0x3B3 },                                  
            { (Reg_Func)sense_skd_xxx               , 0x00E },                                  
            { (Reg_Func)sense_speed_xxx             , 0x00F },
			{ (Reg_Func)sense_manual_box_171        , 0x171 },
			{ (Reg_Func)sense_handbrake_3B3         , 0x3B3 },   

			{ (Reg_Func)sense_rke_3C3               , 0x3C3 }, // ADI - 07.06.2018

			{ (Reg_Func)sense_rpm_fake_rpm  	    , 0x167 },

			{ (Reg_Func)sense_index_446  	        , 0x446 },
			{ (Reg_Func)sense_index_332  	        , 0x332 },

			{ (Reg_Func)sense_index_331  	        , 0x331 },  // in vers 1.01
			
     
        };

		CAN_USR_REG1(CAN_DEV_0 , can_table);

		// 6000 inseamna 6 secunde - merge 
		// 9000 inseamna 9 secunde - merge
		// 30000 inseamna 30 de secunde	- merge
		// 60000 inseamna 60 de secunde - nu merge 
		// 35000 inseamna 35 de secunde - merge
		// 10000 inseamna 10 secunde  - merge	

		trunk_sense_delayed = timeout_f_create ( 5000, (Exec_Func) trunk_status_handler );
		tr_rel_active = FALSE;

		// see if gearbox is already saved - this part in vers. 1_08 
		/*
		if ( nvfs_rd(nvfs_gearbox_feat, &gearbox ) == NVFS_OK )
		{
			if ( gearbox.type == MANUAL_GEARBOX )
			{
				CAR_Gearbox = MANUAL_GEARBOX;
			}
			else if ( gearbox.type == AUTOMATIC_GEARBOX )
			{
				CAR_Gearbox = AUTOMATIC_GEARBOX;
			} 
			else if ( gearbox.type == IDLE_STATE_GEARBOX )
			{
				CAR_Gearbox = IDLE_STATE_GEARBOX;
			}
		}
		// ... and if not make it idle, we'll save it at first action on gearbox
		else
		{
			gearbox.type = IDLE_STATE_GEARBOX;					   
			nvfs_wr_create ( nvfs_gearbox_feat, &gearbox, sizeof(gearbox) );
			nvram_usr_commit() ; 
		}
		*/														 
    }
	sense_331_present = FALSE;
	sense_332_present = FALSE;

}

//--------------------------------------------------------------------------//
