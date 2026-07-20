/** 
 * @file   aid_dhp.h
 * @brief  
 * author  DIRECTED ELECTRONICS
 * @date   10/16/2015
 */

#ifndef __AID_DHP_H__
#define __AID_DHP_H__

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/
#include "device.h"
#include "aes128.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
#define UUID_SIZE 16

// nvfs information kept for each unit

#define UNIT_MAX                 6

#define AID_STATE_CLOSED      0
#define AID_STATE_CLOSED_WTMR 1
#define AID_STATE_SCANNING    2
#define AID_STATE_ADDING      3
#define AID_STATE_ERASING     4

#define AUTH_SIZE sizeof(UInt32)
// information kept in RAM for each unit
struct aid_entry
{
	UInt8 id;
	UInt8 status; // status bits, see below
	UInt8 sensors;  //adjustable sensor number
	UInt8 prev_auth[AUTH_SIZE];
	UInt8 other_auth[AUTH_SIZE];
};
extern struct aid_entry aid_db[UNIT_MAX];
extern UInt8  sort_slot[UNIT_MAX];

extern UInt8 master_prev_auth[];
// bit numbers in aid_db[].status
#define AID_STATUS_BIT_UUID      0
#define AID_STATUS_BIT_KEY       1
#define AID_STATUS_BIT_CONNECTED 2
#define AID_STATUS_BIT_OFFER     3

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/
void aid_process(UInt8 *packet,UInt8 len);
void send_mine(void);
void dhp_powerup(void);
void dhp_pairmode(void);
void dhp_eraseall(void);


char *unit_name(UInt8 i);
UInt8 aid_lookup(UInt8 id);
UInt8 uuid_lookup(UInt8 *uuid);


void dh_random(void);

/* these 2 routines are because we are a little-endian machine */
UInt16 htons(UInt16 x);
UInt32 htonl(UInt32 x);
#define ntohs htons
#define ntohl htonl

extern UInt8 broadcastkey[KEY_SIZE];

void dhp_process(UInt8 *packet, UInt8 len);
void master_fail_process(UInt8 *pkt,UInt8 len);
UInt8 tempsens_aid(UInt8 e_num);
UInt8 check_aid_info(UInt8 aid);

void sensor_sort(void);
unsigned sensor_next(UInt8 *idx, UInt8 *ptr_aid, UInt8 *ptr_chan);
unsigned sensor_number(UInt8 *ptr_aid, UInt8 *ptr_chan);


void auth_generate(UInt8 *key,UInt8 *pkt, unsigned len,UInt8 *prev_auth); // adds auth to packet and updates prev_auth
int auth_verify(UInt8 *pkt,unsigned len); // returns true if packet is authentic.  also updates the sender's aid_db[] entry

void aid_antiprocess(UInt8 id);

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

#endif
