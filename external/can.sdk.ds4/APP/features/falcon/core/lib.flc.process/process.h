/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: process.h 14626 2012-05-24 11:26:47Z martin.bouchard $
/*==========================================================================*/

#ifndef __PROCESS_H__
#define __PROCESS_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "device.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*--------------------------------------------------------------------------*/

typedef  enum
{
    PROCESS_ID_00    = 1u<< 0 ,
    PROCESS_ID_01    = 1u<< 1 ,
    PROCESS_ID_02    = 1u<< 2 ,
    PROCESS_ID_03    = 1u<< 3 ,
    PROCESS_ID_04    = 1u<< 4 ,
    PROCESS_ID_05    = 1u<< 5 ,
    PROCESS_ID_06    = 1u<< 6 ,
    PROCESS_ID_07    = 1u<< 7 ,
    PROCESS_ID_08    = 1u<< 8 ,
    PROCESS_ID_09    = 1u<< 9 ,
    PROCESS_ID_10    = 1u<< 10,
    PROCESS_ID_11    = 1u<< 11,
    PROCESS_ID_12    = 1u<< 12,
    PROCESS_ID_13    = 1u<< 13,
    PROCESS_ID_14    = 1u<< 14,
    PROCESS_ID_15    = 1u<< 15,
    PROCESS_ID_16    = 1u<< 16,
    PROCESS_ID_17    = 1u<< 17,
    PROCESS_ID_18    = 1u<< 18,
    PROCESS_ID_19    = 1u<< 19,
    PROCESS_ID_20    = 1u<< 20,
    PROCESS_ID_21    = 1u<< 21,    
    PROCESS_ID_22    = 1u<< 22,
    PROCESS_ID_23    = 1u<< 23,
    PROCESS_ID_24    = 1u<< 24,
    PROCESS_ID_25    = 1u<< 25,
    PROCESS_ID_26    = 1u<< 26,
    PROCESS_ID_27    = 1u<< 27,
    PROCESS_ID_28    = 1u<< 28,
    PROCESS_ID_29    = 1u<< 29,
    PROCESS_ID_30    = 1u<< 30
}Process_Id ;

/*--------------------------------------------------------------------------*/

typedef struct s_Process_Node  Process_Node ;

typedef Boolean (* Process_Exec )( void* );

/*--------------------------------------------------------------------------*/

typedef enum
{
    PROCESS_SET_START           , 
    PROCESS_SET_EXEC            ,
    PROCESS_SET_STOP            ,
    PROCESS_SET_TIMEOUT         ,
    PROCESS_SET_ID              ,
    PROCESS_SET_BLOCKED_BY_ID   ,
    PROCESS_SET_SUSPENDED_BY_ID 
}Process_Set ; 

/*--------------------------------------------------------------------------*/

typedef struct s_Process_Cfg
{
    UInt8*          name         ;
    Process_Id      own_id       ;
     
    Process_Id      blocked_by   ;
    Process_Id      suspended_by ;

    Process_Exec    start        ;
    Process_Exec    stop         ;
    Process_Exec    exec         ;

    UInt32          timeout      ;    
}Process_Cfg ; 

/*--------------------------------------------------------------------------*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

Process_Node* process_create    ( UInt8 *name ) ;
void          process_set       ( Process_Node *process , Process_Set set , void *set_param );
Process_Node* process_create_cfg( Process_Cfg* const cfg ) ;
void          process_launch    ( Process_Node *process , UInt32 delay );

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/


/*==========================================================================*/

#endif



