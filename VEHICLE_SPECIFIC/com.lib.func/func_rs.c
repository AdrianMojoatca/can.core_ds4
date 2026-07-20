/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: func_rs.c 4279 2012-11-16 10:42:15Z florin.olariu $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
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
Boolean ignore_doors_status;

//extern Boolean AT_Gearbox; // that's from sense_private.h
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

/*--------------------------------------------------------------------------*/

void func_rs_init( void )
{
    tsk_rs_init();
	//tsk_rs_init( (Tsk_Rs_Pts) func_rs_pts_callback );
    
    tsk_rs_register(  TSK_RS_GWR_ON  , (Tsk_Rs_Func)func_rs_gwr_on  );
    tsk_rs_register(  TSK_RS_GWR_OFF , (Tsk_Rs_Func)func_rs_gwr_off );    
    tsk_rs_register(  TSK_RS_IGN_ON  , (Tsk_Rs_Func)func_rs_ign_on  );
    tsk_rs_register(  TSK_RS_IGN_OFF , (Tsk_Rs_Func)func_rs_ign_off );
    tsk_rs_register(  TSK_RS_STR_ON  , (Tsk_Rs_Func)func_rs_str_on  );
    tsk_rs_register(  TSK_RS_STR_OFF , (Tsk_Rs_Func)func_rs_str_off );
}

/*--------------------------------------------------------------------------*/

void func_rs_gwr_on( void )
{
    Can_Msg rs_on_can_msg;
	UInt8 rs_on_counter;

	TRACE_FUNC("[%s]\n\r",__FUNCTION__);

	set_rs_gwr (TRUE);
	
	gpio_iface_rf_loop(TRUE);
}

/*--------------------------------------------------------------------------*/

void func_rs_ign_on( void )
{
    Boolean done        = FALSE ;
    UInt8   retry_count = 10    ; 

	TRACE_FUNC("[%s]\n\r",__FUNCTION__);

    if( !get_ign_status() )
    {
 
		func_rs_pts_callback(TRUE);
		TRACE("\rSHOULD MAKE OUT_1 POS TRUE!");

        while( !done && get_rs_gwr() )
        {                        
            os_tsk_wait( 300 );
            
            if( get_ign_status() || !( -- retry_count ) )
            {
                done = TRUE ; 
            }
        }

		TRACE("\rSHOULD MAKE OUT_1 POS FALSE!");
		func_rs_pts_callback(FALSE);       
        os_tsk_wait( 300 );
    }

	set_rs_ign (TRUE);

	TRACE_FUNC("[%s - %s]\n\r",__FUNCTION__ , get_ign_status() ? "done" : "error");
}            
                    
/*--------------------------------------------------------------------------*/


void func_rs_str_on( void )
{
    Boolean done        = FALSE ;
    UInt8   retry_count = 10    ;
	
	TRACE_FUNC("[%s]\n\r",__FUNCTION__); 
        
    brake_rpm_bypass_on:
    {
        sense_rs_can_brake_skip_enter();
    
	   gpio_iface_brake(TRUE);     

        //os_tsk_wait( 200 );
    }
                          

	//if ( get_ign_status() /*&& (CAR_Gearbox==AUTOMATIC_GEARBOX)*/ ) // commented in vers. 1_14 - 26_09_2019	- custom only !!!!
	if ( get_ign_status() && (CAR_Gearbox==AUTOMATIC_GEARBOX) ) // commented in vers. 1_14 - 26_09_2019	- custom only !!!!
	//if ( get_ign_status() ) // vers. 1_12 - 26_09_2019

	/* removed in vers. 1_17 */
	
	{
		os_tsk_wait(500); // vers. 1_14 - 04_12_2019
		func_rs_pts_callback(TRUE);
	}

	set_rs_start(TRUE); 
}

/*--------------------------------------------------------------------------*/

