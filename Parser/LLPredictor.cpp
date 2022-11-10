//
// Created by MengXiangyu on 2022/10/25.
//
#include "LLPredictor.h"

Token ckp_token;
FILE *output = fopen("output.txt", "w");

LLPredictor::LLPredictor(Scanner &scanner, Token &token) : scanner_(scanner), token_(token) {};

LLPredictor::~LLPredictor() {
}

bool LLPredictor::match(token_type tk_type) {
    if (token_.token_type == tk_type) {
        //token_.print();
        fprintf(output,"%s %s\n",token_.type.c_str(), token_.tk_value.c_str());
        scanner_.nextToken(token_);
        return true;
    }
    //cout<<"Not match for token:"<<token_.type<<" current token value: "<<token_.tk_value<<endl;
    return false;
}



void LLPredictor::UInteger() {
    if (match(INTCON)) {
        //cout<<"<无符号整数>"<<endl;
        fprintf(output, "<无符号整数>\n");
    } else{
        cout<<"Error in:"<<__FUNCTION__ <<". TokenType is:"<<token_.token_type<<endl;
        exit(1);
    }
}

void LLPredictor::Integer() {
    match(PLUS);
    match(MINU);
    UInteger();
    //cout<<"<整数>"<<endl;
    fprintf(output, "<整数>\n");
}

bool LLPredictor::RelationOp() {
    switch (token_.token_type) {
        case LSS:
            match(LSS);
            return true;
        case LEQ:
            match(LEQ);
            return true;
        case GRE:
            match(GRE);
            return true;
        case GEQ:
            match(GEQ);
            return true;
        case EQL:
            match(EQL);
            return true;
        case NEQ:
            match(NEQ);
            return true;
        default:
            cout<<"Error in: "<<__FUNCTION__ <<" Token is"<<token_.type<<endl;
            return false;
    }
}



void LLPredictor::Char() {
    if (match(CHARCON)) {
        return;
    }
    cout<<"Error in:"<<__FUNCTION__ <<". TokenType is:"<<token_.token_type<<endl;
}
void LLPredictor::Program() {
    scanner_.nextToken(token_);
    while (1) {
        if (token_.token_type == CONSTTK) {
            ConstDeclare();
        }
        else if (token_.token_type == INTTK || token_.token_type == CHARTK) {
            VariableDeclare();
            //TODO: fixing duplicate print statement
            ValueFuncDefine();
        }
        else if (token_.token_type == VOIDTK) {
            ckp_token = token_;
            scanner_.setCheckPoint();
            if (VoidFuncDefine()) {
                continue;
            }
            token_ = ckp_token;
            scanner_.GoToCheckpoint();
            MainFunc();
        }
        else break;
    }
    cout<<"<程序>"<<endl;
    fprintf(output, "<程序>\n");

}

void LLPredictor::ConstDefine() {
    if (match(INTTK)) {
        do {
            match(IDENFER);
            match(ASSIGN);
            Integer();
        } while (match(COMMA));
    } else if(match(CHARTK)) {
        do {
            match(IDENFER);
            match(ASSIGN);
            Char();
        } while (match(COMMA));
    }
    //cout<<"<常量定义>"<<endl;
    fprintf(output, "<常量定义>\n");
}

bool LLPredictor::ConstDeclare() {
    if (token_.token_type == CONSTTK) {
        while (match(CONSTTK)) {
            ConstDefine();
            match(SEMICN);
        }
        //cout<<"<常量说明>"<<endl;
        fprintf(output, "<常量说明>\n");
        return true;
    }
    return false;
}

bool LLPredictor::ParaList() {
    if (match(INTTK) || match(CHARTK)) {
        match(IDENFER);
        while (match(COMMA)) {
            // match int or char
            match(INTTK);
            match(CHARTK);
            match(IDENFER);
        }
        //cout<<"<参数表>"<<endl;
        fprintf(output, "<参数表>\n");
        return true;
    }
    //cout<<"<参数表>"<<endl;
    fprintf(output, "<参数表>\n");
    return true;
}

bool LLPredictor::ValueFuncDefine() {
    switch (token_.token_type) {
        case INTTK:
            match(INTTK);
            break;
        case CHARTK:
            match(CHARTK);
            break;
        default:
            return false;
    }
    if (token_.token_type == IDENFER) {
        func_table_[token_.tk_value] = R_VALUE;
    }
    if (!match(IDENFER)) return false;
    //cout<<"<声明头部>"<<endl;
    fprintf(output, "<声明头部>\n");
    if (!match(LPARENT)) return false;
    ParaList();
    if (!match(RPARENT)) return false;
    if (!match(LBRACE)) return false;
    CompoundStmt();
    if (!match(RBRACE)) return false;
    //cout<<"<有返回值函数定义>"<<endl;
    fprintf(output, "<有返回值函数定义>\n");
    return true;
}

