#include "aid_dhp.h"
#include "nvfs.h"
#include "nvfs_usr.h"

#include <string.h>



UInt8 sensadj_slot_count;  // number of registered devices having sensors
UInt8 sensadj_channel_count; // total number of channels in all devices
UInt8 sensadj_current_index = 0; // 0=off, else 1..sensadj_channel_count



// called at the start of sensor adjust mode
void sensadj_sort(void)
{
	UInt32 ser[UNIT_MAX];
	UInt32 serial;
	unsigned i,j,k;
	
	memset(sort_slot,0xFF,UNIT_MAX);
	sensadj_channel_count = 0;
	sensadj_slot_count = 0;
	for(i=1;i<UNIT_MAX;i++)
	{
		if(!(aid_db[i].status & (1<<AID_STATUS_BIT_UUID)) || aid_db[i].sensors==0)
			continue;  // device in this slot has no recognizable sensors

		nvfs_rd_at(unit_name(i), 0, &serial, sizeof(UInt32)); // read serial number only
		serial = htonl(serial);

		for(j=0;j<sensadj_slot_count;j++)
			if(serial < ser[j])
				break;  // found insertion spot
			
		// insert slot at position j
		for(k=sensadj_slot_count;k>j;k--)
		{
			sort_slot[k] = sort_slot[k-1];
			ser[k] = ser[k-1];
		}
		sort_slot[j] = i;
		ser[j] = serial;
		sensadj_slot_count++;
		sensadj_channel_count += aid_db[i].sensors;
	}	
}


// get the index number >0 for this id, channel  (it is the number of LED flashes to display)
UInt8 sensadj_get_index(UInt8 aid, UInt8 channel)
{
	UInt8 base, ss, i;

  if(channel == 0)
    return 0;  // channel 0 is never adjustable
	
	base = 0;
	for(i=0;i<sensadj_slot_count;i++)
	{
		ss = sort_slot[i];
		if(aid_db[ss].id != aid)
		{
			base += aid_db[ss].sensors;
			continue;
		}
		if(channel <= aid_db[ss].sensors)
		{
			return base + channel;  // a non-zero number
		}
	}
	return 0; // this id,channel is not found
}

// get the index number >0 for this id, channel  (it is the number of LED flashes to display)
UInt8 sensadj_get_current_index(void)
{
	return sensadj_current_index;
}


// get the id and channel currently in test mode
Boolean sensadj_get_current_idchan(UInt8 *pid, UInt8 *pchannel)
{
UInt8 i,ss,index;
	
	index = sensadj_current_index;
	
	
	for(i=0;i<sensadj_slot_count;i++)
	{
		ss = sort_slot[i];
		if(index <= aid_db[ss].sensors)
		{
			if(index==0)
				break;
			
			// found
			*pid = aid_db[ss].id;
			*pchannel = index;
			
			if(*pid == 0)
				*pid = 0x1F;  // LONG-TERM FIXME!  use aid 0x1F to represent every device which is not currently connected
			return TRUE;
		}
		index -= aid_db[ss].sensors;
	}
	*pid = 0;
	*pchannel = 0;
	return FALSE;
}


// advance a number of channels
Boolean sensadj_advance(UInt8 number_of_steps)
{
	if(sensadj_current_index)
	{
		sensadj_current_index += number_of_steps;
		while(sensadj_current_index > sensadj_channel_count)
			sensadj_current_index -= sensadj_channel_count;
	}
	return !!sensadj_current_index;
}


// begin testmode with the first sensor device/channel
Boolean sensadj_entertest(void)
{
	sensadj_sort();
	if(sensadj_channel_count)
		sensadj_current_index = 1;
	return sensadj_current_index != 0;	
}
// begin testmode with a specific device/channel 
Boolean sensadj_entertest_idchan(UInt8 id, UInt8 channel)
{
	if(sensadj_entertest())
		sensadj_current_index = sensadj_get_index(id,channel); // try to set for this channel
	return sensadj_current_index != 0;
}


Boolean sensadj_exittest(void)
{
	UInt8 result;
	
	result = !!sensadj_current_index;
	sensadj_current_index = 0; // not testing anymore
	return result;  // true if we were previously testing
}


#if 0

// Given a index , ID and channel number, get the next sensor details and return the sensor number
// note: if ID==0 then first sensor device and channel will be set
//       if chan==0 then first sensor on that device will be set (if it exists)
//
unsigned sensor_next(UInt8 *idx, UInt8 *ptr_aid, UInt8 *ptr_chan)
{
unsigned num,cnt,aid;
unsigned i;

	aid = *ptr_aid;
	for (;;) // this is a loop which is repeated at most 1 time
	{
		num = 0; // each go-around (re)starts at zero
		// start (again) at the top of the list
		for (i = 0; i < UNIT_MAX; i++)
		{
			if(sort_slot[i] == 0xFF)
			{
				*idx = 0;
				break;
			}
			cnt = aid_db[sort_slot[i]].sensors;
			if (cnt && (i>= *idx) )
			{// check for status for the last found sensor device index or next ones
				if( aid_db[sort_slot[i]].status & (1<<AID_STATUS_BIT_CONNECTED) )
					aid = aid_db[sort_slot[i]].id; // select this device
			}
			if ( aid == aid_db[sort_slot[i]].id )
			{// found device
				if (*ptr_chan < cnt)
				{
					*ptr_aid = aid;
					(*ptr_chan)++; // bump caller's channel number
					*idx = i;
					return  num + *ptr_chan;
				}
				// channel number was too big, they really want next device
				aid = 0xFF; // ask for next device
				*ptr_chan = 0; // make sure we pick first sensor
			}
			num += cnt;
		}
		if (num == 0)
			break; // there are no sensors at all!
		aid = 0xFF; // go around again, but stop on first device with sensors
		*idx = 0; //wrap
	}
	*ptr_aid = 0;
	*ptr_chan = 0;
	return 0; // no such sensor
}

// This API is only needed by the test, the product should not need it
// given an ID and a channel number, tell which sensor number it is in the chain
// returs 0 if device and channel is not a valid sensor number
unsigned sensor_number(UInt8 *ptr_aid, UInt8 *ptr_chan)
{
	unsigned num = 0;
	unsigned i, cnt;

	for (i = 0; i < UNIT_MAX; i++)
	{
		cnt = aid_db[sort_slot[i]].sensors;
		if (aid_db[sort_slot[i]].id == *ptr_aid)
		{// found device
			if (*ptr_chan && *ptr_chan <= cnt) // user should specify 1..cnt
				return num + *ptr_chan; // this is the global sensor number for the named sensor
			break;
		}
		num += cnt;
	}
	return 0; // no such sensor or channel
}

#endif
