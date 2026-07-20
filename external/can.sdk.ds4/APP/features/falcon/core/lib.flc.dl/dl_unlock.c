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

void dl_unlock_all(void)
{
	ATOMIC( dl.do_unlock_all = TRUE );
	
	dl_unlock();
}

void dl_unlock(void)
{
#if LIB_DL_DISABLE_ANALOG == 0
	dl_do_lock_out_reset  ();
    dl_do_unlock_out_reset();
#endif
        
    if(dl.unlock_dd_enable==FALSE)
    {
        ATOMIC(dl.do_unlock_all=TRUE);
    }
    
    timeout_q_remove((void *)dl_unlock_all_timeout);
    
    timeout_q_insert((Timeout_Q_Func)dl_unlock_all_timeout,NULL,DL_UNLOCK_ALL_TIMEOUT,1,1);
    
    ATOMIC(dl.do_unlock_after_gwr=(dl.unlock_after_disarm_enable&&dl.lock_status&&!get_ign_status()));
    
    if(dl.lock_status && !get_ign_status())
    {
        if(dl.evt)dl.evt(DL_EVT_DISARM); 
    }
    
    if( (dl.do_unlock_after_gwr==FALSE))
    {    
        if(dl.evt)dl.evt(dl.do_unlock_all?DL_EVT_UNLOCK_ALL:DL_EVT_UNLOCK_DD);         
        
        dl_do_unlock();
    }
    else
    {            
        timeout_q_insert((Timeout_Q_Func)dl_unlock_after_disarm_fails,NULL,DL_DISARM_FAILES_TIMEOUT,1,1);
    }

    ATOMIC(dl.lock_status = FALSE);    
}

//--------------------------------------------------------------------------//

#if LIB_DL_DISABLE_ANALOG == 0
void dl_do_unlock_out_reset(void)
{
    timeout_q_remove((void *)dl_unlock_after_disarm_fails);    
    timeout_q_remove((void *)dl_unlock_ctrl_timeout      );
    
    if(dl.unlock_out)
    {
		dl_ctrl_out(DL_OUT_UNLOCK0);
        
        ATOMIC(dl.unlock_out=FALSE);
    }
}  
#endif

//--------------------------------------------------------------------------//

void dl_do_unlock_after_dis(void)
{
    if(dl.do_unlock_after_gwr==TRUE)
    {
        if(dl.evt)dl.evt(dl.do_unlock_all?DL_EVT_UNLOCK_ALL:DL_EVT_UNLOCK_DD); 
        
        dl_do_unlock();
    }
}

//--------------------------------------------------------------------------//

void dl_do_unlock( void )
{                    
#if LIB_DL_DISABLE_ANALOG == 0
    if(dl.out)
    {
        timeout_q_remove((void *)dl_unlock_ctrl_timeout );
    
        ATOMIC(dl.unlock_out = FALSE);
    
        timeout_q_insert((Timeout_Q_Func)dl_unlock_ctrl_timeout,0,DL_OFFSET,DL_PULSE,dl.do_unlock_all?4:2);
    }
#endif
    
    ATOMIC(dl.do_unlock_all=!dl.do_unlock_all);
}


//--------------------------------------------------------------------------//

void dl_do_unlock_oem(void)
{
    if(get_ign_status() && get_rs_gwr() && (dl.oem_disable==FALSE) )
    {
        ATOMIC(dl.do_unlock_all=TRUE);
        
        if(dl.evt)dl.evt(dl.do_unlock_all?DL_EVT_UNLOCK_ALL:DL_EVT_UNLOCK_DD); 
        
        dl_do_unlock();
    }
}

//--------------------------------------------------------------------------//

#if LIB_DL_DISABLE_ANALOG == 0
Timeout_Q_Status dl_unlock_ctrl_timeout(void)
{
    ATOMIC(dl.unlock_out = !dl.unlock_out);
        
	dl_ctrl_out(dl.unlock_out?DL_OUT_UNLOCK1:DL_OUT_UNLOCK0);
    
    return TIMEOUT_Q_RUNNING;
}
#endif

Timeout_Q_Status dl_unlock_after_disarm_fails(void)
{    
    TRACE("Disarm Failes \n\r");
    
    dl_do_unlock();
    
    return TIMEOUT_Q_COMPLETE;
}

Timeout_Q_Status dl_unlock_all_timeout(void)
{
    if(dl.unlock_dd_enable==TRUE)
    {
        ATOMIC(dl.do_unlock_all=FALSE);  
    }
    
    ATOMIC(dl.do_unlock_after_gwr=FALSE);    

    TRACE("TIMEOUT_UNLOCK_ALL expired\n\r");
    
    return TIMEOUT_Q_COMPLETE;
}

//--------------------------------------------------------------------------//
