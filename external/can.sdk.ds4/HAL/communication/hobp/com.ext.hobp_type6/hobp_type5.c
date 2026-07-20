/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include "hobp_private.h"
#include "nvfs_usr.h"
#include <string.h>

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/
#define ROR_INLINE(x,shift) ((x >> shift) | ((x&0xFFFF) << (16 - shift)))&0xFFFF
#define ROL_INLINE(x,shift) ((x << shift) | ((x&0xFFFF) >> (16 - shift)))&0xFFFF

#define     NB_RETRY            0x10
#define     NB_CHALLENGE        0x1C

#define     START_SECOND_PART   0x0C
#define     END_CHALL_TAB       0x1C
#define     END_COL_TAB1        0x0F
#define     END_LINE_TAB1       0x0F

#define     INDEX_SECOND_LINE   0x10

#define     CHALL_PART_1        0
#define     CHALL_PART_2        1
#define     CHALL_PART_3        2

#define     SIZE_SEQ            0x10
#define     SIZE_ISK            0x19 // 25  Decimal

#define     NBE_SEQ_KEY         16
#define     NBE_SEQ_PTS         2

#define ERROR_GET_SEQ       0xFF
#define ERROR_BUILD_SEQ     0xFF
        
// State Machine 
typedef enum{ 
    FIRST_ELEMENT = 0,
    GET_LINE,
    NEXT_LINE,
    DONE,
}StateMachine;


/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

#define HOBP_SEQ_SIZE     4
#define HOBP_SEQ_TAB_SIZE 16

typedef struct
{
  UInt8 seq[HOBP_SEQ_SIZE] ;
  UInt8 tab[HOBP_SEQ_TAB_SIZE];
}Hobp_Seq;

