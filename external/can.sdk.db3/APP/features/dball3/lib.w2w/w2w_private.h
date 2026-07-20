/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: w2w_private.h 33258 2015-08-19 12:46:35Z martin.bouchard $
/*==========================================================================*/

#ifndef __W2W_PRIVATE_H__
#define __W2W_PRIVATE_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "os.h"
#include "w2w.h"
#include "ied.h"
#include "per_exec.h"
#include "cmd.h"
#include "dbg.h"
#include "gpio.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
#if DBG_W2W == 1
  #warning "DBG_W2W Is Enable"
#endif



#define W2W_OFF  0 
#define W2W_ON   1

//--------------------------------------------------------------------------//
typedef enum
{
  W2W_ENA_GWR     ,
  W2W_ENA_IGN     ,
  W2W_ENA_START   ,
  W2W_ENA_POS1    ,
  W2W_ENA_LOCK    ,
  W2W_ENA_UNLOCK  ,
  W2W_ENA_TRUNK   ,
  W2W_ENA_AUX1    , 
  W2W_ENA_AUX2    ,
  W2W_ENA_AUX3    ,
  W2W_ENA_AUX4    ,
  W2W_ENA_SPARE1  ,
  W2W_ENA_SPARE2  ,
  W2W_ENA_SPARE3  ,
  W2W_ENA_SPARE4  ,
  W2W_ENA_SPARE5  ,

}W2W_Ena_Bit; 

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

Per_Exec_Wake   w2w_execute   (void);
Boolean         w2w_init_nvfs (void);
void            w2w_reset     (void);
void            w2w_set_table (UInt32 table) ;
void            w2w_set_level (void);

void            w2w_dbg_print (Cmd_List cmd) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

extern Ied ied_gwr      ;
extern Ied ied_ign      ;
extern Ied ied_start    ;
extern Ied ied_pos_in1  ;
extern Ied ied_lock     ;
extern Ied ied_unlock   ;
extern Ied ied_trunk    ;
extern Ied ied_aux1     ;
extern Ied ied_aux2     ;
extern Ied ied_aux3     ;
extern Ied ied_push     ;


extern       UInt32 w2w_ena_table   ;
extern const UInt32 w2w_ena_default ;
                    
/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif


