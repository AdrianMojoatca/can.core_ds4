/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: vin_match.u_parse_vin_sym_list.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*            S T A T I C   F U N C T I O N   I N C L U D E                 */
/*==========================================================================*/

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

