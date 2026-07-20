/*<#======================================================================#>*/
/*                          FILE HEADER GOES HERE                           */
/*<#======================================================================#>*/

/*==========================================================================*/
// $Id: vin_match.u_parse_vin_def_list.c 33257 2015-08-19 12:45:49Z martin.bouchard $
/*==========================================================================*/

/*==========================================================================*/
/*            S T A T I C   F U N C T I O N   I N C L U D E                 */
/*==========================================================================*/

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

