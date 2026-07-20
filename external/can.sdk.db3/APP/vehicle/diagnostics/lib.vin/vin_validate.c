/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: vin_validate.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "vin_private.h"
 
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
 
static 	Boolean vin_check_digit_range(const Vin vin) ;
                              
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/
Boolean vin_validate (const Vin vin)            
{       
//   validate syntax and check digit : return status ;
  
  UInt8 index ;
  UInt16 mac  ;

  if(!vin_check_digit_range(vin))
  {
  	return FALSE ;
  }

  if(1 <= vin[0] && vin[0] <= 5)   // check digit does not necessary exist outside North America
    {
      mac = index = 0 ;

      do
      {
      	SInt8 vin_value = vin_check_digit(vin[index]) ;
    
    	if (vin_value >= 0)
    	{
    	   static const UInt8 weight[VIN_SIZE] = {8 , 7 , 6 , 5 , 4 , 3 , 2 , 10 , 0 , 9, 8 , 7 , 6 , 5 , 4 , 3 , 2} ;
    
    	   mac += weight[index] * vin_value ;
    	}
    	else
    	{
          vin_err_add (ERR_VIN_VALUE) ;
              
          return FALSE ;
    	}
      } 
      while(++index < VIN_SIZE) ;
     
     // If the numerical remainder is 10
    //then the Check Digit will be the Letter "X"
      
      if((mac%= 11) == 10)
      {
        mac = 'X' ;
      }
      else
      {
        mac += 0x30 ;
      }
    
      if(vin[8] != mac)
        {
          vin_err_add (ERR_VIN_CHECK_DIGIT) ;
          
          return FALSE ;
        }
    }

  return TRUE ; 
}

//----------------------------------------------------------------------------------------------//

static Boolean vin_check_digit_range (const Vin vin)
{
  UInt8 index = 0 ;

  do
  {
    if(!(('0' <= vin[index] && vin[index] <= '9') || ('A' <= vin[index] && vin[index] <= 'Z')))  return FALSE ;
  }
  while(++index < VIN_SIZE) ;

  return TRUE ;
}

