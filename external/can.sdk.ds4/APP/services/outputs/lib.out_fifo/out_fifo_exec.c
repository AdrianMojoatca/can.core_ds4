/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: out_fifo_exec.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "out_fifo_private.h"
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

//--------------------------------------------------------------------------//

Boolean out_fifo_exec( void *dummy , Out_Fifo out_fifo )
{
    #define OUT_FIFO_NODE	((Out_Fifo_Node*)node)
	
	#define T_OFFSET		OUT_FIFO_NODE->frame.t_offset	
	#define T_ON			OUT_FIFO_NODE->frame.t_on
	#define T_OFF			OUT_FIFO_NODE->frame.t_off
	#define PULSE           OUT_FIFO_NODE->frame.pulse  
	
	Fifo_Node *node ;
    
    _out_fifo_exec :
    {    
        if( out_exec( NULL , out_fifo->out ) )
        {        
            node = fifo_remove( out_fifo->q_p );
    
            if( !node ) return TRUE ; //frame is complete and pennding fifo is empty
         
            if( (OUT_FIFO_NODE->frame_count == OUT_FIFO_LOOP ) ||( OUT_FIFO_NODE->frame_count-- ) )
            {
                out_set( out_fifo->out , OUT_FIFO_NODE->out_func );
				
				out_exec_start( out_fifo->out   , T_OFFSET , T_ON , T_OFF , PULSE ) ;
    
                fifo_insert( out_fifo->q_p , (Fifo_Node *)node );
            }
            else
            {         
                //frame expired///            
				fifo_insert( out_fifo->q_f , (Fifo_Node *)node );                          
            }  
            
            goto _out_fifo_exec ;      
        }
    }

    return FALSE ; 

	#undef OUT_FIFO_NODE 

	#undef T_OFFSET
	#undef T_ON
	#undef T_OFF
	#undef PULSE
}
        
//--------------------------------------------------------------------------//

