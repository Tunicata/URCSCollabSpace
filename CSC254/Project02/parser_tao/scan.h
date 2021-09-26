/* Definitions the scanner shares with the parser.
   Michael L. Scott, 2008-2021.
*/

#ifndef SCAN_H
#define SCAN_H

#include <string>

typedef enum {t_read, t_write, t_if, t_fi, t_do, t_od, t_id, t_check, t_literal, t_gets, t_add,
              t_sub, t_mul, t_div, t_lparen, t_rparen, t_eof, t_equal, t_parens, t_less, t_large, 
              t_lessequal, t_largeequal} token;
              
extern std::string token_image;

extern token scan();

#endif