/*==========================================================================*/
// $Id: dei_sc_check_frame_normal.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "dei_sc_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/
static Boolean dei_sc_frame_normal_changed(  Dei_Rf_Frame *f1 ,  Dei_Rf_Frame *f2 ) ; 
/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

#define SN0_STORED          ( dei_sc.remote[id].sn     &0x0000FF)
#define SN1_STORED          ((dei_sc.remote[id].sn>>8 )&0x0000FF)
#define SN2_STORED          ((dei_sc.remote[id].sn>>16)&0x0000FF)
#define KEY_STORED          ( dei_sc.remote[id].key             )
#define SC_STORED           ( dei_sc.remote[id].count           )
#define SN_TYPE_STORED      ( dei_sc.remote[id].sn_type         )

#define SN0_RECV            (frame->normal.sn0           )
#define SN1_RECV            (sn1                         )
#define SN2_RECV            (frame->normal.pl0           )
#define SC_RECV             (sc                          )
#define CMD_RECV            (cmd                         )
#define PL_RECV             (frame->normal.pl0           )       
#define PRESS_TYPE_RECV     (frame->normal.press_type    )       

#define CMD_IS_WITHOUT_PAYLOAD  ( (CMD_RECV!=0xB7)&&(CMD_RECV!=0xE0)&&(CMD_RECV!=0xE2)&&(CMD_RECV!=0xE4) )
#define CMD_IS_AVAILABLE        ( (CMD_RECV>=0xA0)&&(CMD_RECV<=0xE5) )

//--------------------------------------------------------------------------//


Dei_Sc_Button * dei_sc_check_frame_normal( Dei_Rf_Frame *frame )
{
    UInt32 decrypt; 
    UInt8  sn1 , cmd , batt_low , press_type , pl0 ;
    UInt16 sc ;
    UInt8 id  ;

	static Dei_Rf_Frame last_frame ; 

	if( !dei_sc_frame_normal_changed( &last_frame , frame)  ) return NULL ;
	
	last_frame = *frame ; 
		   
    for( id = 0 ;  id < DEI_RF_ID_COUNT ; id ++ )
    {
        decrypt = keeloq_decrypt( frame->normal.encrypt , KEY_STORED ) ; 

        cmd = decrypt >> 16 ; 
        sn1 = decrypt >> 24 ; 
        sc  = decrypt       ;
        
        pl0 = frame->normal.pl0               ;
        
        batt_low   = frame->normal.batt_low   ; 
        press_type = frame->normal.press_type ; 


        if( SN1_RECV != SN1_STORED )                                                                     
        {
            TRACE_DEI_SC("\n\r[LIB.DEI_SC] invalid SN1 0x%02X\n\r", id ) ; 
            
            goto hhu_frame_error ;
        }
                        
        if((SN_TYPE_STORED == DEI_SC_TYPE_3B_SN) && CMD_IS_WITHOUT_PAYLOAD && (SN2_RECV != SN2_STORED )) 
        {
            TRACE_DEI_SC("\n\r[LIB.DEI_SC] invalid SN2 0x%02X\n\r", id ) ; 
            
            goto hhu_frame_error ;
        }
        
        if((SN_TYPE_STORED != DEI_SC_TYPE_3B_SN) && CMD_IS_WITHOUT_PAYLOAD && (SN2_RECV != 0xFF       )) 
        {
            TRACE_DEI_SC("\n\r[LIB.DEI_SC] invalid SN3 ( !=0xFF ) 0x%02X\n\r", id ) ; 
            
            goto hhu_frame_error ;
        }
                
        if( !CMD_IS_AVAILABLE )
        {
            TRACE_DEI_SC("\n\r[LIB.DEI_SC] invalid command 0x%02X\n\r", id ) ; 
                    
            goto hhu_frame_error ;
        }

        
        if(  ( SC_RECV  == SC_STORED ) && ( PRESS_TYPE_RECV) ) 
        {
            goto hhu_frame_ok    ;
        }
                
        if( !sync_count_check( dei_sc_sync_count[id] , SC_RECV )                    ) 
        {
            TRACE_DEI_SC("\n\r[LIB.DEI_SC] invalid sync count 0x%02X\n\r", id ) ; 

            return NULL ;
        }
        else
        {
            dei_sc.remote[id].count = SC_RECV ;

            goto hhu_frame_ok ;
        };

        hhu_frame_error :
		{
			;
		}
        
    }

    if( id == DEI_RF_ID_COUNT ) return NULL ; 

	TRACE_DEI_SC("\n\r[LIB.DEI_SC] frame ok 0x%02X\n\r", id ) ; 

    hhu_frame_ok :
        
    dei_sc_current_sn = (dei_sc.remote[id].sn_type == DEI_SC_TYPE_3B_SN) ? \
	                         dei_sc.remote[id].sn&0x00FFFFFF : dei_sc.remote[id].sn&0x0000FFFF        ;
    
    hhu_frame_cast :

    ((Dei_Sc_Button*)frame)->button     = (UInt16)(cmd-0xA0)             ; 
    ((Dei_Sc_Button*)frame)->pl         = pl0                            ;
    ((Dei_Sc_Button*)frame)->id         = id                             ;    
    ((Dei_Sc_Button*)frame)->rf_type    = dei_sc.remote[id].rf_type  ;
    ((Dei_Sc_Button*)frame)->batt_low   = batt_low             		     ;
	((Dei_Sc_Button*)frame)->press_type = press_type                   	 ;

    return ((Dei_Sc_Button*)frame) ;
}


//--------------------------------------------------------------------------//

Boolean dei_sc_frame_normal_changed(  Dei_Rf_Frame *f1 ,  Dei_Rf_Frame *f2 )  
{	 
	return (f1->normal.encrypt   != f2->normal.encrypt   ) || \
		   (f1->normal.sn0       != f2->normal.sn0       ) || \
		   (f1->normal.pl0       != f2->normal.pl0       ) || \
		   (f1->normal.batt_low  != f2->normal.batt_low  ) || \
		   (f1->normal.press_type!= f2->normal.press_type) ;
}

//--------------------------------------------------------------------------//

#undef SN0_STORED          
#undef SN1_STORED          
#undef SN2_STORED          
#undef KEY_STORED          
#undef SC_STORED           
#undef TYPE_STORED         


#undef SN0_RECV            
#undef SN1_RECV            
#undef SN2_RECV            
#undef SC_RECV             
#undef CMD_RECV            
#undef PL_RECV             
#undef PRESS_TYPE_RECV     


#undef CMD_IS_WITHOUT_PAYLOAD  
#undef CMD_IS_AVAILABLE        


//---------------------------------------------------------------------------//

