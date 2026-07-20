/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: 15282 2012-06-12 13:29:20Z louis-philippe.rispoli $
/*==========================================================================*/


/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "fw.h"
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

/*--------------------------------------------------------------------------*/

void fw_dbg( int cmd )
{
	switch(cmd)
    {
        case 'l' : cmd_q_insert(INPUT_LOCK1      |INPUT_ON,0);break;
        case 'u' : cmd_q_insert(INPUT_UNLOCK_ALL1|INPUT_ON,0);break;
        case 't' : cmd_q_insert(INPUT_TRUNK      |INPUT_ON,0);break;
        
        case '1' : cmd_q_insert(INPUT_GWR        |INPUT_ON,0);break;
        case '2' : cmd_q_insert(INPUT_RS_IGNITION|INPUT_ON,0);break;
        case '3' : cmd_q_insert(INPUT_RS_STARTER |INPUT_ON,0);break;
        
        case '4' : cmd_q_insert(INPUT_RS_STARTER          ,0);break;
        case '5' : cmd_q_insert(INPUT_RS_IGNITION         ,0);break;
        case '6' : cmd_q_insert(INPUT_GWR                 ,0);break;
        
        case 'b' : 
        {
            static Boolean status = FALSE;
            
            UInt8 i = 0 ; 
            
            for( i = 0 ; i < 10 ; i ++)
            {
                sense_rs_brake_can(!status);
            }
            
            status = !status ;
        }
        break;
        
        case 'i':
        {
            static Boolean status = FALSE;
            
            UInt8 i = 0 ; 
            
            for( i = 0 ; i < 10 ; i ++)
            {
                sense_rs_ign_can(!status);
            }
            
            status = !status ;
        }
        break;
        
        
        case 'd':
        {
            static Boolean status = FALSE;
            
            sense_rs_other_doors_can(!status);
            
            status = !status ;
        }
        break;
        
         
        default : break;
    }
}

/*--------------------------------------------------------------------------*/

