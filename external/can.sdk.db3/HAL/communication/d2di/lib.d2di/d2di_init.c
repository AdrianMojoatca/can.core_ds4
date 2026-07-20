/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "d2di_private.h"
#include "d2di_uart.h"
#include "nvfs.h"
#include "nvfs_usr.h"
#include "d2d.h"
#include "string.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static Boolean d2di_init_nvfs (void);
static Boolean d2di_init_ver       (void);
static Boolean d2di_init_rs_info   (void);
static Boolean d2di_init_protocol_error     (void);
static Boolean d2di_init_type      (void);

static void d2di_reset (void);
static void d2di_validate_mask (void);
static Boolean d2di_init_table (void);
    
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
const NVFS_Entry_Name nvfs_d2di_ver            = "D2DI_VER";
const NVFS_Entry_Name nvfs_d2di_rs_info        = "D2DI_RS_INFO";
const NVFS_Entry_Name nvfs_d2di_protocol_error = "D2DI_PROTOCOL_ERROR";
const NVFS_Entry_Name nvfs_d2di_ena            = "D2DI"; // d2di enable/disable
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

Boolean d2di_init(void)
{
    if(d2di_init_nvfs())
    {
        d2di_uart_init();
        
        memset(&d2di_vars, NULL, sizeof(d2di_vars));
        d2di_vars.rs_info.d2di_lib_version = D2DI_LIB_VERSION;
        
        // assign callbacks for public d2d functions
        d2d_tx_set_func(d2di_tx_code) ;
        d2d_set_get_mask_func (d2di_get_mask);
        d2d_set_get_ena_func  (d2di_get_ena);       

        d2di_vars.tsk_d2di = os_tsk_create (d2di_task , TASK_D2DI_PRI , TASK_D2DI_STK , 0 , TASK_D2DI_NAME);
        d2di_vars.d2di_interframe_timeout = timeout_create(D2DI_INTERFRAME_TIMEOUT_MS);
        
        if(!d2di_vars.tsk_d2di || !d2di_vars.d2di_interframe_timeout)
        {
            #if DBG_D2D == 1
                dbg_tx_text("\n[D2DI_INIT] Init fail! Not enough heap!");
            #endif
            return FALSE;
        }
        
        else
        {
            d2di_scheduler_init();
            os_tsk_start(d2di_vars.tsk_d2di, 0); 
            return TRUE ; 
        }
    }
    
    else
    {
        return FALSE ;
    }
}

//--------------------------------------------------------------------------//
static Boolean d2di_init_nvfs(void)
{
    if(d2di_init_type() == TRUE)
    {
        d2di_validate_mask();
        return (d2di_init_table() && d2di_init_ver() && d2di_init_rs_info() && d2di_init_protocol_error());
    }
    else
    {
        return FALSE;
    }
}

//--------------------------------------------------------------------------//
static Boolean d2di_init_ver()
{
    D2Di_Status status = d2di_nvfs_check(nvfs_d2di_ver , &d2di_vars.version , sizeof(D2Di_Nvfs_Version) , D2DI_VERSION);
    
    if(d2di_vars.version != D2DI_VERSION || status == D2DI_RECREATE)
    {
        nvfs_delete(nvfs_d2di_ver);
        nvfs_delete(nvfs_d2di_rs_info);
        nvfs_delete(nvfs_d2di_protocol_error);
        
        #if DBG_D2D == 1
            dbg_tx_text("\n[D2DI_INIT] D2Di Version Mistmatch Nvfs Delete");
        #endif  
    
        d2di_nvfs_check(nvfs_d2di_ver , &d2di_vars.version , sizeof(D2Di_Nvfs_Version) , D2DI_VERSION);   
    }

    return TRUE ;
}

//--------------------------------------------------------------------------//
static Boolean d2di_init_rs_info (void)
{
    D2Di_Status status = d2di_nvfs_check(nvfs_d2di_rs_info , &d2di_vars.rs_info , sizeof(D2Di_Rs_Info), NULL);
    return (status != D2DI_NOT_VALID);
}

//--------------------------------------------------------------------------//
static Boolean d2di_init_protocol_error (void)
{
    D2Di_Status status = d2di_nvfs_check(nvfs_d2di_protocol_error , &d2di_vars.err , sizeof(D2Di_Error) , NULL);
    return (status != D2DI_NOT_VALID);
}

