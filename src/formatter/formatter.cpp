#include "formatter.h"
#include "parser.h"
#include <string>
#include <utility>
#include <fstream>
#include <iostream>
#include <unordered_map>

namespace formatter {
    Formatter::Formatter(FILE *input,bool debug,std::string output):
        debug(debug),output(output),parser(input,debug)
    {
        parser.parse();
    }

    Formatter::~Formatter() {
        parser.~Parser();
    }

    static const std::unordered_map<std::string, std::string> operatorReplacements = {
        {"PLUS", "+"},
        {"MINUS", "-"},
        {"MUL", "*"},
        {"DIV", "/"},
        {"MOD", "%"},
        {"EQ", "=="},
        {"NEQ", "!="},
        {"LT", "<"},
        {"GT", ">"},
        {"LE", "<="},
        {"GE", ">="}
    };

    // 辅助函数，递归输出表达式但不加分号和换行（主要用于for头部）
    void Formatter::formatExprNoSemi(FILE* out, parser::ASTNode* node) {
        if (!node) return;
        using NT = parser::NodeType;
        switch (node->type) {
            case NT::ExprStmt:
                if (!node->children.empty()) {
                    formatExprNoSemi(out, node->children[0]);
                }
                break;
            case NT::AssignExpr:
                formatExprNoSemi(out, node->children[0]);
                fprintf(out, " = ");
                formatExprNoSemi(out, node->children[1]);
                break;
            case NT::LogicalOrExpr:
                formatExprNoSemi(out, node->children[0]);
                fprintf(out, " || ");
                formatExprNoSemi(out, node->children[1]);
                break;
            case NT::LogicalAndExpr:
                formatExprNoSemi(out, node->children[0]);
                fprintf(out, " && ");
                formatExprNoSemi(out, node->children[1]);
                break;
            case NT::EqualityExpr:
            case NT::RelationalExpr:
            case NT::AdditiveExpr:
            case NT::MultiplicativeExpr:
                formatExprNoSemi(out, node->children[0]);
                fprintf(out, " %s ", operatorReplacements.at(node->token).c_str());
                formatExprNoSemi(out, node->children[1]);
                break;
            case NT::UnaryExpr:
                fprintf(out, "%s", operatorReplacements.at(node->token).c_str());
                formatExprNoSemi(out, node->children[0]);
                break;
            case NT::PostfixExpr:
                formatExprNoSemi(out, node->children[0]);
                if (node->children.size() > 1) {
                    fprintf(out, "(");
                    formatExprNoSemi(out, node->children[1]);
                    fprintf(out, ")");
                }
                break;
            case NT::ArgList:
                for (size_t i = 0; i < node->children.size(); ++i) {
                    formatExprNoSemi(out, node->children[i]);
                    if (i + 1 < node->children.size()) fprintf(out, ", ");
                }
                break;
            case NT::ArrayAccess:
                formatExprNoSemi(out, node->children[0]);
                fprintf(out, "[");
                formatExprNoSemi(out, node->children[1]);
                fprintf(out, "]");
                break;
            case NT::TypeSpec:
            case NT::Identifier:
            case NT::LongConst:
            case NT::IntConst:
            case NT::FloatConst:
            case NT::CharConst:
            case NT::StringConst:
                fprintf(out, "%s", node->token.c_str());
                break;
            default:
                for (auto* child : node->children) formatExprNoSemi(out, child);
                break;
        }
    }

