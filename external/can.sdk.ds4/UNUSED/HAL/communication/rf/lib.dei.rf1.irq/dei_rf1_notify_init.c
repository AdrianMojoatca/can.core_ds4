/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dei_rf1_notify_init.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_rf1_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

//---------------------------------------------------------------------------//

static const Dei_Rf1_Notify_Table dei_rf1_notify_table[]=
{
	{ NOTIFY_CAR_FINDER        , (Notify_Func)dei_rf1_notify_car_finder      },	
    { NOTIFY_LOCK              , (Notify_Func)dei_rf1_notify_lock            },	
	{ NOTIFY_RS_START          , (Notify_Func)dei_rf1_notify_rs_on           },
	{ NOTIFY_ERROR             , (Notify_Func)dei_rf1_notify_rs_error        },
	{ NOTIFY_RS_STOP           , (Notify_Func)dei_rf1_notify_rs_off          },
	{ NOTIFY_UNLOCK            , (Notify_Func)dei_rf1_notify_unlock          },
	{ NOTIFY_PROGRAMMING_ENTER , (Notify_Func)dei_rf1_notify_programming_on  },
	{ NOTIFY_PROGRAMMING_LEAVE , (Notify_Func)dei_rf1_notify_programming_off },
	{ NOTIFY_REMOTE_DELETE     , (Notify_Func)dei_rf1_notify_remote_delete   },
	{ NOTIFY_RANGE_CHECK       , (Notify_Func)dei_rf1_notify_car_finder      },	
};

//---------------------------------------------------------------------------//

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//---------------------------------------------------------------------------//

void dei_rf1_notify_init (void)
{    
	UInt16 i ; 
	
	for( i = 0 ; i < sizeof( dei_rf1_notify_table )/sizeof( struct s_Dei_Rf1_Notify_Table ) ; i++ )
	{
		notify_add( dei_rf1_notify_table[i].nf , dei_rf1_notify_table[i].func );
	}	    
} 

//---------------------------------------------------------------------------//
