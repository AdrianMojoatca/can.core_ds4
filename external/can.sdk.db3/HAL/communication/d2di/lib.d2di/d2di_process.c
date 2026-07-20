/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "d2di_private.h"
#include "cmd.h"
#include "bitfield.h"
#include "d2d.h"
#include "w2w.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
static D2Di_Remote_Id d2di_remote_id = D2DI_DRIVER1;

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/
static void d2di_not_implemented       (void);
static void d2di_process_lock          (void);
static void d2di_reset_states          (void);
static void d2di_process_unlock        (void);
static void d2di_process_unlock_driver (void);

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

/* Process a decoded packet. If a code is not supported we hit the default case
 * and return right away. We can receive either commands or requests. Both are
 * processed right away. */
void d2di_process_frame(UInt8 *d2di_frame)
{
    UInt8 d2di_code = d2di_frame[D2DI_CODE_POS];
    UInt8 dest = d2di_frame[D2DI_DEST_ADDRESS_POS];
    
    // process only messages sent to transponder bypass or doorlock interface
    if(dest != D2DI_ADDRESS_TB && dest != D2DI_ADDRESS_DL && dest != D2DI_ADDRESS_BROADCAST)
    {
        return;
    }
    
    #if DBG_D2D == 1
        dbg_tx_text("\n[D2DI_PROCESS] Received cmd: ");
        dbg_tx_h8(d2di_code);
    #endif
    
    switch(d2di_code)
    {
        case D2DI_CODE_RESET:
            d2di_reset_states();
            break;
        
        case D2DI_CODE_ANS_PROTO_VERSION:
            d2di_vars.rs_info.protocol_ver = d2di_frame[D2DI_FIRST_DATA_BYTE];
            #if DBG_D2D == 1
                dbg_tx_text("\n[D2DI_PROCESS] RS protocol version: ");
                dbg_tx_h8(d2di_vars.rs_info.protocol_ver);
            #endif
            nvfs_wr(nvfs_d2di_rs_info, &d2di_vars.rs_info);
            break;
        
        case D2DI_CODE_ANS_MANUFACTURER_ID:
            d2di_vars.rs_info.manufacturer_id = d2di_frame[D2DI_FIRST_DATA_BYTE];
            #if DBG_D2D == 1
                dbg_tx_text("\n[D2DI_PROCESS] RS manufacturer id: ");
                dbg_tx_h8(d2di_vars.rs_info.manufacturer_id);
            #endif
            nvfs_wr(nvfs_d2di_rs_info, &d2di_vars.rs_info);
            break;
       
        case D2DI_CODE_LOCK:
            d2di_process_lock();
            break;
        
        case D2DI_CODE_UNLOCK:
            d2di_process_unlock();
            break;
        
        case D2DI_CODE_ARM:
            if(d2di_mask_is_enabled(FN_ARM))
            {
              cmd_q_insert(INPUT_ARM | INPUT_ON, D2DI_CMD_TIMEOUT);               
            }
            break;
        
        case D2DI_CODE_DISARM:
            if(d2di_mask_is_enabled(FN_DISARM))
            {
                cmd_q_insert(INPUT_DISARM | INPUT_ON, D2DI_CMD_TIMEOUT);
            }
            break;
        
        case D2DI_CODE_GWR_ON:
            if(d2di_mask_is_enabled(FN_GWR_ON))
            {
                cmd_q_insert(INPUT_GWR | INPUT_ON, D2DI_CMD_TIMEOUT);
                w2w_dis(FN_GWR_ON);
            }
            break;
        
        case D2DI_CODE_GWR_OFF:
            if(d2di_mask_is_enabled(FN_GWR_OFF))
            {
                cmd_q_insert(INPUT_GWR, D2DI_CMD_TIMEOUT);
                w2w_dis(FN_GWR_OFF);
            }
            break;
        
        case D2DI_CODE_TRUNK:
            if(d2di_mask_is_enabled(FN_TRUNK_REM1_ON))
            {
                cmd_q_insert(INPUT_TRUNK | INPUT_ON, D2DI_CMD_TIMEOUT);
                w2w_dis(FN_TRUNK_REM1_ON);
            }
            break;
        
        case D2DI_CODE_PANIC_ON:
            if(d2di_mask_is_enabled(FN_PANIC_REM1_ON))
            {
                cmd_q_insert(INPUT_PANIC | INPUT_ON, D2DI_CMD_TIMEOUT);
            }
            break;
        
        case D2DI_CODE_PANIC_OFF:
            cmd_q_insert(INPUT_PANIC, D2DI_CMD_TIMEOUT);
            break;
        
        case D2DI_CODE_AUX1:
            if(d2di_mask_is_enabled(FN_AUX1_REM1_ON))
            {
                cmd_q_insert(INPUT_AUX1 | INPUT_ON, D2DI_CMD_TIMEOUT);
                w2w_dis(FN_AUX1_REM1_ON);
            }
            break;
        
        case D2DI_CODE_AUX2:
            if(d2di_mask_is_enabled(FN_AUX2_REM1_ON))
            {
                cmd_q_insert(INPUT_AUX2 | INPUT_ON, D2DI_CMD_TIMEOUT);
                w2w_dis(FN_AUX2_REM1_ON);
            }
            break;
        
        case D2DI_CODE_AUX3:
            if(d2di_mask_is_enabled(FN_AUX3_REM1_ON))
            {
                cmd_q_insert(INPUT_AUX3 | INPUT_ON, D2DI_CMD_TIMEOUT);
                w2w_dis(FN_AUX3_REM1_ON);
            }
            break;
        
        case D2DI_CODE_AUX4:
            if(d2di_mask_is_enabled(FN_AUX3_REM1_ON))
            {
                cmd_q_insert(INPUT_AUX4 | INPUT_ON, D2DI_CMD_TIMEOUT);
                w2w_dis(FN_AUX4_REM1_ON);
            }
            break;
        
        case D2DI_CODE_UNLOCK_DRIVER:
            d2di_process_unlock_driver();
            break;
        
        case D2DI_CODE_POWER_ACC_RELAY_ON:
            if(d2di_mask_is_enabled(FN_ACC_ON))
            {
                cmd_q_insert(INPUT_RS_ACCESSORY | INPUT_ON, D2DI_CMD_TIMEOUT);
            }
            break;
        
        case D2DI_CODE_POWER_ACC_RELAY_OFF:
            if(d2di_mask_is_enabled(FN_ACC_ON))
            {
                cmd_q_insert(INPUT_RS_ACCESSORY, D2DI_CMD_TIMEOUT);
            }
            break;
        
        case D2DI_CODE_POWER_IGN_RELAY_ON:
            if(d2di_mask_is_enabled(FN_IGN_ON))
            {
                cmd_q_insert(INPUT_RS_IGNITION | INPUT_ON, D2DI_CMD_TIMEOUT);
            }
            break;
        
        case D2DI_CODE_POWER_IGN_RELAY_OFF:
            if(d2di_mask_is_enabled(FN_IGN_OFF))
            {
                cmd_q_insert(INPUT_RS_IGNITION, D2DI_CMD_TIMEOUT);
            }
            break;
        
        case D2DI_CODE_POWER_START_RELAY_ON:
            if(d2di_mask_is_enabled(FN_START_ON))
            {
                cmd_q_insert(INPUT_RS_STARTER | INPUT_ON, D2DI_CMD_TIMEOUT);
            }
            break;
        
        case D2DI_CODE_POWER_START_RELAY_OFF:
            if(d2di_mask_is_enabled(FN_START_OFF))
            {
                cmd_q_insert(INPUT_RS_STARTER, D2DI_CMD_TIMEOUT);
            }
            break;
        
        case D2DI_CODE_PARKING_ON:
            if(d2di_mask_is_enabled(FN_PARKING_ON))
            {
                cmd_q_insert(INPUT_PARKING | INPUT_ON, D2DI_CMD_TIMEOUT);
            }
            break;
            
        case D2DI_CODE_PARKING_OFF:
            if(d2di_mask_is_enabled(FN_PARKING_OFF))
            {
                cmd_q_insert(INPUT_PARKING, D2DI_CMD_TIMEOUT);
            }
            break;
            
        case D2DI_CODE_HORN_ON:
            if(d2di_mask_is_enabled(FN_CHIRP))
            {
                cmd_q_insert(INPUT_CHIRP | INPUT_ON, D2DI_CMD_TIMEOUT);
            }
            break;
        
        case D2DI_CODE_ACC_ON:
            cmd_q_insert(INPUT_ACCESSORY_ON, D2DI_CMD_TIMEOUT);
            break;
        
        case D2DI_CODE_ACC_OFF:
            cmd_q_insert(INPUT_ACCESSORY_OFF, D2DI_CMD_TIMEOUT);
            break;
        
        case D2DI_CODE_IGN_ON:
            if(d2di_mask_is_enabled(FN_CAR_IGN_ON))
            {
                cmd_q_insert(INPUT_IGNITION_ON, D2DI_CMD_TIMEOUT);
            }
            break;
        
        case D2DI_CODE_IGN_OFF:
            if(d2di_mask_is_enabled(FN_CAR_IGN_OFF))
            {
                cmd_q_insert(INPUT_IGNITION_OFF, D2DI_CMD_TIMEOUT);
            }
            break;
        
        case D2DI_CODE_START_ON:
            cmd_q_insert(INPUT_STARTER_ON, D2DI_CMD_TIMEOUT);
            break;
        
        case D2DI_CODE_START_OFF:
            cmd_q_insert(INPUT_STARTER_OFF, D2DI_CMD_TIMEOUT);
            break;
        
        case D2DI_CODE_DRIVER1:
            d2di_remote_id = D2DI_DRIVER1;
            break;
        
        case D2DI_CODE_DRIVER2:
            d2di_remote_id = D2DI_DRIVER2;
            break;
        
        case D2DI_CODE_DRIVER3:
            d2di_remote_id = D2DI_DRIVER3;
            break;
        
        case D2DI_CODE_DRIVER4:
            d2di_remote_id = D2DI_DRIVER4;
            break;
        
        default:
            #if DBG_D2D == 1
                dbg_tx_text("\n[D2DI_PROCES]Command not supported!");
            #endif
            break;
    }
}