const Hobp_Seq hobp_seq_table[] =
{

  /*          Seq         |                               Table                                           */
  {0x33, 0xFF, 0xFE, 0xFF , 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF },  
  {0x66, 0x44, 0x43, 0x44 , 0x0, 0x8, 0xC, 0xE, 0xF, 0x2, 0x3, 0x9, 0x6, 0xD, 0xB, 0xA, 0x5, 0x7, 0x4, 0x1 },
  {0xBB, 0x55, 0x56, 0x55 , 0x0, 0xA, 0xB, 0xD, 0x4, 0xF, 0xE, 0xC, 0x6, 0x1, 0x2, 0x3, 0x8, 0x7, 0x9, 0x5 }, // Key Type  
  {0xBB, 0x55, 0x58, 0x55 , 0x1, 0x2, 0xB, 0xD, 0x8, 0xF, 0x4, 0xC, 0xE, 0xA, 0x0, 0x3, 0x5, 0x6, 0x9, 0x7 }, // Key Type    
  {0xBB, 0x11, 0x16, 0x11 , 0x1, 0xF, 0x4, 0xB, 0xD, 0xC, 0xE, 0x8, 0x7, 0x5, 0x6, 0x3, 0xA, 0x2, 0x0, 0x9 },
  {0x77, 0x99, 0x93, 0x99 , 0x2, 0x8, 0xC, 0xE, 0x9, 0x1, 0x0, 0x3, 0xB, 0xF, 0x4, 0x5, 0xA, 0xD, 0x6, 0x7 },
  {0xBB, 0x55, 0x54, 0x55 , 0x2, 0xA, 0x0, 0x9, 0xE, 0xF, 0x5, 0xC, 0x6, 0x7, 0xD, 0x3, 0x4, 0xB, 0x8, 0x1 },  
  {0xEE, 0xDD, 0xDA, 0xDD , 0x2, 0xC, 0xD, 0xB, 0x1, 0xA, 0x4, 0x0, 0x8, 0x7, 0xE, 0x9, 0x5, 0xF, 0x3, 0x6 }, 
  {0xBB, 0x77, 0x7A, 0x77 , 0x2, 0xD, 0x1, 0x9, 0xA, 0x6, 0x7, 0xF, 0x5, 0x8, 0xE, 0x3, 0x0, 0xB, 0xC, 0x4 },  
  {0x00, 0x66, 0x63, 0x66 , 0x3, 0x2, 0x4, 0xE, 0x7, 0x5, 0xF, 0xC, 0xD, 0x8, 0xA, 0x6, 0x1, 0xB, 0x0, 0x9 }, 
  {0x00, 0xEE, 0xEF, 0xEE , 0x3, 0x5, 0x0, 0x1, 0x7, 0x6, 0x9, 0xA, 0x8, 0xC, 0x4, 0x2, 0xD, 0xB, 0xF, 0xE }, // Key Type    
  {0x00, 0x55, 0x59, 0x55 , 0x3, 0xB, 0x7, 0x0, 0xA, 0xF, 0x1, 0x5, 0x4, 0xE, 0x9, 0x6, 0xC, 0xD, 0x8, 0x2 }, 
  {0x00, 0xEE, 0xE1, 0xEE , 0x3, 0xE, 0xB, 0x4, 0x2, 0x5, 0x8, 0xC, 0x1, 0xD, 0xA, 0x9, 0x0, 0x7, 0xF, 0x6 },
  {0xCC, 0x55, 0x53, 0x55 , 0x4, 0x2, 0xD, 0xE, 0x1, 0xF, 0x9, 0x7, 0xB, 0x5, 0x0, 0x6, 0x3, 0x8, 0xC, 0xA },  
  {0x11, 0xBB, 0xBF, 0xBB , 0x4, 0x3, 0x8, 0xA, 0x6, 0x1, 0x2, 0x0, 0x9, 0x7, 0xC, 0xF, 0xB, 0x5, 0xD, 0xE },  
  {0xCC, 0x88, 0x84, 0x88 , 0x4, 0xC, 0x1, 0x9, 0xE, 0x6, 0x7, 0xA, 0xF, 0x8, 0x0, 0x5, 0x3, 0xB, 0x2, 0xD },
  {0xEE, 0xBB, 0xB1, 0xBB , 0x4, 0xE, 0x1, 0x5, 0x6, 0xA, 0x8, 0x0, 0x2, 0xB, 0x9, 0xF, 0x7, 0xC, 0x3, 0xD },
  {0xFF, 0xBB, 0xBE, 0xBB , 0x5, 0x2, 0x6, 0x7, 0xB, 0x4, 0xC, 0x1, 0x8, 0x9, 0x0, 0xF, 0xD, 0xA, 0xE, 0x3 },
  {0xFF, 0x99, 0x98, 0x99 , 0x5, 0x2, 0xC, 0x7, 0x9, 0xA, 0x1, 0x4, 0xE, 0xF, 0x0, 0xB, 0x8, 0x6, 0xD, 0x3 },
  {0x77, 0x88, 0x8C, 0x88 , 0x5, 0xC, 0x7, 0xD, 0x4, 0xA, 0x1, 0x3, 0xF, 0xB, 0x9, 0x6, 0xE, 0x8, 0x0, 0x2 }, // Key Type 
  {0x22, 0xFF, 0xF6, 0xFF , 0x6, 0x0, 0x3, 0x8, 0x9, 0x5, 0xE, 0xB, 0x2, 0x7, 0xC, 0xA, 0x4, 0x1, 0xD, 0xF },
  {0x22, 0x44, 0x4E, 0x44 , 0x6, 0x4 ,0x3, 0x8, 0xF, 0x0, 0x7, 0x1, 0xA, 0xD, 0x2, 0x9, 0x5, 0xB, 0xE, 0xC },
  {0xDD, 0x44, 0x41, 0x44 , 0x6, 0xE, 0x5, 0xA, 0xF, 0x1, 0x0, 0x2, 0x8, 0x9, 0xB, 0x7, 0x4, 0x3, 0xC, 0xD }, // Key Type
  {0x77, 0xFF, 0xF4, 0xFF , 0x7, 0x6, 0xA, 0x4, 0xE, 0x8, 0xB, 0x3, 0xC, 0x9, 0x0, 0x2, 0xD, 0x5, 0x1, 0xF }, // Key Type
  {0x77, 0x33, 0x3E, 0x33 , 0x7, 0x9, 0x2, 0xF, 0x6, 0x5, 0x4, 0x3, 0x1, 0xA, 0x0, 0x8, 0xB, 0xD, 0xE, 0xC },  
  {0x44, 0x66, 0x6D, 0x66 , 0x7, 0xD, 0x1, 0x0, 0x3, 0x6, 0xF, 0xA, 0x4, 0x5, 0x9, 0x2, 0xC, 0xE, 0xB, 0x8 }, // Key Type
  {0x11, 0x44, 0x4C, 0x44 , 0x9, 0x3, 0x0, 0x6, 0xF, 0x1, 0x8, 0x2, 0xB, 0x4, 0x5, 0x7, 0xE, 0xD, 0xA, 0xC },
  {0x66, 0xBB, 0xBD, 0xBB , 0x9, 0x5, 0xD, 0x7, 0x4, 0xC, 0x3, 0xA, 0x0, 0x6, 0x8, 0xF, 0x2, 0xE, 0x1, 0xB },
  {0x44, 0xBB, 0xB9, 0xBB , 0x9, 0xD, 0x8, 0x2, 0x3, 0x5, 0xB, 0xC, 0x6, 0xE, 0x7, 0xF, 0x1, 0x4, 0xA, 0x0 },
  {0x11, 0x44, 0x43, 0x44 , 0xA, 0x3, 0x5, 0xE, 0xF, 0x4, 0x8, 0xB, 0x9, 0x0, 0x2, 0x7, 0x6, 0xD, 0xC, 0x1 }, // Key Type
  {0x66, 0xCC, 0xCD, 0xCC , 0xA, 0xD, 0xC, 0x0, 0x7, 0xB, 0x3, 0x4, 0x2, 0x1, 0x6, 0x9, 0xF, 0xE, 0x8, 0x5 }, // Key Type
  {0xCC, 0x99, 0x95, 0x99 , 0xA, 0xD, 0x7, 0xB, 0x1, 0xE, 0x9, 0x2, 0x5, 0xF, 0x4, 0xC, 0x3, 0x0, 0x8, 0x6 }, // Key Type
  {0x11, 0xEE, 0xEA, 0xEE , 0xB, 0x3, 0x0, 0x8, 0x1, 0x6, 0xA, 0x5, 0x7, 0xC, 0xE, 0x2, 0x9, 0x4, 0xF, 0xD }, // Key Type  
  {0xDD, 0x77, 0x73, 0x77 , 0xB, 0x5, 0xA, 0xE, 0x6, 0x0, 0x8, 0xF, 0xC, 0x1, 0x4, 0x2, 0x9, 0x3, 0xD, 0x7 },
  {0x33, 0xBB, 0xBD, 0xBB , 0xB, 0x7, 0x0, 0x3, 0xA, 0x1, 0x5, 0x8, 0x6, 0x2, 0xD, 0xF, 0xC, 0xE, 0x4, 0x9 },
  {0x44, 0xDD, 0xDB, 0xDD , 0xB, 0x7, 0x8, 0x2, 0x3, 0xA, 0x6, 0x1, 0x9, 0x0, 0x4, 0xE, 0xD, 0xF, 0xC, 0x5 }, // Key Type  
  {0x55, 0xCC, 0xC9, 0xCC , 0xB, 0x9, 0x5, 0x2, 0x0, 0x3, 0x1, 0x6, 0xA, 0xE, 0x8, 0xC, 0xF, 0x7, 0xD, 0x4 }, // Key Type  
  {0xAA, 0x11, 0x19, 0x11 , 0xB, 0xF, 0x8, 0x0, 0x6, 0x9, 0x1, 0x5, 0xD, 0xE, 0x3, 0x7, 0x4, 0x2, 0xC, 0xA },
  {0x44, 0x99, 0x9C, 0x99 , 0xC, 0x9, 0x0, 0xB, 0x3, 0x6, 0x2, 0xD, 0x4, 0xF, 0xA, 0x5, 0xE, 0x1, 0x7, 0x8 }, // Key Type  
  {0x66, 0x22, 0x25, 0x22 , 0xD, 0x2, 0xF, 0x1, 0xC, 0xE, 0x3, 0x4, 0x5, 0x7, 0x0, 0x8, 0xA, 0x9, 0xB, 0x6 },
  {0xDD, 0xEE, 0xE7, 0xEE , 0xD, 0x5, 0x4, 0x6, 0x0, 0x1, 0xA, 0xE, 0x7, 0xB, 0x9, 0xC, 0x2, 0x3, 0xF, 0x8 },
  {0x88, 0x77, 0x71, 0x77 , 0xD, 0xE, 0x1, 0x4, 0x9, 0xA, 0xB, 0xF, 0x3, 0x6, 0x8, 0x7, 0x0, 0x2, 0x5, 0xC },
  {0xBB, 0x11, 0x1A, 0x11 , 0xD, 0xF, 0xB, 0x8, 0x4, 0x9, 0x5, 0xC, 0x7, 0x0, 0xE, 0x3, 0xA, 0x1, 0x2, 0x6 },
  {0x55, 0x88, 0x80, 0x88 , 0xE, 0x1, 0xC, 0x6, 0xD, 0x3, 0x0, 0x7, 0xF, 0xA, 0x9, 0x8, 0xB, 0x4, 0x2, 0x5 }, // Key Type    
  {0x11, 0xDD, 0xD0, 0xDD , 0xE, 0x3, 0x4, 0x1, 0x8, 0x7, 0x0, 0x2, 0xA, 0xB, 0xC, 0xD, 0x6, 0xF, 0x5, 0x9 },
  {0xFF, 0xEE, 0xE0, 0xEE , 0xE, 0x5, 0xA, 0x8, 0x9, 0xD, 0xB, 0x1, 0x6, 0x7, 0x0, 0xC, 0x4, 0x2, 0xF, 0x3 },
  {0xFF, 0x00, 0x0E, 0x00 , 0xF, 0x1, 0x9, 0x6, 0xA, 0x4, 0x7, 0x5, 0x2, 0x8, 0xC, 0x0, 0xD, 0xB, 0xE, 0x3 }, // Key Type   
  {0xEE, 0x00, 0x07, 0x00 , 0xF, 0x5, 0x2, 0x6, 0x1, 0xA, 0x9, 0xE, 0x4, 0xC, 0xD, 0x7, 0x0, 0xB, 0x3, 0x8 },
  {0xAA, 0x00, 0x03, 0x00 , 0xF, 0x5, 0xB, 0xE, 0x9, 0xC, 0x8, 0x0, 0xD, 0x2, 0x3, 0x6, 0x4, 0x1, 0xA, 0x7 },
  {0x00, 0xFF ,0xFA, 0xFF , 0x3, 0x5, 0xA, 0x8, 0xD, 0xC, 0x9, 0x1, 0x2, 0x6, 0xE, 0xB, 0x7, 0x4, 0x0, 0xF },
};

