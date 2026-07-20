/*==========================================================================*/
// $Id: dei_sc_check_frame_programming.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "dei_sc_private.h"
/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
#define     DEI_SC_MASTER_KEY           0xCB4195F222922334
/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
static Boolean dei_sc_remote_is_programmed     ( Dei_Rf_Frame *frame );
static Boolean dei_sc_frame_programming_changed(  Dei_Rf_Frame *f1 ,  Dei_Rf_Frame *f2 );  
/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

//--------------------------------------------------------------------------//

Dei_Sc_Button * dei_sc_check_frame_programming( Dei_Rf_Frame *frame )
{
	static Dei_Rf_Frame last_frame ; 

	if( !dei_sc_frame_programming_changed( &last_frame , frame)  ) return NULL ;
	
	last_frame = *frame ; 
	    
    if( ((frame->programming.sn&0xFFFFFFE)==0) || ((frame->programming.sn&0xFFFFFFE)==0xFFFFFFFE) ) 
    {
        TRACE_DEI_SC("\n\r[LIB.DEI_SC] invalid new sn %08X\n\r" , frame->programming.sn );
        
        return NULL ; 
    }

    if( ((frame->programming.pl&0xFE) != DEI_SC_TYPE_2B_SN) && (( frame->programming.pl&0xFE) != DEI_SC_TYPE_3B_SN) ) 
    {
        TRACE_DEI_SC("\n\r[LIB.NOTIFYOTE_DEI] invalid pl %02X\n\r" , frame->programming.pl );
        
        return NULL ;     
    }
    
    if( dei_sc_remote_is_programmed( frame ) )
    {
        TRACE_DEI_SC("\n\r[LIB.DEI_SC] sn already stored%08X\n\r" , frame->programming.sn );
        
        ((Dei_Sc_Button *)frame)->button = DEI_SC_BUTTON_PROGRAMMING_DONE ;     
                   
        return  ((Dei_Sc_Button *)frame) ;  
    }

    TRACE_DEI_SC("\n\r[LIB.DEI_SC] new remote stored at %02X\n\r" , dei_sc.index );
        
    compute_key:
    {
        UInt32 remote_key_msb , remote_key_lsb;
	
	    remote_key_lsb             =  (frame->programming.sn & 0x0FFFFFFF)|0x20000000 ;
	    remote_key_msb             =  (frame->programming.sn & 0x0FFFFFFF)|0x60000000 ;
			
	    remote_key_msb             =  keeloq_decrypt( remote_key_msb  , DEI_SC_MASTER_KEY );
	    remote_key_lsb             =  keeloq_decrypt( remote_key_lsb  , DEI_SC_MASTER_KEY );

        dei_sc.remote[dei_sc.index].key =  (((UInt64)remote_key_msb<<32)&0xFFFFFFFF00000000)|(((UInt64)remote_key_lsb>>0 )&0x00000000FFFFFFFF)	;    
    }
     
    dei_sc.remote[ dei_sc.index].sn      = frame->programming.sn         ;
    dei_sc.remote[ dei_sc.index].sn_type = frame->programming.pl&0xFE    ; 
    dei_sc.remote[ dei_sc.index].rf_type = frame->programming.pl&0x01    ; 
    dei_sc.remote[ dei_sc.index].count   = frame->programming.sync_count ; 

    dei_sc_current_sn = (dei_sc.remote[dei_sc.index].sn_type == DEI_SC_TYPE_3B_SN)?\
	                         dei_sc.remote[dei_sc.index].sn&0x00FFFFFF                    :\
                             dei_sc.remote[dei_sc.index].sn&0x0000FFFF                    ; 
													    
    
    
    sync_count_update( dei_sc_sync_count[dei_sc.index] , dei_sc.remote[dei_sc.index].count )  ;
        
    dei_rf_set_id( dei_sc.index , dei_sc_current_sn ) ; 
    
    
	((Dei_Sc_Button *)frame)->button = DEI_SC_BUTTON_PROGRAMMING_DONE ; 
	((Dei_Sc_Button *)frame)->id     = dei_sc.index                   ;

	
	dei_sc.index =( ++ dei_sc.index) % DEI_RF_ID_COUNT ; 
        
    return ((Dei_Sc_Button *)frame) ;
}

//--------------------------------------------------------------------------//

Boolean dei_sc_remote_is_programmed( Dei_Rf_Frame *frame )
{
    UInt8 i ; 
    
    for( i = 0 ; i < DEI_RF_ID_COUNT ; i ++ )
    {
        if( dei_sc.remote[i].sn == frame->programming.sn )    
        {
            dei_sc.remote[i].count = frame->programming.sync_count ; 

            sync_count_update( dei_sc_sync_count[i] , dei_sc.remote[i].count )  ;

            dei_sc_current_sn = (dei_sc.remote[i].sn_type == DEI_SC_TYPE_3B_SN) ? \
			                         dei_sc.remote[i].sn&0x00FFFFFF                     : \
									 dei_sc.remote[i].sn&0x0000FFFF                     ; 
        
            return TRUE ; 
        }
    }

    return FALSE ; 
}

//--------------------------------------------------------------------------//

Boolean dei_sc_frame_programming_changed(  Dei_Rf_Frame *f1 ,  Dei_Rf_Frame *f2 )  
{	 
	return (f1->programming.sn   		!= f2->programming.sn        )||\
		   (f1->programming.sync_count  != f2->programming.sync_count)||\
		   (f1->programming.pl       	!= f2->programming.pl        );
		   
}

//--------------------------------------------------------------------------//