//--------------------------------------------------------------------------//
/*Receives the D2D code (NOT d2di code) and returns if it is enabled in the config_d2d. */
Boolean d2di_mask_is_enabled(UInt8 d2d_code)
{
    return d2di_get_mask(d2d_code) && d2di_get_ena(d2d_code);
}

//--------------------------------------------------------------------------//
/* This means remote starter reset. */
static void d2di_reset_states(void)
{
    if(d2di_mask_is_enabled(FN_START_OFF))
    {
        cmd_q_insert(INPUT_RS_STARTER, D2DI_CMD_TIMEOUT);
    }
    
    if(d2di_mask_is_enabled(FN_IGN_OFF))
    {
        cmd_q_insert(INPUT_RS_IGNITION, D2DI_CMD_TIMEOUT);
    }
    
    if(d2di_mask_is_enabled(FN_ACC_ON))
    {
        cmd_q_insert(INPUT_RS_ACCESSORY, D2DI_CMD_TIMEOUT);
    }
    
    if(d2di_mask_is_enabled(FN_GWR_OFF))
    {
        cmd_q_insert(INPUT_GWR, D2DI_CMD_TIMEOUT);
    }
}

//--------------------------------------------------------------------------//
static void d2di_process_lock(void)
{
    switch(d2di_remote_id)
    {
        case D2DI_DRIVER1:
            if(d2di_mask_is_enabled(FN_LOCK_REM1_ON))
            {
                cmd_q_insert(INPUT_LOCK1 | INPUT_ON, D2DI_CMD_TIMEOUT);
                w2w_dis(FN_LOCK_REM1_ON);
            }
            break;
        
        case D2DI_DRIVER2:
            if(d2di_mask_is_enabled(FN_LOCK_REM2_ON))
            {
                cmd_q_insert(INPUT_LOCK2 | INPUT_ON, D2DI_CMD_TIMEOUT);
                w2w_dis(FN_LOCK_REM2_ON);
                
            }
           break; 
        
        case D2DI_DRIVER3:
            if(d2di_mask_is_enabled(FN_LOCK_REM3_ON))
            {
                cmd_q_insert(INPUT_LOCK3 | INPUT_ON, D2DI_CMD_TIMEOUT);
                w2w_dis(FN_LOCK_REM3_ON);
            }
            break;
        
        case D2DI_DRIVER4:
            if(d2di_mask_is_enabled(FN_LOCK_REM3_ON))
            {
                cmd_q_insert(INPUT_LOCK3 | INPUT_ON, D2DI_CMD_TIMEOUT);
                w2w_dis(FN_LOCK_REM3_ON);
            }
            break;
    }
    
}