static Hobp_Seq hobp_custom_seq_table= {0xFF, 0xFF, 0xFF, 0xFF} ;

static const UInt8 HOBP_SEQ_TABLE_SIZE = sizeof_array(hobp_seq_table) ;


static const unsigned char u8TabChallenge[NB_CHALLENGE][3]=
{
    { 0x12 , 0x00 , 0x34},
    { 0x12 , 0x10 , 0x34},
//  { 0x12 , 0x20 , 0x34},
//  { 0x12 , 0x30 , 0x34},
    { 0x12 , 0x40 , 0x34},
    { 0x12 , 0x50 , 0x34},
    { 0x12 , 0x60 , 0x34},
    { 0x12 , 0x70 , 0x34},
    { 0x12 , 0x80 , 0x34},
//  { 0x12 , 0x90 , 0x34},
    { 0x12 , 0xA0 , 0x34},
//  { 0x12 , 0xB0 , 0x34}, 
    { 0x12 , 0xC0 , 0x34},
    { 0x12 , 0xD0 , 0x34},
    { 0x12 , 0xE0 , 0x34},
    { 0x12 , 0xF0 , 0x34},
//------------------------  
    { 0x00, 0x00 ,  0x00},  
    { 0x01, 0x00 ,  0x10},
    { 0x02, 0x00 ,  0x20},
    { 0x03, 0x00 ,  0x30},
    { 0x04, 0x00 ,  0x40},
    { 0x05, 0x00 ,  0x50},
    { 0x06, 0x00 ,  0x60},
    { 0x07, 0x00 ,  0x70},  
    { 0x08, 0x00 ,  0x80},
    { 0x09, 0x00 ,  0x90},
    { 0x0A, 0x00 ,  0xA0},
    { 0x0B, 0x00 ,  0xB0},
    { 0x0C, 0x00 ,  0xC0},
    { 0x0D, 0x00 ,  0xD0},
    { 0x0E, 0x00 ,  0xE0},  
    { 0x0F, 0x00 ,  0xF0},
};

