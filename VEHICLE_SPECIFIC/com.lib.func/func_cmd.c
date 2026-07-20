/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: niss_bp_init.c 1282 2010-07-13 19:33:32Z florin.olariu $
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
static void func_execute_gas_cap( void );
static void func_execute_aux    ( Func_Aux_Type aux_type );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
extern Boolean unlocked_status;
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

/*--------------------------------------------------------------------------*/

void func_cmd_lock( void )
{    
    process_launch( func_prc[ get_ign_status() ? FUNC_LOCK : FUNC_ARM ] , 0 );
    
    if( (func_cfg->comfort==COMFORT_ON) && ( !get_ign_status() || get_rs_gwr() ) )
    {
        //process_launch( func_prc[ FUNC_WRU ] , 0 ) ;
    }    
    if( !get_ign_status() )
    {
        process_launch( func_prc[ FUNC_LIGHT_DISABLE ] , 0 );        
    }

	TRACE_FUNC("[%s]\n\r",__FUNCTION__);
}
                   
/*--------------------------------------------------------------------------*/

void func_cmd_unlock( void )
{            
	timeout_q_remove( (void*)func_gwr_off_relock );


    if( !get_ign_status() )
    {

		process_launch( func_prc[ FUNC_DISARM ] , 0 );        
    }            
        
    else process_launch( func_prc[ FUNC_UNLOCK ] , 0 );                  
    
	TRACE_FUNC("[%s]\n\r",__FUNCTION__);                    
}
                   
/*--------------------------------------------------------------------------*/

void func_cmd_trunk( void )
{    
    timeout_q_remove( (void*)func_gwr_off_relock );
	
	/*if( !get_ign_status() )        
    {
        process_launch( func_prc[ FUNC_DISARM ] , 0 );    
    }*/
            
    process_launch( func_prc[ FUNC_TRUNK_RELEASE ] , 0 ); 
	
	TRACE_FUNC("[%s]\n\r",__FUNCTION__);       
}
                   
/*--------------------------------------------------------------------------*/

void func_cmd_gwr_off( void )
{            
    timeout_q_remove( (void*)func_gwr_off_relock );

	timeout_q_insert( (Timeout_Q_Func)func_gwr_off_relock , NULL , FUNC_GWR_OFF_RELOCK_TIMEOUT , 1 , 1 ); 

	TRACE_FUNC("[%s]\n\r",__FUNCTION__);
}
                   
/*--------------------------------------------------------------------------*/

Timeout_Q_Status func_gwr_off_relock( void )
{
	if( get_ss_lock() && !get_ign_status() )
	{
		process_launch( func_prc[ FUNC_ARM ] , 0 );        
	}

	TRACE_FUNC("[%s]\n\r",__FUNCTION__);
	
	return TIMEOUT_Q_COMPLETE ; 
}		

/*--------------------------------------------------------------------------*/

void func_cmd_oem_lock( void )
{        
    timeout_q_remove( (void*)func_gwr_off_relock );
	
	process_launch( func_prc[ FUNC_OEM_LOCK ] , 0 );              
    
    if((func_cfg->comfort==COMFORT_ON) && ( !get_ign_status() || get_rs_gwr()))
    {
        //process_launch( func_prc[ FUNC_WRU ] , 0 );
    }
    
    process_launch( func_prc[ FUNC_LIGHT_DISABLE ] , 0 );    
	
	TRACE_FUNC("[%s]\n\r",__FUNCTION__);                
}
                   
/*--------------------------------------------------------------------------*/

void func_cmd_oem_unlock( void )
{    
    timeout_q_remove( (void*)func_gwr_off_relock );
	
	process_launch( func_prc[ FUNC_LIGHT_DISABLE ] , 0 );        

    process_launch( func_prc[ FUNC_OEM_UNLOCK ] , 0 ); 
	
	TRACE_FUNC("[%s]\n\r",__FUNCTION__);       
}
                   
/*--------------------------------------------------------------------------*/

void func_cmd_oem_trunk( void )
{    
	timeout_q_remove( (void*)func_gwr_off_relock );

    process_launch( func_prc[ FUNC_LIGHT_DISABLE ] , 0 );        

    //process_launch( func_prc[ FUNC_OEM_TRUNK ] , 0 );   
	process_launch( func_prc[ FUNC_TRUNK_RELEASE ] , 0 ); // ADI
	
	TRACE_FUNC("[%s]\n\r",__FUNCTION__);     
}
                   
/*--------------------------------------------------------------------------*/

void func_cmd_light_on( void *arg )
{    
    ATOMIC( func_light_done = FALSE );
        
    process_launch( func_prc[ FUNC_LIGHT ] , 0 );    
	//func_light_start();
}
                   
/*--------------------------------------------------------------------------*/

void func_cmd_light_off( void *arg )
{    
    ATOMIC( func_light_done = TRUE );                    
	//func_light_stop();
}

/*--------------------------------------------------------------------------*/

void func_cmd_aux1( void )
{
    func_execute_aux( func_cfg->aux1 );    
}

/*--------------------------------------------------------------------------*/

void func_cmd_aux2( void )
{
    func_execute_aux( func_cfg->aux2 );    
}

/*--------------------------------------------------------------------------*/

void func_cmd_aux3( void )
{
    func_execute_aux( func_cfg->aux3 );    
}

/*--------------------------------------------------------------------------*/

void func_execute_aux( Func_Aux_Type aux_type )
{
	switch( aux_type )
	{
		/*case AUX_WRU       : process_launch( func_prc[ FUNC_WRU        ] , 0 ); break;
        case AUX_WRD_FRONT : process_launch( func_prc[ FUNC_WRD_FRONT  ] , 0 ); break;
        case AUX_WRD_REAR  : process_launch( func_prc[ FUNC_WRD_REAR   ] , 0 ); break;
        
        case AUX_GAS_CAP   : func_execute_gas_cap(); break;
        */
        
        default : break ; 
	}
}

/*--------------------------------------------------------------------------*/	

void func_execute_gas_cap( void )
{
	if( !get_ign_status() )        
	{
        process_launch( func_prc[ FUNC_DISARM ] , 0 );    
    }
            
    //process_launch( func_prc[ FUNC_GAS_CAP    ] , 0 );                
}

/*--------------------------------------------------------------------------*/


