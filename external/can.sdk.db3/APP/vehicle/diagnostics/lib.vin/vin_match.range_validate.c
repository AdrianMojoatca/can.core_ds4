/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: vin_match.range_validate.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*            S T A T I C   F U N C T I O N   I N C L U D E                 */
/*==========================================================================*/

static Boolean vin_range_validate (const Vin_Sym_Range * const vin_sym_range)
{
  #define SAME_TYPE (vin_sym_range->min.is_numeric == vin_sym_range->max.is_numeric)
  #define LIMITS_OK (vin_sym_range->min.sym        <= vin_sym_range->max.sym       ) 

  return  SAME_TYPE && LIMITS_OK ;

  #undef  LIMITS_OK
  #undef  SAME_TYPE
}