/*------ DIF  TAB 2 & TAB 3 --------*/
static UInt8 u8DifTab;

/*--------    I N D E X     --------*/ 
static UInt8 u8IndexeOrder  = 0;
static UInt8 u8IndexeLine   = 0;
static UInt8 u8IndexeCol    = 0;
static UInt8 u8IndexeChall  = 0;

/*--------    S T A T E     --------*/ 
static UInt8 u8State = FIRST_ELEMENT;

/*-----  I N D E X   R E T R Y -----*/ 
static UInt8 u8Retry     = 0;
static UInt8 u8RetryTab2 = 0;

/*--------      I N I T     --------*/ 
static UInt8 u8InitGetChall = NOT_OK;
static UInt8 u8InitGetTab1  = NOT_OK;


/*--------      V A R I A B L E S   G L O B A L        --------*/ 
static UInt8 * u8IndexeSeq; 

static UInt8 u8LUT1    [256];
//static UInt8 u8LUT3    [256];

static UInt8 u8LUT_SEQ2[16] ;
static UInt8 u8Poly[2];

//static UInt8 u8FirstTab2;
static UInt8 u8FirstLineTab3[16]={0};
//static UInt8 u8LUT_SEQ1[16]     ={0};

/*==========================================================================*/
/*               P R O T O T Y P E    F U N C T I O N S                     */
/*==========================================================================*/
static UInt8 * igetSeqFromStatus(const UInt8 * status) ;
static UInt8   generateSequence2( UInt8 * u8Seq1 , UInt8 * u8Seq2 );   


static int iReverseSeed( const UInt8 * u8Response);
static int getValueSeq1( const UInt8 * u8Challenge);

static UInt8 controlChallenge(UInt8  * u8Response);    
static UInt8 checkFirstElement( UInt8 u8Value , UInt8 * u8Tab1Col1);
static UInt8 nextChallenge( UInt8  * u8Challenge);

static void getDataIsk    (unsigned char * u8Isk) ;
static void generate_List( unsigned char * TAB_DIF, unsigned char * LIST_FIND);

void createTab1(void) ;

/*==========================================================================*/
/*                    A L G O   H O N D A  5  V 2                           */
/*==========================================================================*/
void algo_Honda_V2(const UInt8 * cChallenge, UInt8 * cResponse)
{

    // Declarations Variables
    int iValue, iCarry;
    int iSeed, iSeedEx;
    int iLoop_number;
    int u8Niblet1,u8Niblet2,u8Niblet3,u8Niblet4;
    
    // Get Value From 
    iValue = getValueSeq1( cChallenge);
    
    //Calcul Seed with Poly Number 
    iSeed=(((iValue&0xFF00)>>8) + u8Poly[0])& 0XFF;
    iCarry = ((iValue & 0xFF) + u8Poly[1]);
    iSeed = (iSeed << 8) + iCarry;
    
    // Get Loop Number
    iLoop_number = (u8IndexeSeq[ (cChallenge[1] & 0xF0) >> 4] >> 2);
    
    // Rol Seed with Loop Number
    iSeed = ROR_INLINE(iSeed, iLoop_number);
    
    // Get Niblet From Seq2
    u8Niblet1 = u8LUT_SEQ2[(iSeed & 0xF000) >> 12];
    u8Niblet2 = u8LUT_SEQ2[(iSeed & 0x0F00) >> 8];
    u8Niblet3 = u8LUT_SEQ2[(iSeed & 0x00F0) >> 4];
    u8Niblet4 = u8LUT_SEQ2[(iSeed & 0x000F)];
    
    // Calculate the Response
    cResponse[0] = ((u8Niblet1 << 4) + u8Niblet2) & 0xFF;
    cResponse[1] =  u8LUT1[cChallenge[1]];
    cResponse[2] = ((u8Niblet3 << 4) + u8Niblet4) & 0xFF;

  #if DBG_HOBP == 1
    hobp_dbg_print("\nHoBp Algo C: " , cChallenge , 3) ;
    hobp_dbg_print("\nHoBp Algo R: " , cResponse  , 3) ;
  #endif

}

