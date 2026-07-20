/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: vin_match.c_parse_vin_sym.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*            S T A T I C   F U N C T I O N   I N C L U D E                 */
/*==========================================================================*/

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

