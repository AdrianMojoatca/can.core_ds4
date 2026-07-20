/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: sense_notify_rke.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "sense_notify_private.h"
#include "status.h"
#include "cmd.h"
#include "rf.h"
#include "timeout_f.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

typedef enum 
{

  SENSE_NOTIFY_RKE_LOCK      , 
  SENSE_NOTIFY_RKE_UNLOCK    ,
  SENSE_NOTIFY_RKE_TRUNK     ,
  SENSE_NOTIFY_RKE_PANIC     ,
  SENSE_NOTIFY_RKE_AUX1      ,
  SENSE_NOTIFY_RKE_AUX2      ,
  SENSE_NOTIFY_RKE_AUX3      ,
  SENSE_NOTIFY_RKE_AUX4      ,
  SENSE_NOTIFY_RKE_START     ,
  SENSE_NOTIFY_RKE_STOP      ,

}Rke_Bit ;

UInt8 input_cmd [] = 
{
  INPUT_OEM_LOCK             ,
  INPUT_OEM_UNLOCK           ,
  INPUT_OEM_TRUNK            ,
  INPUT_OEM_PANIC            ,
  INPUT_OEM_AUX1             ,
  INPUT_OEM_AUX2             ,
  INPUT_OEM_AUX3             ,
  INPUT_OEM_AUX4             ,
  INPUT_OEM_RS_ON            ,
  INPUT_OEM_RS_OFF           ,
} ;                            


/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static void sense_notify_rke          (Rke_Bit rke_bit) ;
static void sense_notify_rke_dbg_print(Rke_Bit rke_bit) ;



/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void sense_notify_rke_idle(void)
{
  Boolean  smart_key = rke.bit.smart_key ; 

  rke.word          = (NULL) ;
  rke.bit.smart_key = smart_key ;
  rke.bit.equipped  = TRUE ;
  rke.bit.idle      = TRUE ;
}

//--------------------------------------------------------------------------------
void sense_notify_rke_lock(void)
{
  sense_notify_rke(SENSE_NOTIFY_RKE_LOCK) ;
  
  if(!doorlock_status.bit.is_support)
  {
    rf_icon_set(ICON_LOCK, TRUE);

    #if DBG_SENSE == 1
      sense_dbg_print("\nSense Update Rf" , NULL , NULL) ;
    #endif 
  
  }
}
//--------------------------------------------------------------------------------
void sense_notify_rke_handle_lock(void)
{
	sense_notify_lock_active   = TRUE ;
 
	
  if ( (!sense_notify_lock_set))  
  {
      sense_notify_lock_active = FALSE ;  
      sense_notify_lock_set = TRUE ;
      sense_notify_rke_lock();
		  timeout_start(timeout_unlock);

  }
	else
	{
	    sense_notify_arm();	
	}
  
}
//--------------------------------------------------------------------------------
void sense_notify_rke_unlock(void)
{


  sense_notify_rke(SENSE_NOTIFY_RKE_UNLOCK) ;
  
  if(!doorlock_status.bit.is_support)
  {
    rf_icon_clr(ICON_LOCK, TRUE);

    #if DBG_SENSE == 1
      sense_dbg_print("\nSense Update Rf" , NULL , NULL) ;
    #endif 
  }
  
}
//--------------------------------------------------------------------------------
void sense_notify_rke_trunk(void)
{
  sense_notify_rke(SENSE_NOTIFY_RKE_TRUNK) ;
}

//--------------------------------------------------------------------------------
void sense_notify_rke_aux1(void)
{
  sense_notify_rke(SENSE_NOTIFY_RKE_AUX1) ;
}

//--------------------------------------------------------------------------------
void sense_notify_rke_aux2(void)
{
  sense_notify_rke(SENSE_NOTIFY_RKE_AUX2) ;
}

//--------------------------------------------------------------------------------
void sense_notify_rke_aux3(void)
{
  sense_notify_rke(SENSE_NOTIFY_RKE_AUX3) ;
}

//--------------------------------------------------------------------------------
void sense_notify_rke_aux4(void)
{
  sense_notify_rke(SENSE_NOTIFY_RKE_AUX4) ;
}

