/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: vin_match.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*                             I N C L U D E S                              */
/*==========================================================================*/

#include <setjmp.h>

#include "vin_private.h"

/*==========================================================================*/
/*      D E F I N E S  -  E N U M E R A T I O N S  -  T Y P E D E F S       */
/*==========================================================================*/

typedef const char * Token_Position ;

//--------------------------------------------------------------------------//

typedef struct
{
  char     sym            ;
  unsigned is_numeric : 1 ;

} Vin_Sym ;

//--------------------------------------------------------------------------//

typedef struct
{
  Vin_Sym min ;
  Vin_Sym max ;

} Vin_Sym_Range ;

//--------------------------------------------------------------------------//

typedef struct
{
  const UInt8  * vin           ;
  Vin_Status     vin_status    ;
  Token_Position vin_def_token ;

  jmp_buf        exception_vec ;

} Parse_Context ;

/*==========================================================================*/
/*                  F U N C T I O N   P R O T O T Y P E S                   */
/*==========================================================================*/

static void    vin_u_parse_vin_def       (Parse_Context * const parse_context) ;
static void    vin_u_parse_vin_def_list  (Parse_Context * const parse_context) ;
static void    vin_u_parse_vin_part      (Parse_Context * const parse_context) ;
static UInt8   vin_u_parse_vin_index     (Parse_Context * const parse_context) ;
static void    vin_u_parse_vin_sym       (Parse_Context * const parse_context , Vin_Sym * const vin_sym  ) ;
static void    vin_u_parse_vin_sym_list  (Parse_Context * const parse_context , UInt8     const vin_digit) ;
static void    vin_u_parse_eol           (Parse_Context * const parse_context) ;
static void    vin_u_parse_token         (Parse_Context * const parse_context , char const token) ;
               
static void    vin_c_parse_ws            (Parse_Context * const parse_context) ;
static Boolean vin_c_parse_vin_sym       (Parse_Context * const parse_context , Vin_Sym       * const vin_sym      ) ;
static Boolean vin_c_parse_vin_sym_range (Parse_Context * const parse_context , Vin_Sym_Range * const vin_sym_range) ;
static Boolean vin_c_parse_token         (Parse_Context * const parse_context , char const token) ; 

static void    vin_token_position_set    (Parse_Context * const parse_context , Token_Position * const token_position) ;
static void    vin_token_position_get    (Parse_Context * const parse_context , Token_Position * const token_position) ;

static void    vin_throw_exception       (Parse_Context * const parse_context , Vin_Status const vin_status) ;

static Boolean vin_range_validate        (const Vin_Sym_Range * const vin_sym_range                        ) ;
static Boolean vin_range_search          (const Vin_Sym_Range * const vin_sym_range , UInt8 const vin_digit) ;

static Boolean vin_sym_is_digit          (char const vin_sym) ;
static Boolean vin_sym_is_letter         (char const vin_sym) ;

/*==========================================================================*/
/*                            V A R I A B L E S                             */
/*==========================================================================*/

/*==========================================================================*/
/*                 F U N C T I O N   D E F I N I T I O N S                  */
/*==========================================================================*/

Vin_Status vin_match (const char vin_def [] , Vin const vin)
{
  Parse_Context parse_context ;

  parse_context.vin           = vin       ;
  parse_context.vin_def_token = vin_def   ;
  parse_context.vin_status    = VIN_MATCH ;

  if (setjmp (parse_context.exception_vec) == 0)
    {
      vin_u_parse_vin_def (& parse_context) ;
    }

  return parse_context.vin_status ;
}

/*==========================================================================*/
/*            S T A T I C   F U N C T I O N   I N C L U D E S               */
/*==========================================================================*/

/// Unconditional parse functions \\\

#include "vin_match.u_parse_eol.c"
#include "vin_match.u_parse_token.c"
#include "vin_match.u_parse_vin_part.c"
#include "vin_match.u_parse_vin_index.c"
#include "vin_match.u_parse_vin_def.c"
#include "vin_match.u_parse_vin_def_list.c"
#include "vin_match.u_parse_vin_sym.c"
#include "vin_match.u_parse_vin_sym_list.c"

/// Conditional parse functions \\\

#include "vin_match.c_parse_ws.c"
#include "vin_match.c_parse_token.c"
#include "vin_match.c_parse_vin_sym.c"
#include "vin_match.c_parse_vin_sym_range.c"

/// Token position management functions \\\

#include "vin_match.token_position_set.c"
#include "vin_match.token_position_get.c"

/// Exception throw functions \\\

#include "vin_match.throw_exception.c"

/// Support Functions \\\

#include "vin_match.sym_is_digit.c"
#include "vin_match.sym_is_letter.c"

#include "vin_match.range_search.c"
#include "vin_match.range_validate.c"

