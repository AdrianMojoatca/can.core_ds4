/*==========================================================================*/
// $Id: fiq_register.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "fiq_private.h"
#include "isr.h"

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

void fiq_register (Fiq_Func fiq_func)
{
  ATOMIC_ALL
    (
 	  init_fiq:
	    {
		   static Boolean do_init = TRUE ;

		   if(do_init)
		     {
			    do_init = FALSE ;

			   	fiq_init() ;
			 }   
		}
 
      add_to_list :
        {
					Fiq_Entry * fiq_entry = NULL;
					
					fiq_entry = fiq_entry_request () ;
    
          fiq_entry -> fiq_func = fiq_func ;
          fiq_entry -> next     = fiq_list ;
    
          fiq_list = fiq_entry ;
        }
    )
}

//--------------------------------------------------------------------------//
#ifdef USE_HI_PRI_FIQ // must define USE_HI_PRI_FIQ in config_firmware when used
#warning "fiq_register_hi_pri should be removed, use fiq_unregister to accelerate fiq"
Boolean fiq_register_hi_pri (Hi_Pri_Func func)
{
  Boolean status  = FALSE ;

  ATOMIC_ALL
    (
      if (hi_pri_vec == void_sentinel)
        {
          hi_pri_vec = func ;

          status  = TRUE ;
        }
    )  


  return status ;

}
#endif
//--------------------------------------------------------------------------//