/*==========================================================================*/
/*                          G E T   P O L Y                                 */
/*==========================================================================*/
int getPolynome( const UInt8 * u8Challenge, const UInt8 * u8Response, const UInt8 * u8Challenge2, const UInt8 * u8Response2, const UInt8 * iMMOStatus)
{

    // Declarations Variables
  int iValue, i;
  int iSeed;
  UInt8 iLoop_number;
  UInt8 u8TabValidation[3];
    
    // Get Indexe Get Seq 1
  
  if((iMMOStatus[0] == NULL) && (iMMOStatus[1] == NULL) && (iMMOStatus[2] == NULL) && (iMMOStatus[3] == NULL))
    {
      #if DBG_HOBP == 1
        hobp_dbg_print("\nHoBp Seq Invalid " , iMMOStatus , 4) ;
      #endif

      return NOT_OK ;
    }
  else
    {
      u8IndexeSeq = igetSeqFromStatus(iMMOStatus) ;
    
      if(!u8IndexeSeq)
        {
          #if DBG_HOBP == 1
            hobp_dbg_print("\nHoBp Seq Not Found" , NULL , NULL) ;
          #endif
    
          return  NOT_OK ;
    
        }
    
      #if DBG_HOBP == 1
        hobp_dbg_print("\nHoBp Seq =" , u8IndexeSeq , 16) ;
      #endif
    
    
      // Calcul Seq 2
      iValue = generateSequence2( u8IndexeSeq, &u8LUT_SEQ2[0]);
        
      if (iValue == ERROR_BUILD_SEQ)
        {
          hobp_err_add(HOBP_ERR_SEQ2) ;
          
          #if DBG_HOBP == 1
            hobp_dbg_print("\nHoBp Seq 2 Error" , NULL , NULL) ;
          #endif
          
          return NOT_OK;
        }
    
      #if DBG_HOBP == 1
        hobp_dbg_print("\nHoBp Seq2=" , &u8LUT_SEQ2[0] , sizeof_array(u8LUT_SEQ2)) ;
      #endif
    
      createTab1();
    
      // Get Seed
      iSeed = iReverseSeed(u8Response);
    
      // Get Loop Number
      iLoop_number = (u8IndexeSeq[ (u8Challenge[1] & 0xF0) >> 4 ] >> 2); 
    
        
      // Rol Seed with Loop Number
      iSeed = ROL_INLINE(iSeed, iLoop_number);
    
      // Get Value From 
      iValue = getValueSeq1( u8Challenge);
     
      // Calcul Poly 
      u8Poly[0] = (((iSeed & 0xFF00) >> 8) - ((iValue & 0xFF00) >> 8)) & 0x0FF;
      u8Poly[1] = ((iSeed & 0xFF) - (iValue & 0xFF)) & 0x00FF;
    
        // Validation
      algo_Honda_V2( u8Challenge2, u8TabValidation);
    
      #if DBG_HOBP == 1
        hobp_dbg_print("\nHoBp Poly       = " , &u8Poly[0]         , 2) ;
        hobp_dbg_print("\nHoBp Challenge2 = " , u8Challenge2       , 3) ;
        hobp_dbg_print("\nHoBp Response2  = " , u8Response2        , 3) ;
        hobp_dbg_print("\nHoBp Validation = " ,&u8TabValidation[0] , 3) ;
      #endif
      
      if(memcmp(&u8TabValidation[0] , u8Response2 , 3))  //if ((u8TabValidation[0] != u8Response2[0]) || (u8TabValidation[2] != u8Response2[2]))
        {
           // Adjust Polynomial number
           u8Poly[0] = u8Poly[0] - 1;

          // Validation n1
          algo_Honda_V2( u8Challenge2, u8TabValidation);
    
          #if DBG_HOBP == 1
            hobp_dbg_print("\nHoBp Poly       = " , &u8Poly[0]         , 2) ;
            hobp_dbg_print("\nHoBp Challenge2 = " , u8Challenge2       , 3) ;
            hobp_dbg_print("\nHoBp Response2  = " , u8Response2        , 3) ;
            hobp_dbg_print("\nHoBp Validation = " ,&u8TabValidation[0] , 3) ;
          #endif
    
          if(memcmp(&u8TabValidation[0] , u8Response2 , 3))  //if ((u8TabValidation[0] != u8Response2[0]) || (u8TabValidation[2] != u8Response2[2]))
            {
              hobp_err_add(HOBP_ERR_VALIDATION_1) ;

              //return NOT_OK;
							return OK;
            }
          
          // Validation n2
//          algo_Honda_V2( u8Challenge, u8TabValidation);
//          
//          #if DBG_HOBP == 1
//            hobp_dbg_print("\nHoBp Poly       = " , &u8Poly[0]         , 2) ;
//            hobp_dbg_print("\nHoBp Challenge  = " , u8Challenge        , 3) ;
//            hobp_dbg_print("\nHoBp Response   = " , u8Response         , 3) ;
//            hobp_dbg_print("\nHoBp Validation = " ,&u8TabValidation[0] , 3) ;
//          #endif

//          if(memcmp(&u8TabValidation[0] , u8Response , 3))  //if ((u8TabValidation[0] != u8Response[0]) || (u8TabValidation[2] != u8Response[2]))
//            {
//              hobp_err_add(HOBP_ERR_VALIDATION_2) ;
//              
//              return NOT_OK;
//            }
          return OK;  
        }
      else
        {
          return OK;
        }          
    }
}

/*==========================================================================*/
/*                  CONVERT KEY2GO ISK to TABLE                             */
/*==========================================================================*/
void convert_isk_to_table(const UInt8 * seq , const UInt8 * isk)
{
  UInt8 n ;

  memcpy(&hobp_custom_seq_table.seq[0] , seq , HOBP_SEQ_SIZE) ;

  for( n = 0 ; n < HOBP_SEQ_TAB_SIZE ; n+=2)
    {
      hobp_custom_seq_table.tab[n]   = *isk >> 4   ; 
      hobp_custom_seq_table.tab[n+1] = *isk & 0x0F ;

      *isk++ ;
    }

  #if DBG_HOBP == 1
    hobp_dbg_print("\nHoBp New Table Create= " , &hobp_custom_seq_table.tab[0] , HOBP_SEQ_TAB_SIZE) ;
  #endif

}

