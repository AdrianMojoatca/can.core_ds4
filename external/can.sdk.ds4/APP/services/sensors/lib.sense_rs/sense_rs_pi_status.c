/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: sense_rs_pi_status.c 33405 2015-08-26 21:21:53Z suchita.yadav $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "sense_rs_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static Boolean sense_rs_status_neg1(void){ return IN_NEGATIVE_1;} 
static Boolean sense_rs_status_neg2(void){ return IN_NEGATIVE_2;}
static Boolean sense_rs_status_neg3(void){ return IN_NEGATIVE_3;} 
static Boolean sense_rs_status_neg4(void){ return IN_NEGATIVE_4;} 
static Boolean sense_rs_status_neg5(void){ return IN_NEGATIVE_5;} 

#ifdef DEV_LPC_236x		// TODO : need to add new configuration
static Boolean sense_rs_status_neg6(void){ return IN_NEGATIVE_6;} 
#else
static Boolean sense_rs_status_neg6(void){ return TRUE;} 
#endif
static Boolean sense_rs_status_neg7(void){ return IN_NEGATIVE_7;} 
                                                         
static Boolean sense_rs_status_pos1(void){ return IN_POSITIVE_1;} 
static Boolean sense_rs_status_pos2(void){ return IN_POSITIVE_2;} 
static Boolean sense_rs_status_pos3(void){ return IN_POSITIVE_3;}
 
static Boolean sense_rs_status_np1 (void){ return IN_NEG_AND_POS1;} 
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*--------------------------------------------------------------------------*/


/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

/*--------------------------------------------------------------------------*/

Boolean sense_rs_pi_status( Sense_Rs_Input_Selector selector )
{
#ifdef PLATFORM_CM800
    static const Sense_Rs_Input_Read in_status[SENSE_RS_IN_COUNT] = 
    {
        sense_rs_status_neg1 , sense_rs_status_neg2 , sense_rs_status_neg3 , sense_rs_status_neg4 , sense_rs_status_neg5 ,
        sense_rs_status_neg6 , sense_rs_status_neg7 ,
    
        sense_rs_status_pos1 , sense_rs_status_pos2 , sense_rs_status_pos3 , 
    
        sense_rs_status_np1
    };
#else
    static const Sense_Rs_Input_Read in_status[SENSE_RS_IN_COUNT] = 
    {
        sense_rs_status_neg1 , sense_rs_status_neg3 , sense_rs_status_neg4 , sense_rs_status_neg5 ,
        sense_rs_status_neg7 ,
    
        sense_rs_status_pos1 , sense_rs_status_pos2 , sense_rs_status_pos3 , 
    
        sense_rs_status_np1
    };
#endif
    
    UInt8 function = sense_rs_input_config->inputs[selector];
    
    if( selector < SENSE_RS_IN_COUNT )
    {
        return (in_status[selector])();
    }

    return FALSE ;
}


/*--------------------------------------------------------------------------*/