bool LLPredictor::VoidFuncDefine() {
    Token tk_cp1;
    if (token_.token_type != VOIDTK) return false;
    tk_cp1 = token_;
    scanner_.nextToken(token_);
    //scanner_.setCheckPoint();
    if(token_.token_type != IDENFER) {
        return false;
    }
    tk_cp1.print();
    fprintf(output,"%s %s\n",tk_cp1.type.c_str(), tk_cp1.tk_value.c_str());
    // Add Identifier as void function name into func_table_
    func_table_[token_.tk_value] = R_VOID;
    match(IDENFER);
    if (!match(LPARENT)) return false;
    ParaList();
    if (!match(RPARENT)) return false;
    if (!match(LBRACE)) return false;
    // TODO: Add compound statement func here
    CompoundStmt();
    if (!match(RBRACE)) return false;
    //cout<<"<无返回值函数定义>"<<endl;
    fprintf(output, "<无返回值函数定义>\n");
    return true;
}

bool LLPredictor::VariableDefine() {
    Token tk_cp1, tk_cp2;
    switch (token_.token_type) {
        case INTTK:
        case CHARTK:
            tk_cp1 = token_;
            scanner_.nextToken(token_);
            break;
        default:
            return false;
    }

        if (token_.token_type != IDENFER) return false;
        tk_cp2 = token_;
        scanner_.nextToken(token_);
        if (token_.token_type == LPARENT) {
            //TODO: Distinguish return value between detect function and not matching
            return false;
        }
        // Ensure that it is not Value-Return Function, print previous stored token
        tk_cp1.print();
        fprintf(output,"%s %s\n",tk_cp1.type.c_str(), tk_cp1.tk_value.c_str());
        tk_cp2.print();
        fprintf(output,"%s %s\n",tk_cp2.type.c_str(), tk_cp2.tk_value.c_str());
        if (match(LBRACK)) {
            // Case for array
            UInteger();
            if (!match(RBRACK)) return false;
        }
    while (match(COMMA)) {
        if (!match(IDENFER)) return false;
        if (match(LBRACK)) {
            // Case for array
            UInteger();
            if (!match(RBRACK)) return false;
        }
    }
    //cout<<"<变量定义>"<<endl;
    fprintf(output, "<变量定义>\n");
    return true;
}

void LLPredictor::VariableDeclare() {
    int var_define_count = 0;
    do {
        scanner_.setCheckPoint();
        ckp_token = token_;
        if (!VariableDefine()) {
            // recovery to checkpoint
            // and then finish var declare
            scanner_.GoToCheckpoint();
            token_ = ckp_token;
            break;
        }
        match(SEMICN);
        ++var_define_count;
    } while (token_.token_type == INTTK || token_.token_type == CHARTK);
    if (var_define_count > 0) {
        //cout<<"变量说明>"<<endl;
        fprintf(output, "<变量说明>\n");
    }
}

void LLPredictor::VarParaList() {
    if (token_.token_type == RPARENT) {
        // no parameter to call this function
    }
    else {
        Expression();
        while (match(COMMA)) {
            Expression();
        }
    }
    //cout<<"<值参数表>"<<endl;
    fprintf(output, "<值参数表>\n");
}

// Return true means accepte a Function Call Statement
// False if it 's not a function call
bool LLPredictor::FunCall() {
    Token tk_cp1;
    string func_name;
    if(token_.token_type != IDENFER) return false;
    // Reserve func name
    func_name = token_.tk_value;
    tk_cp1 = token_;
    scanner_.nextToken(token_);
    if (token_.token_type != LPARENT) {
        return false;
    }
    tk_cp1.print();
    fprintf(output,"%s %s\n",tk_cp1.type.c_str(), tk_cp1.tk_value.c_str());
    if (!match(LPARENT)) return false;
    VarParaList();
    if(!match(RPARENT)) return false;

    //TODO()
    // Build Table to store/search Function information
    auto iter = func_table_.find(func_name);
    if (iter == func_table_.end()) return false;

    if (iter->second == R_VOID) {
        //cout<<"<无返回值函数调用语句>"<<endl;
        fprintf(output, "<无返回值函数调用语句>\n");
    } else if(iter->second == R_VALUE) {
        //cout<<"<有返回值函数调用语句>"<<endl;
        fprintf(output, "<有返回值函数调用语句>\n");
    }
    return true;
}

