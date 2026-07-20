/*==========================================================================*/
// $Id: about_hw_info.c 33258 2015-08-19 12:46:35Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "device.h"
#include "hw_adc.h"
#include "hw_ver_code.h"
#include "nvfs_usr.h"
#include "nvfs.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
#define MIN_VALUE 		32
#define RANGE_VALUE		64
/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/
                                   
UInt8 about_hw_info (void)
{
  #ifdef DEV_LPC_176x
	UInt8 hw_ver =0;
	
	nvfs_rd (nvfs_hw_ver , &hw_ver);
	
	return hw_ver;
	
  #else
	
  UInt16 value = hw_adc_convert (CHANNEL1) ;

  if (value < MIN_VALUE)    		                        return NO_VERSION ;	
  else if (value < (MIN_VALUE + (1 * RANGE_VALUE))) 		return HW_VER_8  ;
  else if (value < (MIN_VALUE + (2 * RANGE_VALUE))) 		return HW_VER_4  ;
  else if (value < (MIN_VALUE + (3 * RANGE_VALUE))) 		return HW_VER_9  ;
  else if (value < (MIN_VALUE + (4 * RANGE_VALUE))) 		return HW_VER_2  ;
  else if (value < (MIN_VALUE + (5 * RANGE_VALUE))) 		return HW_VER_10 ;
  else if (value < (MIN_VALUE + (6 * RANGE_VALUE))) 		return HW_VER_5  ;
  else if (value < (MIN_VALUE + (7 * RANGE_VALUE))) 		return HW_VER_11 ;
  else if (value < (MIN_VALUE + (8 * RANGE_VALUE))) 		return HW_VER_1  ;
  else if (value < (MIN_VALUE + (9 * RANGE_VALUE))) 		return HW_VER_12 ;
  else if (value < (MIN_VALUE + (10 * RANGE_VALUE)))		return HW_VER_6  ;
  else if (value < (MIN_VALUE + (11 * RANGE_VALUE)))		return HW_VER_13 ;
  else if (value < (MIN_VALUE + (12 * RANGE_VALUE)))		return HW_VER_3  ;
  else if (value < (MIN_VALUE + (13 * RANGE_VALUE)))		return HW_VER_14 ;
  else if (value < (MIN_VALUE + (14 * RANGE_VALUE)))		return HW_VER_7  ;
  else		                                                return HW_VER_15 ;
	#endif

}		

