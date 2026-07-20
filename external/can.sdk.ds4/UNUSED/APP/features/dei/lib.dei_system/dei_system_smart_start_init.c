/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_system_smart_start_init.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_system_private.h"
#include "rf.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void dei_system_smart_start_lock			 ( void );
static void dei_system_smart_start_unlock		 ( void );
static void dei_system_smart_start_valet_on		 ( void );
static void dei_system_smart_start_valet_off	 ( void );
static void dei_system_smart_start_engine_started( void );
static void dei_system_smart_start_engine_running( void );
static void dei_system_smart_start_engine_stopped( void );
static void dei_system_smart_start_engine_runtime( void );
static void dei_system_smart_start_panic		 ( void );
static void dei_system_smart_start_trigger_on	 ( void );
static void dei_system_smart_start_trigger_off	 ( void );
static void dei_system_smart_start_warn_away	 ( void );
static void dei_system_smart_start_light_on		 ( void );
static void dei_system_smart_start_light_off	 ( void );

static void dei_system_smart_start_door_open     ( void );
static void dei_system_smart_start_door_close    ( void );
static void dei_system_smart_start_trunk_open    ( void );
static void dei_system_smart_start_trunk_close   ( void );
static void dei_system_smart_start_hood_open     ( void );
static void dei_system_smart_start_hood_close    ( void );
static void dei_system_smart_start_ignition_on   ( void );
static void dei_system_smart_start_ignition_off  ( void );

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

/*--------------------------------------------------------------------------*/

void dei_system_smart_start_init( void )
{			
	//---------------------------------------------------------------------------------------
	//--- Register the events that are comming from Notify server
	//---------------------------------------------------------------------------------------

	//notify_register:
    {
        static const Notify_Array array[] =
        {
            {NOTIFY_LOCK                    , (Notify_Func)dei_system_smart_start_lock          },
        	{NOTIFY_UNLOCK                  , (Notify_Func)dei_system_smart_start_unlock        },
        	{NOTIFY_VALET_ON                , (Notify_Func)dei_system_smart_start_valet_on      },
        	{NOTIFY_VALET_OFF               , (Notify_Func)dei_system_smart_start_valet_off     },
            {NOTIFY_RS_START                , (Notify_Func)dei_system_smart_start_engine_started},
        	{NOTIFY_RS_SUCCESSFULLY_STARTED , (Notify_Func)dei_system_smart_start_engine_running},
            {NOTIFY_RUNTIME_REMAINING       , (Notify_Func)dei_system_smart_start_engine_runtime},
        	{NOTIFY_RS_STOP                 , (Notify_Func)dei_system_smart_start_engine_stopped},
        	{NOTIFY_PANIC                   , (Notify_Func)dei_system_smart_start_panic         },
        	{NOTIFY_WARNING                 , (Notify_Func)dei_system_smart_start_warn_away     },
        };

        NOTIFY_ADD(array);
    }
	
	//---------------------------------------------------------------------------------------
	//--- Register the events that are comming from Cmd server
	//---------------------------------------------------------------------------------------
	//cmd_register:
    {
        static const Cmd_Array1 cmd_array[] = 
        {            
            {INPUT_TRIGGER_ON           , (Exec_Func )dei_system_smart_start_trigger_on  },
        	{INPUT_TRIGGER_OFF          , (Exec_Func )dei_system_smart_start_trigger_off },
        	{INPUT_PARKING_ON           , (Exec_Func )dei_system_smart_start_light_on    },
        	{INPUT_PARKING_OFF          , (Exec_Func )dei_system_smart_start_light_off   },
            {INPUT_ALL_DOOR_STATUS_OPEN , (Exec_Func )dei_system_smart_start_door_open   },
	        {INPUT_ALL_DOOR_STATUS_CLOSE, (Exec_Func )dei_system_smart_start_door_close  },
            {INPUT_TRUNK_STATUS_OPEN    , (Exec_Func )dei_system_smart_start_trunk_open  },
	        {INPUT_TRUNK_STATUS_CLOSE   , (Exec_Func )dei_system_smart_start_trunk_close },
            {INPUT_HOOD_STATUS_OPEN     , (Exec_Func )dei_system_smart_start_hood_open   },
	        {INPUT_HOOD_STATUS_CLOSE    , (Exec_Func )dei_system_smart_start_hood_close  },
            {INPUT_IGNITION_ON          , (Exec_Func )dei_system_smart_start_ignition_on },
	        {INPUT_IGNITION_OFF         , (Exec_Func )dei_system_smart_start_ignition_off}
        };

        CMD_LUT_INSERT1(cmd_array , EXEC_MODE_NORMAL);
    }
}    

