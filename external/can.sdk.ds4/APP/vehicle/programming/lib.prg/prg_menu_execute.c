/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: prg_menu_execute.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "prg_private.h"
#ifdef PLATFORM_933
#include "cmd.h"
#endif
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
#ifdef DEV_LPC_236x
void prg_menu_execute( void )
{
	#define EVT		PRG_EVT_DOOR_CLOSED | PRG_EVT_IGN_OFF | PRG_EVT_PUSH_EXT_PRESS

	UInt16 		evt                 ;
	Boolean 	do_not_exit = FALSE ; 

	wake_enter_flagged(WAKE_PRG_MENU);

	TRACE_DEI_PRG("\n\r[PRM_MENU_ENABLE]\n\r");
	
	ATOMIC
	( 
		prg_menu = 0 	 ;
		prg_exit = FALSE ;
	);
	
	for(;; )
	{	
		evt = os_evt_wait( prg_evt ,  EVT  , EVT  , 15000 );
					
		if(!! (evt & PRG_EVT_PUSH_EXT_PRESS)  )
		{
			TRACE_DEI_PRG("\n\r[PRM_MENU_ENTER]\n\r");
      
			switch( prg_menu )
			{
				case 1 : do_not_exit = prg_menu_remote_programming();break;
				case 2 : do_not_exit = prg_menu_remote_delete()     ;break;
				case 3 : do_not_exit = prg_menu_zap()               ;break;
				case 4 : do_not_exit = prg_menu_tacho_delete()      ;break;
				
				default:
				{
					TRACE_DEI_PRG("\n\r[PRG_MENU_SELECTION_ERROR]\n\r") ; 
					
					prg_exit = TRUE;
					do_not_exit = FALSE;    //Force exit
				}
				break;
			}
		
			if( do_not_exit )
			{
				TRACE_DEI_PRG("\n\r[PRG_MENU_WAIT_NEW_SELECT]\n\r");
				continue;
			}
		}
		
		if( prg_exit )
		{
			reg_execute( prg_reg , PRG_FUNC_EXIT , NULL ) ; 

			prg_exit = FALSE ; 
		}		
				
	}

	TRACE_DEI_PRG("\n\r[PRM_MENU_EXIT]\n\r");

	wake_leave_flagged(WAKE_PRG_MENU);

	#undef EVT
}

#else
//--------------------------------------------------------------------------//

void	wait_menu_enter_event(void)
{
    for(;;)
    {  
      TRACE_DEI_PRG("\n\r[PRM_MENU_ENTER]\n\r");

      if(prg_menu && prg_menu <= 4)
        dei_system_prg_mode_set(TRUE);
    
      switch( prg_menu )
      {
        case 1 : 
          if(!prg_menu_remote_programming())
            return;
        break;
        case 2 :
          if(!prg_menu_remote_delete())
            return;
          break;
        case 3 :
          if(!prg_menu_zap()) 
            return;
          break;
        case 4 :
          if(!prg_menu_tacho_delete()) 
            return;
          break;
        
        default:
          TRACE_DEI_PRG("\n\r[PRG_MENU_SELECTION_ERROR]\n\r") ; 
          prg_exit = TRUE;
          return;
      }
    
      TRACE_DEI_PRG("\n\r[PRG_MENU_WAIT_NEW_SELECT]\n\r");
    }	
}    

void prg_menu_execute( void )
{
#ifdef PLATFORM_933  
	#define EVT		PRG_EVT_DOOR_CLOSED | PRG_EVT_IGN_OFF | PRG_EVT_PUSH_EXT_PRESS | PRG_EVT_BLE_PRG_ENTER | PRG_EVT_BLE_PRG_EXIT
#else  
	#define EVT		PRG_EVT_DOOR_CLOSED | PRG_EVT_IGN_OFF | PRG_EVT_PUSH_EXT_PRESS 
#endif  
	UInt16 		evt                 ;

	wake_enter_flagged(WAKE_PRG_MENU);

	TRACE_DEI_PRG("\n\r[PRG_MENU_ENABLE]\n\r");

    prg_exit = FALSE ;
    prg_menu = 0 	 ;
	
		evt = os_evt_wait( prg_evt ,  EVT  , EVT  , 15000 );
  
		if(
#ifdef PLATFORM_933        
      (!! (evt & (PRG_EVT_PUSH_EXT_PRESS | PRG_EVT_BLE_PRG_ENTER | PRG_EVT_BLE_PRG_EXIT)))     
#else    
      (!! (evt & PRG_EVT_PUSH_EXT_PRESS ))
#endif    
    )
		{
#ifdef PLATFORM_933      
			if((evt & PRG_EVT_BLE_PRG_ENTER))
			{
				prg_menu = prg_menu_get();
			}
#endif      
    
       wait_menu_enter_event();
      
			 //if( prg_exit )
			 {
					reg_execute( prg_reg , PRG_FUNC_EXIT , NULL ) ; 

					prg_exit = FALSE ; 
			 }		
		}

	TRACE_DEI_PRG("\n\r[PRG_MENU_EXIT]\n\r");

	wake_leave_flagged(WAKE_PRG_MENU);

	#undef EVT
}

#endif
