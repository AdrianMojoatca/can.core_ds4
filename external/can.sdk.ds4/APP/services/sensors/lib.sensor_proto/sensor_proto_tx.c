/*==========================================================================*/
// $Id: dei_sc_tx.c 33256 2015-08-19 12:44:53Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "sensor_proto_private.h"
#include "aid_dhp.h"
#include "status_system.h"
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

Sensor_Proto_Tx_Frame *frame  ;


void  sensor_proto_tx( UInt8 pld_num, UInt8 dest_aid, UInt8 sens_chanl , UInt8* pld, UInt8 pld_len)
{
    Sensor_Proto_Tx_Frame *frame  ;
	   void *ptr;

		frame = (Sensor_Proto_Tx_Frame *)q_remove( sensor_proto_tx_q_free , 0 ) ; 
		
    if( frame )
		{
			ptr = sensor_proto_payload_build(pld_num, dest_aid, sens_chanl, pld, pld_len);
			memcpy(frame->pld,ptr,pld_size);
			frame->pld_size	= pld_size;

			q_insert( sensor_proto_tx_q_pending , (Q_Msg *) frame ) ; 
		}			
}

//--------------------------------------------------------------------------//
// forget any bugfixes sent to ngss
void sensors_status_invalidate(void)
{
	UInt8 i;
	for(i=0;i<UNIT_MAX;i++)
	{
		if(aid_db[i].sensors) // only "DEI-" uuids can have this set
			aid_db[i].status &= ~(1<<AID_STATUS_BIT_ENABLED); // clear this bit
	}
}
//--------------------------------------------------------------------------//



void sensor_proto_arm(void)
{
	UInt8 i,mask;
	UInt8 buf[4]; // enough for name
		
	for(i=0;i<UNIT_MAX;i++)
	{
		if(aid_db[i].sensors && !(aid_db[i].status & (1<<AID_STATUS_BIT_ENABLED)))
		{	// aid_db[i].sensors is non-zero only for "DEI-" uuids
			nvfs_rd_at(unit_name(i),8,buf,4); // read rest of name
			if(memcmp(buf,"NGSS",4)==0)			// check if this is ngss
			{
				mask = 0x06; sensor_proto_tx(PLD_E1, aid_db[i].id, 1, &mask,1); // bugfix: enable SHOCK report bits
				os_tsk_wait(100);
				mask = 0x05; sensor_proto_tx(PLD_E1, aid_db[i].id, 2, &mask,1); // bugfix: enable TILT report bits
				os_tsk_wait(100);
			}
			aid_db[i].status |= (1<<AID_STATUS_BIT_ENABLED); // we've enabled this unit, no need to re-check
		}
	}
	
	
	mask = 0xFF;	sensor_proto_tx(PLD_E1, 0xFF, 0, &mask,1);
}

// send a reminder to sensor id if the system is armed
void sensor_proto_reminder(UInt8 unit)
{
UInt8 mask = 0xFF; // all possible statuses
					if(get_ss_arm() && !get_ss_valet())
						sensor_proto_tx(PLD_E1,aid_db[unit].id,0,&mask,1);
}
//--------------------------------------------------------------------------//

void sensor_proto_disarm(void)
{
	UInt8 alert_bit[1] = {0x00};
	sensor_proto_tx(PLD_E1, 0xFF, 0, alert_bit,1);
}


void sensor_proto_armdisarm(void)
{
	if(get_ss_arm())
		sensor_proto_arm();
	else
		sensor_proto_disarm();
}
