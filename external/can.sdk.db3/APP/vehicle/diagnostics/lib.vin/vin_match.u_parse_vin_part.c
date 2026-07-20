/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: vin_match.u_parse_vin_part.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*            S T A T I C   F U N C T I O N   I N C L U D E                 */
/*==========================================================================*/

static void vin_u_parse_vin_part (Parse_Context * const parse_context)
{
  UInt8 vin_index = vin_u_parse_vin_index (parse_context) ;

  vin_c_parse_ws           (parse_context) ;

  vin_u_parse_token        (parse_context , ':') ;

  vin_c_parse_ws           (parse_context) ;

  vin_u_parse_vin_sym_list (parse_context , parse_context->vin[vin_index]) ;
}

