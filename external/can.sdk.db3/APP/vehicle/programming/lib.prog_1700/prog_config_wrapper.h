/*<#======================================================================#>*/
/*               WRAPPER CONFIG PROGRAMARE - DOAR DB3                       */
/*<#======================================================================#>*/

/*==========================================================================*/
// DEPRECATED (DB3): wrapper de compatibilitate mentinut temporar
// Configuratia activa este furnizata prin contractul DB3 din core
// Nu adaugati dependinte noi pe acest wrapper
/*==========================================================================*/

#ifndef __PROG_CONFIG_WRAPPER_H__
#define __PROG_CONFIG_WRAPPER_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

// Configurare contract DB3 (single boundary catre config_prog.h in core contract)
#include "core_contract_db3_prog_config.h"

/*==========================================================================*/
/*      D E F I N E S  -  C O N F I G   S T R U C T U R E                  */
/*==========================================================================*/

/*
 * FILOZOFIA WRAPPER-ULUI (DEPRECATED):
 * 
 * Stare curenta (DB3):
 *   - Sursa de adevar pentru configuratie este contractul DB3
 *   - Boundary unic catre config_prog.h ramane in core_contract_db3_prog_config.c
 *   - Acest fisier este pastrat doar pentru compatibilitate tranzitorie
 * 
 * Directie:
 *   - Noile implementari trebuie sa includa direct core_contract_db3_prog_config.h
 *   - Wrapper-ul nu trebuie extins functional
 */

/* ===========================
    API PUBLIC
    =========================== */

/*--------------------------------------------------------------------------*/
/* Flag-uri Enable/Disable pentru functionalitati */
/*--------------------------------------------------------------------------*/

typedef struct {
    /* Functionalitati Trunk / AUX */
    UInt8  trunk_enable              ;
    UInt8  right_slide_enable        ;
    UInt8  left_slide_enable         ;
    UInt8  tail_glass_enable         ;
    UInt8  gas_cap_enable            ;
    UInt8  head_lights_enable        ;
    UInt8  sunroof_closure_enable    ;
    UInt8  windows_closure_enable    ;
    UInt8  windows_vent_enable       ;

    /* Niveluri scaune incalzite */
    UInt8  heated_seats_level_high_enable ;
    UInt8  heated_seats_level_med_enable  ;
    UInt8  heated_seats_level_low_enable  ;

    /* Enable-uri pentru configurare functionalitati */
    UInt8  unlock_driver_enable      ;
    UInt8  relock_enable             ;
    UInt8  auto_lock_enable          ;
    UInt8  auto_unlock_enable        ;
    UInt8  trunk_enable_feat         ;  /* Enable de feature diferit de optiune */
    UInt8  aux1_enable               ;
    UInt8  aux2_enable               ;
    UInt8  aux3_enable               ;
    UInt8  aux4_enable               ;
    UInt8  rf_output_enable          ;
    UInt8  horn_confirmation_enable  ;
    UInt8  comfort_closure_enable    ;
    UInt8  oem_security_enable       ;
    UInt8  key_ign_arm_defeat_enable ;
    UInt8  light_control_enable      ;
    UInt8  ext_light_supervision_enable ;
    UInt8  heated_seats_enable       ;
    UInt8  defroster_enable          ;
    UInt8  seats_temp_control_enable ;
    UInt8  seats_temp_level_enable   ;
    UInt8  ctl_doorlock_enable       ;
    UInt8  secure_lock_enable        ;

} Prog_Core_Config_Features_t ;

/*--------------------------------------------------------------------------*/
/* Valori pentru functionalitati (optiuni selectate) */
/*--------------------------------------------------------------------------*/

typedef struct {
    UInt8  unlock_driver_value       ;
    UInt8  relock_value              ;
    UInt8  auto_lock_value           ;
    UInt8  auto_unlock_value         ;
    UInt8  trunk_default             ;
    UInt8  aux1_default              ;
    UInt8  aux2_default              ;
    UInt8  aux3_default              ;
    UInt8  aux4_default              ;
    UInt8  rf_output_value           ;
    UInt8  horn_confirmation_value   ;
    UInt8  comfort_closure_value     ;
    UInt8  oem_security_value        ;
    UInt8  key_ign_arm_defeat_value  ;
    UInt8  light_control_value       ;
    UInt8  ext_light_supervision_value ;
    UInt8  heated_seats_value        ;
    UInt8  defroster_value           ;
    UInt8  seats_temp_control_value  ;
    UInt8  seats_temp_level_value    ;
    UInt8  ctl_doorlock_value        ;
    UInt8  ctl_doorlock_ignition_cfg ;
    UInt8  ctl_doorlock_brake_cfg    ;
    UInt8  ctl_doorlock_speed_cfg    ;
    UInt8  secure_lock_value         ;
    UInt8  secure_lock_smart_cfg  ;

} Prog_Core_Config_Values_t ;

/*--------------------------------------------------------------------------*/
/* Pachet complet de configurare */
/*--------------------------------------------------------------------------*/

typedef struct {
    Prog_Core_Config_Features_t  features ;
    Prog_Core_Config_Values_t    values   ;
} Prog_Core_Config_t ;

/*==========================================================================*/
/*                  W R A P P E R   M A C R O S                             */
/*==========================================================================*/

/*
 * STATUS DB3:
 * Macro-urile PROG_CFG_* sunt definite in contractul DB3.
 * Acest wrapper ramane doar pentru compatibilitate tranzitorie.
 */

/*
 * Macro-urile PROG_CFG_* principale sunt furnizate de
 * core_contract_db3_prog_config.h.
 * Păstrăm doar alias-urile legacy folosite ocazional.
 */
#define PROG_CFG_EXT_LIGHT_SUPERVISION_EN  PROG_CFG_EXT_LIGHT_SUPERVISION_ENABLE
#define PROG_CFG_EXT_LIGHT_SUPERVISION_VAL PROG_CFG_EXT_LIGHT_SUPERVISION_VALUE

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*
 * Notă: nu sunt planificate extensii functionale pentru acest wrapper.
 * Noile integrari trebuie sa foloseasca direct contractul DB3.
 */

/*
void prog_config_wrapper_init(Prog_Core_Config_t* cfg);
void prog_config_wrapper_apply(const Prog_Core_Config_t* cfg);
void prog_config_wrapper_read(Prog_Core_Config_t* cfg_out);
*/

/*==========================================================================*/
/*                        D E P R E C A T E D  N O T E                     */
/*==========================================================================*/

/*
 * Acest fisier este mentinut doar pentru tranzitie.
 * Pentru cod nou, includeti direct core_contract_db3_prog_config.h.
 */

#endif /* __PROG_CONFIG_WRAPPER_H__ */

