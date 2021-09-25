/* Complete recursive descent parser for the calculator language.
   Builds on figure 2.16 in the text.  Prints a trace of productions
   predicted and tokens matched.  Does no error recovery: prints
   "syntax error" and dies on invalid input.
   Michael L. Scott, 2008-2021.
*/

#include "stdio.h"
#include "stdlib.h"

#include "scan.h"

const char* names[] = {"read", "write", "id", "literal", "gets", "add",
                       "sub", "mul", "div", "lparen", "rparen", "eof",
                       "if", "fi", "do", "od", "check", "eqeq", "neq", "gt", "st", "gte", "ste"};

static token upcoming_token;

void error () {
    fputs ("syntax error\n", stderr);
    exit (1);
}

void match (token expected) {
    if (upcoming_token == expected) {
        printf ("matched %s", names[upcoming_token]);
        if (upcoming_token == t_id || upcoming_token == t_literal)
            printf (": %s", token_image);
        putchar ('\n');
        upcoming_token = scan ();
    }
    else error ();
}

void program ();
void stmt_list ();
void stmt ();
void rel ();
void expr ();
void expr_tail ();
void term ();
void term_tail ();
void factor ();
void factor_tail ();
void rel_op ();
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
            puts ("predict program --> stmt_list eof\n");
            stmt_list ();
            match (t_eof);
            break;
        default: error ();
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
            puts ("predict stmt_list --> stmt stmt_list\n");
            stmt ();
            stmt_list ();
            break;
        case t_eof:
        case t_fi:
        case t_od:
            puts ("predict stmt_list --> epsilon\n");
            break;          /* epsilon production */
        default: error ();
    }
}

void stmt () {
    switch (upcoming_token) {
        case t_id:
            puts ("predict stmt --> id gets expr\n");
            match (t_id);
            match (t_gets);
            rel ();
            break;
        case t_read:
            puts ("predict stmt --> read id\n");
            match (t_read);
            match (t_id);
            break;
        case t_write:
            puts ("predict stmt --> write expr\n");
            match (t_write);
            rel ();
            break;
        case t_if:
            puts ("predict stmt --> if rel stmt_list fi\n");
            match (t_if);
            rel ();
            stmt_list ();
            match (t_fi);
            break;
        case t_do:
            puts ("predict stmt --> do stmt_list od\n");
            match (t_do);
            stmt_list ();
            match (t_od);
            break;
        case t_check:
            puts ("predict stmt --> check rel\n");
            match (t_check);
            rel ();
            break;

        default: error ();
    }
}

void rel () {
    switch (upcoming_token) {
        case t_id:
        case t_literal:
        case t_lparen:
            puts ("predict rel --> expr expr_tail\n");
            expr ();
            expr_tail ();
            break;
        default: error ();
    }
}

void expr () {
    switch (upcoming_token) {
        case t_id:
        case t_literal:
        case t_lparen:
            puts ("predict expr --> term term_tail\n");
            term ();
            term_tail ();
            break;
        default: error ();
    }
}

void expr_tail () {
        switch (upcoming_token) {
        case t_eqeq:
        case t_neq:
        case t_gt:
        case t_st:
        case t_gte:
        case t_ste:
            puts ("predict expr_tail --> rel_op expr\n");
            rel_op ();
            expr ();
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
            puts ("predict expr_tail --> epsilon\n");
            break;          /* epsilon production */
        default: error ();
    }
}

void term () {
    switch (upcoming_token) {
        case t_id:
        case t_literal:
        case t_lparen:
            puts ("predict term --> factor factor_tail\n");
            factor ();
            factor_tail ();
            break;
        default: error ();
    }
}

void term_tail () {
    switch (upcoming_token) {
        case t_add:
        case t_sub:
            puts ("predict term_tail --> add_op term term_tail\n");
            add_op ();
            term ();
            term_tail ();
            break;
        case t_eqeq:
        case t_neq:
        case t_gt:
        case t_st:
        case t_gte:
        case t_ste:
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
            puts ("predict term_tail --> epsilon\n");
            break;          /* epsilon production */
        default: error ();
    }
}

void factor () {
    switch (upcoming_token) {
        case t_literal:
            puts ("predict factor --> literal\n");
            match (t_literal);
            break;
        case t_id :
            puts ("predict factor --> id\n");
            match (t_id);
            break;
        case t_lparen:
            puts ("predict factor --> lparen rel rparen\n");
            match (t_lparen);
            rel ();
            match (t_rparen);
            break;
        default: error ();
    }
}

void factor_tail () {
    switch (upcoming_token) {
        case t_mul:
        case t_div:
            puts ("predict factor_tail --> mul_op factor factor_tail\n");
            mul_op ();
            factor ();
            factor_tail ();
            break;
        case t_add:
        case t_sub:
        case t_eqeq:
        case t_neq:
        case t_gt:
        case t_st:
        case t_gte:
        case t_ste:
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
            puts ("predict factor_tail --> epsilon\n");
            break;          /* epsilon production */
        default: error ();
    }
}

void rel_op () {
    switch (upcoming_token) {
        case t_eqeq:
            puts ("predict rel_op --> eqeq\n");
            match (t_eqeq);
            break;
        case t_neq:
            puts ("predict rel_op --> neq\n");
            match (t_neq);
            break;
        case t_gt:
            puts ("predict rel_op --> gt\n");
            match (t_gt);
            break;
        case t_st:
            puts ("predict rel_op --> st\n");
            match (t_st);
            break;
        case t_gte:
            puts ("predict rel_op --> gte\n");
            match (t_gte);
            break;
        case t_ste:
            puts ("predict rel_op --> ste\n");
            match (t_ste);
            break;
        default: error ();
    }
}

void add_op () {
    switch (upcoming_token) {
        case t_add:
            puts ("predict add_op --> add\n");
            match (t_add);
            break;
        case t_sub:
            puts ("predict add_op --> sub\n");
            match (t_sub);
            break;
        default: error ();
    }
}

void mul_op () {
    switch (upcoming_token) {
        case t_mul:
            puts ("predict mul_op --> mul\n");
            match (t_mul);
            break;
        case t_div:
            puts ("predict mul_op --> div\n");
            match (t_div);
            break;
        default: error ();
    }
}

int main () {
    upcoming_token = scan ();
    program ();
    return 0;
}
