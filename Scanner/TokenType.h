//
// Created by MengXiangyu on 2022/10/23.
//
#pragma once
#include <iostream>
#include <unordered_map>
using namespace std;
enum token_type {
    IDENFER = 0,       // Identifier
    INTCON,        // INT Literal
    CHARCON,       // CHAR Literal
    STRCON,        // STR Literal
    CONSTTK,       // keyword: const
    INTTK,         // keyword: int
    CHARTK,        // keyword: char
    VOIDTK,        // keyword: void
    MAINTK,        // keyword: main
    IFTK,          // keyword: if
    ELSETK,        // keyword: else
    DOTK,          // keyword: do
    WHILETK,       // keyword: while
    FORTK,         // keyword: for
    SCANFTK,       // keyword: scanf
    PRINTFTK,      // keyword: printf
    RETURNTK,      // keyword: return
    PLUS,          // operation: +
    MINU,          // operation: -
    MULT,          // operation: *
    DIV,           // operation: /
    LSS,           // operation: <
    LEQ,           // operation: <=
    GRE,           // operation: >
    GEQ,           // operation: >=
    EQL,           // operation: ==
    NEQ,           // operation: !=
    ASSIGN,        // assign stmt: =
    SEMICN,        // ;
    COMMA,         // ,
    LPARENT,       // (
    RPARENT,       // )
    LBRACK,        // [
    RBRACK,        // ]
    LBRACE,        // {
    RBRACE,        // }
    EOFTK,         // End of File
};

class Token {
public:
    Token(Token &token) {
        token_type = token.token_type;
        type = token.type;
        tk_value = token.tk_value;
    }
    Token(){token_type = 0; type = "", tk_value= "";};
    void load(int tk_type, string &tp, string &tk_val){
        this->token_type = tk_type;
        this->type = tp;
        this->tk_value = tk_val;
    }

    void print(){
        cout<<type<<' '<<tk_value<<endl;

    }
    int token_type;
    string type;
    string tk_value ;
};

using KEYWORDS = unordered_map<string, pair<string, token_type>>;
extern KEYWORDS KWLIST;
void KW_INIT();