    // 递归格式化输出AST节点为C代码
    void Formatter::formatASTNode(FILE* out, parser::ASTNode* node, int indent) {
        if (!node) return;
        auto printIndent = [&](int n) { for (int i = 0; i < n; ++i) fprintf(out, "    "); };
        using NT = parser::NodeType;
        switch (node->type) {
            case NT::Program:
            case NT::ExternalDeclList:
                for (auto* child : node->children) formatASTNode(out, child, indent);
                break;
            case NT::FunctionDecl: {
                printIndent(indent);
                formatASTNode(out, node->children[0], 0); // type
                fprintf(out, " ");
                formatASTNode(out, node->children[1], 0); // ident
                fprintf(out, "(");
                // 参数列表输出
                if (node->children.size() > 2 && node->children[2]) {
                    formatASTNode(out, node->children[2], 0);
                }
                fprintf(out, ");\n");
                break;
            }
            case NT::FunctionDef: {
                printIndent(indent);
                formatASTNode(out, node->children[0], 0); // type
                fprintf(out, " ");
                formatASTNode(out, node->children[1], 0); // ident
                fprintf(out, "(");
                // 参数列表输出
                if (node->children.size() > 2 && node->children[2]) {
                    formatASTNode(out, node->children[2], 0);
                }
                fprintf(out, ")\n");
                // 复合语句体
                if (node->children.size() > 3 && node->children[3]) {
                    formatASTNode(out, node->children[3], indent);
                }
                break;
            }
            case NT::ParamList: {
                for (size_t i = 0; i < node->children.size(); ++i) {
                    formatASTNode(out, node->children[i], 0);
                    if (i + 1 < node->children.size()) fprintf(out, ", ");
                }
                break;
            }
            case NT::Param: {
                formatASTNode(out, node->children[0], 0); // type
                fprintf(out, " ");
                formatASTNode(out, node->children[1], 0); // ident
                // 无论有无第三个子节点，只要是数组类型都输出
                for (size_t i = 2; i < node->children.size(); ++i) {
                    if (node->children[i] && node->children[i]->type == NT::ArrayType) {
                        formatASTNode(out, node->children[i], 0); // arrayType
                    }
                }
                break;
            }
            case NT::ArrayType: {
                for (auto* dim : node->children) {
                    fprintf(out, "[");
                    formatASTNode(out, dim, 0);
                    fprintf(out, "]");
                }
                break;
            }
            case NT::VarDecl:
            case NT::LocalVarDecl: {
                printIndent(indent);
                formatASTNode(out, node->children[0], 0); // type
                fprintf(out, " ");
                formatASTNode(out, node->children[1], 0); // ident
                // 数组类型
                int idx = 2;
                if (node->children.size() > idx && node->children[idx]->type == NT::ArrayType) {
                    formatASTNode(out, node->children[idx], 0);
                    ++idx;
                }
                // 赋值
                if (node->children.size() > idx) {
                    fprintf(out, " = ");
                    formatASTNode(out, node->children[idx], 0);
                }
                fprintf(out, ";\n");
                break;
            }
            case NT::CompoundStmt: {
                printIndent(indent);
                fprintf(out, "{\n");
                // 局部变量定义
                if (!node->children.empty())
                    formatASTNode(out, node->children[0], indent + 1);
                // 语句列表
                if (node->children.size() > 1)
                    formatASTNode(out, node->children[1], indent + 1);
                printIndent(indent);
                fprintf(out, "}\n");
                break;
            }
            case NT::VarDeclList: {
                for (auto* child : node->children) formatASTNode(out, child, indent);
                break;
            }
            case NT::StmtList: {
                for (auto* child : node->children) formatASTNode(out, child, indent);
                break;
            }
            case NT::ExprStmt: {
                printIndent(indent);
                if (!node->children.empty()) {
                    formatASTNode(out, node->children[0], 0);
                }
                fprintf(out, ";\n");
                break;
            }
            case NT::IfStmt: {
                printIndent(indent);
                fprintf(out, "if (");
                formatASTNode(out, node->children[0], 0);
                fprintf(out, ")\n");
                formatASTNode(out, node->children[1], indent);
                if (node->children.size() == 3) {
                    printIndent(indent);
                    fprintf(out, "else\n");
                    formatASTNode(out, node->children[2], indent);
                }
                break;
            }
            case NT::WhileStmt: {
                printIndent(indent);
                fprintf(out, "while (");
                formatASTNode(out, node->children[0], 0);
                fprintf(out, ")\n");
                formatASTNode(out, node->children[1], indent);
                break;
            }
            case NT::ForStmt: {
                printIndent(indent);
                fprintf(out, "for (");
                formatExprNoSemi(out, node->children[0]); fprintf(out, "; ");
                formatExprNoSemi(out, node->children[1]); fprintf(out, "; ");
                formatExprNoSemi(out, node->children[2]);
                fprintf(out, ")\n");
                formatASTNode(out, node->children[3], indent);
                break;
            }
            case NT::ReturnStmt: {
                printIndent(indent);
                fprintf(out, "return");
                if (!node->children.empty()) {
                    fprintf(out, " ");
                    formatASTNode(out, node->children[0], 0);
                }
                fprintf(out, ";\n");
                break;
            }
            case NT::BreakStmt: {
                printIndent(indent);
                fprintf(out, "break;\n");
                break;
            }
            case NT::ContinueStmt: {
                printIndent(indent);
                fprintf(out, "continue;\n");
                break;
            }
            case NT::AssignExpr: {
                formatASTNode(out, node->children[0], 0);
                fprintf(out, " = ");
                formatASTNode(out, node->children[1], 0);
                break;
            }
            case NT::LogicalOrExpr: {
                formatASTNode(out, node->children[0], 0);
                fprintf(out, " || ");
                formatASTNode(out, node->children[1], 0);
                break;
            }
            case NT::LogicalAndExpr: {
                formatASTNode(out, node->children[0], 0);
                fprintf(out, " && ");
                formatASTNode(out, node->children[1], 0);
                break;
            }
            case NT::EqualityExpr: {
                formatASTNode(out, node->children[0], 0);
                fprintf(out, " %s ", operatorReplacements.at(node->token).c_str());
                formatASTNode(out, node->children[1], 0);
                break;
            }
            case NT::RelationalExpr: {
                formatASTNode(out, node->children[0], 0);
                fprintf(out, " %s ", operatorReplacements.at(node->token).c_str());
                formatASTNode(out, node->children[1], 0);
                break;
            }
            case NT::AdditiveExpr: {
                formatASTNode(out, node->children[0], 0);
                fprintf(out, " %s ", operatorReplacements.at(node->token).c_str());
                formatASTNode(out, node->children[1], 0);
                break;
            }
            case NT::MultiplicativeExpr: {
                formatASTNode(out, node->children[0], 0);
                fprintf(out, " %s ", operatorReplacements.at(node->token).c_str());
                formatASTNode(out, node->children[1], 0);
                break;
            }
            case NT::UnaryExpr: {
                fprintf(out, "%s", operatorReplacements.at(node->token).c_str());
                formatASTNode(out, node->children[0], 0);
                break;
            }
            case NT::PostfixExpr: {
                formatASTNode(out, node->children[0], 0); // ident
                if (node->children.size() > 1) {
                    fprintf(out, "(");
                    formatASTNode(out, node->children[1], 0);
                    fprintf(out, ")");
                }
                break;
            }
            case NT::ArgList: {
                for (size_t i = 0; i < node->children.size(); ++i) {
                    formatASTNode(out, node->children[i], 0);
                    if (i + 1 < node->children.size()) fprintf(out, ", ");
                }
                break;
            }
            case NT::ArrayAccess: {
                formatASTNode(out, node->children[0], 0);
                fprintf(out, "[");
                formatASTNode(out, node->children[1], 0);
                fprintf(out, "]");
                break;
            }
            case NT::TypeSpec:
            case NT::Identifier:
            case NT::LongConst:
            case NT::IntConst:
            case NT::FloatConst:
            case NT::CharConst:
            case NT::StringConst: {
                fprintf(out, "%s", node->token.c_str());
                break;
            }
            case NT::ParenthesizedExpr: {
                fprintf(out, "(");
                if (!node->children.empty()) {
                    formatASTNode(out, node->children[0], 0);
                }
                fprintf(out, ")");
                break;
            }
            case NT::LineComment: {
                printIndent(indent);
                fprintf(out, "%s\n", node->token.c_str());
                break;
            }
            case NT::BlockComment: {
                printIndent(indent);
                fprintf(out, "%s\n", node->token.c_str());
                break;
            }
            default:
                for (auto* child : node->children) formatASTNode(out, child, indent);
                break;
        }
    }

    void Formatter::format() {
        std::string outFile = output;
        if (outFile.empty()) outFile = "formatted.c";
        FILE *out = fopen(outFile.c_str(), "w");
        if (!out) {
            std::cerr << "Cannot open output file: " << outFile << std::endl;
            return;
        }
        parser::ASTNode* root = parser.parse();
        formatASTNode(out, root, 0);
        fclose(out);
    }
}