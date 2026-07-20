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

//#include "vin_match.u_parse_eol.c"
static void vin_u_parse_eol (Parse_Context * const parse_context)
{
  if (parse_context->vin_def_token [0] != 0)
    {
      vin_throw_exception (parse_context , VIN_ERR_SYNTAX) ;
    }
}


//#include "vin_match.u_parse_token.c"
static void vin_u_parse_token (Parse_Context * const parse_context , const char token)
{
  if (!vin_c_parse_token (parse_context , token))
    {
      vin_throw_exception (parse_context , VIN_ERR_SYNTAX) ;
    }
}



//#include "vin_match.u_parse_vin_part.c"
static void vin_u_parse_vin_part (Parse_Context * const parse_context)
{
  UInt8 vin_index = vin_u_parse_vin_index (parse_context) ;

  vin_c_parse_ws           (parse_context) ;

  vin_u_parse_token        (parse_context , ':') ;

  vin_c_parse_ws           (parse_context) ;

  vin_u_parse_vin_sym_list (parse_context , parse_context->vin[vin_index]) ;
}



//#include "vin_match.u_parse_vin_index.c"
static UInt8 vin_u_parse_vin_index (Parse_Context * const parse_context)  
{
  UInt16 vin_index = 0 ;

  Token_Position token_position ;

  Boolean at_least_one_digit = FALSE ;

  vin_token_position_get (parse_context , & token_position) ;

  while (vin_sym_is_digit (parse_context->vin_def_token [0]))
    {
      at_least_one_digit = TRUE ;

      vin_index *= 10 ;
      vin_index += *parse_context->vin_def_token++ - '0' ;

      if (vin_index > VIN_SIZE)
        {
          break ;
        }
    }

  if (!at_least_one_digit)
    {
      vin_throw_exception    (parse_context , VIN_ERR_SYNTAX) ;
    }

  if (vin_index == 0 || vin_index > VIN_SIZE)
    {
      vin_token_position_set (parse_context , & token_position) ;

      vin_throw_exception    (parse_context , VIN_ERR_INDEX) ;
    }

  return vin_index - 1;
}



//#include "vin_match.u_parse_vin_def.c"
static void vin_u_parse_vin_def (Parse_Context * const parse_context)
{
  vin_c_parse_ws           (parse_context) ;

  vin_u_parse_vin_def_list (parse_context) ;

  vin_c_parse_ws           (parse_context) ;

  vin_u_parse_eol          (parse_context) ;
}


//#include "vin_match.u_parse_vin_def_list.c"
static void vin_u_parse_vin_def_list (Parse_Context * const parse_context)
{
  Token_Position token_position ;

  for (;;)
    {
      vin_u_parse_vin_part   (parse_context) ;

      vin_token_position_get (parse_context , & token_position) ;

      vin_c_parse_ws         (parse_context) ;

      if (!vin_c_parse_token (parse_context , ','))
        {
          vin_token_position_set (parse_context , & token_position) ;

          return ;
        }

      vin_c_parse_ws         (parse_context) ;
    }
}



//#include "vin_match.u_parse_vin_sym.c"
static void vin_u_parse_vin_sym (Parse_Context * const parse_context , Vin_Sym * const vin_sym)
{
  if (!vin_c_parse_vin_sym (parse_context , vin_sym))
    {
      vin_throw_exception  (parse_context , VIN_ERR_SYNTAX) ;
    }
}


//#include "vin_match.u_parse_vin_sym_list.c"
static void vin_u_parse_vin_sym_list (Parse_Context * const parse_context , UInt8 const vin_digit)
{
  Vin_Sym_Range vin_sym_range ;

  Boolean match_found = FALSE ;

  Boolean at_least_one_range = FALSE ;

  while (vin_c_parse_vin_sym_range (parse_context , & vin_sym_range))
    {
      at_least_one_range = TRUE  ;

      match_found |= vin_range_search (& vin_sym_range , vin_digit) ;
    }

  if (!at_least_one_range)
    {
      vin_throw_exception (parse_context , VIN_ERR_SYNTAX) ;
    }

  if (!match_found)
    {
      parse_context->vin_status = VIN_NOMATCH ;
    }
}





