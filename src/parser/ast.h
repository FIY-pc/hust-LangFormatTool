#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include "lexer.h"

namespace parser {
    enum NodeType {
        Unknown,
        // 非终结符
        Program,
        ExternalDeclList,
        FunctionDef,
        FunctionDecl,
        VarDecl,
        LocalVarDecl, // 局部变量定义
        VarDeclList, // 局部变量定义列表
        ParamList,
        Param,
        CompoundStmt,
        StmtList,

        ExprStmt,
        IfStmt,
        WhileStmt,
        ForStmt,
        ReturnStmt,
        BreakStmt,
        ContinueStmt,

        Expr,
        AssignExpr,
        LogicalOrExpr,
        LogicalAndExpr,
        EqualityExpr,
        RelationalExpr,
        AdditiveExpr,
        MultiplicativeExpr,
        UnaryExpr,
        PostfixExpr,
        ArgList,
        PrimaryExpr,
        TypeSpec,
        // 终结符
        Identifier,
        LongConst,
        IntConst,
        FloatConst,
        CharConst,
        StringConst
    };

    static std::unordered_set<NodeType> terminalNodes = {
        Identifier,LongConst, IntConst, FloatConst, CharConst, StringConst
    };

    // 判断节点类型是否为终结符
    inline bool isTerminalNode(NodeType nodeType) {
        if (terminalNodes.find(nodeType) != terminalNodes.end()) {
            return true;
        }
        return false;
    }

    // TokenType到NodeType的映射表
    static std::unordered_map<lexer::TokenKind, NodeType> tokenToNodeType = {
        {lexer::TokenKind::IDENT, Identifier},
        {lexer::TokenKind::LONG_CONST, LongConst},
        {lexer::TokenKind::INT_CONST, IntConst},
        {lexer::TokenKind::FLOAT_CONST, FloatConst},
        {lexer::TokenKind::CHAR_CONST, CharConst},
        {lexer::TokenKind::STRING_CONST, StringConst}
    };

    // 获取TokenKind对应的NodeType
    inline NodeType getTypeFromTokenKind(lexer::TokenKind kind) {
        if (tokenToNodeType.find(kind) != tokenToNodeType.end()) {
            return tokenToNodeType[kind];
        }
        return Unknown;
    }

    struct ASTNode {
        NodeType type;
        std::vector<ASTNode*> children;
        std::string token;

        void print(int depth = 0) {
            for (int i = 0; i < depth; ++i) std::cout << "  ";
            std::cout << type;
            if (!token.empty()) std::cout << ": " << token;
            std::cout << std::endl;
            for (auto child : children) {
                child->print(depth + 1);
            }
        }
        ~ASTNode() {
            for (auto child : children) {
                delete child;
            }
            delete this;
        }
    };
}

#endif //AST_H