//--------------------------------------------------------------------------//
static Boolean d2di_init_type (void)
{
    D2Di_Status status = d2di_nvfs_check(nvfs_config_port , &d2di_vars.type , sizeof(D2D_Type) , D2D_TYPE1);
    
    if(d2di_vars.type == D2D_TYPE_I && status != D2DI_NOT_VALID)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//--------------------------------------------------------------------------//
D2Di_Status d2di_nvfs_check(NVFS_Entry_Name const entry_name , void * ram_srce , UInt16 entry_size , UInt8 default_value)
{
    if ((nvfs_size_is_valid(entry_name, entry_size) == NVFS_OK) && (nvfs_rd (entry_name , ram_srce)== NVFS_OK))
    {
        #if DBG_D2D == 1
            dbg_tx_text("\n[D2DI_INIT] ")     ;
            dbg_tx_text(entry_name) ;
            dbg_tx_text(" OK")      ;
        #endif

        return (D2DI_VALID) ;
    } 

    nvfs_delete(entry_name) ;
    
    #if DBG_D2D == 1
        dbg_tx_text("\n[D2DI_INIT]Delete ")   ;
        dbg_tx_text(entry_name)          ;
    #endif
        
    memset(ram_srce, default_value , entry_size) ;    
    
    if (nvfs_wr_create(entry_name, ram_srce , entry_size) == NVFS_OK)
    {
        #if DBG_D2D == 1
            dbg_tx_text("\n[D2DI_INIT] Re-Create with ") ;
            dbg_tx_h8  (default_value)      ;
        #endif
        return D2DI_RECREATE ; 
    }
    
    else
    {
        #if DBG_D2D == 1
            dbg_tx_text("\n[D2DI_INIT] Re-Create FAIL") ;
        #endif
    }    
   
    return D2DI_NOT_VALID ;  
}

//--------------------------------------------------------------------------//
static void d2di_validate_mask (void)
{
  UInt8     index ;
  
  if (nvfs_rd(nvfs_d2di_ena , &d2di_ena_table) == NVFS_OK)
  {
    // Loop through the D2D mask from NVFS and if a discrepancy is found with the firmware's default mask (support bits), reset the D2D NVFS variables
    for (index = 0 ; index < sizeof_array (d2di_ena_table.d2di_mask) ; index++)
    {
      if (d2di_ena_table.d2di_mask[index] ^ d2di_ena_default.d2di_mask[index])
      {
        #if DBG_D2D == 1
          dbg_tx_text ("\n[D2DI_INIT] Found discrepancy in D2Di mask for byte ") ;
          dbg_tx_u    (index , 2) ;
          dbg_tx_text (" --> D2Di NVFS variables will be reinitialized]\n") ;
        #endif
        
        // Reset the D2D NVFS variables since the mask is different than the default one
        d2di_reset() ;
      
        // Exit
        return ;
      }
    }
  }
  else
  {
    // Reset the (other) D2D NVFS variables since the mask doesn't even exist
    d2di_reset() ;
  }
}

//--------------------------------------------------------------------------//
static void d2di_reset (void)
{
    nvfs_delete(nvfs_d2di_ena);
    nvfs_delete(nvfs_d2di_protocol_error);
}

//--------------------------------------------------------------------------//
static Boolean d2di_init_table (void)
{

  UInt16 size = 0 ;

  if ((nvfs_rd_size(nvfs_d2di_ena , &size) == NVFS_OK))
    {
      if((size == sizeof(d2di_ena_table)) && (nvfs_rd(nvfs_d2di_ena , &d2di_ena_table) == NVFS_OK))
      {
          #if DBG_D2D == 1
              dbg_tx_text ("\n[D2DI_INIT] nvfs_d2di_ena correct read") ;
          #endif
          return (TRUE) ;
      }
    }

  nvfs_delete(nvfs_d2di_ena);

  switch (nvfs_wr_create(nvfs_d2di_ena, &d2di_ena_default , sizeof(d2di_ena_default)))
    {
      case NVFS_OK :  
        {
           #if DBG_D2D == 1
              dbg_tx_text ("\n[D2DI_INIT] nvfs_d2di_ena re-created") ;
           #endif
           return d2di_init_table() ;
        }  
    }      

  return FALSE ;
}
