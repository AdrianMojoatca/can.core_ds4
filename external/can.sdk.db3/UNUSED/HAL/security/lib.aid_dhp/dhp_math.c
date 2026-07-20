
#include "device.h"
#include <string.h>

#include "dhp_math.h"

DH_VAR dh_acc;
DH_VAR dh_negP;

// read network byte order into host128 format
void nbo2host(DH_VAR to, const UInt8 *from)
{
UInt32 x;
unsigned i,j;
	
	for(i=4;i--;)
	{
		x = 0;
		for(j=0;j<4;j++)
		{
			x = (x<<8) | *from++;
		}
		to[i] = x;
	}
}
void dh_load_nbo(const UInt8 *from)
{
	nbo2host(dh_acc,from);
}

// store host128 
void host2nbo(UInt8 *to,DH_VAR from)
{
UInt32 x;
unsigned i,j;
	
	for(i=4;i--;)
	{
		x = from[i];
		for(j=0;j<4;j++)
		{
			*to++ = (x>>24);
			x<<=8;
		}
	}
}

void dh_store_nbo(UInt8 *to)
{
	host2nbo(to,dh_acc);
}

void mov128(DH_VAR to, DH_VAR from)
{
unsigned i;
	for(i=0;i<4;i++)
		to[i] = from[i];
}

void dh_set(UInt8 quick)
{
	memset(dh_acc, 0, sizeof(dh_acc));
	dh_acc[0] = quick;  // little-endian lsb is first in the array
}


unsigned add128(DH_VAR a, DH_VAR b, DH_VAR sum)
{
	unsigned i,c = 0;
  UInt32 s;
	
  for(i=0;i<4;i++)
	{		
		s = a[i] + b[i];
		if(s < a[i])
		{
			if(c)
				s++;
			c=1;
		}
		else if(c)
		{
			if(++s)
				c = 0;
		}
		// else no overflow, no previous carry
		sum[i] = s;
	}
	return c;
}

// set dh_negP to the two-complement negative of dh_acc
void dh_setP(void)
{
unsigned i;
	
	for(i=0;i<4;i++)
	{
		dh_negP[i] = ~dh_acc[i];
	}
	dh_set(1);
	add128(dh_negP,dh_acc,dh_negP);
}


// add modulo P
void dh_add(DH_VAR what)
{
DH_VAR tmp;
DH_VAR tmp2;
unsigned c,v;
	
	  c = add128(what,dh_acc,tmp); // carry from straight add
	  v = add128(dh_negP,tmp,tmp2); // subtract P
		if(c || v)
		{
			mov128(dh_acc,tmp2);
		}
		else // !c && !v
		{
			mov128(dh_acc,tmp);
		}
}

// multiply modulo P  NOTE: can't pass point to dh_acc!!!
void dh_mul(DH_VAR what)
{
DH_VAR tmp;
unsigned i;
	
	mov128(tmp,dh_acc);
	dh_set(0);
	for(i=128;i--;)
	{
		dh_add(dh_acc);
		if(what[i>>5] & (1<<(i & 31)))
			dh_add(tmp);
	}
}

// exponentiate modulo P
void dh_exp(DH_VAR exp)
{
DH_VAR base;
DH_VAR tmp;
unsigned i;
	
	mov128(base,dh_acc);
	dh_set(1);
	for(i=128;i--;)
	{
		mov128(tmp,dh_acc);
		dh_mul(tmp); // square dh_acc
		
		if(exp[i>>5] & (1<<(i & 31)))
			dh_mul(base);
	}
}


 

