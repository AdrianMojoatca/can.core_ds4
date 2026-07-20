/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: config_firmware.h 82 2008-03-21 19:35:35Z martin.bouchard $
/*==========================================================================*/

//             <<< Use Configuration Wizard in Context Menu >>>             //

#ifndef __CONFIG_CAN_H__
#define __CONFIG_CAN_H__

#define CORE_CONFIG_CAN_VERSION_1_01

/*==========================================================================*/
/*                                N O T E S                                 */
/*==========================================================================*/
/* This file is included by 'device.h' and represents global compile-time   */
/* configuration parameters.  These parameters can be edited through the    */
/* configuration tool in the RealView IDE.                                  */
/*==========================================================================*/

//------------------------------------------------------------------------//
// <h> CAN0 CONFIGURATION(FT or SW)
// <q0> 1. FT/SW SELECTION
// <i>  FT = checked
// <i>  SW = unchecked
// <o1> 2. BAUD RATE  <0=>33.3K  <1=>50K  <2=>83.3  <3=>100K  <4=>125K	 <5=>250K  <6=>500K
// <o2> 3. RX BUFFER SIZE      <1-255>
// <o3> 4. TX BUFFER SIZE      <1-255>
// </h>
#define CAN_DEV_0_FT_SW                1   // 1 = FT     , 0 = SW
#define CAN_DEV_0_SPEED                6   // 
#define CAN_DEV_0_RX_BUFFER_SIZE       5
#define CAN_DEV_0_TX_BUFFER_SIZE       5

// <h> CAN1 CONFIGURATION(HS)
// <o0> 1.BAUD RATE  <0=>125K  <1=>250K  <2=>500K
// <o1> 2.RX BUFFER SIZE      <1-255>
// <o2> 3.TX BUFFER SIZE      <1-255>
// </h>
#define CAN_DEV_1_SPEED                0// 
#define CAN_DEV_1_RX_BUFFER_SIZE       5
#define CAN_DEV_1_TX_BUFFER_SIZE       5
//------------------------------------------------------------------------//

#endif