//--------------------------------------------------------------------------//
static void d2di_process_unlock(void)
{
    switch(d2di_remote_id)
    {
        case D2DI_DRIVER1:
            if(d2di_mask_is_enabled(FN_UNLOCK_ALL_REM1_ON))
            {
                cmd_q_insert(INPUT_UNLOCK_ALL1 | INPUT_ON, D2DI_CMD_TIMEOUT);
                w2w_dis(FN_UNLOCK_ALL_REM1_ON);
            }
            break;
        
        case D2DI_DRIVER2:
            if(d2di_mask_is_enabled(FN_UNLOCK_ALL_REM2_ON))
            {
                cmd_q_insert(INPUT_UNLOCK_ALL2 | INPUT_ON, D2DI_CMD_TIMEOUT);
                w2w_dis(FN_UNLOCK_ALL_REM2_ON);
            }
            break;
        
        case D2DI_DRIVER3:
            if(d2di_mask_is_enabled(FN_UNLOCK_ALL_REM3_ON))
            {
                cmd_q_insert(INPUT_UNLOCK_ALL3 | INPUT_ON, D2DI_CMD_TIMEOUT);
                w2w_dis(FN_UNLOCK_ALL_REM3_ON);
            }
            break;
        
        case D2DI_DRIVER4:
            if(d2di_mask_is_enabled(FN_UNLOCK_ALL_REM3_ON))
            {
                cmd_q_insert(INPUT_UNLOCK_ALL3 | INPUT_ON, D2DI_CMD_TIMEOUT);
                w2w_dis(FN_UNLOCK_ALL_REM3_ON);
            }
            break;
    }
}

