/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: d2d.h 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

#ifndef __D2D_H__
#define __D2D_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "os.h"
#include "d2d_code.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

#define D2D_OFF  0
#define D2D_ON   1

typedef struct s_D2D_Entry D2D_Entry ;

typedef void    (* D2D_Lut_Func) (void) ;

typedef void    (* D2D_Tx_Func       )  (UInt8 code) ;
typedef void    (* D2D_Tx_Frame_Func )  (UInt8      * data  , UInt8 size) ;
typedef Boolean (* D2D_Get_Func     )  (UInt8 code) ;
 
/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

void    d2d_init            (void)                              ;
void    d2d_init_reset      (void)                              ;
    
int     d2d_rx              (TskTimeout   timeout)              ;                                          
void    d2d_tx              (UInt8 code)                        ;
void    d2d_tx_frame        (UInt8      * data  , UInt8 size)   ;
       
void    d2d_lut_insert      (UInt8 d2d_code, D2D_Lut_Func func) ;
                                                                
Boolean d2d_get_mask        (UInt8 code)                        ;
Boolean d2d_get_ena         (UInt8 code)                        ;

void    d2d_set_get_mask_func   (D2D_Get_Func func) ;
void    d2d_set_get_ena_func    (D2D_Get_Func func) ;                                                               
void    d2d_tx_set_func         (D2D_Tx_Func  func) ;
void    d2d_tx_frame_set_func   (D2D_Tx_Frame_Func func) ;

void    d2d_insert_d2d_code     (UInt8 code)    ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

extern const D2D_Entry input_table[] ;

extern const UInt8 INPUT_NUM_FUNC    ;

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/

/*==========================================================================*/

#endif

