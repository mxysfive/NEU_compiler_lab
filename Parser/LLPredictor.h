//
// Created by MengXiangyu on 2022/10/25.
//

#pragma once
#include <iostream>
#include "../Scanner/Scanner.h"

using namespace std;

enum FUNC_TYPE{R_VALUE, R_VOID};

// Implementation for LL(1) predictor
class LLPredictor {
public:
    LLPredictor(Scanner &scanner, Token &token);
    void Program();
    ~LLPredictor();
private:
    bool match(token_type tk_type);
    void Char();
    void UInteger();
    void Integer();
    bool RelationOp();
    // For const
    bool ConstDeclare();
    void ConstDefine();
    // For Variable
    void VariableDeclare();
    bool VariableDefine();
    bool ParaList();
    void VarParaList();
    bool ValueFuncDefine();
    bool VoidFuncDefine();
    bool FunCall();
    bool MainFunc();
    void Expression();
    bool Term();
    bool Factor();
    void Condition();
    bool LoopStmt();
    bool AssignStmt();
    bool IFStmt();
    bool ReadStmt();
    bool WriteStmt();
    bool ReturnStmt();
    bool Statement();
    bool StatementList();
    bool CompoundStmt();
private:
    Scanner scanner_;
    Token token_;
    unordered_map<string, FUNC_TYPE> func_table_;
};




