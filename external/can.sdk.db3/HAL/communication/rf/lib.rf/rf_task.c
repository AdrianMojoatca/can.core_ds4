/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: rf_task.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "rf_private.h"
#include "rftd.h"
#include "rftd2.h"
#include "prog.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static void rf_process(Rf_Id rf_id) ;
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

void rf_task ( void )
{  
	if( os_res_request(rf_res_update , RF_RES_TIMEOUT) )
	{     
    	  SInt8 rf_option =	prog_retrieve_option(FEAT_RF_OUTPUT)  ;
         
		  if((rf_option = prog_retrieve_option(FEAT_RF_OUTPUT))!= -1 )
          { 
      		  rf_process((Rf_Id)rf_option);
          }

		os_res_release(rf_res_update) ;
	}
}

//--------------------------------------------------------------------------//

static void rf_process(Rf_Id rf_id) 
{
  switch (rf_id) // the first time enter = reset field only
    {
      case NO_RF   			:                   	break ;
			
      case RF_TD  			: rftd_update();    	break ;

	  case RF_TD2	    	: rftd2_update(RFTD2_UPDATE_UPON_CHANGE) ; break ;
			
      case RF_DTS_  		:   					break ;
			
      case RF_DEI_  		:    					break ;
			
      case RF_AST_  		:   					break ;
			
	  case RF_ATST_ 		: 	                	break ;
			
      default       		: 				    	break ;// error no id support added error to lib error 
    }
}
