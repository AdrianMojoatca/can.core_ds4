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

void dl_tr_trigger_oem(void)
{
#if LIB_DL_DISABLE_ANALOG == 0
    dl_do_trunk_out_reset();
#endif
    
    if( dl.tr_lock_status )
    {
        ATOMIC(dl.tr_status=TRUE);
        
        ATOMIC(dl.tr_oem=TRUE); 

        timeout_q_remove((Timeout_Q_Func)dl_tr_timeout) ; 
        timeout_q_insert((Timeout_Q_Func)dl_tr_timeout,NULL,DL_TR_TIMEOUT,1,1) ;

        TRACE("\n\r[TR_TRIGGER_OEM]\n\r");

    }
	
	if(get_ign_status() && get_rs_gwr())
    {
		if(dl.lock_status)
		{
			ATOMIC( dl.do_unlock_all = TRUE );

			dl_do_unlock();

#if LIB_DL_DISABLE_ANALOG == 0
			dl_do_trunk(TRUE);//do it later
#endif
		}
		else
		{
#if LIB_DL_DISABLE_ANALOG == 0
			dl_do_trunk(FALSE);//do it now
#endif
		}
	   
		if(dl.evt)dl.evt(DL_EVT_TRUNK); 
    }    
}         

void dl_tr_trigger_rs( void )
{
#if LIB_DL_DISABLE_ANALOG == 0
   dl_do_trunk_out_reset();
#endif
    
   if(dl.lock_status)
   {
		ATOMIC( dl.do_unlock_all = TRUE );

		dl_do_unlock();

#if LIB_DL_DISABLE_ANALOG == 0
		dl_do_trunk(TRUE);//do it later
#endif
   }
   else
   {
#if LIB_DL_DISABLE_ANALOG == 0
	   dl_do_trunk(FALSE);//do it now
#endif
   }
   
   if(!get_ign_status())
   {
       if(dl.evt)dl.evt(DL_EVT_DISARM); 
   }
   
   if(dl.evt)dl.evt(DL_EVT_TRUNK); 
   
   if( dl.tr_lock_status )
   {
        ATOMIC(dl.tr_status=TRUE);
        ATOMIC(dl.tr_oem  =FALSE); 

        timeout_q_remove((Timeout_Q_Func)dl_tr_timeout) ; 
        timeout_q_insert((Timeout_Q_Func)dl_tr_timeout,NULL,DL_TR_TIMEOUT,1,1) ;

        TRACE("\n\r[TR_TRIGGER_RS]\n\r");
		
		
   }
     
} 

#if LIB_DL_DISABLE_ANALOG == 0
void dl_do_trunk(Boolean later)
{
    if(dl.out)
    {
        timeout_q_remove((void *)dl_trunk_ctrl_timeout );
    
        dl.trunk_out = FALSE;
    
        timeout_q_insert((Timeout_Q_Func)dl_trunk_ctrl_timeout,NULL,later?DL_OFFSET+2*DL_PULSE:0,DL_PULSE,2);
    }
}
#endif

#if LIB_DL_DISABLE_ANALOG == 0
void dl_do_trunk_out_reset(void)
{
    timeout_q_remove((void *)dl_lock_ctrl_timeout );
    
    if(dl.trunk_out)
    {
		dl_ctrl_out(DL_OUT_TRUNK0);
        
        ATOMIC(dl.trunk_out=FALSE);
    }
}  
#endif
    
#if LIB_DL_DISABLE_ANALOG == 0
Timeout_Q_Status dl_trunk_ctrl_timeout(void)
{
    ATOMIC(dl.trunk_out = !dl.trunk_out);
        
	dl_ctrl_out(dl.trunk_out?DL_OUT_TRUNK1:DL_OUT_TRUNK0);
    
    return TIMEOUT_Q_RUNNING;
}
#endif

void dl_tr_timeout( void )
{
    if( dl.tr_lock_status && dl.tr_status  )
    {
        ATOMIC(dl.tr_status=FALSE);
        
        if( dl.tr_oem )
        {
            ATOMIC( dl.tr_oem = FALSE ) ;

            timeout_q_remove((Timeout_Q_Func)dl_tr_car_rearm ) ;
            timeout_q_insert((Timeout_Q_Func)dl_tr_car_rearm , NULL , DL_TR_REARM_TIMEOUT , 1 , 1);         
            
        }
        else
		{
			timeout_q_remove( (Timeout_Q_Func)dl_tr_car_relock );
			timeout_q_insert( (Timeout_Q_Func)dl_tr_car_relock , NULL,DL_TR_RELOCK_TIMEOUT , 1 , 1 );         
		}
    }
}
        
void dl_tr_car_rearm( void )
{
    if( dl.tr_lock_status )
    {
        cmd_q_insert( INPUT_OEM_LOCK , DL_CMD_INSERT_TIMEOUT ) ;

        TRACE("\n\r[TR_RS_ARM]\n\r");
    }
}
    
void dl_tr_car_relock(void)
{    
    if( dl.tr_lock_status )
    {
		#ifdef PLATFORM_CM800
		// CM800
		
		#else
		// DBALL
			cmd_q_insert( INPUT_LOCK1 | INPUT_ON    , DL_CMD_INSERT_TIMEOUT ) ;   

			TRACE("\n\r[TR_RELOCK]\n\r");
		#endif
    }
}        

void dl_tr_reset_lock( void )
{	
    ATOMIC(dl.lock_status    = TRUE);
    ATOMIC(dl.tr_lock_status = TRUE);
	
	dl_tr_reset();
    
    dl_do_lock_oem();
}

void dl_tr_reset_unlock(void)
{
	ATOMIC(dl.lock_status    = FALSE);
    ATOMIC(dl.tr_lock_status = FALSE);
	
	dl_tr_reset();
    
    dl_do_unlock_oem();
    dl_do_relock_reset    ();
}

void dl_tr_reset_doors(void)
{
    if( dl.tr_lock_status )
    {
        timeout_q_remove( (Timeout_Q_Func)dl_tr_car_rearm  ) ;
        timeout_q_remove( (Timeout_Q_Func)dl_tr_car_relock ) ;		
        timeout_q_remove( (Timeout_Q_Func)dl_tr_timeout    ) ;
    
        ATOMIC(dl.tr_status     =FALSE);
        ATOMIC(dl.tr_oem        =FALSE); 
        ATOMIC(dl.tr_lock_status=FALSE);

        TRACE("\n\r[TRUNK_RELEASE_TRIGGER_RESET]\n\r");    
    }
}

void dl_tr_reset( void )
{
    if( dl.tr_status )
    {
        timeout_q_remove( (Timeout_Q_Func)dl_tr_car_rearm  ) ;
        timeout_q_remove( (Timeout_Q_Func)dl_tr_car_relock ) ;		
        timeout_q_remove( (Timeout_Q_Func)dl_tr_timeout    ) ;
    
    	ATOMIC(dl.tr_status=FALSE);
        ATOMIC(dl.tr_oem   =FALSE); 

        TRACE("\n\r[TRUNK_RELEASE_TRIGGER_RESET]\n\r");    
    }
}         

//--------------------------------------------------------------------------//