/// Conditional parse functions \\\

//#include "vin_match.c_parse_ws.c"
static void vin_c_parse_ws (Parse_Context * const parse_context)
{
  #define WS_1 parse_context->vin_def_token [0] == ' '
  #define WS_2 parse_context->vin_def_token [0] == '\t'

  while  (WS_1 || WS_2)
    {
      parse_context->vin_def_token++ ;
    }

  #undef  WS_2
  #undef  WS_1
}



//#include "vin_match.c_parse_token.c"
static Boolean vin_c_parse_token (Parse_Context * const parse_context , const char token)
{
  if (parse_context->vin_def_token [0] == token)
    {
      parse_context->vin_def_token++ ;

      return TRUE ;
    }

  return FALSE ;
}



//#include "vin_match.c_parse_vin_sym.c"
static Boolean vin_c_parse_vin_sym (Parse_Context * const parse_context , Vin_Sym * const vin_sym)
{
  if (vin_sym_is_digit (parse_context->vin_def_token [0]))
    {
      vin_sym->is_numeric = TRUE  ;
    }
  else if (vin_sym_is_letter (parse_context->vin_def_token [0]))
    {
      vin_sym->is_numeric = FALSE ;
    }
  else
    {
      return FALSE ;
    }

  vin_sym->sym = *parse_context->vin_def_token++ ;

  return TRUE ;
}



//#include "vin_match.c_parse_vin_sym_range.c"
static Boolean vin_c_parse_vin_sym_range (Parse_Context * const parse_context , Vin_Sym_Range * const vin_sym_range)
{
  if (vin_c_parse_vin_sym (parse_context , & vin_sym_range->min))
    {
      if (vin_c_parse_token (parse_context , '-'))
        {
          vin_u_parse_vin_sym (parse_context , & vin_sym_range->max) ;

          if (!vin_range_validate (vin_sym_range))
            {
              vin_throw_exception (parse_context , VIN_ERR_RANGE) ;
            }
        }
      else
        {
          vin_sym_range->max = vin_sym_range->min ;
        }

      return TRUE ;
    }

  return FALSE ;
}




/// Token position management functions \\\

//#include "vin_match.token_position_set.c"
static void vin_token_position_set   (Parse_Context * const parse_context , Token_Position * const token_position)
{
  parse_context->vin_def_token = *token_position ;
}


//#include "vin_match.token_position_get.c"
static void vin_token_position_get   (Parse_Context * const parse_context , Token_Position * const token_position)
{
  *token_position = parse_context->vin_def_token ;
}

/// Exception throw functions \\\

//#include "vin_match.throw_exception.c"
static void vin_throw_exception (Parse_Context * const parse_context , const Vin_Status vin_status)
{
  parse_context->vin_status = vin_status     ;

  longjmp (parse_context->exception_vec , 1) ;
}


/// Support Functions \\\

//#include "vin_match.sym_is_digit.c"
static Boolean vin_sym_is_digit (char const vin_sym)
{
  return '0' <= vin_sym && vin_sym <= '9' ;
}



//#include "vin_match.sym_is_letter.c"
static Boolean vin_sym_is_letter (char const vin_sym)
{
  return 'A' <= vin_sym && vin_sym <= 'Z' ;
}



//#include "vin_match.range_search.c"
static Boolean vin_range_search (const Vin_Sym_Range * const vin_sym_range , UInt8 const vin_digit)
{
  char sym = vin_sym_range->min.sym ;

  do
    {
      if ((UInt8) sym == vin_digit)
        {
          return TRUE ;
        }
    }
  while (sym++ < vin_sym_range->max.sym) ;

  return FALSE ;
}



//#include "vin_match.range_validate.c"
static Boolean vin_range_validate (const Vin_Sym_Range * const vin_sym_range)
{
  #define SAME_TYPE (vin_sym_range->min.is_numeric == vin_sym_range->max.is_numeric)
  #define LIMITS_OK (vin_sym_range->min.sym        <= vin_sym_range->max.sym       ) 

  return  SAME_TYPE && LIMITS_OK ;

  #undef  LIMITS_OK
  #undef  SAME_TYPE
}


