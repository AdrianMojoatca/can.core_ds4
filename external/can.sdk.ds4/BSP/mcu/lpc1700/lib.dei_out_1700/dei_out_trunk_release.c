/*==========================================================================*/
// $Id: dei_out_trunk_release.c 17934 2012-10-29 19:23:36Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_out_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
//static void dei_out_trunk_release_timeout( Boolean timeout_expired );
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
Boolean dei_out_trunk_release_cmd;
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

void dei_out_trunk_release_on( void  *payload )
{
  #if 0
    if(DEI_OUT_PAYLOAD( payload ) & CMD_TRUNK_WITH_DISARM)
    {
        out_q_remove( out_q_main , (Out_Func)dei_out_trunk_release_timeout , FALSE ); 
        prg_out_trunk_release( TRUE );
        if(dei_feature_get(DEI_FEAT_CH2_OUTPUT_TYPE) == DEI_TRUNK_1SEC_PULSE)
        {
					out_q_insert( out_q_main , (Out_Func)dei_out_trunk_release_timeout , 1*86 , 1 , 1 , 1 ) ; 
        }
        else  //CUSTOM_FEATURE_O2: DEI_TRUNK_CONSTANT_OUTPUT
        {
					out_q_insert( out_q_main , (Out_Func)dei_out_trunk_release_timeout , 1 , 80 , 1 , 1 ) ; 
        }
    }
#endif    
  	dei_out_trunk_release_cmd = TRUE;
    
    if( DEI_OUT_PAYLOAD( payload ) & CMD_TRUNK_WITH_DISARM )
    {
        if(dei_feature_get(DEI_FEAT_CH2_OUTPUT_TYPE) == DEI_TRUNK_1SEC_PULSE)
        {
            DEI_OUT_TRUNK_RELEASE( 50 ,1*86 , 1 , 1 ) ;
        }
        else  //CUSTOM_FEATURE_O2: DEI_TRUNK_CONSTANT_OUTPUT
        {
					  if(get_cmd_source_d2d())
						{
							set_cmd_source_d2d(FALSE);
							DEI_OUT_TRUNK_RELEASE( 50 ,1*86 , 1 , 1 ) ;
						}
						else
              DEI_OUT_TRUNK_RELEASE( 50 ,26*100 , 1 , 1 ) ;    
        }
    }
    else
    {
        DEI_OUT_TRUNK_RELEASE( 2 , 30*100 , 1 , 1 ) ;   
    }           
}

//--------------------------------------------------------------------------//

void dei_out_trunk_release_off( void *payload )
{
#if 0  
	if( dei_out_trunk_release_cmd && (dei_feature_get(DEI_FEAT_CH2_OUTPUT_TYPE) != DEI_TRUNK_1SEC_PULSE ))
	 {
		 dei_out_trunk_release_cmd = FALSE;
  		prg_out_trunk_release( FALSE );
	 }
#endif
	if( (dei_feature_get(DEI_FEAT_CH2_OUTPUT_TYPE) == DEI_TRUNK_1SEC_PULSE) && dei_out_trunk_release_cmd )
	 {
		 dei_out_trunk_release_cmd = FALSE;
			os_tsk_wait( 800 ); 		// 800ms delay to finish 1 second pulse on trunk release output
	 }

    DEI_OUT_TRUNK_RELEASE_RESET();   
}

#if 0
//--------------------------------------------------------------------------//  

void dei_out_trunk_release_timeout( Boolean timeout_expired )
{
	dei_out_trunk_release_cmd = TRUE;
	
	if( timeout_expired && (dei_feature_get(DEI_FEAT_CH2_OUTPUT_TYPE) == DEI_TRUNK_1SEC_PULSE ))
	{
		prg_out_trunk_release( FALSE );
	}
}
#endif
