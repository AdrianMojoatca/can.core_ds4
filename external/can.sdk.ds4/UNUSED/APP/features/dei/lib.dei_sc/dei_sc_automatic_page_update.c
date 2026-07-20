/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_sc_automatic_page_update.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_sc_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

typedef struct s_Dei_Sc_Apu
{
    UInt8 count;
    Vrtc  vrtc ;    
}Dei_Sc_Apu;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void dei_sc_automatic_page_update_execute( void );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

static Dei_Sc_Apu   dei_sc_apu_vars;

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void dei_sc_automatic_page_update_init( void )
{
	dei_sc_apu_vars.count = 0 ; 

	dei_sc_apu_vars.vrtc = vrtc_f_create( (Exec_Func)dei_sc_automatic_page_update_execute ) ; 

    cmd_register:
    {
        static const Cmd_Array1 cmd_array[] = 
        {
        	{ INPUT_OEM_LOCK        , (Exec_Func)dei_sc_automatic_page_update }, 
        	{ INPUT_OEM_UNLOCK      , (Exec_Func)dei_sc_automatic_page_update }, 
            { INPUT_OEM_ARM         , (Exec_Func)dei_sc_automatic_page_update }, 
        	{ INPUT_OEM_DISARM      , (Exec_Func)dei_sc_automatic_page_update }, 
        	{ INPUT_RF_LOCK         , (Exec_Func)dei_sc_automatic_page_update }, 
        	{ INPUT_RF_UNLOCK       , (Exec_Func)dei_sc_automatic_page_update }, 
        	{ INPUT_RF_VALET_TOOGLE , (Exec_Func)dei_sc_automatic_page_update }, 
        	{ INPUT_GWR_OFF		    , (Exec_Func)dei_sc_automatic_page_update },
        	{ INPUT_TRIGGER_OFF     , (Exec_Func)dei_sc_automatic_page_update },
        };

        CMD_LUT_INSERT1(cmd_array,EXEC_MODE_NORMAL);
    }
}

//--------------------------------------------------------------------------//

void dei_sc_automatic_page_update( void )
{
	ATOMIC( dei_sc_apu_vars.count = 0 ); 

	vrtc_stop( dei_sc_apu_vars.vrtc ) ; 

	vrtc_update( dei_sc_apu_vars.vrtc , DEI_SC_VRTC_TIMEOUT_FIRST_RF_UPDATE ) ; 

	vrtc_start( dei_sc_apu_vars.vrtc ) ; 
}

//--------------------------------------------------------------------------//

void dei_sc_automatic_page_update_execute( void )
{
	UInt8 count ; 

	ATOMIC( count = dei_sc_apu_vars.count );
	
	if( ++count != DEI_SC_COUNT_RF_UPDATE )
	{		
		dei_sc_icon_update_page() ; 

		vrtc_update( dei_sc_apu_vars.vrtc , DEI_SC_VRTC_TIMEOUT_RF_UPDATE ) ; 

		if( count != DEI_SC_COUNT_RF_UPDATE )
		{
			vrtc_start( dei_sc_apu_vars.vrtc ) ; 	
			
			TRACE_DEI_SC("\n\r[DEI_SC_AUTOMATIC_PAGE_UPDATE]\n\r");	
		}
	}

	ATOMIC( dei_sc_apu_vars.count = count );
}

//--------------------------------------------------------------------------//

