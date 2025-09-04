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
        Unknown,            // 未知节点
        // 非终结符
        Program,            // 程序
        ExternalDeclList,   // 外部声明列表
        FunctionDef,        // 函数定义
        FunctionDecl,       // 函数声明
        VarDecl,            // 外部变量定义
        LocalVarDecl,       // 局部变量定义
        VarDeclList,        // 局部变量定义列表
        ParamList,          // 参数列表
        Param,              // 参数
        CompoundStmt,       // 复合语句
        StmtList,           // 语句列表

        ExprStmt,           // 表达式语句
        IfStmt,             // 条件语句
        WhileStmt,          // while循环语句
        ForStmt,            // for循环语句
        ReturnStmt,         // 返回语句
        BreakStmt,          // break语句
        ContinueStmt,       // continue语句

        Expr,               // 表达式
        AssignExpr,         // 赋值表达式
        LogicalOrExpr,      // 逻辑或表达式
        LogicalAndExpr,     // 逻辑与表达式
        EqualityExpr,       // 相等表达式
        RelationalExpr,     // 关系表达式
        AdditiveExpr,       // 加法表达式
        MultiplicativeExpr, // 乘法表达式
        UnaryExpr,          // 一元表达式
        PostfixExpr,        // 后缀表达式
        ArgList,            // 函数调用参数列表
        PrimaryExpr,        // 基本表达式
        TypeSpec,           // 类型说明符
        ArrayType,          // 数组类型说明符
        ArrayAccess,        // 数组访问表达式
        ParenthesizedExpr,    // 括号表达式
        // 终结符
        Identifier,         // 标识符
        LongConst,          // 长整型常量
        IntConst,           // 整型常量
        FloatConst,         // 浮点型常量
        CharConst,          // 字符型常量
        StringConst,        // 字符串常量
        LineComment,         // 行注释
        BlockComment        // 块注释
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
