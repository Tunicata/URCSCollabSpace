/* Simple ad-hoc scanner for the calculator language.
   Dies on lexical errors.
   Michael L. Scott, 2008-2021.
*/

#include "scan.h"

char token_image[MAX_TOKEN_LEN];

token scan() {
    static int c = ' ';
        /* next available char; extra (int) width accommodates EOF */
    int i = 0;              /* index into token_image */

    /* skip white space */
    while (isspace(c)) {
        c = getchar();
    }
    if ((c == EOF) || (c == '$'))
        return t_eof;
    if (isalpha(c)) {
        do {
            token_image[i++] = c;
            if (i >= MAX_TOKEN_LEN) {
                cout << "max token length exceeded" << endl;
                exit(1);
            }
            c = getchar();
        } while (isalpha(c) || isdigit(c) || c == '_');
        token_image[i] = '\0';
        if (!((string)token_image).compare("read")) return t_read;
        else if (!((string)token_image).compare("write")) return t_write;
        else if (!((string)token_image).compare("if")) return t_if;
        else if (!((string)token_image).compare("fi")) return t_fi;
        else if (!((string)token_image).compare("do")) return t_do;
        else if (!((string)token_image).compare("od")) return t_od;
        else if (!((string)token_image).compare("check")) return t_check;
        else return t_id;
    }
    else if (isdigit(c)) {
        do {
            token_image[i++] = c;
            c = getchar();
        } while (isdigit(c));
        token_image[i] = '\0';
        return t_literal;
    } else switch (c) {
        case ':':
            if ((c = getchar()) != '=') {
                cerr << "expected '=' after ':', got '" << (char)c << "(0x" << hex << c << ")'" << endl;
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
        case '=':
            if ((c = getchar()) != '=') {
                cerr << "expected '=' after '=', got '" << (char)c << "(0x" << hex << c << ")'" << endl;
                exit(1);
            } else {
                c = getchar();
                return t_eqeq;
            }
            break;
        case '>':
            if ((c = getchar()) == '=') {
                c = getchar();
                return t_gte;
            } else {
                c = getchar();
                return t_gt;
            }
            break;
        case '<':
            if ((c = getchar()) == '=') {
                c = getchar();
                return t_ste;
            } else if ((c = getchar()) == '>') {
                c = getchar();
                return t_neq;
            } else {
                c = getchar();
                return t_st;
            }
            break;
        default:
            cerr << "unexpected character '" << (char)c << "(0x" << hex << c << ")'" << endl;
            exit(1);
    }
}
