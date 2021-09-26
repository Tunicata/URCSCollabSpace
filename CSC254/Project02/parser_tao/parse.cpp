/* Complete recursive descent parser for the calculator language.
   Builds on figure 2.16 in the text.  Prints a trace of productions
   predicted and tokens matched.  Does no error recovery: prints
   "syntax error" and dies on invalid input.
   Michael L. Scott, 2008-2021.
*/

#include <iostream>
#include <string>
#include <sstream>
#include "stdlib.h"

#include "scan.h"

static token upcoming_token;
static std::stringstream ss;

std::string names[] = {"read", "write", "if", "fi", "do", "od", "id", "check", "literal", "gets", "add",
              "sub", "mul", "div", "lparen", "rparen", "eof", "equal", "parens", "less", "large", 
              "lessequal", "largeequal"};


#define ERROR(FN) {\
    del();\
    FN();\
}

std::string match(token l);

std::string error (token r, token l) {
    std::cerr << "syntax error: " << "expected " << names[l] << ", found " << names[r] << std::endl;
    upcoming_token = scan();
    if (r == t_eof) {
        return "";
    }
    return match(l);
}

std::string match (token expected) {
    if (upcoming_token == expected) {
        // std::cout << "matched " << names[upcoming_token];
        // if (upcoming_token == t_id || upcoming_token == t_literal)
        //     std::cout << ": " << token_image;
        // std::cout << std::endl;
        std::string result;
        if (upcoming_token == t_id || upcoming_token == t_literal)
            result = token_image;
        upcoming_token = scan ();
        return result;
    }
    else return error (upcoming_token, expected);
}

void del () {
    std::cerr << "unexpected token " << names[upcoming_token];
    if (upcoming_token == t_id || upcoming_token == t_literal)
        std::cerr << ": " << token_image;
    std::cerr << std::endl;
    upcoming_token = scan ();
    if (upcoming_token == t_eof) {
        exit(-1);
    }
}

void program ();
void stmt_list ();
void stmt ();
void r ();
void expr ();
void term ();
void factor ();
void expr_tail ();
void term_tail ();
void factor_tail ();
void r_op ();
void add_op ();
void mul_op ();

void program () {
    switch (upcoming_token) {
        case t_id:
        case t_read:
        case t_write:
        case t_if:
        case t_do:
        case t_check: 
        case t_eof:
            // std::cout << "predict program --> stmt_list eof" << std::endl;
            ss << "(program [" << std::endl;
            stmt_list ();
            match (t_eof);
            ss << "] )" << std::endl;
            break;
        default: ERROR (program)
    }
}

void stmt_list () {
    switch (upcoming_token) {
        case t_id:
        case t_read:
        case t_write:
        case t_if:
        case t_do:
        case t_check:
            // std::cout << "predict stmt_list --> stmt stmt_list" << std::endl;
            stmt ();
            stmt_list ();
            break;
        case t_fi:
        case t_od:
        case t_eof:
            // std::cout << "predict stmt_list --> epsilon" << std::endl;
            break;
        default: ERROR (stmt_list)
    }
}

void stmt () {
    switch (upcoming_token) {
        case t_id:
            // std::cout << "predict stmt --> id gets expr" << std::endl;
            ss << "(:= \"" << match (t_id) << '"';
            match (t_gets);
            r ();
            ss << ')' << std::endl;
            break;
        case t_read:
            // std::cout << "predict stmt --> read id" << std::endl;
            match (t_read);
            ss << "(read \"" << match (t_id) << "\")" << std::endl;
            break;
        case t_write:
            // std::cout << "predict stmt --> write expr" << std::endl;
            match (t_write);
            ss << "(write ";
            r ();
            ss << ")" << std::endl;
            break;
        case t_if:
            // std::cout << "predict stmt --> if expr stmt_list fi" << std::endl;
            match (t_if);
            ss << "(if " << std::endl;
            r ();
            stmt_list();
            match (t_fi);
            ss << ") " << std::endl ;
            break;
        case t_do:
            // std::cout << "predict stmt --> do stmt_list od" << std::endl;
            match (t_do);
            ss << "(do [" << std::endl;
            stmt_list();
            match (t_od);
            ss << "]" << std::endl;
            break;
        case t_check:
            // std::cout << "predict stmt --> check expr" << std::endl;
            match (t_check);
            ss << "(check ";
            r();
            ss << ") " << std::endl; 
            break;
        default: ERROR (stmt)
    }
}

void r () {
    switch (upcoming_token) {
        case t_id:
        case t_literal:
        case t_lparen:
            // std::cout << "predict r --> expr expr_tail" << std::endl;
            expr ();
            expr_tail ();
            break;
        default: ERROR (r)
    }
}

void expr () {
    switch (upcoming_token) {
        case t_id:
        case t_literal:
        case t_lparen:
            // std::cout << "predict expr --> term term_tail" << std::endl;
            term ();
            term_tail ();
            break;
        default: ERROR (expr)
    }
}

