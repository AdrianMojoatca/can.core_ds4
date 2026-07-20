/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: main.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "os.h"
#include "sys.h"
#include "gpio.h"
#include "wake.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

//--------------------------------------------------------------------------//

#define TASK_MAIN_NAME  "main"
#define TASK_MAIN_PRI   3      
#define TASK_MAIN_STK   0x0380

//--------------------------------------------------------------------------//

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//#define DO_ERASE_BOOT
#ifdef DO_ERASE_BOOT

#define   CMD_PREPARE_SECTORS_FOR_WR 50
#define   CMD_ERASE_SECTORS          52
typedef UInt32 Iap_Status;

void iap_execute (UInt32 i_args [] , UInt32 o_args [])
{
  ATOMIC_ALL (((void (*)(UInt32 * , UInt32 *)) 0x1FFF1FF1) (i_args , o_args)) ;
}


Iap_Status iap_sector_prepare (const UInt8 sector_start , const UInt8 sector_end)
{
  UInt32 arg_i [5] , arg_o [5] ;

  arg_i [0] = CMD_PREPARE_SECTORS_FOR_WR ;
  arg_i [1] = sector_start               ;
  arg_i [2] = sector_end                 ;

  iap_execute (arg_i , arg_o) ;

  return (Iap_Status) arg_o [0] ;
}
Iap_Status iap_sector_erase (const UInt8 sector_start , const UInt8 sector_end)
{
  UInt32 arg_i [5] , arg_o [5] ;

  arg_i [0] = CMD_ERASE_SECTORS  ;
  arg_i [1] = sector_start       ;
  arg_i [2] = sector_end         ;
  arg_i [3] = CPU_CLOCK_HZ /1000 ;
                           
  iap_execute (arg_i , arg_o) ;

  return (Iap_Status) arg_o [0] ;
}

void erase_boot(void)
{
	CRITICAL_ALL;
	C_ENTER_ALL;
	{
	iap_sector_prepare(0,0);
	iap_sector_erase(0,0);
	}
	C_LEAVE_ALL;
}

#endif // DO_ERASE_BOOT

int main (void)
{
	gpio_init();
	wake_clock_init();
	sys_init() ;
	os_sys_idle_func = sys_idle_func ;

#ifdef DO_ERASE_BOOT	
	erase_boot();
#endif
	os_execute (task_main , TASK_MAIN_PRI , TASK_MAIN_STK , 0 , TASK_MAIN_NAME , 0 , OS_STK_SIZE) ;
}

