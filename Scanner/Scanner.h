//
// Created by MengXiangyu on 2022/10/24.
//

#pragma once

#include "TokenType.h"


using namespace std;
class Scanner {
    friend Token;
public:
    Scanner(const char *f_name);
    Scanner(const Scanner &scanner);
    void nextToken(Token &token);
    void setCheckPoint();
    void GoToCheckpoint();
private:
inline  int nextChar();             // Get next char from input file
inline  void putBack();             // Char buffer
inline  void skip();                // Skip useless character
inline  string scanIntLit();        // Scan int literal
inline  string scanText();          // Scan string
private:
    int ch = 0;
    int put_back = 0;
    FILE *fp;
    long file_read_offset_ = 0;
};


