/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: prog_set_option.c 23628 2013-11-15 18:18:39Z louis-philippe.rispoli $
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

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

Boolean prog_set_option (UInt8 option_nb, Feature_Name feature_name)
{
    UInt8 i ;

#if DBG_PROG == 1

  static const char * featstring[] = 
  {
    {"FEAT_UNLOCK_DRIVER        " },
    {"FEAT_RELOCK               " },
    {"FEAT_AUTO_LOCK            " },
    {"FEAT_AUTO_UNLOCK          " },
    {"FEAT_TRUNK                " },
    {"FEAT_AUX1                 " },
    {"FEAT_AUX2                 " },
    {"FEAT_AUX3                 " },
    {"FEAT_AUX4                 " },
    {"FEAT_RF_OUTPUT            " },
    {"FEAT_HORN_CONFIRMATION    " },
    {"FEAT_COMFORT_CLOSURE      " },
    {"FEAT_OEM_SECURITY         " },
    {"FEAT_KEY_IGN_ARM_DEFEAT   " },
    {"FEAT_LIGHT_CONTROL        " },
    {"FEAT_EXT_LIGHT_SUPERVISION" },
    {"FEAT_HEATED_SEATS         " },
    {"FEAT_DEFROSTER            " },
    {"FEAT_SEATS_TEMP_CONTROL   " },
    {"FEAT_SEATS_TEMP_LEVEL     " },
    {"FEAT_CTL_DOORLOCK         " },
    {"FEAT_SECURE_LOCK          " },
    {"FEAT_CUSTOM1              " },
    {"FEAT_CUSTOM2              " },
    {"FEAT_CUSTOM3              " },
    {"FEAT_CUSTOM4              " },
	{"FEAT_RUNTIME              " },
  };

  dbg_tx_crlf() ;
  dbg_tx_text(featstring[feature_name]) ;

  dbg_tx_text(" Option ") ;
  dbg_tx_u   (option_nb , 2)      ;

#endif


    for (i = 0; i < FEATURE_NUM; i++)
    {
        if (feature_table[i].feature == feature_name)
        {
            if(option_nb <= feature_table[i].option_number)
            {
                feature_table[i].option_current = option_nb ;

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