bool LLPredictor::Factor() {
    Token tk_cp;
    switch (token_.token_type) {
        case INTCON:
            Integer();
            //cout<<"<因子>"<<endl;
            fprintf(output, "<因子>\n");
            return true;
        case CHARCON:
            Char();
            //cout<<"<因子>"<<endl;
            fprintf(output, "<因子>\n");
            return true;
        case LPARENT:
            match(LPARENT);
            Expression();
            match(RPARENT);
            //cout<<"<因子>"<<endl;
            fprintf(output, "<因子>\n");
            return true;
        case IDENFER:
            scanner_.setCheckPoint();
            tk_cp = token_;
            if(FunCall()) {
                //cout<<"<因子>"<<endl;
                fprintf(output, "<因子>\n");
                return true;
            }
            // If not FunCall, In grammar, we need to check whether next token is LBRACK
            scanner_.GoToCheckpoint();
            token_ = tk_cp;
            if (!match(IDENFER)) return false;
            if (match(LBRACK)) {
                Expression();
                match(RBRACK);
            }
            //cout<<"<因子>"<<endl;
            fprintf(output, "<因子>\n");
            return true;
    }
    return false;
}

bool LLPredictor::Term() {
    if(!Factor()) return false;
    if (match(MULT) || match(DIV)) {
        Factor();
    }
    //cout<<"<项>"<<endl;
    fprintf(output, "<项>\n");
    return true;
}

void LLPredictor::Expression() {
    // Attention: Only matching case, nextToken will be called.
    match(PLUS);
    match(MINU);
    Term();
    while(match(PLUS) || match(MINU)) {
        Term();
    }
    //cout<<"<表达式>"<<endl;
    fprintf(output, "<表达式>\n");
}

void LLPredictor::Condition() {
    Expression();
    if (RelationOp()) {
        Expression();
    }
    //cout<<"<条件>"<<endl;
    fprintf(output, "<条件>\n");
}

bool LLPredictor::AssignStmt() {
    if (!match(IDENFER)) return false;

    if (match(LBRACK)) {
        Expression();
        if (!match(RBRACK)) return false;
    }
    if (!match(ASSIGN)) return false;
    Expression();
    //cout<<"<赋值语句>"<<endl;
    fprintf(output, "<赋值语句>\n");
    return true;
}

bool LLPredictor::IFStmt() {
    if (!match(IFTK)) return false;
    if (!match(LPARENT)) return false;
    Condition();
    if (!match(RPARENT)) return false;
    Statement();
    if (match(ELSETK)) {
        Statement();
    }
    //cout<<"<条件语句>"<<endl;
    fprintf(output, "<条件语句>\n");
    return true;
}

bool LLPredictor::LoopStmt() {
    switch (token_.token_type) {
        case WHILETK:
            if (!match(WHILETK)) return false;
            if (!match(LPARENT)) return false;
            Condition();
            if (!match(RPARENT)) return false;
            Statement();
            break;
        case DOTK:
            if (!match(DOTK)) return false;
            Statement();
            if (!match(WHILETK)) return false;
            if (!match(LPARENT)) return false;
            Condition();
            if(!match(RPARENT)) return false;
            break;
        case FORTK:
            if (!match(FORTK)) return false;
            if (!match(LPARENT)) return false;
            if (!match(IDENFER)) return false;
            if (!match(ASSIGN)) return false;
            Expression();
            if (!match(SEMICN)) return false;
            Condition();
            if (!match(SEMICN)) return false;
            if (!match(IDENFER)) return false;
            if (!match(ASSIGN)) return false;
            if (!match(IDENFER)) return false;
            switch (token_.token_type) {
                case PLUS:
                    match(PLUS);
                case MINU:
                    match(MINU);
                default:
                    cout<<"Wrong syntax in For-loop statement"<<endl;
            }
            UInteger();
            //cout<<"<步长>"<<endl;
            fprintf(output, "<步长>\n");
            if (!match(RPARENT)) return false;
            Statement();
            break;
        default:
            //cout<<__FUNCTION__ <<": Not a keyword for Loop"<<endl;
            return false;
    }
    //cout<<"<循环语句>"<<endl;
    fprintf(output, "<循环语句>\n");
}

