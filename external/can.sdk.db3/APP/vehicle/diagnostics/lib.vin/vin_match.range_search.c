/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: vin_match.range_search.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*            S T A T I C   F U N C T I O N   I N C L U D E                 */
/*==========================================================================*/

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

