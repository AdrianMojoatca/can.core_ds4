/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: rsr_feature_check_content.c 29910 2015-02-12 16:26:05Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "rsr_feature_private.h"
#include "led_usr.h"
#include "os.h"
#include "dbg.h"
#include "gpio.h"
#include "core_contract_db3_gpio_input.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static void rsr_feature_signal_error(void);

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void rsr_feature_check_content(void)
{
	Boolean error = FALSE ;

    UInt8  i ;
        
    for( i = 0 ; i < sizeof_array(rsr_feature_check_list) ; i++ )
    {
        if ( rsr_feature_check_list[i].check && rsr_feature_enabled((Rsr_Feature_Index)i) )
        {
            if ( rsr_feature_get((Rsr_Feature_Index)i) < rsr_feature_check_list[i].min_value ||
                 rsr_feature_get((Rsr_Feature_Index)i) > rsr_feature_check_list[i].max_value )
            {
#if DBG_RSR_FEAT == 1
  dbg_tx_text("*** rsr_feat: ERROR on feature # ");
  dbg_tx_h8(i);
  dbg_tx_text(":\r\n");
  dbg_tx_text("*** rsr_feat: value = ");
  dbg_tx_h8(rsr_feature_get((Rsr_Feature_Index)i));
  dbg_tx_text("\r\n");
#endif
				rsr_feature_err_add ( RS_FEAT_ERR_INVALID_VALUE );
				rsr_feature_set((Rsr_Feature_Index)i, rsr_feature_default_table[i].value);
                error = TRUE  ; 
            }
        }
    }

    if ( error )
    {
        rsr_feature_signal_error();
		rsr_feature_save();
    }
	else
	{
#if DBG_RSR_FEAT == 1
  dbg_tx_text("*** rsr_feat: features loaded successfully!\r\n");
#endif
	}
}

//--------------------------------------------------------------------------//

void rsr_feature_check_size(void)
{
    static UInt16 size ;
    
    if ( nvfs_rd_size( nvfs_feat_rs , &size ) == NVFS_OK )
    {
        if ( size != sizeof(rsr_feature_list) )
        {
			rsr_feature_err_add ( RS_FEAT_ERR_INVALID_SIZE );
            nvfs_delete ( nvfs_feat_rs );
            rsr_feature_signal_error();
        }
    }
}

//--------------------------------------------------------------------------//

static void rsr_feature_signal_error(void)
{
	led_usr_op_err(RSR_FEATURE_ERROR , 255);

	// Wait for button to be pushed and released - note: polarity is inverted
    while (core_contract_db3_gpio_input_read(CORE_DB3_GPIO_INPUT_BUTTON_IN))
	{
		os_tsk_wait(100);
	}
    while (!core_contract_db3_gpio_input_read(CORE_DB3_GPIO_INPUT_BUTTON_IN))
	{
   		os_tsk_wait(100);
	}

}


