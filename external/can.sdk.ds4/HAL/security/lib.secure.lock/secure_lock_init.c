/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: secure_lock_init.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "secure_lock_private.h"
#include "prog.h"
#include "cmd.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void secure_lock_door_open  (void * arg) ;
static void secure_lock_door_close (void * arg) ;
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
 
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void secure_lock_init(void)
{
	cmd_register :
    {
        static const Cmd_Array1 cmd_array[] = 
        {                
            {INPUT_LOCK1 | INPUT_ON     , secure_lock_lock		    },	 
	        {INPUT_LOCK2 | INPUT_ON     , secure_lock_lock		    },
	        {INPUT_LOCK3 | INPUT_ON     , secure_lock_lock		    },
	        {INPUT_OEM_LOCK 		    , secure_lock_lock		    },	
	        {INPUT_GWR_ON			    , secure_lock_gwr_on		},
	        {INPUT_GWR_OFF			    , secure_lock_gwr_off		},
	        {INPUT_RS_STARTER_ON	    , secure_lock_starter_on	},
	        {INPUT_RS_STARTER_OFF	    , secure_lock_starter_off	},
	        {INPUT_STARTER_ON		    , secure_lock_starter_on	},
	        {INPUT_STARTER_OFF		    , secure_lock_starter_off	},
            {INPUT_ALL_DOOR_STATUS_OPEN , secure_lock_door_open	    },
            {INPUT_ALL_DOOR_STATUS_CLOSE, secure_lock_door_close	},           
        };

        CMD_LUT_INSERT1(cmd_array,EXEC_MODE_NORMAL);
    }
	
	secure_lock.timeout         = timeout_f_create (SECURE_REARM_TIMEOUT_NORMAL, secure_lock_rearm) ;	
    secure_lock.starter_timeout = timeout_f_create (SECURE_LOCK_STARTER_TIMEOUT, secure_lock_starter_timeout) ;
    
    #if DBG_SECURE_LOCK == 1
      TRACE("\nSL Init Done") ;
    #endif
}
//--------------------------------------------------------------------------//
static void secure_lock_door_open  (void * arg) 
{
  secure_lock.door_open = TRUE ;

  #if DBG_SECURE_LOCK == 1
    TRACE("\nSL Door Open") ;
  #endif  
}
//--------------------------------------------------------------------------//
static void secure_lock_door_close (void * arg) 
{
  secure_lock.door_open = FALSE ;
  
  #if DBG_SECURE_LOCK == 1
    TRACE("\nSL Door Close") ;
  #endif  
}
