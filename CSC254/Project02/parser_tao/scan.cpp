/* Simple ad-hoc scanner for the calculator language.
   Dies on lexical errors.
   Michael L. Scott, 2008-2021.
*/

#include "cstdio"
#include "stdlib.h"
#include "string.h"
#include "ctype.h"

#include <iostream>
#include <string>

#include "scan.h"

std::string token_image;

token scan() {
    static int c = ' ';
    token_image.clear();

    /* skip white space */
    while (isspace(c)) {
        c = getchar();
    }
    if ((c == EOF) || (c == '$'))
        return t_eof;
    if (isalpha(c)) {
        do {
            token_image.append(1, (char)c);
            c = getchar();
        } while (isalpha(c) || isdigit(c) || c == '_');
        if (! token_image.compare("read")) return t_read;
        else if (! token_image.compare("write")) return t_write;
        else if (! token_image.compare("if")) return t_if;
        else if (! token_image.compare("fi")) return t_fi;
        else if (! token_image.compare("do")) return t_do;
        else if (! token_image.compare("od")) return t_od;
        else if (! token_image.compare("check")) return t_check;
        else return t_id;
    }
    else if (isdigit(c)) {
        do {
            token_image.append(1, (char) c);
            c = getchar();
        } while (isdigit(c));
        return t_literal;
    } else switch (c) {
        case ':':
            if ((c = getchar()) != '=') {
                std::cerr << "expected '=' after ':', got '" << (char)c << "(0x" << std::hex << c << ')' << "'" << std::endl;
                exit(1);
            } else {
                c = getchar();
                return t_gets;
            }
            break;
        case '+': c = getchar(); return t_add;
        case '-': c = getchar(); return t_sub;
        case '*': c = getchar(); return t_mul;
        case '/': c = getchar(); return t_div;
        case '(': c = getchar(); return t_lparen;
        case ')': c = getchar(); return t_rparen;
        case '=': {
            if ((c = getchar()) == '='){
                c = getchar();
                return t_equal;
            }
            else
            {
                std::cerr << "xxx" << std::endl;
                exit(1);
            }
        }
        case '<': {
            if ((c = getchar()) == '>'){
                c = getchar();
                return t_parens;
            }
            else if (c == '='){
                c = getchar();
                return t_lessequal;
            }
            else{
                c = getchar();
                return t_less;
            }
        }
        case '>': {
            if ((c = getchar()) == '='){
                c = getchar();
                return t_largeequal;
            }
            else{
                c = getchar();
                return t_large;
            }
        }

        default:
            std::cerr << "unexpected character '" << (char)c << "(0x" << std::hex << c << ')' << "'" << std::endl;
            c = getchar();
            return scan();
    }
}