void term () {
    switch (upcoming_token) {
        case t_id:
        case t_literal:
        case t_lparen:
            // std::cout << "predict term --> factor factor_tail" << std::endl;
            factor ();
            factor_tail ();
            break;
        default: ERROR (term)
    }
}

void factor () {
    switch (upcoming_token) {
        case t_literal:
            // std::cout << "predict factor --> literal" << std::endl;
            ss << "(num \"" << match (t_literal) << "\")";
            break;
        case t_id :
            // std::cout << "predict factor --> id" << std::endl;
            ss << "(id \"" << match (t_id) << "\"))";
            break;
        case t_lparen:
            // std::cout << "predict factor --> lparen expr rparen" << std::endl;
            match (t_lparen);
            expr ();
            match (t_rparen);
            break;
        default: ERROR (factor)
    }
}

void expr_tail () {
     switch (upcoming_token) {
        case t_equal: 
        case t_parens: 
        case t_less: 
        case t_large: 
        case t_lessequal: 
        case t_largeequal: 
            // std::cout << "predict expr_tail --> r_op expr" << std::endl;
            r_op();
            expr();
            break;
        case t_rparen:
        case t_id:
        case t_read:
        case t_write:
        case t_if:
        case t_do:
        case t_check:
        case t_fi:
        case t_od:
        case t_eof:
            // std::cout << "predict expr_tail --> epsilon" << std::endl;
            break;          /* epsilon production */
        default: ERROR (expr_tail)
     }
}

void term_tail () {
    switch (upcoming_token) {
        case t_add:
        case t_sub:
            // std::cout << "predict term_tail --> add_op term term_tail" << std::endl;
            add_op ();
            term ();
            term_tail ();
            break;
        case t_equal: 
        case t_parens: 
        case t_less: 
        case t_large: 
        case t_lessequal: 
        case t_largeequal:
        case t_rparen:
        case t_id:
        case t_read:
        case t_write:
        case t_if:
        case t_do:
        case t_check:
        case t_fi:
        case t_od:
        case t_eof:
            // std::cout << "predict term_tail --> epsilon" << std::endl;
            break;          /* epsilon production */
        default: ERROR (term_tail)
    }
}

void factor_tail () {
    switch (upcoming_token) {
        case t_mul:
        case t_div:
            // std::cout << "predict factor_tail --> mul_op factor factor_tail" << std::endl;
            mul_op ();
            factor ();
            factor_tail ();
            break;
        case t_add:
        case t_sub:
        case t_equal:
        case t_parens:
        case t_less:
        case t_large:
        case t_lessequal:
        case t_largeequal:
        case t_rparen:
        case t_id:
        case t_read:
        case t_write:
        case t_if:
        case t_do:
        case t_check:
        case t_fi:
        case t_od:
        case t_eof:
            // std::cout << "predict factor_tail --> epsilon" << std::endl;
            break;          /* epsilon production */
        default: ERROR (factor_tail)
    }
}

void r_op () {
    switch (upcoming_token) {
        case t_equal:
            // std::cout << "predict r_op --> equal" << std::endl;
            match (t_equal);
            ss << "== ";
            break;
        case t_parens:
            // std::cout << "predict r_op --> parens" << std::endl;
            match (t_parens);
            ss << "<> ";
            break;
        case t_less:
            // std::cout << "predict r_op --> less" << std::endl;
            match (t_less);
            ss << "(< ";
            break;
        case t_large:
            // std::cout << "predict r_op --> large" << std::endl;
            match (t_large);
            ss << "(> ";
            break;
        case t_lessequal:
            // std::cout << "predict r_op --> lessequal" << std::endl;
            match (t_lessequal);
            ss << "(<= ";
            break;
        case t_largeequal:
            // std::cout << "predict r_op --> largeequal" << std::endl;
            match (t_largeequal);
            ss << "(>= ";
            break;
        default: ERROR (r_op)
    }
}

void add_op () {
    switch (upcoming_token) {
        case t_add:
            // std::cout << "predict add_op --> add" << std::endl;
            match (t_add);
            ss << " + ";
            break;
        case t_sub:
            // std::cout << "predict add_op --> sub" << std::endl;
            match (t_sub);
            ss << " - ";
            break;
        default: ERROR (add_op)
    }
}

void mul_op () {
    switch (upcoming_token) {
        case t_mul:
            // std::cout << "predict mul_op --> mul" << std::endl;
            match (t_mul);
            ss << " * ";
            break;
        case t_div:
            // std::cout << "predict mul_op --> div" << std::endl;
            match (t_div);
            ss << " \\ ";
            break;
        default: ERROR (mul_op)
    }
}

int main () {
    upcoming_token = scan ();
    program ();
    std::cout << ss.str();
    return 0;
}
