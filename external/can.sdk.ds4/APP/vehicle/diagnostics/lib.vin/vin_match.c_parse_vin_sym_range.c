/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: vin_match.c_parse_vin_sym_range.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*            S T A T I C   F U N C T I O N   I N C L U D E                 */
/*==========================================================================*/

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

