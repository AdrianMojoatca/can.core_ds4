/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: prog_set_option.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "prog_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

static void prog_sync_runtime_contract_option(Feature_Name feature_name, UInt8 option_nb)
{
    switch (feature_name)
    {
        case FEAT_UNLOCK_DRIVER:
            set_prog_feat_unlock_driver_value((uint32_t)option_nb);
            break;
        case FEAT_RELOCK:
            set_prog_feat_relock_value((uint32_t)option_nb);
            break;
        case FEAT_AUTO_LOCK:
            set_prog_feat_auto_lock_value((uint32_t)option_nb);
            break;
        case FEAT_AUTO_UNLOCK:
            set_prog_feat_auto_unlock_value((uint32_t)option_nb);
            break;
        case FEAT_TRUNK:
            set_prog_feat_trunk_default((uint32_t)option_nb);
            break;
        case FEAT_AUX1:
            set_prog_feat_aux1_default((uint32_t)option_nb);
            break;
        case FEAT_AUX2:
            set_prog_feat_aux2_default((uint32_t)option_nb);
            break;
        case FEAT_AUX3:
            set_prog_feat_aux3_default((uint32_t)option_nb);
            break;
        case FEAT_AUX4:
            set_prog_feat_aux4_default((uint32_t)option_nb);
            break;
        case FEAT_RF_OUTPUT:
            set_prog_feat_rf_output_value((uint32_t)option_nb);
            break;
        case FEAT_HORN_CONFIRMATION:
            set_prog_feat_horn_confirmation_value((uint32_t)option_nb);
            break;
        case FEAT_COMFORT_CLOSURE:
            set_prog_feat_comfort_closure_value((uint32_t)option_nb);
            break;
        case FEAT_OEM_SECURITY:
            set_prog_feat_oem_security_value((uint32_t)option_nb);
            break;
        case FEAT_KEY_IGN_ARM_DEFEAT:
            set_prog_feat_key_ign_arm_defeat_value((uint32_t)option_nb);
            break;
        case FEAT_LIGHT_CONTROL:
            set_prog_feat_light_control_value((uint32_t)option_nb);
            break;
        case FEAT_EXT_LIGHT_SUPERVISION:
            set_prog_feat_ext_light_supervision_value((uint32_t)option_nb);
            break;
        case FEAT_HEATED_SEATS:
            set_prog_feat_heated_seats_value((uint32_t)option_nb);
            break;
        case FEAT_DEFROSTER:
            set_prog_feat_defroster_value((uint32_t)option_nb);
            break;
        case FEAT_SEATS_TEMP_CONTROL:
            set_prog_feat_seats_temp_control_value((uint32_t)option_nb);
            break;
        case FEAT_SEATS_TEMP_LEVEL:
            set_prog_feat_seats_temp_level_value((uint32_t)option_nb);
            break;
        case FEAT_CTL_DOORLOCK:
            set_prog_feat_ctl_doorlock_value((uint32_t)option_nb);
            break;
        case FEAT_SECURE_LOCK:
            set_prog_feat_secure_lock_value((uint32_t)option_nb);
            break;
        case FEAT_CUSTOM1:
            set_prog_feat_custom1_default_value((uint32_t)option_nb);
            break;
        case FEAT_CUSTOM2:
            set_prog_feat_custom2_value((uint32_t)option_nb);
            break;
        case FEAT_CUSTOM3:
            set_prog_feat_custom3_value((uint32_t)option_nb);
            break;
        case FEAT_CUSTOM4:
            set_prog_feat_custom4_value((uint32_t)option_nb);
            break;
        default:
            break;
    }
}

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

Boolean prog_set_option (UInt8 option_nb, Feature_Name feature_name)
{
    UInt8 i ;

#if DBG_PROG == 1
//  StrConst featstring[26][FEAT_COUNT] = 
//  {
//    {"FEAT_UNLOCK_DRIVER        " },
//    {"FEAT_RELOCK               " },
//    {"FEAT_AUTO_LOCK            " },
//    {"FEAT_AUTO_UNLOCK          " },
//    {"FEAT_TRUNK                " },
//    {"FEAT_AUX1                 " },
//    {"FEAT_AUX2                 " },
//    {"FEAT_AUX3                 " },
//    {"FEAT_AUX4                 " },
//    {"FEAT_RF_OUTPUT            " },
//    {"FEAT_HORN_CONFIRMATION    " },
//    {"FEAT_COMFORT_CLOSURE      " },
//    {"FEAT_OEM_SECURITY         " },
//    {"FEAT_KEY_IGN_ARM_DEFEAT   " },
//    {"FEAT_LIGHT_CONTROL        " },
//    {"FEAT_EXT_LIGHT_SUPERVISION" },
//    {"FEAT_HEATED_SEATS         " },
//    {"FEAT_DEFROSTER            " },
//    {"FEAT_SEATS_TEMP_CONTROL   " },
//    {"FEAT_SEATS_TEMP_LEVEL     " },
//    {"FEAT_CTL_DOORLOCK         " },
//    {"FEAT_SECURE_LOCK          " },
//    {"FEAT_CUSTOM1              " },
//    {"FEAT_CUSTOM2              " },
//    {"FEAT_CUSTOM3              " },
//    {"FEAT_CUSTOM4              " },
//  };
//
//  dbg_tx_crlf() ;
//  dbg_tx_text(&featstring[feature_name][0]) ;
//
  dbg_tx_text("\r\nset Option :") ;
  dbg_tx_u   (option_nb , 2)      ;

#endif


    for (i = 0; i < FEATURE_NUM; i++)
    {
        if (feature_table[i].feature == feature_name)
        {
            if(option_nb <= feature_table[i].option_number)
            {
                feature_table[i].option_current = option_nb ;

                prog_sync_runtime_contract_option(feature_name, option_nb);

                prog_feat_table_write () ;

                if(prog_vars.exit_func != NULL)
                {
                    prog_vars.exit_func() ;
                }
                return TRUE ;
            }
        }   
    }   
    return FALSE ;
}

