#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include <iostream>

namespace parser {
    enum NodeType {
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
        IntConst,
        FloatConst,
        CharConst,
        StringConst
    };

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
