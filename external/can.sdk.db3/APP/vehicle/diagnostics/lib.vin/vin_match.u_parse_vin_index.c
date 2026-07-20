/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: vin_match.u_parse_vin_index.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*            S T A T I C   F U N C T I O N   I N C L U D E                 */
/*==========================================================================*/

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

