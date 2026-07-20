/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "d2di_private.h"
#include "config_d2d.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/


/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
D2Di_Vars  d2di_vars  ;
D2Di_Ena_Bit d2di_ena_table;

const D2Di_Ena_Bit d2di_ena_default =
{
  {
    D2D_MASK_0  ,D2D_MASK_1  ,D2D_MASK_2  ,D2D_MASK_3  ,D2D_MASK_4  ,D2D_MASK_5  ,D2D_MASK_6  ,D2D_MASK_7  ,
    D2D_MASK_8  ,D2D_MASK_9  ,D2D_MASK_10 ,D2D_MASK_11 ,D2D_MASK_12 ,D2D_MASK_13 ,D2D_MASK_14 ,D2D_MASK_15 ,
    D2D_MASK_16 ,D2D_MASK_17 ,D2D_MASK_18 ,D2D_MASK_19 ,D2D_MASK_20 ,D2D_MASK_21 ,D2D_MASK_22 ,D2D_MASK_23 ,
    D2D_MASK_24 ,D2D_MASK_25 ,D2D_MASK_26 ,D2D_MASK_27 ,D2D_MASK_28 ,D2D_MASK_29 ,D2D_MASK_30 ,D2D_MASK_31 ,
  },

  {
    D2D_ENA_0   ,D2D_ENA_1   ,D2D_ENA_2   ,D2D_ENA_3   ,D2D_ENA_4   ,D2D_ENA_5   ,D2D_ENA_6   ,D2D_ENA_7   ,
    D2D_ENA_8   ,D2D_ENA_9   ,D2D_ENA_10  ,D2D_ENA_11  ,D2D_ENA_12  ,D2D_ENA_13  ,D2D_ENA_14  ,D2D_ENA_15  ,
    D2D_ENA_16  ,D2D_ENA_17  ,D2D_ENA_18  ,D2D_ENA_19  ,D2D_ENA_20  ,D2D_ENA_21  ,D2D_ENA_22  ,D2D_ENA_23  ,
    D2D_ENA_24  ,D2D_ENA_25  ,D2D_ENA_26  ,D2D_ENA_27  ,D2D_ENA_28  ,D2D_ENA_29  ,D2D_ENA_30  ,D2D_ENA_31  ,
  }
};
