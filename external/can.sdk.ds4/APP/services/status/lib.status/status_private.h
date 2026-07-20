/*==========================================================================*/
// $Id: status_private.h 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

#ifndef _STATUS_PRIVATE_H_
#define _STATUS_PRIVATE_H_

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "status.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

//this equation is required to rearrange bit writing operation since bitfield will write 
//left to right and we need to write right to left into memory 
//	STATUS_BIT = (VARS_INDEX*SIZE_OF_VARS +SIZE_OF_VARS-1) - (BIT - VARS_INDEX*SIZE_OF_VARS)
#define REVERSE_BIT(bit) (((((bit/8))*8)+7)-((bit-((bit/8)*8))))	

//--------------------------------------------------------------------------//

typedef struct
{
  Boolean 		is_valid  [ALL_STATUS] ;
  Status_Src	updater   [ALL_STATUS] ;

} Status_Entry ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

extern Status_Entry status_entry ;
#if STATUS_MILEAGE_ENABLE == 1
extern Mileage mileage ;
#endif
#if STATUS_TPMS_ENABLE == 1
extern Tpms tpms ;
#endif

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/


//--------------------------------------------------------------------------//

#endif




