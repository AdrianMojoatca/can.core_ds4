/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

#ifndef __D2DI_H__
#define __D2DI_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "nvfs.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
// Use this number to keep track of library version
// It is written in the d2di_rs_info NVFS entry
#define D2DI_LIB_VERSION        16

typedef enum
{
    D2DI_ERR_CORRUPT_PACKET     , // general packet decode failure
    D2DI_ERR_WAKE_ERROR         , // first packet failed (wake issue)
    D2DI_ERR_INTERFRAME_TIMEOUT , // interframe delay exceeded

    D2DI_ERR_COUNT 

} D2Di_Error_Code ;

//--------------------------------------------------------------------------//
typedef enum
{
    D2DI_NOT_VALID = 0 ,
    D2DI_VALID     = 1 ,
    D2DI_RECREATE  = 2 
} D2Di_Status ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

void d2di_scheduler_init(void);
Boolean d2di_decode_frame(UInt8 **d2di_packet);
Boolean d2di_mask_is_enabled(UInt8 d2d_code);
void d2di_tx(UInt8 code);
Boolean d2di_init(void);
void d2di_err_add (D2Di_Error_Code code);
D2Di_Status d2di_nvfs_check(NVFS_Entry_Name const entry_name , void * ram_srce , UInt16 entry_size , UInt8 default_value);

#endif
