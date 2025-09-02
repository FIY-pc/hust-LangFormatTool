#ifndef PARSER_H
#define PARSER_H
#include "lexer.h"
#include "ast.h"
#include "token.h"

namespace parser {
    class Parser {
    public:
        explicit Parser(FILE *file, bool debug = false,std::string output="ast.txt");
        explicit Parser(lexer::Lexer &lexer, bool debug = false,std::string output="ast.txt");
        ~Parser();
        ASTNode* parse(); // 解析输入的Token序列，返回AST根节点
        void outputAST(std::string& filename);
        bool debug = false;
        std::string output;
        lexer::Lexer lexer;
        void debugLog(const std::string& funcName, int pos) const {
            if (!debug) return;
            if (pos < tokens.size()) {
                const auto& tk = tokens[pos];
                std::cout << "[DEBUG] " << funcName << " pos=" << pos << " token=[" << lexer::TokenKindToString(tk.kind) << "] '" << tk.text << "' (line " << tk.line << ", col " << tk.column << ")" << std::endl;
            } else {
                std::cout << "[DEBUG] " << funcName << " pos=" << pos << " (end of tokens)" << std::endl;
            }
        }
    private:
        ASTNode* root;
        std::vector<lexer::Token> tokens;
        int pos;
        void error(const std::string& msg) const;

        // 顶层结构
        ASTNode* parseProgram();
        ASTNode* parseExternalDeclList();
        ASTNode* parseExternalDecl();

        // 变量声明
        ASTNode* parseVarDecl();
        ASTNode* parseLocalVarDecl();

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
    };
}

#endif //PARSER_H