/*--------------------------------------------------------------------------*/

void dei_system_smart_start_lock( void )
{
	rf_icon_clr(ICON_RS_PANIC ,FALSE);
    rf_icon_clr(ICON_PANIC    ,FALSE);
    
    if( get_ss_valet() )
    {
        rf_icon_set(ICON_LOCK     , FALSE);
        rf_icon_clr(ICON_RS_ARMED , TRUE);                
    }
    else
    {
        rf_icon_set(ICON_LOCK     , FALSE);
        rf_icon_set(ICON_RS_ARMED , TRUE);                
    }                    
}

/*--------------------------------------------------------------------------*/

void dei_system_smart_start_unlock( void )
{
	rf_icon_clr(ICON_RS_PANIC ,FALSE);
    rf_icon_clr(ICON_PANIC    ,FALSE);
     
	rf_icon_clr(ICON_LOCK     ,FALSE);
    rf_icon_clr(ICON_RS_ARMED ,TRUE);
}

/*--------------------------------------------------------------------------*/

void dei_system_smart_start_valet_on( void )
{
	rf_icon_set(ICON_VALET,TRUE);
}

/*--------------------------------------------------------------------------*/

void dei_system_smart_start_valet_off( void )
{
	rf_icon_clr(ICON_VALET,TRUE);
}

/*--------------------------------------------------------------------------*/

void dei_system_smart_start_engine_started( void )
{
	rf_icon_clr  (ICON_STARTED  , FALSE) ;     
    rf_icon_set  (ICON_TIME     , FALSE) ;
      
    rf_sound_set ((Rf_Sound_Def)get_rs_runtime_programmed(), TRUE);      
}	

/*--------------------------------------------------------------------------*/

void dei_system_smart_start_engine_running( void )
{
	 rf_icon_clr  (ICON_STARTED , FALSE)  ;
  
     rf_sound_set ((Rf_Sound_Def)get_rs_runtime_programmed(), FALSE)  ;

     rf_icon_set  (ICON_TIME    , FALSE)  ;
     rf_icon_set  (ICON_SUCCESS , TRUE)   ; 
}	

/*--------------------------------------------------------------------------*/

void dei_system_smart_start_engine_runtime( void )
{
     rf_sound_set ((Rf_Sound_Def)get_rs_runtime_remaining(), TRUE);    
}

/*--------------------------------------------------------------------------*/

void dei_system_smart_start_engine_stopped( void )
{
	rf_sound_set ((Rf_Sound_Def)0 , FALSE);  
    
    if(get_gearbox_type() == GEARBOX_MANUAL)
	{
		rf_icon_clr(ICON_STARTED , FALSE) ;  
        rf_icon_clr(ICON_SUCCESS , FALSE);

		if( get_rs_mts() == TRUE )
		{
			rf_icon_clr(ICON_NOT_READY,TRUE);
		}
		else
		{
			rf_icon_set(ICON_NOT_READY,TRUE);
		}
	}
	else
	{
		rf_icon_clr(ICON_SUCCESS  , FALSE);
        rf_icon_clr(ICON_STARTED  , FALSE) ;  
		rf_icon_clr(ICON_NOT_READY, TRUE );
	}		
}	

/*--------------------------------------------------------------------------*/

void dei_system_smart_start_panic( void )
{
	rf_icon_set(ICON_RS_PANIC ,TRUE);
}

