/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: can_usr.h 19497 2013-02-25 13:41:22Z martin.bouchard $
/*==========================================================================*/

#ifndef __CAN_USR_H__
#define __CAN_USR_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "can.h"
#include "reg.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*--------------------------------------------------------------------------*/

typedef enum 
{
	CAN0_ENABLE           = 1<<0 , 
	CAN0_RX_BUFFER_ENABLE = 1<<1 ,
	CAN0_TX_BUFFER_ENABLE = 1<<2 ,

	CAN1_ENABLE           = 1<<3 , 
	CAN1_RX_BUFFER_ENABLE = 1<<4 ,
	CAN1_TX_BUFFER_ENABLE = 1<<5 ,    
}Can_Usr_Setup ;

/*--------------------------------------------------------------------------*/

typedef struct s_Can_Usr_Array1
{
    Reg_Func func   ; 
    UInt32   can_id ;

}Can_Usr_Array1 ;

#define CAN_USR_REG1(id ,x)     do\
                                {\
                                    UInt8 i;\
                                    for(i=0;i<sizeof_array(x);i++)\
										can_usr_rx_reg_slow(id,x[i].func,x[i].can_id);\
                                }while(0)

/*--------------------------------------------------------------------------*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
void    can_usr_init        ( Can_Usr_Setup setup );
void    can_usr_rx_reg_fast ( Can_Dev_Id dev_id ,  Reg_Func rx_quick_func );
void    can_usr_rx_reg_slow ( Can_Dev_Id dev_id ,  Reg_Func rx_slow_func , UInt32 can_id                          );
Boolean can_usr_tx          ( Can_Dev_Id dev_id ,  Can_Msg *can_msg      , Boolean do_it_fast , TskTimeout timeout);
                                
void    can_usr_rx_filter_add    (Can_Dev_Id dev_id , UInt32 can_id);
void    can_usr_rx_filter_enable (Can_Dev_Id dev_id );
void    can_usr_rx_filter_disable(Can_Dev_Id dev_id );                               
                                
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/               

#endif