/*==========================================================================*/
/*                  G E T   S E Q   F R O M   S T A T U S                   */
/*==========================================================================*/
static UInt8 * igetSeqFromStatus(const UInt8 * status)
{
  UInt8 n ;

  #if DBG_HOBP == 1
    hobp_dbg_print("\nHoBp Seq:" , status , 4) ;
  #endif

  for(n = 0 ; n < HOBP_SEQ_TABLE_SIZE ; n++)
    {
      if(!memcmp(status , &hobp_seq_table[n].seq[0] , HOBP_SEQ_SIZE))
        {
          return  (UInt8 *)&hobp_seq_table[n].tab[0] ;    //
        }
    }

  if(!memcmp(status , &hobp_custom_seq_table.seq[0] , HOBP_SEQ_SIZE))
    {
      return  (UInt8 *)&hobp_custom_seq_table.tab[0] ;    //
    }

  return NULL ;

}
/*==========================================================================*/
/*                 G E N E R A T E      S E Q U E N C E    2                */
/*==========================================================================*/
static UInt8 generateSequence2( UInt8 * u8Seq1 , UInt8 * u8Seq2 )
{
    
    // Declarations Variables
    UInt8 u8Ind0 , u8Ind1=0;
    UInt8 u8TempoValues;
    UInt8 u8Pos = 0;

    // Instructions
    for( u8Ind0 = 0 ; u8Ind0 < SIZE_SEQ ; u8Ind0++){
        while( u8Seq1[u8Ind1] != u8Ind0){
            u8Ind1++;

            if(u8Ind1 == SIZE_SEQ)
              {
                return ERROR_BUILD_SEQ;
              }           
        }
        u8Seq2[u8Ind0] = u8Ind1;
        u8Ind1 = 0;
    }
    
    return 0;
}


/*==========================================================================*/
/*                        I N I T    L I B                                  */
/*==========================================================================*/
void initBypassLib(void)
{
    // Init Index
    u8IndexeOrder  = 0;
    u8IndexeLine   = 0;
    u8IndexeCol    = 0;
    u8IndexeChall  = 0;

    // Init State Machine
    u8State  = FIRST_ELEMENT;

    // Init Retry count
    u8Retry        = 0;
    u8RetryTab2    = 0;

    // Init Flag 1st time in function
    u8InitGetChall = NOT_OK;
    u8InitGetTab1  = NOT_OK;
		#if DBG_HOBP == 1
				hobp_dbg_print("\nHoBp init bypass  " , NULL , NULL ) ;
		#endif
}