//--------------------------------------------------------------------------------
void sense_notify_rke_panic(void)
{
  sense_notify_rke(SENSE_NOTIFY_RKE_PANIC) ;
}

//--------------------------------------------------------------------------------
void sense_notify_rke_start(void)
{
  sense_notify_rke(SENSE_NOTIFY_RKE_START) ;
}

//--------------------------------------------------------------------------------
void sense_notify_rke_stop(void)
{
  sense_notify_rke(SENSE_NOTIFY_RKE_STOP) ;
}

//--------------------------------------------------------------------------------
static void sense_notify_rke(Rke_Bit rke_bit)
{
  sense_notify_rke_idle() ;
  
  rke.bit.idle = FALSE ;

  switch(rke_bit)
    {
      case SENSE_NOTIFY_RKE_LOCK  : rke.bit.lock   = TRUE ; break ;
      case SENSE_NOTIFY_RKE_UNLOCK: rke.bit.unlock = TRUE ; break ;
      case SENSE_NOTIFY_RKE_TRUNK : rke.bit.trunk  = TRUE ; break ;
      case SENSE_NOTIFY_RKE_PANIC : rke.bit.panic  = TRUE ; break ;
      case SENSE_NOTIFY_RKE_AUX1  : rke.bit.aux1   = TRUE ; break ;
      case SENSE_NOTIFY_RKE_AUX2  : rke.bit.aux2   = TRUE ; break ;
      case SENSE_NOTIFY_RKE_AUX3  : rke.bit.aux3   = TRUE ; break ;
      case SENSE_NOTIFY_RKE_AUX4  : rke.bit.aux4   = TRUE ; break ;
      case SENSE_NOTIFY_RKE_START : rke.bit.start  = TRUE ; break ;
      case SENSE_NOTIFY_RKE_STOP  : rke.bit.stop   = TRUE ; break ;                        
    }

  cmd_q_insert ((Cmd_List)input_cmd[rke_bit], COMMAND_INSERT_TIMEOUT);

 
  sense_notify_rke_dbg_print(rke_bit) ;
}

//--------------------------------------------------------------------------------
static void sense_notify_rke_dbg_print(Rke_Bit rke_bit)
{
  #if DBG_SENSE == 1

  sense_notify_dbg_print("\nSense Rke " , NULL , NULL) ; 

  switch(rke_bit)
    {
      case SENSE_NOTIFY_RKE_LOCK  : sense_notify_dbg_print("LOCK"  ,NULL, NULL) ; break ;
      case SENSE_NOTIFY_RKE_UNLOCK: sense_notify_dbg_print("UNLOCK",NULL, NULL) ; break ;
      case SENSE_NOTIFY_RKE_TRUNK : sense_notify_dbg_print("TRUNK" ,NULL, NULL) ; break ;
      case SENSE_NOTIFY_RKE_PANIC : sense_notify_dbg_print("PANIC" ,NULL, NULL) ; break ;
      case SENSE_NOTIFY_RKE_AUX1  : sense_notify_dbg_print("AUX1"  ,NULL, NULL) ; break ;
      case SENSE_NOTIFY_RKE_AUX2  : sense_notify_dbg_print("AUX2"  ,NULL, NULL) ; break ;
      case SENSE_NOTIFY_RKE_AUX3  : sense_notify_dbg_print("AUX3"  ,NULL, NULL) ; break ;
      case SENSE_NOTIFY_RKE_AUX4  : sense_notify_dbg_print("AUX4"  ,NULL, NULL) ; break ;
      case SENSE_NOTIFY_RKE_START : sense_notify_dbg_print("START" ,NULL, NULL) ; break ;
      case SENSE_NOTIFY_RKE_STOP  : sense_notify_dbg_print("STOP"  ,NULL, NULL) ; break ;
    }
  #endif
}
//--------------------------------------------------------------------------------
void sense_notify_rke_timeout(void * arg) 
{ 
    sense_notify_lock_set = FALSE;
    
    if (sense_notify_lock_active && doorlock_status.bit.lock)
    {
       sense_notify_rke_handle_lock(); 
    }

}
