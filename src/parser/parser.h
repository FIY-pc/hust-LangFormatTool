#ifndef PARSER_H
#define PARSER_H
#include "lexer.h"
#include "ast.h"
#include "token.h"

namespace parser {
    class Parser {
    public:
        Parser(FILE *file);
        Parser(lexer::Lexer &lexer);
        ~Parser();
        ASTNode* parse(); // 解析输入的Token序列，返回AST根节点
    private:
        lexer::Lexer lexer;
        ASTNode* root;
        std::vector<lexer::Token> tokens;

        // 顶层结构
        ASTNode* parseProgram();
        ASTNode* parseExternalDeclList();
        ASTNode* parseExternalDecl();

        // 变量声明
        ASTNode* parseVarDecl();

        // 函数声明与定义
        ASTNode* parseFunctionDecl();
        ASTNode* parseFunctionDef();
        ASTNode* parseParamList();
        ASTNode* parseParamListTail();
        ASTNode* parseParam();

        // 复合语句与语句列表
        ASTNode* parseCompoundStmt();
        ASTNode* parseStmtList();

        // 各类语句
        ASTNode* parseStmt();
        ASTNode* parseExprStmt();
        ASTNode* parseIfStmt();
        ASTNode* parseWhileStmt();
        ASTNode* parseForStmt();
        ASTNode* parseReturnStmt();
        ASTNode* parseBreakStmt();
        ASTNode* parseContinueStmt();

        // 表达式相关
        ASTNode* parseExpr();
        ASTNode* parseAssignExpr();
        ASTNode* parseLogicalOrExpr();
        ASTNode* parseLogicalAndExpr();
        ASTNode* parseEqualityExpr();
        ASTNode* parseRelationalExpr();
        ASTNode* parseAdditiveExpr();
        ASTNode* parseMultiplicativeExpr();
        ASTNode* parseUnaryExpr();
        ASTNode* parsePostfixExpr();
        ASTNode* parseArgList();
        ASTNode* parsePrimaryExpr();

        // 类型说明符
        ASTNode* parseTypeSpec();
        int pos;
    };
}

#endif //PARSER_H