/*==========================================================================*/
/*                      G E T     C H A L L E N G E                         */
/*==========================================================================*/
unsigned char getChallenge( unsigned char  * u8Challenge, unsigned char * u8Response,  UInt8 (*u8TabResponse)[2]){

    // Declarations Variables
    unsigned char  u8Ind = 0 ,  u8Ind2 = 0;
    unsigned char u8ReturnValue;
    unsigned char u8Tempo[END_CHALL_TAB][2] = {0};

    // Instructions
    
    // First Time in the Function, Not yet Responses, Get 1st Challenge 
    if( u8InitGetChall == NOT_OK)
      {
        
        // Init Retry Variables
        u8Retry = 0;
        u8RetryTab2 = 0;
        
        // Set Next Challenge to process
        u8Challenge[0]= u8TabChallenge[u8IndexeChall][0];
        u8Challenge[1]= u8TabChallenge[u8IndexeChall][1];
        u8Challenge[2]= u8TabChallenge[u8IndexeChall][2];   
        
        // Init is Done
        u8InitGetChall = OK ;
    }else{
    
        // Test If Response is Correct XX XY XX
        u8ReturnValue = controlChallenge( u8Response);
        
        if( u8ReturnValue == NOT_OK ){
        // If the Response is not Correct, Retry Process
            
            // Incremente Retry Variable
            u8Retry++;
            
            // End of the retry process after X times
            if(u8Retry == NB_RETRY ){
                u8Retry = 0;
                return PROCESS_ERROR ;
            }
            
            if( u8IndexeChall < START_SECOND_PART)
              {
                // Second part of the Challenge List
                
                // Set Next Challenge to process
                u8Challenge[0]= u8TabChallenge[u8IndexeChall][0];
                u8Challenge[1]= u8TabChallenge[u8IndexeChall][1] + u8Retry;
                u8Challenge[2]= u8TabChallenge[u8IndexeChall][2];
            
              }
            else
              {
                u8IndexeChall = START_SECOND_PART ;
                u8RetryTab2++;
                    
                if(u8RetryTab2 == NB_RETRY )
                  {
                    u8RetryTab2 = 0;

                    return PROCESS_ERROR ;
                  }
                
                // Set Next Challenge to process
                u8Challenge[0]= u8TabChallenge[u8IndexeChall][0];
                u8Challenge[1]= u8TabChallenge[u8IndexeChall][1] + (u8RetryTab2<<4);
                u8Challenge[2]= u8TabChallenge[u8IndexeChall][2];           
              }
            
        }else{
            // Incremente Indexe challenge
            u8IndexeChall++; 
            
            // Init Retry Variables
            u8Retry = 0;
            
            // Save Reponse on the table    
            u8TabResponse[u8IndexeChall-1][0] = u8Response[0];
            u8TabResponse[u8IndexeChall-1][1] = u8Response[2];
                    
            // Set Next Challenge to Do
            
            if(u8IndexeChall >= END_CHALL_TAB){     
            
                // Save Reponse on the table    
                u8TabResponse[u8IndexeChall-1][0] = u8Response[0];
                u8TabResponse[u8IndexeChall-1][1] = u8Response[2];
                
                // Get Indexe Retry
                u8TabResponse[u8IndexeChall][0] = 0;
                u8TabResponse[u8IndexeChall][1] = u8RetryTab2;
                
                // Reset retry 
                u8RetryTab2 = 0;
                
                // Get into the right Order To file with Key2Go, FIX
                for(u8Ind = 0 ; u8Ind < INDEX_SECOND_LINE; u8Ind++){
                    u8Tempo[u8Ind][0] =  u8TabResponse[ START_SECOND_PART + u8Ind][0];
                    u8Tempo[u8Ind][1] =  u8TabResponse[ START_SECOND_PART + u8Ind][1];
                }
                
                for(u8Ind2 = 0 ; u8Ind2 < START_SECOND_PART ; u8Ind2++){
                    u8Tempo[u8Ind+ u8Ind2][0] =  u8TabResponse[u8Ind2][0];
                    u8Tempo[u8Ind+ u8Ind2][1] =  u8TabResponse[u8Ind2][1];
                }
                
                for(u8Ind = 0 ; u8Ind < END_CHALL_TAB; u8Ind++){
                    u8TabResponse[u8Ind][0] = u8Tempo[u8Ind][0];
                    u8TabResponse[u8Ind][1] = u8Tempo[u8Ind][1];
                }
                
                // All process done
                return PROCESS_CHALLENGE_DONE ;
            }
        }
        
        // Set Next Challenge to process
        u8Challenge[0]= u8TabChallenge[u8IndexeChall][0];
        u8Challenge[1]= u8TabChallenge[u8IndexeChall][1]+(u8RetryTab2<<4);
        u8Challenge[2]= u8TabChallenge[u8IndexeChall][2];
    }
    
    // Return Value when process is ok
    return PROCESS_OK ;
}
/*==========================================================================*/
/*                          G E T    T A B    1                             */
/*==========================================================================*/
UInt8 getTable1( UInt8  * u8Challenge, UInt8 * u8Response, UInt8  * u8Tab1Col1, UInt8 (*u8Tab1)[16]  ){

    // Declarations Variables
    UInt8 u8Return;
    UInt8 u8endProcess = PROCESS_OK;
    
    // Instructions
    // First Time in the Function, Not yet Responses, Get 1st Challenge 
    if( u8InitGetTab1 == NOT_OK){
        
        // Set Next Challenge to process
        u8Challenge[0]= 0x00;
        u8Challenge[1]= 0x00;
        u8Challenge[2]= 0x00;   
        
        // Incremente Index in Challenge Table
        u8IndexeChall++;
        
        // Init is Done
        u8InitGetTab1 = OK;
        
        // Processing OK
        u8endProcess = PROCESS_OK;
    }else{
    
        // Test If Response is correct
        u8Return = controlChallenge(u8Response);
            
        if( u8Return == NOT_OK)
          {
            // If the Response is not Correct, Retry Process
            
            // Incremente Retry Variable
            u8Retry++;
            
            // Response Not OK , Retry
            u8Challenge[0] = u8Retry;
            u8Challenge[1] = (u8IndexeCol<<4) + u8IndexeLine ; // Not Change
            u8Challenge[2] = 0x00; // Not Change
            
            // End of the retry process after X times
            if(u8Retry == NB_RETRY){
                u8Retry = 0;
                u8endProcess = PROCESS_ERROR; // Error Process
            }
            
        }else{
            u8Retry = 0;
            // Challenge OK 
            
            // State Machine
            switch(u8State){
            
                //----------------------------------------- 
                case FIRST_ELEMENT:
                    // Check first element of Each Line of the Tab 1 
                    u8Return = checkFirstElement( u8Response[1], u8Tab1Col1);
                    
                    // Save Tableau de 16
                    u8Tab1Col1[u8IndexeCol] = u8Response[1];
                    
                    if(u8IndexeCol == END_COL_TAB1){
                        // End of the Process 
                        u8endProcess = PROCESS_TABLE1_DONE;
                    }else if( u8Return == 1 ){
                    
                        // Challenge the First Element of the Next Line
                        u8IndexeCol++;
                        nextChallenge( u8Challenge);
                    }else{
                    
                        // Save Value
                        u8Tab1[u8IndexeOrder][u8IndexeLine] = u8Response[1];
                        
                        // Challenge the next element of the current Line       
                        u8IndexeLine++;
                        nextChallenge( u8Challenge);    
                        
                        // Change State
                        u8State = GET_LINE;
                    }

                  #if DBG_HOBP == 1
                    hobp_dbg_print("\nHoBp Tab1Col1:   " , &u8Tab1Col1[u8IndexeCol] , 1) ;
                  #endif

                
                break;
                
                //-----------------------------------------
                case GET_LINE:
                    // Save Value
                    u8Tab1[u8IndexeOrder][u8IndexeLine] = u8Response[1];
            
                    // Process All the Line Challenge
                    u8IndexeLine++;
                    
                    u8Return = nextChallenge( u8Challenge); 
                    
                    if( u8IndexeLine == END_LINE_TAB1 ){
                        // All the line Challenge are done
                        // Change the State
                        u8State = NEXT_LINE;
                    }
                    
                break;  
                
                //-----------------------------------------     
                case NEXT_LINE:
                    // Save Value of the Last Challenge of the current line
                    u8Tab1[u8IndexeOrder][u8IndexeLine] = u8Response[1];
                    
                    // Incremente Indexe
                    u8IndexeOrder++;
                    u8IndexeCol++;
                    
                    if( u8IndexeCol == END_COL_TAB1 ){
                        // End of the Process 
                        u8endProcess = PROCESS_TABLE1_DONE;
                    }else{
                        // Move to the First Element of the Next Line
                        u8IndexeLine= 0  ;
                        nextChallenge( u8Challenge);    
                        // Change the State
                        u8State = FIRST_ELEMENT;
                    }
                    
                break;
                
                //-----------------------------------------     
            }
        }
    }
    return u8endProcess;
}

