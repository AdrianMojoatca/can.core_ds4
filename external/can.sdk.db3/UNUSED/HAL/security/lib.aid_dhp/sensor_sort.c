#include "aid_dhp.h"
#include "nvfs.h"
#include "nvfs_usr.h"

#include <string.h>

/*---------------------------------------------------------------------------*/
//
// This API should be called at the start of sensor adjust mode.
// It will establish the sensor order number of each sensor.
// During sensor adjust mode, if any new devices are plugged in, they will NOT be in this list.

// the sort is optimized to minimize the number of database reads  (nvfs reads)
// assuming ALL units have configurable sensors
//
// UNIT_MAX   average # of reads, given random initial UUID order
//     4           7
//     6          14
//     8          22
//     12         40
//     16         59
//    100        629

// but there is no need to sort uuids of units having no configurable sensors!

// We can reduce the average number of reads in our test conditions to: 1, 3, 6, 13, 20, 234
// using the following optimization
#define SENSOR_OPT_SKIP_SENSORLESS  // undefine this if we need to sort uuid for other reasons than sensor

void sensor_sort(void)
{
	unsigned i,j, hi,lo,x, tmp;
	unsigned char buf0[UUID_SIZE];
	unsigned char buf1[UUID_SIZE];
#ifdef SENSOR_OPT_SKIP_SENSORLESS
	unsigned with_sensors;
#endif
	// sort based on uuid
	memset(sort_slot, 0xFF, UNIT_MAX);
	j = 0;
	for (i = 0; i<UNIT_MAX; i++)
	{
		if (aid_db[i].status & (1 << AID_STATUS_BIT_UUID))
			sort_slot[j++] = i;
	}

#ifdef SENSOR_OPT_SKIP_SENSORLESS
	with_sensors = 0; // so far, none with sensors have been found
	if (aid_db[sort_slot[0]].sensors)
		with_sensors = 1; // first already found
#endif

	for (i = 1; i < UNIT_MAX; i++)
	{
		if (sort_slot[i] == 0xFF)
			break; // early end of list
		// everything before i is already sorted

		hi = i; // we expect i to fit somewhere below here
		lo = 0; // we expect i to be here or higher

#ifdef SENSOR_OPT_SKIP_SENSORLESS
		if (aid_db[sort_slot[i]].sensors == 0)  // no sensors?
		{// no need to read or sort this, we can put all sensorless at the beginning of the list
			hi = lo = 0; // insert all sensorless at the beginning (unsorted is not a problem)
		}
		else if (++with_sensors == 1) // is this the first sensor?
			hi = lo = i; // put at end (so far), we don't need to check.
		else
#endif
		{// read it, in order to find out where it should go in the list
			nvfs_rd_at(unit_name(sort_slot[i]), 0, buf0, UUID_SIZE);
		}

		// while we still need to find the right place in the list
		while (lo < hi)
		{
			x = (hi + lo) >> 1;
			nvfs_rd_at(unit_name(sort_slot[x]), 0, buf1, UUID_SIZE);

			if (memcmp(buf0, buf1, UUID_SIZE) < 0)
				hi = x;  // i goes below x
			else
				lo = x + 1; // i goes above lo
		}
		// the unit at position i belongs at lo
		tmp = sort_slot[i];
		for (j = i; j>lo; j--)
			sort_slot[j] = sort_slot[j - 1]; // slide these up by one
		sort_slot[j] = tmp; // now inserted in its proper place
	}

};


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
			if ( aid == aid_db[sort_slot[i]].id)
			{// found device
				if (*ptr_chan < cnt)
				{
					*ptr_aid = aid;
					(*ptr_chan)++; // bump caller's channel number
					*idx = i;
					return  num + *ptr_chan;
				}
				// channel number was too big, they really want next device
				aid = 0; // ask for next device
				*ptr_chan = 0; // make sure we pick first sensor
			}
			num += cnt;
		}
		if (num == 0)
			break; // there are no sensors at all!
		aid = 0; // go around again, but stop on first device with sensors
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