/*--------------------------------------------------------------------------*/

void dei_system_smart_start_trigger_on( void )
{
	rf_icon_clr(ICON_TRUNK    ,FALSE);
	rf_icon_clr(ICON_DOOR     ,FALSE);
	rf_icon_clr(ICON_HOOD     ,FALSE);
	rf_icon_clr(ICON_IGNITION ,FALSE);
	rf_icon_clr(ICON_SHOCK    ,FALSE);

	if( get_ss_sensor_trigger  (1) )rf_icon_set(ICON_SHOCK   ,FALSE);
	if( get_ss_door_trigger    () )rf_icon_set(ICON_DOOR    ,FALSE);
	if( get_ss_hood_trigger    () )rf_icon_set(ICON_HOOD    ,FALSE);
	if( get_ss_trunk_trigger   () )rf_icon_set(ICON_TRUNK   ,FALSE);
	if( get_ss_ign_trigger     () )rf_icon_set(ICON_IGNITION,FALSE);
		
	rf_icon_set(ICON_RS_HORN , FALSE);
    rf_icon_set(ICON_HORN    , TRUE);
}

/*--------------------------------------------------------------------------*/

void dei_system_smart_start_trigger_off( void )
{
	rf_icon_clr(ICON_TRUNK    ,FALSE);
	rf_icon_clr(ICON_DOOR     ,FALSE);
	rf_icon_clr(ICON_HOOD     ,FALSE);
	rf_icon_clr(ICON_IGNITION ,FALSE);
	rf_icon_clr(ICON_SHOCK    ,FALSE);
	
	if( get_door_status ())rf_icon_set(ICON_DOOR    ,FALSE);
	if( get_hood_status ())rf_icon_set(ICON_HOOD    ,FALSE);
	if( get_trunk_status())rf_icon_set(ICON_TRUNK   ,FALSE);
	if( get_ign_status  ())rf_icon_set(ICON_IGNITION,FALSE);
	
    
    rf_icon_clr(ICON_RS_HORN  ,FALSE );
    rf_icon_clr(ICON_HORN     ,TRUE );
}

/*--------------------------------------------------------------------------*/

void dei_system_smart_start_warn_away( void )
{
	rf_icon_set(ICON_WARN,FALSE );
	rf_icon_clr(ICON_WARN,TRUE);
}

/*--------------------------------------------------------------------------*/

void dei_system_smart_start_light_on( void )
{
	rf_icon_set(ICON_PARKING,FALSE);
}

/*--------------------------------------------------------------------------*/

void dei_system_smart_start_light_off( void )
{
	rf_icon_clr(ICON_PARKING, FALSE);
}

/*--------------------------------------------------------------------------*/

void dei_system_smart_start_door_open( void )
{
    rf_icon_set(ICON_DOOR , TRUE );
}    

/*--------------------------------------------------------------------------*/

void dei_system_smart_start_door_close( void )
{
    rf_icon_clr(ICON_DOOR , TRUE );
}

/*--------------------------------------------------------------------------*/

void dei_system_smart_start_trunk_open( void )
{
    rf_icon_set(ICON_TRUNK , TRUE );
}    

/*--------------------------------------------------------------------------*/

void dei_system_smart_start_trunk_close( void )
{
    rf_icon_clr(ICON_TRUNK , TRUE );
}

/*--------------------------------------------------------------------------*/

void dei_system_smart_start_hood_open( void )
{
    rf_icon_set(ICON_HOOD , TRUE );
}    

/*--------------------------------------------------------------------------*/

void dei_system_smart_start_hood_close( void )
{
    rf_icon_clr(ICON_HOOD , TRUE );
}

/*--------------------------------------------------------------------------*/

void dei_system_smart_start_ignition_on( void )
{
    rf_icon_set(ICON_IGNITION , TRUE );
}    

/*--------------------------------------------------------------------------*/

void dei_system_smart_start_ignition_off( void )
{
    rf_icon_clr(ICON_IGNITION , TRUE );
}

/*--------------------------------------------------------------------------*/
