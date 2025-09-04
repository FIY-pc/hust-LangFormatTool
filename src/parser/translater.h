#ifndef TRANSLATER_H
#define TRANSLATER_H
#include "ast.h"

namespace parser {
    static const std::unordered_map<NodeType, std::string> nodeTypeToString = {
        {Program, "Program"},
        {ExternalDeclList, "ExternalDeclList"},
        {FunctionDef, "FunctionDef"},
        {FunctionDecl, "FunctionDecl"},
        {VarDecl, "VarDecl"},
        {LocalVarDecl, "LocalVarDecl"},
        {ParamList, "ParamList"},
        {Param, "Param"},
        {CompoundStmt, "CompoundStmt"},
        {StmtList, "StmtList"},
        {ExprStmt, "ExprStmt"},
        {IfStmt, "IfStmt"},
        {WhileStmt, "WhileStmt"},
        {ForStmt, "ForStmt"},
        {ReturnStmt, "ReturnStmt"},
        {BreakStmt, "BreakStmt"},
        {ContinueStmt, "ContinueStmt"},
        {AssignExpr, "AssignExpr"},
        {LogicalOrExpr, "LogicalOrExpr"},
        {LogicalAndExpr, "LogicalAndExpr"},
        {EqualityExpr, "EqualityExpr"},
        {RelationalExpr, "RelationalExpr"},
        {AdditiveExpr, "AdditiveExpr"},
        {MultiplicativeExpr, "MultiplicativeExpr"},
        {UnaryExpr, "UnaryExpr"},
        {PostfixExpr, "PostfixExpr"},
        {ArgList, "ArgList"},
        {PrimaryExpr, "PrimaryExpr"},
        {TypeSpec, "TypeSpec"},
        {ArrayType, "ArrayType"},
        {ArrayAccess, "ArrayAccess"},
        {Identifier, "Identifier"},
        {LongConst, "LongConst"},
        {IntConst, "IntConst"},
        {FloatConst, "FloatConst"},
        {CharConst, "CharConst"},
        {StringConst, "StringConst"}
    };

    inline std::string getNodeTypeString(NodeType type) {
        auto it = nodeTypeToString.find(type);
        if (it != nodeTypeToString.end()) {
            return it->second;
        }
        return "Unknown";
    }

    static const std::unordered_map<NodeType, std::string> nodeTypeToCNString = {
        {Program, "程序"},
        {ExternalDeclList, "外部声明列表"},
        {FunctionDef, "函数定义"},
        {FunctionDecl, "函数声明"},
        {VarDecl, "变量声明"},
        {LocalVarDecl, "局部变量声明"},
        {ParamList, "参数列表"},
        {Param, "参数"},
        {CompoundStmt, "复合语句"},
        {StmtList, "语句列表"},
        {ExprStmt, "表达式语句"},
        {IfStmt, "if语句"},
        {WhileStmt, "while语句"},
        {ForStmt, "for语句"},
        {ReturnStmt, "return语句"},
        {BreakStmt, "break语句"},
        {ContinueStmt, "continue语句"},
        {AssignExpr, "赋值表达式"},
        {LogicalOrExpr, "逻辑或表达式"},
        {LogicalAndExpr, "逻辑与表达式"},
        {EqualityExpr, "相等表达式"},
        {RelationalExpr, "关系表达式"},
        {AdditiveExpr, "加法表达式"},
        {MultiplicativeExpr, "乘法表达式"},
        {UnaryExpr, "一元表达式"},
        {PostfixExpr, "后缀表达式"},
        {ArgList, "实参列表"},
        {PrimaryExpr, "基本表达式"},
        {TypeSpec, "类型说明符"},
        {ArrayType, "数组类型"},
        {ArrayAccess, "数组访问"},
        {Identifier, "标识符"},
        {LongConst, "长整型常量"},
        {IntConst, "整型常量"},
        {FloatConst, "浮点型常量"},
        {CharConst, "字符型常量"},
        {StringConst, "字符串常量"}
    };

    inline std::string getNodeTypeCNString(NodeType type) {
        auto it = nodeTypeToCNString.find(type);
        if (it != nodeTypeToCNString.end()) {
            return it->second;
        }
        return "未知节点";
    }
}

#endif //TRANSLATER_H
