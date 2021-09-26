/* Complete recursive descent parser for the calculator language.
   Builds on figure 2.16 in the text.  Prints a trace of productions
   predicted and tokens matched.  Does no error recovery: prints
   "syntax error" and dies on invalid input.
   Michael L. Scott, 2008-2021.
*/

#include <iostream>
#include <string>
#include <stack>

#include "scan.h"

using namespace std;

const string names[] = {"read", "write", "id", "literal", "gets", "add",
                       "sub", "mul", "div", "lparen", "rparen", "eof",
                       "if", "fi", "do", "od", "check", "eqeq", "neq", "gt", "st", "gte", "ste"};

static token upcoming_token;
static bool error_flag;
static stack<string> preop_handler;
string tab = "";

#define ERROR(FN) {\
    del();\
    FN();\
}

string match(token expected);
string error(token r, token l) ;
void del();

string match (token expected) {
    if (upcoming_token == expected) {
        cout << "matched " << names[upcoming_token];
        string result;
        if (upcoming_token == t_id || upcoming_token == t_literal)
            cout << ": " << token_image;
            result = token_image;
        cout << endl;
        upcoming_token = scan ();
        return result;
    }
    else return error (upcoming_token, expected);

    return "";
}

string error (token r, token l) {
    std::cerr << "syntax error: " << "expected " << names[l] << ", found " << names[r] << std::endl;
    upcoming_token = scan();
    if (r == t_eof) {
        return "";
    }
    return match(l);
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

string program ();
string stmt_list ();
string stmt ();
string rel ();
string expr ();
string expr_tail ();
string term ();
string term_tail ();
string factor ();
string factor_tail ();
string rel_op ();
string add_op ();
string mul_op ();

string program () {
    string ast = "(program [";
    tab += "  ";
    switch (upcoming_token) {
        case t_id:
        case t_read:
        case t_write:
        case t_if:
        case t_do:
        case t_check:
        case t_eof:
            cout << "predict program --> stmt_list eof\n" << endl;
            ast += stmt_list ();
            match (t_eof);
            ast += "]";
            ast += ")";
            break;
            tab.resize (tab.size () - 2);
            ast += tab;
        default: ERROR (program);
    }
    return ast;
}

string stmt_list () {
    string ast = "\n";
    switch (upcoming_token) {
        case t_id:
        case t_read:
        case t_write:
        case t_if:
        case t_do:
        case t_check:
            cout << "predict stmt_list --> stmt stmt_list\n" << endl;
            ast += stmt ();
            ast += stmt_list ();
            break;
        case t_eof:
        case t_fi:
        case t_od:
            cout << "predict stmt_list --> epsilon\n" << endl;
            break;          /* epsilon production */
        default: ERROR (stmt_list)
    }
    return ast;
}

string stmt () {
    string ast = (tab + "(");
    switch (upcoming_token) {
        case t_id:
            cout << "predict stmt --> id gets expr\n" << endl;
            ast += ":= \"" + match(t_id) + "\" ";
            match (t_gets);
            ast += rel ();
            break;
        case t_read:
            cout << "predict stmt --> read id\n" << endl;
            match (t_read);
            ast += "read (id \"" + match(t_id) + "\")";
            break;
        case t_write:
            cout << "predict stmt --> write expr\n" << endl;
            match (t_write);
            ast += "write ";
            ast += rel ();
            break;
        case t_if:
            tab += "  ";
            cout << "predict stmt --> if rel stmt_list fi\n" << endl;
            match (t_if);
            ast += "if ";
            ast += rel ();
            ast += " [";
            ast += stmt_list ();
            match (t_fi);
            tab.resize (tab.size () - 2);
            ast += (tab + "]");
            break;
        case t_do:
            tab += "  ";
            cout << "predict stmt --> do stmt_list od\n" << endl;
            match (t_do);
            ast += "do [ ";
            ast += stmt_list ();
            match (t_od);
            tab.resize (tab.size () - 2);
            ast += (tab + "]");
            break;
        case t_check:
            cout << "predict stmt --> check rel\n" << endl;
            match (t_check);
            ast += "check ";
            ast += rel ();
            break;
        default: ERROR (stmt)
    }
    ast += ")";
    return ast;
}

string rel () {
    string ast = "";
    switch (upcoming_token) {
        case t_id:
        case t_literal:
        case t_lparen:
            cout << "predict rel --> expr expr_tail\n" << endl;
            preop_handler.push(expr ());
            // ast += expr ();
            ast += expr_tail ();
            break;
        default: ERROR (rel)
    }
    return ast;
}

string expr () {
    string ast = "";
    switch (upcoming_token) {
        case t_id:
        case t_literal:
        case t_lparen:
            cout << "predict expr --> term term_tail\n" << endl;
            preop_handler.push(term ());
            // ast += term ();
            ast += term_tail ();
            break;
        default: ERROR (expr)
    }
    return ast;
}

string expr_tail () {
        string ast ="";
        switch (upcoming_token) {
        case t_eqeq:
        case t_neq:
        case t_gt:
        case t_st:
        case t_gte:
        case t_ste:
            cout << "predict expr_tail --> rel_op expr\n" << endl;
            ast += ("(" + rel_op () + " " + preop_handler.top() + " ");
            preop_handler.pop();
            preop_handler.push(expr ());
            ast += preop_handler.top();
            preop_handler.pop();
            ast += ")";
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
            cout << "predict expr_tail --> epsilon\n" << endl;
            ast += preop_handler.top();
            preop_handler.pop();
            break;          /* epsilon production */
        default: ERROR (expr_tail)
    }
    return ast;
}

string term () {
    string ast = "";
    switch (upcoming_token) {
        case t_id:
        case t_literal:
        case t_lparen:
            cout << "predict term --> factor factor_tail\n" << endl;
            preop_handler.push(factor ());
            ast += factor_tail ();
            break;
        default: ERROR (term)
    }
    return ast;
}

string term_tail () {
    string ast = "";
    switch (upcoming_token) {
        case t_add:
        case t_sub:
            cout << "predict term_tail --> add_op term term_tail\n" << endl;
            ast += ("(" + add_op () + " " + preop_handler.top() + " ");
            preop_handler.pop();
            preop_handler.push(term ());
            ast += (term_tail () + ")");
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
            cout << "predict term_tail --> epsilon\n" << endl;
            ast += preop_handler.top();
            preop_handler.pop();
            break;          /* epsilon production */
        default: ERROR (term_tail)
    }
    return ast;
}

string factor () {
    string ast = "(";
    switch (upcoming_token) {
        case t_literal:
            cout << "predict factor --> literal\n" << endl;
            ast += "num \"" + match (t_literal) + "\"";
            break;
        case t_id :
            cout << "predict factor --> id\n" << endl;
            ast += "id \"" + match (t_id) + "\"";
            break;
        case t_lparen:
            cout << "predict factor --> lparen rel rparen\n" << endl;
            match (t_lparen);
            ast += rel ();
            match (t_rparen);
            break;
        default: ERROR (factor)
    }
    ast += ")";
    return ast;
}

string factor_tail () {
    string ast = "";
    switch (upcoming_token) {
        case t_mul:
        case t_div:
            cout << "predict factor_tail --> mul_op factor factor_tail\n" << endl;
            ast += ("(" + mul_op () + " " + preop_handler.top() + " ");
            preop_handler.pop();
            preop_handler.push(factor ());
            ast += (factor_tail () + ")");
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
            cout << "predict factor_tail --> epsilon\n" << endl;
            ast += preop_handler.top();
            preop_handler.pop();
            break;          /* epsilon production */
        default: ERROR (factor_tail)
    }
    return ast;
}

string rel_op () {
    string ast = "";
    switch (upcoming_token) {
        case t_eqeq:
            cout << "predict rel_op --> eqeq\n" << endl;
            match (t_eqeq);
            ast += "==";
            break;
        case t_neq:
            cout << "predict rel_op --> neq\n" << endl;
            match (t_neq);
            ast += "<>";
            break;
        case t_gt:
            cout << "predict rel_op --> gt\n" << endl;
            match (t_gt);
            ast += ">";
            break;
        case t_st:
            cout << "predict rel_op --> st\n" << endl;
            match (t_st);
            ast += "<";
            break;
        case t_gte:
            cout << "predict rel_op --> gte\n" << endl;
            match (t_gte);
            ast += ">=";
            break;
        case t_ste:
            cout << "predict rel_op --> ste\n" << endl;
            match (t_ste);
            ast += "<=";
            break;
        default: ERROR (rel_op)
    }
    return ast;
}

string add_op () {
    string ast = "";
    switch (upcoming_token) {
        case t_add:
            cout << "predict add_op --> add\n" << endl;
            match (t_add);
            ast += "+";
            break;
        case t_sub:
            cout << "predict add_op --> sub\n" << endl;
            match (t_sub);
            ast += "-";
            break;
        default: ERROR (add_op)
    }
    return ast;
}

string mul_op () {
    string ast = "";
    switch (upcoming_token) {
        case t_mul:
            cout << "predict mul_op --> mul\n" << endl;
            match (t_mul);
            ast += "*";
            break;
        case t_div:
            cout << "predict mul_op --> div\n" << endl;
            match (t_div);
            ast += "/";
            break;
        default: ERROR (mul_op)
    }
    return ast;
}

int main () {
    upcoming_token = scan ();
    string ast = program ();

    cout << endl << ast << endl;
    return 0;
}
