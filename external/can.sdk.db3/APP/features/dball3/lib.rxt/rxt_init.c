/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: rxt_init.c 33258 2015-08-19 12:46:35Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "rxt_private.h"
#include "cmd.h"

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

Boolean rxt_init (Rxt_Func * start_func, Rxt_Func * stop_func)
{
	Boolean init_status = rxt_init_nvfs() ;

	ATOMIC
	(	 	
		rxt.enable 				= init_status ;
		rxt.stopping			= FALSE ;
		rxt.oem_start			= FALSE ;
		rxt.start_func 			= start_func ;
		rxt.stop_func 			= stop_func ;
		rxt.default_runtime 	= RXT_RUNTIME ;
		rxt.attempt             = 0 ;
		rxt.state 				= RXT_STATE_STOP ;
		rxt.count 				= 0 ;
		rxt.ext_count		    = 0 ;
		rxt.ready.byte 			= 0xFF ;
	)
    /*cmd_init :*/
    {            
        static const Cmd_Array1 cmd_array[] = 
          {
            {INPUT_GWR                  |INPUT_ON   , rxt_gwr_on },
            {INPUT_START                |INPUT_ON   , rxt_toggle },
            {INPUT_STOP                 |INPUT_ON   , rxt_stop   },
            {INPUT_EXTEND_RUNTIME       |INPUT_ON   , rxt_extend_runtime },
            {INPUT_REQ_RUNTIME          |INPUT_ON   , rxt_req_runtime }
          };
        CMD_LUT_INSERT1(cmd_array,EXEC_MODE_NORMAL);
    }

    if (RXT_FEAT_3X_LOCK_START_ENABLED == 1)
      {
        static const Cmd_Array1 cmd_array[] = 
        {
          { INPUT_OEM_LOCK   , rxt_oem_lock         } ,
          { INPUT_OEM_UNLOCK , rxt_oem_timeout_stop } ,
          { INPUT_OEM_TRUNK  , rxt_oem_timeout_stop } ,
          { INPUT_OEM_AUX1   , rxt_oem_timeout_stop } ,
          { INPUT_OEM_AUX2   , rxt_oem_timeout_stop } 
        };

        CMD_LUT_INSERT1(cmd_array,EXEC_MODE_NORMAL);
        
        rxt.lock_start_timeout = timeout_create ( RXT_FEAT_3X_LOCK_START_DEF_VALUE * 1000 );
      }

	rxt.timeout = timeout_f_create (RXT_START_TIMEOUT   , rxt_timeout) ;
	
	return init_status ;	
}

//---------------------------------------------------------------------------//
