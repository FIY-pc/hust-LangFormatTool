#include "parser.h"
#include "ast.h"
#include <vector>
#include <fstream>
#include <iostream>

namespace parser {
    // 输出缩进
    static void printIndent(std::ofstream& out, int indent) {
        for (int i = 0; i < indent; ++i) out << "    ";
    }

    // 递归输出AST
    static void outputASTNode(std::ofstream& out, ASTNode* node, int indent = 0) {
        if (!node) return;
        switch (node->type) {
            case NodeType::VarDecl: {
                printIndent(out, indent);
                out << "外部变量定义:\n";
                // 类型
                if (!node->children.empty() && node->children[0]->type == NodeType::TypeSpec) {
                    printIndent(out, indent + 1);
                    out << "类型: " << node->children[0]->token << "\n";
                }
                // 变量名
                printIndent(out, indent + 1);
                out << "变量名:\n";
                if (node->children.size() > 1 && node->children[1]->type == NodeType::Identifier) {
                    printIndent(out, indent + 2);
                    out << "ID: " << node->children[1]->token << "\n";
                }
                // 初始化表达式
                if (node->children.size() > 2) {
                    printIndent(out, indent + 1);
                    out << "初始化表达式:\n";
                    outputASTNode(out, node->children[2], indent + 2);
                }
                break;
            }
            case NodeType::LocalVarDecl: {
                printIndent(out, indent);
                out << "局部变量定义:\n";
                // 类型
                if (!node->children.empty() && node->children[0]->type == NodeType::TypeSpec) {
                    printIndent(out, indent + 1);
                    out << "类型: " << node->children[0]->token << "\n";
                }
                // 变量名
                printIndent(out, indent + 1);
                out << "变量名:\n";
                if (node->children.size() > 1 && node->children[1]->type == NodeType::Identifier) {
                    printIndent(out, indent + 2);
                    out << "ID: " << node->children[1]->token << "\n";
                }
                // 初始化表达式
                if (node->children.size() > 2) {
                    printIndent(out, indent + 1);
                    out << "初始化表达式:\n";
                    outputASTNode(out, node->children[2], indent + 2);
                }
                break;
            }
            case NodeType::FunctionDef: {
                printIndent(out, indent);
                out << "函数定义:\n";
                // 类型
                if (!node->children.empty() && node->children[0]->type == NodeType::TypeSpec) {
                    printIndent(out, indent + 1);
                    out << "类型: " << node->children[0]->token << "\n";
                }
                // 函数名
                if (node->children.size() > 1 && node->children[1]->type == NodeType::Identifier) {
                    printIndent(out, indent + 1);
                    out << "函数名: " << node->children[1]->token << "\n";
                }
                // 参数
                if (node->children.size() > 2 && node->children[2]->type == NodeType::ParamList) {
                    printIndent(out, indent + 1);
                    out << "函数参数:\n";
                    for (auto* param : node->children[2]->children) {
                        if (param->type == NodeType::Param) {
                            printIndent(out, indent + 2);
                            out << "类型: " << param->children[0]->token << ", 参数名: " << param->children[1]->token << "\n";
                        }
                    }
                }
                // 复合语句
                if (!node->children.empty() && node->children.back()->type == NodeType::CompoundStmt) {
                    printIndent(out, indent + 1);
                    out << "复合语句:\n";
                    outputASTNode(out, node->children.back(), indent + 2);
                }
                break;
            }
            case NodeType::FunctionDecl: {
                printIndent(out, indent);
                out << "函数声明:\n";
                // 类型
                if (!node->children.empty() && node->children[0]->type == NodeType::TypeSpec) {
                    printIndent(out, indent + 1);
                    out << "类型: " << node->children[0]->token << "\n";
                }
                // 函数名
                if (node->children.size() > 1 && node->children[1]->type == NodeType::Identifier) {
                    printIndent(out, indent + 1);
                    out << "函数名: " << node->children[1]->token << "\n";
                }
                // 参数
                if (node->children.size() > 2 && node->children[2]->type == NodeType::ParamList) {
                    printIndent(out, indent + 1);
                    out << "函数参数:\n";
                    for (auto* param : node->children[2]->children) {
                        if (param->type == NodeType::Param) {
                            printIndent(out, indent + 2);
                            out << "类型: " << param->children[0]->token << ", 参数名: " << param->children[1]->token << "\n";
                        }
                    }
                }
                break;
            }
            case NodeType::CompoundStmt: {
                // 复合语句的变量定义和语句部分
                printIndent(out, indent);
                out << "复合语句的变量定义:\n";
                if (!node->children.empty() && node->children[0]->type == NodeType::VarDeclList) {
                    outputASTNode(out, node->children[0], indent + 1);
                }
                printIndent(out, indent);
                out << "复合语句的语句部分:\n";
                if (node->children.size() > 1 && node->children[1]->type == NodeType::StmtList) {
                    outputASTNode(out, node->children[1], indent + 1);
                }
                break;
            }
            case NodeType::VarDeclList: {
                for (auto* child : node->children) {
                    outputASTNode(out, child, indent);
                }
                break;
            }
            case NodeType::StmtList: {
                for (auto* child : node->children) {
                    outputASTNode(out, child, indent);
                }
                break;
            }
            case NodeType::IfStmt: {
                printIndent(out, indent);
                out << "条件语句(IF_THEN_ELSE):\n";
                printIndent(out, indent + 1);
                out << "条件:\n";
                outputASTNode(out, node->children[0], indent + 2);
                printIndent(out, indent + 1);
                out << "IF子句:\n";
                outputASTNode(out, node->children[1], indent + 2);
                if (node->children.size() > 2) {
                    printIndent(out, indent + 1);
                    out << "ELSE子句:\n";
                    outputASTNode(out, node->children[2], indent + 2);
                }
                break;
            }
            case NodeType::ExprStmt: {
                printIndent(out, indent);
                out << "表达式语句:\n";
                for (auto* child : node->children) {
                    outputASTNode(out, child, indent + 1);
                }
                break;
            }
            case NodeType::ReturnStmt: {
                printIndent(out, indent);
                out << "返回语句:\n";
                for (auto* child : node->children) {
                    outputASTNode(out, child, indent + 1);
                }
                break;
            }
            case NodeType::AssignExpr: {
                printIndent(out, indent);
                out << "赋值表达式 (ASSIGNOP):\n";
                if (node->children.size() > 0) {
                    printIndent(out, indent + 1);
                    out << "左值:\n";
                    outputASTNode(out, node->children[0], indent + 2);
                }
                if (node->children.size() > 1) {
                    printIndent(out, indent + 1);
                    out << "右值:\n";
                    outputASTNode(out, node->children[1], indent + 2);
                }
                break;
            }
            case NodeType::LogicalAndExpr: {
                printIndent(out, indent);
                out << "逻辑与表达式 (&&):\n";
                for (auto* child : node->children) {
                    outputASTNode(out, child, indent + 1);
                }
                break;
            }
            case NodeType::LogicalOrExpr: {
                printIndent(out, indent);
                out << "逻辑或表达式 (||):\n";
                for (auto* child : node->children) {
                    outputASTNode(out, child, indent + 1);
                }
                break;
            }
            case NodeType::EqualityExpr: {
                printIndent(out, indent);
                out << "相等表达式 (" << node->token << "):\n";
                for (auto* child : node->children) {
                    outputASTNode(out, child, indent + 1);
                }
                break;
            }
            case NodeType::RelationalExpr: {
                printIndent(out, indent);
                out << "关系表达式 (" << node->token << "):\n";
                for (auto* child : node->children) {
                    outputASTNode(out, child, indent + 1);
                }
                break;
            }
            case NodeType::AdditiveExpr: {
                printIndent(out, indent);
                out << "加减表达式 (" << node->token << "):\n";
                for (auto* child : node->children) {
                    outputASTNode(out, child, indent + 1);
                }
                break;
            }
            case NodeType::MultiplicativeExpr: {
                printIndent(out, indent);
                out << "乘除模表达式 (" << node->token << "):\n";
                for (auto* child : node->children) {
                    outputASTNode(out, child, indent + 1);
                }
                break;
            }
            case NodeType::UnaryExpr: {
                printIndent(out, indent);
                out << "一元表达式 (" << node->token << "):\n";
                for (auto* child : node->children) {
                    outputASTNode(out, child, indent + 1);
                }
                break;
            }
            case NodeType::PostfixExpr: {
                printIndent(out, indent);
                out << "函数调用:\n";
                if (!node->children.empty() && node->children[0]->type == NodeType::Identifier) {
                    printIndent(out, indent + 1);
                    out << "函数名: " << node->children[0]->token << "\n";
                }
                if (node->children.size() > 1 && node->children[1]->type == NodeType::ArgList) {
                    printIndent(out, indent + 1);
                    out << "参数列表:\n";
                    outputASTNode(out, node->children[1], indent + 2);
                }
                break;
            }
            case NodeType::ArgList: {
                for (auto* child : node->children) {
                    outputASTNode(out, child, indent);
                }
                break;
            }
            case NodeType::Identifier: {
                printIndent(out, indent);
                out << "ID: " << node->token << "\n";
                break;
            }
            case NodeType::IntConst: {
                printIndent(out, indent);
                out << "INT_CONST: " << node->token << "\n";
                break;
            }
            case NodeType::FloatConst: {
                printIndent(out, indent);
                out << "FLOAT_CONST: " << node->token << "\n";
                break;
            }
            case NodeType::CharConst: {
                printIndent(out, indent);
                out << "CHAR_CONST: " << node->token << "\n";
                break;
            }
            case NodeType::StringConst: {
                printIndent(out, indent);
                out << "STRING_CONST: " << node->token << "\n";
                break;
            }
            case NodeType::Program: {
                printIndent(out, indent);
                out << "Program(程序):\n";
                for (auto* child : node->children) {
                    outputASTNode(out, child, indent + 1);
                }
                break;
            }
            case NodeType::ExternalDeclList: {
                printIndent(out, indent);
                out << "ExternalDeclList(外部声明列表):\n";
                for (auto* child : node->children) {
                    outputASTNode(out, child, indent + 1);
                }
                break;
            }
            case NodeType::WhileStmt: {
                printIndent(out, indent);
                out << "循环语句(WHILE):\n";
                printIndent(out, indent + 1);
                out << "条件:\n";
                outputASTNode(out, node->children[0], indent + 2);
                printIndent(out, indent + 1);
                out << "循环体:\n";
                outputASTNode(out, node->children[1], indent + 2);
                break;
            }
            case NodeType::ForStmt: {
                printIndent(out, indent);
                out << "循环语句(FOR):\n";
                printIndent(out, indent + 1);
                out << "初始化:\n";
                outputASTNode(out, node->children[0], indent + 2);
                printIndent(out, indent + 1);
                out << "条件:\n";
                outputASTNode(out, node->children[1], indent + 2);
                printIndent(out, indent + 1);
                out << "步进:\n";
                outputASTNode(out, node->children[2], indent + 2);
                printIndent(out, indent + 1);
                out << "循环体:\n";
                outputASTNode(out, node->children[3], indent + 2);
                break;
            }
            case NodeType::BreakStmt: {
                printIndent(out, indent);
                out << "BREAK语句\n";
                break;
            }
            case NodeType::ContinueStmt: {
                printIndent(out, indent);
                out << "CONTINUE语句\n";
                break;
            }
            default:
                for (auto* child : node->children) {
                    outputASTNode(out, child, indent);
                }
                break;
        }
    }

    void Parser::outputAST(const std::string& filename) {
        std::ofstream out(filename);
        if (!out.is_open()) {
            std::cerr << "Cannot open output file: " << filename << std::endl;
            return;
        }
        if (!root) {
            std::cerr << "No AST to output." << std::endl;
            return;
        }
        outputASTNode(out, root, 0);
        out.close();
    }
}