void func_rs_str_off( void )
{            
    TRACE_FUNC("[%s]\n\r",__FUNCTION__);

	//if ( CAR_Gearbox ) 	// commented in vers. 1_12 - 26_09_2019	 - custom only !!!!

	/* removed in vers. 1_17 */
	
	//if ( CAR_Gearbox )
	{
		func_rs_pts_callback(FALSE);
	}
    
    os_tsk_wait( 200 );
    
	gpio_iface_brake(FALSE);

	os_tsk_wait( 300 );

	// vers. 1_06 - 19_12_2018	- extra time to ignore the brake
	//os_tsk_wait(5000);
	    
    sense_rs_can_brake_skip_leave(); 
	
	set_rs_start(FALSE); 	    
}   

/*--------------------------------------------------------------------------*/

void func_rs_ign_off( Boolean do_shutdown )
{
    Boolean done        = FALSE ;
    UInt8   retry_count = 10    ;
	
	TRACE_FUNC("[%s]\n\r",__FUNCTION__);


    if( do_shutdown /*&& get_ign_status()*/ )
	//if( do_shutdown && get_ign_status() )
    {
        TRACE_FUNC("[%s - do shutdown]\n\r",__FUNCTION__);
		
		func_rs_pts_callback(TRUE);
        
        while( !done )
        {                        
            os_tsk_wait( 300 );
            
            if( !get_ign_status() || !( -- retry_count ) )
            {
                done = TRUE ; 
            }
        }

		func_rs_pts_callback(FALSE);        
    }
	
	gpio_iface_rf_loop(FALSE);

	set_rs_ign (FALSE);	
}

/*--------------------------------------------------------------------------*/

void do_rap ( void )
{
	// ADI - 19.01.2017
	ignore_doors_status = TRUE;
	//OUT_RELAY2(TRUE);
	gpio_iface_rap(TRUE);
	os_tsk_wait(500);
	//OUT_RELAY2(FALSE);
	gpio_iface_rap(FALSE);
	
	os_tsk_wait(500);
	ignore_doors_status = FALSE; 
}
void func_rs_gwr_off( Boolean do_shutdown )
{
    Can_Msg rs_off_can_msg;
	UInt8 rs_off_counter;
	
	TRACE_FUNC("[%s]\n\r",__FUNCTION__);
	
	GLOBAL_LOCK_446[6] = GLOBAL_331[2];
	GLOBAL_LOCK_446[7] = GLOBAL_331[3];
	
	rs_off_can_msg.data._8_[0]=0x02;
	rs_off_can_msg.data._8_[1]= GLOBAL_LOCK_446[1];
	rs_off_can_msg.data._8_[2]=GLOBAL_LOCK_446[2];
	rs_off_can_msg.data._8_[3]=(GLOBAL_LOCK_446[3] += 0x0D);
	rs_off_can_msg.data._8_[4]=GLOBAL_LOCK_446[4];
	rs_off_can_msg.data._8_[5]=(GLOBAL_LOCK_446[5] += 0x0D);
	rs_off_can_msg.data._8_[6]=GLOBAL_LOCK_446[6];
	rs_off_can_msg.data._8_[7]=GLOBAL_LOCK_446[7];
	rs_off_can_msg.id = 0x446;
	rs_off_can_msg.length = 8;
	rs_off_can_msg.is_ext = 0;
	rs_off_can_msg.is_rtr = 0;

	for ( rs_off_counter = 0; rs_off_counter < 3; rs_off_counter++)
	{
		rs_off_can_msg.data._8_[3]=(GLOBAL_LOCK_446[3] += 0x0D);
		rs_off_can_msg.data._8_[5]=(GLOBAL_LOCK_446[5] += 0x0D);
		can_usr_tx( CAN_DEV_0 , &rs_off_can_msg , FALSE , 100 );
		os_tsk_wait(100);
	}

	GLOBAL_UNLOCK_446[3] = GLOBAL_LOCK_446[3];
	GLOBAL_UNLOCK_446[5] = GLOBAL_LOCK_446[5];

	set_rs_gwr (FALSE);

	//return;		
    
	func_rs_pts_callback(FALSE);

	gpio_iface_brake(FALSE);
	
	gpio_iface_rf_loop(FALSE);	
	
	do_rap();
	    
}

/*--------------------------------------------------------------------------*/

