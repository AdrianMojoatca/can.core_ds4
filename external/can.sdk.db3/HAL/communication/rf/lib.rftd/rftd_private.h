/*==========================================================================*/
// $Id: rftd_private.h 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

#ifndef _RFTD_PRIVATE_H_
#define _RFTD_PRIVATE_H_

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "os.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
typedef union s_Rftd_Byte1
{
  struct
    {
      unsigned lock     : 1 ;
      unsigned armed    : 1 ;
      unsigned ignition : 1 ;
      unsigned parking  : 1 ;
      unsigned siren    : 1 ;
      unsigned valet    : 1 ;
      unsigned started  : 1 ;
      unsigned panic    : 1 ;
    }bit;

  UInt8 byte ;

}Rftd_Byte1;
//--------------------------------------------------------------------------//
typedef union s_Rftd_Byte2
{
  struct
   {
     unsigned door      : 1 ;
     unsigned trunk     : 1 ;
     unsigned hood      : 1 ;
     unsigned warn      : 1 ;
     unsigned shock     : 1 ;
     unsigned time      : 1 ;
     unsigned temp      : 1 ;
     unsigned temp_type : 1 ; //(0 = °F 	1 = °C)
   }bit ;

  UInt8 byte ;

}Rftd_Byte2;

//--------------------------------------------------------------------------//
typedef union s_Rftd_Byte3
{
   struct
   {
	 unsigned not_ready : 1 ;
     unsigned reserve   : 7 ;
   }bit ;

  UInt8 byte ;

}Rftd_Byte3;

//--------------------------------------------------------------------------//
typedef union s_Rftd_Byte4
{
  struct
    {
	  unsigned	tone  : 4 ;
	  unsigned	chirp : 4 ;

	}bit ;

   UInt8  byte ;

}Rftd_Byte4;
//--------------------------------------------------------------------------//
typedef struct s_Rftd
{
  volatile Rftd_Byte1 	byte1   ;
  volatile Rftd_Byte2 	byte2   ;
  volatile Rftd_Byte3 	byte3   ;
  volatile Rftd_Byte4 	byte4   ;

}Rftd ;


/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

void rftd_sound (void)		     ;
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
extern Rftd rftd            ;

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/


//--------------------------------------------------------------------------//

#endif