/*==========================================================================*/
/*                  C O N T R O L   C H A L L E N G E                       */
/*==========================================================================*/
static UInt8 controlChallenge(unsigned char  * u8Response){
    // Declarations Variables
    UInt8 u8Digit[6]={0}; 
    int i;
    
    // Instructions
    for( i = 0 ; i < 3 ; i++ ){
        u8Digit[(i*2)] = (u8Response[i] >> 4) & 0x0F;
        u8Digit[(i*2)+1] = u8Response[i] & 0x0F;
    }

    // Test If Response is Correct XX XY XX
    if( (u8Digit[0] == u8Digit[1] )  && (u8Digit[1] == u8Digit[2] )  &&
        (u8Digit[2] == u8Digit[4] )  && (u8Digit[4] == u8Digit[5] ) ){
        
        return NOT_OK;
    
    }else{
    
        return OK;
    }
}

/*==========================================================================*/
/*                  C H E C K   F I R S T   E L E M E N T                   */
/*==========================================================================*/
static UInt8 checkFirstElement( UInt8 u8Value , UInt8 * u8Tab1Col1){

    // Declaration Variables
    UInt8 u8Loop; 
    
    // Instructions
    for( u8Loop = 0 ; u8Loop < u8IndexeCol ; u8Loop++){
        // If the value is in the Tab, Return true 
        if( u8Tab1Col1[u8Loop] == u8Value){
            return 1;
        }
    }
    
    return 0;
}

/*==========================================================================*/
/*                      N E X T     C H A L L E N G E                       */
/*==========================================================================*/
static UInt8 nextChallenge( UInt8  * u8Challenge){
    
    u8Challenge[0] = 0x00;
    u8Challenge[1] = (u8IndexeCol<<4) + u8IndexeLine ;
    u8Challenge[2] = 0x00;
    
    return 0;
}

/*==========================================================================*/
/*                       C R E A T E    T A B    1                          */
/*==========================================================================*/
void createTab1(void)
{

    // Declarations Variables
    int i;
    int i2 = 0 ;  
    int iIndexTab = 0;
    int iIndexList = 0;

    // Instructions
    for( i = 0 ; i <= END_COL_TAB1 ; i++){
        
        u8LUT1[iIndexTab] = hobp_vars.nvfs_entry.u8Tab1Col1[i] ;

        iIndexTab++;

        while( hobp_vars.nvfs_entry.u8Tab1Col1[i] != hobp_vars.nvfs_entry.u8Tab1[iIndexList][0] )
          {
            iIndexList++;
          };
        
        for( i2 = 1 ; i2 <= END_LINE_TAB1; i2++)
          {
            u8LUT1[iIndexTab] = hobp_vars.nvfs_entry.u8Tab1[iIndexList][i2] ;
            
            iIndexTab++;


          }
        iIndexList = 0;
    }
}

/*==========================================================================*/
/*                     R E V E R S E    S E E D                             */
/*==========================================================================*/
static int iReverseSeed( const UInt8 * u8Response){

    // Declarations Variables
    int i, i1, iReturnValue, iResponse13;
    int iSeed[16];

    // Instructions
    iResponse13 = (u8Response[0] << 8) + u8Response[2];

    iSeed[0] = (iResponse13 & 0xF000) >> 12;
    iSeed[1] = (iResponse13 & 0x0F00) >> 8;
    iSeed[2] = (iResponse13 & 0x00F0) >> 4;
    iSeed[3] = (iResponse13 & 0x000F);

    for(i =0; i<4 ; i++){
        for(i1 =0; i1< SIZE_SEQ ; i1++){
            if (iSeed[i] == u8LUT_SEQ2[i1]){
                iSeed[i] = i1;
                break;
            }
        }
    }
    iReturnValue = (iSeed[0] << 12) + (iSeed[1] << 8) + (iSeed[2] << 4) + iSeed[3];

    return (iReturnValue&0xFFFF);
}

/*==========================================================================*/
/*                   G E T    V A L U E    S E Q 1                          */
/*==========================================================================*/
static int getValueSeq1( const UInt8 * u8Challenge)
{

    int iValue[4];
    int iReturnValue;

    iValue[0] = u8IndexeSeq[ (u8Challenge[0] >> 4) & 0xF];
    iValue[1] = u8IndexeSeq[  u8Challenge[0]       & 0xF];
    iValue[2] = u8IndexeSeq[ (u8Challenge[2] >> 4) & 0xF];
    iValue[3] = u8IndexeSeq[  u8Challenge[2]       & 0xF];

    iReturnValue = (iValue[0]<<12)+ (iValue[1]<<8) + (iValue[2]<<4) + iValue[3];
    
    return (iReturnValue&0xFFFF);
 
}  

