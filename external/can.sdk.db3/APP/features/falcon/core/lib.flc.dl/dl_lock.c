/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: out_create.c 1046 2010-04-08 12:51:11Z florin.olariu $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dl_private.h"
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

void dl_lock(void)
{
    if(dl.relock_window) return ;
    
#if LIB_DL_DISABLE_ANALOG == 0
    dl_do_lock_out_reset  ();
    dl_do_unlock_out_reset();
#endif
    
    ATOMIC(dl.do_unlock_all=FALSE);
        
    if(!get_ign_status())
    {
        if(dl.evt)dl.evt(DL_EVT_ARM);                        
    }
        
    dl_do_lock();   

    ATOMIC(dl.lock_status = TRUE);
}

//--------------------------------------------------------------------------//

void dl_do_lock(void)
{
    if(dl.evt)dl.evt(DL_EVT_LOCK);
        
#if LIB_DL_DISABLE_ANALOG == 0
    if(dl.out)
    {
        timeout_q_remove((void *)dl_lock_ctrl_timeout );
    
        dl.lock_out = FALSE;
    
        timeout_q_insert((Timeout_Q_Func)dl_lock_ctrl_timeout,0,DL_OFFSET,DL_PULSE,2);
    }
#endif
}

//--------------------------------------------------------------------------//

void dl_do_lock_oem(void)
{
    if(get_ign_status() && get_rs_gwr() && (dl.oem_disable==FALSE) )
    {
        dl_do_lock();
    }        
}

//--------------------------------------------------------------------------//

#if LIB_DL_DISABLE_ANALOG == 0
void dl_do_lock_out_reset(void)
{
    timeout_q_remove((void *)dl_lock_ctrl_timeout );
    
    if(dl.lock_out)
    {
		dl_ctrl_out(DL_OUT_LOCK0);
        
        ATOMIC(dl.lock_out=FALSE);
    }
}  
#endif
    
//--------------------------------------------------------------------------//    

#if LIB_DL_DISABLE_ANALOG == 0
Timeout_Q_Status dl_lock_ctrl_timeout(void)
{
    ATOMIC(dl.lock_out = !dl.lock_out);
        
	dl_ctrl_out(dl.lock_out?DL_OUT_LOCK1:DL_OUT_LOCK0);
    
    return TIMEOUT_Q_RUNNING;
}
#endif

//--------------------------------------------------------------------------//

