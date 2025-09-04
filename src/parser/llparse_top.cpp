#include "ast.h"
#include "parser.h"

namespace parser {
    // 顶层结构

    // program -> external_decl_list
    // external_decl_list -> external_decl external_decl_list | ε
    // external_decl -> function_def | function_decl | var_decl

    // 解析程序
    ASTNode *Parser::parseProgram() {
        debugLog("parseProgram", pos);
        ASTNode* declList = parseExternalDeclList();
        debugLog("parseProgram_exit", pos);
        if (!declList) {
            error("program: expected at least one external declaration");
            return nullptr;
        }
        auto* node = new ASTNode{NodeType::Program};
        node->children.push_back(declList);
        root = node;
        return root;
    }

    // 解析外部声明列表
    ASTNode *Parser::parseExternalDeclList() {
        debugLog("parseExternalDeclList", pos);
        auto* node = new ASTNode{NodeType::ExternalDeclList};
        while (true) {
            int backup = pos;
            ASTNode* decl = parseExternalDecl();
            if (!decl) {
                pos = backup;
                break;
            }
            node->children.push_back(decl);
        }
        debugLog("parseExternalDeclList_exit", pos);
        // 如果没有任何外部声明，允许为空（不报错）
        if (node->children.empty()) {
            delete node;
            return nullptr;
        }
        return node;
    }

    // 解析外部声明
    ASTNode *Parser::parseExternalDecl() {
        debugLog("parseExternalDecl", pos);
        // 遇到EOF_TOKEN，说明文件结束
        if (pos >= tokens.size() || tokens[pos].kind == lexer::TokenKind::EOF_TOKEN) {
            return nullptr;
        }
        int backup = pos;
        ASTNode* node = nullptr;

        // 顶层注释
        if (pos < tokens.size() &&
            (tokens[pos].kind == lexer::TokenKind::LINE_COMMENT || tokens[pos].kind == lexer::TokenKind::BLOCK_COMMENT)) {
            auto kind = tokens[pos].kind;
            auto* node = new ASTNode{kind == lexer::TokenKind::LINE_COMMENT ? LineComment : BlockComment};
            node->token = tokens[pos].text;
            pos++;
            return node;
        }

        node = parseFunctionDef();
        if (node) { debugLog("parseExternalDecl_funcdef", pos); return node; }
        pos = backup;

        node = parseFunctionDecl();
        if (node) { debugLog("parseExternalDecl_funcdecl", pos); return node; }
        pos = backup;

        node = parseVarDecl();
        if (node) { debugLog("parseExternalDecl_vardecl", pos); return node; }
        pos = backup;

        error("external_decl: expected function_def/function_decl/var_decl");
        return nullptr;
    }
}