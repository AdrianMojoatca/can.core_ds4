
#include "device.h"
/// Executes in asm to integrate this in CRTO.asm ///

__asm void xk401_io_init (void)
{
// IO PIN SETTING
//for XK800: p2.11 RX RF

  ldr r0,=0x00400426
  ldr r1,=0x3FFFC014	// FIO0PIN
  str r0,[r1]

  ldr r0,=0x00020000
  ldr r1,=0x3FFFC034	// FIO1PIN
  str r0,[r1]

  ldr r0,=0x00002000
  ldr r1,=0x3FFFC054	// FIO2PIN		
  str r0,[r1]

  ldr r0,=0x00000000
  ldr r1,=0x3FFFC074	// FIO3PIN
  str r0,[r1]

  ldr r0,=0x10000000
  ldr r1,=0x3FFFC094	// FIO4PIN
  str r0,[r1]

// IO DIRECTION SETTING

  ldr r0,=0xDE5FF6E6
  ldr r1,=0x3FFFC000	// FIO0DIR	
  str r0,[r1]

  ldr r0,=0xFFFFFBFF
  ldr r1,=0x3FFFC020	// FIO1DIR
  str r0,[r1]

  ldr r0,=0xFFFFFA00
  ldr r1,=0x3FFFC040	// FIO2DIR	
  str r0,[r1]

  ldr r0,=0x06000000
  ldr r1,=0x3FFFC060	// FIO3DIR
  str r0,[r1]

  ldr r0,=0x10000000
  ldr r1,=0x3FFFC080	// FIO4DIR
  str r0,[r1]

// IO PULL UP/DOWN SETTING
 
  ldr r0,=0xAAAAAAAA
  ldr r1,=0xE002C040	// PINMODE0
  str r0,[r1]

  ldr r0,=0xA2AAAAAA
  ldr r1,=0xE002C044	// PINMODE1
  str r0,[r1]

  ldr r0,=0xAAAAAAAA
  ldr r1,=0xE002C048	// PINMODE2
  str r0,[r1]

  ldr r0,=0xAAAAAAAA
  ldr r1,=0xE002C04C	// PINMODE3
  str r0,[r1]

  ldr r0,=0x0AAAAAAA
  ldr r1,=0xE002C050	// PINMODE4
  str r0,[r1]

  ldr r0,=0
  ldr r1,=0xE002C054	// PINMODE5
  str r0,[r1]

  ldr r0,=0
  ldr r1,=0xE002C058	// PINMODE6
  str r0,[r1]

  ldr r0,=0x00280000
  ldr r1,=0xE002C05C	// PINMODE7
  str r0,[r1]

  ldr r0,=0
  ldr r1,=0xE002C060	// PINMODE8
  str r0,[r1]

  ldr r0,=0x02000000
  ldr r1,=0xE002C064	// PINMODE9
  str r0,[r1]

  mov pc,lr

}

