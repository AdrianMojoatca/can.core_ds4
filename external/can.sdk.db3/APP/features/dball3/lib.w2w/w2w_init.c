/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: w2w_init.c 33258 2015-08-19 12:46:35Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "w2w_private.h"
#include "fiq.h"
#include "irq_user.h"
#include "core_contract_db3_gpio_input.h"
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

void w2w_init (void)
{
    if(!w2w_init_nvfs())
      {
        // error ?
      }

    os_tsk_wait (100) ; // delay to make sure the positive inputs will have time to discharge their capacitor
                        // that were charged by the pull_up resistor of the reset state (60 ms discharge time)
    
        
        ied_gwr      = ied_create (core_contract_db3_gpio_input_read(CORE_DB3_GPIO_INPUT_NEGATIVE_7), TIMEOUT_IED_GWR    , TIMEOUT_IED_GWR     );
        ied_ign      = ied_create (core_contract_db3_gpio_input_read(CORE_DB3_GPIO_INPUT_POSITIVE_2), TIMEOUT_IED_IGN    , TIMEOUT_IED_IGN     );
        ied_start    = ied_create (core_contract_db3_gpio_input_read(CORE_DB3_GPIO_INPUT_POSITIVE_3), TIMEOUT_IED_START  , TIMEOUT_IED_START   );
        ied_pos_in1  = ied_create (core_contract_db3_gpio_input_read(CORE_DB3_GPIO_INPUT_POSITIVE_1), TIMEOUT_IED_POS_IN1, TIMEOUT_IED_POS_IN1 );
        ied_lock     = ied_create (core_contract_db3_gpio_input_read(CORE_DB3_GPIO_INPUT_NEGATIVE_1), TIMEOUT_IED_LOCK   , TIMEOUT_IED_LOCK    );
        ied_unlock   = ied_create (core_contract_db3_gpio_input_read(CORE_DB3_GPIO_INPUT_NEGATIVE_2), TIMEOUT_IED_UNLOCK , TIMEOUT_IED_UNLOCK  );
        ied_trunk    = ied_create (core_contract_db3_gpio_input_read(CORE_DB3_GPIO_INPUT_NEGATIVE_3), TIMEOUT_IED_TRUNK  , TIMEOUT_IED_TRUNK   );
        ied_aux1     = ied_create (core_contract_db3_gpio_input_read(CORE_DB3_GPIO_INPUT_NEGATIVE_4), TIMEOUT_IED_AUX1   , TIMEOUT_IED_AUX1    );
        ied_aux2     = ied_create (core_contract_db3_gpio_input_read(CORE_DB3_GPIO_INPUT_NEGATIVE_5), TIMEOUT_IED_AUX2   , TIMEOUT_IED_AUX2    );
        ied_aux3     = ied_create (core_contract_db3_gpio_input_read(CORE_DB3_GPIO_INPUT_NEGATIVE_6), TIMEOUT_IED_AUX3   , TIMEOUT_IED_AUX3    );
        ied_push     = ied_create (core_contract_db3_gpio_input_read(CORE_DB3_GPIO_INPUT_BUTTON_IN)   , TIMEOUT_IED_PUSH   , TIMEOUT_IED_PUSH    );
                                           
    per_exec_init();
    per_exec_register( (Per_Exec_Func)w2w_execute );

    w2w_set_level () ;
}

//---------------------------------------------------- //
