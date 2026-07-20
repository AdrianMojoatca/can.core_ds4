/*==========================================================================*/
// $Id: status_mileage.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "status_private.h"

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
Boolean status_set_mileage (UInt32 value, Status_Mileage status, Status_Src source)
{
  //if(source == status_entry.updater[STATUS_MILEAGE])
	{	
	  switch(status)
		{
          case STATUS_MILEAGE_DWORD    : 	mileage.dword   = value ;   break ;
          case STATUS_MILEAGE_WORD_HI  :	mileage.word[1] = value ;	break ;
          case STATUS_MILEAGE_WORD_LO  :	mileage.word[0] = value ;	break ;
          case STATUS_MILEAGE_BYTE_HI  :	mileage.byte[2] = value ;	break ;
          case STATUS_MILEAGE_BYTE_MED :	mileage.byte[1] = value ;	break ;
          case STATUS_MILEAGE_BYTE_LO  :    mileage.byte[0] = value ;	break ;      
		}

		status_entry.is_valid[STATUS_MILEAGE] = TRUE ;

		return TRUE ;
	}

//	return FALSE ;
}

//--------------------------------------------------------------------------//

SInt32 status_get_mileage (Status_Mileage status)
{
	//if(status_entry.is_valid[STATUS_MILEAGE])
	{
		switch(status)
		{
			case STATUS_MILEAGE_DWORD    : 	return(mileage.dword) ;
			case STATUS_MILEAGE_WORD_HI  :	return(mileage.word[0]) ;
			case STATUS_MILEAGE_WORD_LO  :	return(mileage.word[1]) ;
			case STATUS_MILEAGE_BYTE_HI  :	return(mileage.byte[2]) ;
			case STATUS_MILEAGE_BYTE_MED :	return(mileage.byte[1]) ;	
			case STATUS_MILEAGE_BYTE_LO  :	return(mileage.byte[0]) ;	      
		}
	}
	
	return -1 ;	
}

//--------------------------------------------------------------------------//

