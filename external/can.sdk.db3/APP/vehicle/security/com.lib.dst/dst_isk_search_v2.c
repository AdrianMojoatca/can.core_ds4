
/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: dst_isk_search.c 16978 2012-09-01 18:03:05Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "dst_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static dst_isk_generate_key(const UInt8 uid[3] , const UInt8 pwd , const UInt8 id, UInt8 key[5]) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*        I N L I N E   F U N C T I O N S   &   T E M P L A T E S           */
/*==========================================================================*/
Boolean dst_isk_search_v2(Dst_Log * log, UInt8 isk[DSTCRYPT_KEY80_SIZE])
{  
  UInt8 pwd  = log->sel_addr;

  do
    {
      if(((pwd & 0xF0) == 0x30) || ((pwd & 0xF0) == 0x50) || ((pwd & 0xF0) == 0x90) || ((pwd & 0xF0) == 0xB0) || ((pwd & 0xF0) == 0xD0))
        {
          if(((pwd & 0x0F) == 4) || ((pwd & 0x0F) == 2) || ((pwd & 0x0F) == 0)) //if((pwd & 0x06) || (pwd & 0x0F) == 0)//(pwd & 0x0F == 0) ||       
            {
              UInt8 id = log->user_data ;
                        
              #if DBG_DST == 1  
               dst_dbg_print("\nDst Build Mask Pwd: " , &pwd , 1) ;
              #endif           
              
              do
                {
                  UInt8 challenge [DST_CHALLENGE_SIZE]  ;
                  UInt8 key       [DSTCRYPT_KEY40_SIZE] ;
                  
                  memcpy(&challenge[0] , &log->challenge[0] , DST_CHALLENGE_SIZE) ;
                  
                  dst_isk_generate_key(log->uid , pwd , id , key) ;

                  dstcrypt_set_isk(&key[0] , DSTCRYPT_KEY40_SIZE) ;
        
                  dstcrypt(&challenge[0] , DST_40) ;
                      
                  if(!memcmp(&challenge[0] , &log->response[0] , DST_SIGNATURE_SIZE) && dstcrypt_write_isk(&key[0] , DSTCRYPT_KEY40_SIZE))
                    {                  
                      memcpy(&isk[0] , &key[0] , DSTCRYPT_KEY40_SIZE) ;
                      
                      log->sel_addr  = pwd ;
                      log->user_data = id  ;
                      
                      return TRUE ;
                    }             
                }
              while((++id) , (id != log->user_data) ) ;
       
            }                    
        }      
    }
  while((pwd -=2) ,  (pwd != log->sel_addr)) ;
 
  return FALSE ;

}
//--------------------------------------------------------------------------//
static dst_isk_generate_key(const UInt8 uid[3] , const UInt8 pwd , const UInt8 id, UInt8 key[DSTCRYPT_KEY40_SIZE])
{
  UInt8 i       ;
  UInt8 work[5] ;
  UInt8 mx[5]   ;

// Load work register with transponder information
  
  work[0] =uid[2];
  work[1] =uid[1];
  work[2] =uid[0]; 
  work[3] =pwd ;
  work[4] =id  ;  

  // Shift work register left by 1
  for(i = 0 ; i < 5 ; i++)
  {
  	work[i]<<=1;
    
  	if((i < 4) && (work[i+1] & 0x80))
      {
        work[i]|=0x01;
      }  		
  }

	// Setup magicXOR register based on the transponder PW
  if((pwd & 0x84)==0x84)		// Toyota smartkey / Lexus smartaccess (PW={94,D4})
	{
		if(pwd!=0xB4)	// Toyota/Lexus smartkey
		{
			mx[0]=0x06;
			mx[1]=0x46;
			mx[2]=0x90;
			mx[3]=0x46;
			mx[4]=0x24;
		}
		else // Prius key (replacement key from parts)
		{
			mx[0]=0xAE;
			mx[1]=0x8A;
			mx[2]=0xAE;
			mx[3]=0x8A;
			mx[4]=0xAE;
		}
	}
	else if((pwd & 0x84)==0x04)		// Toyota Prius key system (PW={34,54,B4})
	{
		mx[0]=0xAE;
		mx[1]=0x8A;
		mx[2]=0xAE;
		mx[3]=0x8A;
		mx[4]=0xAE;
	}
	else if((pwd & 0x02)==0x02)		// Toyota regular key (PW={32,52,92,B2,D2})
	{
		mx[0]=0x6A;
		mx[1]=0x46;
		mx[2]=0x6A;
		mx[3]=0x46;
		mx[4]=0x6A;
	}
	else								// Lexus regular key (PW={30,50,90,B0,D0})
	{			
		mx[0]=0x06;
		mx[1]=0x20;
		mx[2]=0x06;
		mx[3]=0x20;
		mx[4]=0x06;
	}
	
	// Generate the transponder cryptkey based on work register, magicXOR and transponder ID
  key[0] = work[4] ^ id ^ mx[4] ;
  key[1] = work[3] ^ id ^ mx[3] ;
  key[2] = work[2] ^ id ^ mx[2] ;
  key[3] = work[1] ^ id ^ mx[1] ;
  key[4] = work[0] ^ id ^ mx[0] ;

}

 
