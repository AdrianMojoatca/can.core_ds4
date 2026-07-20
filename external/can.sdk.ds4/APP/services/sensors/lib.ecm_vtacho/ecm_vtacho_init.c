/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: ecm_vtacho_init.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "ecm_vtacho_private.h"
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

void ecm_vtacho_init( Ecm_Vtacho_Ref ref , UInt8 fine_tune )
{
	ecm_vtacho_vars.ref       = ref ; 
	ecm_vtacho_vars.fine_tune = fine_tune ; 

	ecm_vtacho_vars.state   = NULL ;
	ecm_vtacho_vars.res     = os_res_create();
    ecm_vtacho_vars.running = FALSE ;

	cmd_register:
    {
        static Cmd_Array1 cmd_array[] = 
        {
            { INPUT_GWR_ON         , (Exec_Func)ecm_vtacho_rs_gwr   },
        	{ INPUT_RS_IGNITION_ON , (Exec_Func)ecm_vtacho_rs_ign   },
        	{ INPUT_RS_STARTER_ON  , (Exec_Func)ecm_vtacho_rs_start },
            { INPUT_IGN_STATUS_ON  , (Exec_Func)ecm_vtacho_vign_on  },
        };

        CMD_LUT_INSERT1(cmd_array , EXEC_MODE_NORMAL);
    }

	voltage_init();

	TRACE_DEI_ECM("\n\r[ECM_VTACHO_INIT REF : %d  FINE_TUNE : %d ]\n\r" , ref , fine_tune );
#if DEBUG_ECM_TACHO == 1
  if(ref > 0.1)
      ecm_vtacho_trace(ref);
#endif
}

//--------------------------------------------------------------------------//
void ecm_vtacho_trace( double volt_val )
{
#if DEBUG_ECM_TACHO == 1
	//TRACE("\n\r[VOLTAGE %.02f]\n\r" , get_voltage() );
	#define FRACTION_MASK  0xFF
	UInt16 volt_tmp;
	volt_tmp = (volt_val*256.0)/1;   //convert to integer
	
#ifdef PLATFORM_933	
	if(dbgclitx_room() < 200)
    return;
#endif

	TRACE("\n\r VOLTAGE[V]: ");
	TRACE("%d.", (volt_tmp>>8));
	volt_tmp = (volt_tmp & FRACTION_MASK) * 10;
	TRACE("%c", (volt_tmp>>8)+'0');
	volt_tmp = (volt_tmp & FRACTION_MASK) * 10;
	TRACE("%c\n\r", (volt_tmp>>8)+'0');
#endif
}