//--------------------------------------------------------------------------//
static void d2di_process_unlock_driver(void)
{
    switch(d2di_remote_id)
    {
        case D2DI_DRIVER1:
            if(d2di_mask_is_enabled(FN_UNLOCK_DR_REM1_ON))
            {
                cmd_q_insert(INPUT_UNLOCK_DRIVER1 | INPUT_ON, D2DI_CMD_TIMEOUT);
                w2w_dis(FN_UNLOCK_DR_REM1_ON);
            }
            break;
        
        case D2DI_DRIVER2:
            if(d2di_mask_is_enabled(FN_UNLOCK_DR_REM2_ON))
            {
                cmd_q_insert(INPUT_UNLOCK_DRIVER2 | INPUT_ON, D2DI_CMD_TIMEOUT);
                w2w_dis(FN_UNLOCK_DR_REM2_ON);
            }
            break;
        
        case D2DI_DRIVER3:
            if(d2di_mask_is_enabled(FN_UNLOCK_DR_REM3_ON))
            {
                cmd_q_insert(INPUT_UNLOCK_DRIVER3 | INPUT_ON, D2DI_CMD_TIMEOUT);
                w2w_dis(FN_UNLOCK_DR_REM3_ON);
            }
            break;
        
        case D2DI_DRIVER4:
            if(d2di_mask_is_enabled(FN_UNLOCK_DR_REM3_ON))
            {
                cmd_q_insert(INPUT_UNLOCK_DRIVER3 | INPUT_ON, D2DI_CMD_TIMEOUT);
                w2w_dis(FN_UNLOCK_DR_REM3_ON);
            }
            break;
    }
}

//--------------------------------------------------------------------------//
static void d2di_not_implemented(void)
{
     #if DBG_D2D == 1
        dbg_tx_text("\n[D2DI_PROCESS]Not implemented");
    #endif
}

//--------------------------------------------------------------------------//
Boolean d2di_get_ena (UInt8 code)
{
    return (bitfield_rd(d2di_ena_table.d2di_ena , code , code)) ;
}

//--------------------------------------------------------------------------//
Boolean d2di_get_mask (UInt8 code)
{
    return (bitfield_rd(d2di_ena_table.d2di_mask , code , code)) ;
}
