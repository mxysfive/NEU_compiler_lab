//
// Created by MengXiangyu on 2022/10/24.
//
#include <cstdio>
#include <string>
#include "Scanner.h"
using namespace std;
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

KEYWORDS KWLIST;

Scanner::Scanner(const char *f_name) {
    fp = fopen(f_name, "r");
    if (fp == nullptr) {
        fprintf(stderr, "Can't open file: %s", f_name);
        exit(1);
    }
}

Scanner::Scanner(const Scanner &scanner) {
    this->ch = scanner.ch;
    this->put_back = scanner.put_back;
    this->fp = scanner.fp;
}



inline int Scanner::nextChar() {
    if (unlikely(put_back != 0)) {
        ch = put_back;
        put_back = 0;
        return ch;
    } else {
        ch = fgetc(fp);
        return ch;
    }
}

inline void Scanner::putBack() {
    put_back = ch;
}

inline void Scanner::skip() {
    while((ch >= '\0' && ch <= ' ')|| ch == '\n' || ch == '\t') {
        ch = fgetc(fp);
    }
    // ch hit at a non ' '-like character
}

inline string Scanner::scanIntLit() {
    string str_ret;
    while(isdigit(ch)) {
        str_ret += char(ch);
        ch = nextChar();
    }
    putBack();
    return  str_ret;
}

inline string Scanner::scanText() {
    string str_ret;
    while(isalpha(ch) || isdigit(ch) || ch == '_') {
        str_ret += char(ch);
        ch = nextChar();
    }
    putBack();
    return str_ret;
}


void Scanner::nextToken(Token &token) {
    ch = nextChar();
    skip();
    static string str;
    static string str_value;
    switch (ch) {
        case EOF:
            token.token_type = EOFTK;
            break;
        case '+':
            str = "PLUS";
            str_value = "+";
            token.load(PLUS, str, str_value);
            break;
        case '-':
            str = "MINU";
            str_value = "-";
            token.load(MINU, str, str_value);
            break;
        case '*':
            str = "MULT";
            str_value = "*";
            token.load(MULT, str, str_value);
            break;
        case '/':
            str = "DIV";
            str_value = "/";
            token.load(DIV, str, str_value);
            break;
        case '(':
            str = "LPARENT";
            str_value = "(";
            token.load(LPARENT, str, str_value);
            break;
        case ')':
            str = "RPARENT";
            str_value = ")";
            token.load(RPARENT, str, str_value);
            break;
        case '[':
            str = "LBRACK";
            str_value = "[";
            token.load(LBRACK, str, str_value);
            break;
        case ']':
            str = "RBRACK";
            str_value = "]";
            token.load(RBRACK, str, str_value);
            break;
        case '{':
            str = "LBRACE";
            str_value = "{";
            token.load(LBRACE, str, str_value);
            break;
        case '}':
            str = "RBRACE";
            str_value = "}";
            token.load(RBRACE, str, str_value);
            break;
        case '=':
            if ((ch = nextChar()) == '=') {
                str = "EQL";
                str_value = "==";
                token.load(EQL, str, str_value);
            } else {
                putBack();
                str = "ASSIGN";
                str_value = "=";
                token.load(ASSIGN, str, str_value);
            }
            break;
        case '<':
            if ((ch = nextChar()) == '=') {
                str = "LEQ";
                str_value = "<=";
                token.load(LEQ, str, str_value);
            } else {
                putBack();
                str = "LSS";
                str_value = "<";
                token.load(LSS, str, str_value);
            }
            break;
        case '>':
            if ((ch = nextChar()) == '=') {
                str = "GEQ";
                str_value = ">=";
                token.load(GEQ, str, str_value);
            } else {
                putBack();
                str = "GRE";
                str_value = ">";
                token.load(GRE, str, str_value);
            }
            break;
        case ',':
            str = "COMMA";
            str_value = ",";
            token.load(COMMA, str, str_value);
            break;
        case ';':
            str = "SEMICN";
            str_value = ";";
            token.load(SEMICN, str, str_value);
            break;
        case '!':
            if ((ch = nextChar()) == '=') {
                str = "NEQ";
                str_value = "!=";
                token.load(NEQ, str, str_value);
            } else {
                putBack();
                fprintf(stderr, "wrong token at line %d\n", __LINE__);
                exit(1);
            }
            break;
        case '\'':
            str = "CHARCON";
            ch = nextChar();
            str_value = char(ch);
            if ((ch = nextChar()) != '\'') {
                fprintf(stderr, "Illegal char type at line %d\n", __LINE__);
            }
            token.load(CHARCON, str, str_value);
            break;
        case '"':
            str = "STRCON";
            str_value = "";
            ch = nextChar();
            while (likely(ch != '"')) {
                if ((ch >= '#' && ch <= '~') || ch == ' ' || ch == '!') {
                    str_value += char(ch);
                }
                ch = nextChar();
            }
            token.load(STRCON, str, str_value);
            break;
        default:
            if (isdigit(ch)) {
                str = "INTCON";
                str_value = scanIntLit();
                token.load(INTCON, str, str_value);
            }
            else if (isalpha(ch) || ch == '_') {
                str_value = scanText();
/*                if (KWLIST.empty()) {
                    fprintf(stderr, "Not Initialize KWLIST\n");
                    exit(1);
                }*/
                auto iter = KWLIST.find(str_value);
                if (iter == KWLIST.end()) {
                    // It is an identifier
                    str = "IDENFR";
                    token.load(IDENFER, str, str_value);
                } else if (iter != KWLIST.end()) {
                    token.load(iter->second.second, iter->second.first, str_value);
                }

            }
    }
}

void Scanner::setCheckPoint() {
    file_read_offset_ = ftell(fp);
    if (put_back != 0) --file_read_offset_;
}

void Scanner::GoToCheckpoint() {
    fseek(fp, file_read_offset_, SEEK_SET);
    put_back = 0;
}

void KW_INIT() {
    KWLIST["const"] = make_pair("CONSTTK", CONSTTK);
    KWLIST["int"] = make_pair("INTTK", INTTK);
    KWLIST["char"] = make_pair("CHARTK", CHARTK);
    KWLIST["void"] = make_pair("VOIDTK", VOIDTK);
    KWLIST["main"] = make_pair("MAINTK", MAINTK);
    KWLIST["if"] = make_pair("IFTK", IFTK);
    KWLIST["else"] = make_pair("ELSETK", ELSETK);
    KWLIST["do"] = make_pair("DOTK", DOTK);
    KWLIST["while"] = make_pair("WHILETK", WHILETK);
    KWLIST["for"] = make_pair("FORTK", FORTK);
    KWLIST["scanf"] = make_pair("SCANFTK", SCANFTK);
    KWLIST["printf"] = make_pair("PRINTFTK", PRINTFTK);
    KWLIST["return"] = make_pair("RETURNTK", RETURNTK);
}



