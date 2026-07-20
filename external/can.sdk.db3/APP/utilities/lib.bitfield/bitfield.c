
#include "bitfield.h"


// for some reason, legacy "bitfield" bits were numbered msbit 0,  lsbit 7
static const UInt8 masktable[8] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 }; // (1<<(7-(num & 7)))

Boolean bitfield_bit (const UInt8 bits [] , UInt16 num)
{
  return !!(bits[num>>3] & masktable[num & 7]);
}

void bitfield_set (UInt8 bits [] , UInt16 num)
{
  bits[num>>3] |= masktable[num & 7];
}

void bitfield_clr (UInt8 bits [] , UInt16 num)
{
  bits[num>>3] &= ~masktable[num & 7];
}

void bitfield_setval (UInt8 bits [] , UInt16 num, Boolean val)
{
	if(val)
	  bitfield_set(bits,num);
	else
		bitfield_clr(bits,num);
}




// legacy APIs for bitfield_rd and bitfield_wr


static UInt8 append_rd (UInt32 * acc , UInt8 lsb , UInt8 msb , UInt8 data)
{
  UInt8 numbits = msb - lsb + 1 ;

  UInt8 mask = 0xFF >> 8 - numbits ;

  *acc <<= numbits ;

  *acc |= (data >> 7 - msb) & mask ;

  return numbits ;
}




UInt32 bitfield_rd (const UInt8 bits [] , UInt16 lsb , UInt16 msb)
{
  UInt32 acc = 0 ;

  const  UInt8 * lhs = & bits [lsb / 8] ;
  const  UInt8 * rhs = & bits [msb / 8] ;

  do
    {
      lsb += append_rd (&acc , lsb % 8 , lhs == rhs ? msb % 8 : 7 , *lhs) ;

      lhs++ ;
    }
  while (lsb <= msb) ;

  return acc ;
}

//--------------------------------------------------------------------------//


static UInt8 append_wr (UInt8 * bits , UInt8 lsb , UInt8 msb , UInt32 * data)
{
  UInt8 acc ;

  UInt8 numbits = msb - lsb + 1 ;
  
  UInt8 mask = 0xFF >> 8 - numbits ;

  mask <<= 7 - msb ;

  acc    = ((UInt8) *data) << 7 - msb ;

  acc   &=  mask ;

  *bits &= ~mask ;

  *bits |=  acc  ;

  *data >>= numbits ;

  return numbits ;
}


void bitfield_wr (UInt8 bits [] , UInt16 lsb , UInt16 msb , UInt32 data)
{
  SInt32  msb_s = msb ;

  UInt8 * lhs = & bits [lsb / 8] ;
  UInt8 * rhs = & bits [msb / 8] ;

  do
    {
      msb_s -= append_wr (rhs , lhs == rhs ? lsb % 8 : 0 , msb % 8 , &data) ;

      msb    = (UInt16) msb_s ;

      rhs-- ;
    }
  while (msb_s >= lsb) ;
}

//--------------------------------------------------------------------------//

// These routines are incompatible with legacy "bitfield" APIs, since the bit numbering is different....


UInt32 bitfield_rd_u (UInt32 bits , UInt8 lsb , UInt8 msb)
{
  UInt8 nbits = msb - lsb + 1 ;

  if (nbits >= 32)
    {
      return bits ;
    }
  else
    {
      return (bits >> lsb) & (((UInt32) 1 << nbits) - 1) ;
    }
}


void bitfield_wr_u (UInt32 * bits , UInt8 lsb , UInt8 msb , UInt32 data)
{
  UInt8 nbits = msb - lsb + 1 ;

  if (nbits >= 32)
    {
      *bits = data ;
    }
  else
    {
      UInt32 mask = ((UInt32) 1 << nbits) - 1 ;

      *bits &= ~ (mask         << lsb) ;
      *bits |=   (mask & data) << lsb  ;
    }
}