bool LLPredictor::ReadStmt() {
    if (!match(SCANFTK)) return false;
    if (!match(LPARENT)) return false;
    match(IDENFER);
    while (match(COMMA)) {
        match(IDENFER);
    }
    if (!match(RPARENT)) return false;
    //cout<<"<读语句>"<<endl;
    fprintf(output, "<读语句>\n");
    return true;
}

bool LLPredictor::WriteStmt() {
    if (!match(PRINTFTK)) return false;
    if (!match(LPARENT)) return false;
    if (token_.token_type == STRCON) {
        match(STRCON);
        //cout<<"<字符串>"<<endl;
        fprintf(output, "<字符串>\n");
        if (match(COMMA)) { Expression(); }
    }
    else {
        Expression();
    }
    if (!match(RPARENT)) return false;
    //cout<<"<写语句>"<<endl;
    fprintf(output, "<写语句>\n");
    return true;
}

bool LLPredictor::ReturnStmt() {
    if (!match(RETURNTK)) return false;
    if (match(LPARENT)) {
        Expression();
        if (!match(RPARENT)) return false;
        //cout<<"<返回语句>"<<endl;
        fprintf(output, "<返回语句>\n");
        return true;
    }
    //cout<<"<返回语句>"<<endl;
    fprintf(output, "<返回语句>\n");
    return true;
}

bool LLPredictor::Statement() {
    Token ckp_token;
    switch (token_.token_type) {
        case IFTK:
            IFStmt();
            //cout<<"<语句>"<<endl;
            fprintf(output, "<语句>\n");
            if (!match(SEMICN)) return false;
            return true;
        case WHILETK:
        case DOTK:
        case FORTK:
            LoopStmt();
            //cout<<"<语句>"<<endl;
            fprintf(output, "<语句>\n");
            if (!match(SEMICN)) return false;
            //cout<<"<语句>"<<endl;
            fprintf(output, "<语句>\n");
            return true;
        case LBRACE:
            match(LBRACE);
            StatementList();
            if (!match(RBRACE)) {
                return false;
            }
            //cout<<"<语句>"<<endl;
            fprintf(output, "<语句>\n");
            return true;
        case IDENFER:
            scanner_.setCheckPoint();
            ckp_token = token_;
            // Function call
            if (FunCall()) {
                if (!match(SEMICN)) return false;
                //cout<<"<语句>"<<endl;
                fprintf(output, "<语句>\n");
                return true;
            }
            // If not Function call, it must be AssignStmt
            scanner_.GoToCheckpoint();
            token_ = ckp_token;
            AssignStmt();
            if (!match(SEMICN)) return false;
            //cout<<"<语句>"<<endl;
            fprintf(output, "<语句>\n");
            return true;
        case SCANFTK:
            ReadStmt();
            if (!match(SEMICN)) return false;
            //cout<<"<语句>"<<endl;
            fprintf(output, "<语句>\n");
            return true;
        case PRINTFTK:
            WriteStmt();
            if (!match(SEMICN)) return false;
            //cout<<"<语句>"<<endl;
            fprintf(output, "<语句>\n");
            return true;
        case RETURNTK:
            ReturnStmt();
            if (!match(SEMICN)) return false;
            //cout<<"<语句>"<<endl;
            fprintf(output, "<语句>\n");
            return true;
        default:
            return true;

    }
}

bool LLPredictor::MainFunc() {
    if (!match(VOIDTK)) return false;
    if (!match(MAINTK)) return false;
    if (!match(LPARENT)) return false;
    if (!match(RPARENT)) return false;
    if (!match(LBRACE)) return false;
    // TODO: add compound statement function here
    CompoundStmt();
    if (!match(RBRACE)) return false;
    //cout<<"<主函数>"<<endl;
    fprintf(output, "<主函数>\n");
    return true;
}

bool LLPredictor::StatementList() {
    while (token_.token_type == IFTK || token_.token_type == FORTK
    || token_.token_type == WHILETK || token_.token_type == DOTK
    || token_.token_type == IDENFER || token_.token_type == SCANFTK
    || token_.token_type == PRINTFTK || token_.token_type == RETURNTK ) {
        Statement();
    }
    //cout<<"<语句列>"<<endl;
    fprintf(output, "<语句列>\n");
    return true;
}

bool LLPredictor::CompoundStmt() {
    if (token_.token_type == CONSTTK) ConstDeclare();
    if (token_.token_type == INTTK || token_.token_type == CHARTK) VariableDeclare();
    // If not above two branch, execute StatementList()
    StatementList();
    //cout<<"<复合语句>"<<endl;
    fprintf(output, "<复合语句>\n");
    return true;
}
