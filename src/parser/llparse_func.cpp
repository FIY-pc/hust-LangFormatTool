#include "ast.h"
#include "parser.h"

namespace parser {
    // 函数声明：type_spec IDENT LP param_list RP SEMI
    ASTNode *Parser::parseFunctionDecl() {
        debugLog("parseFunctionDecl", pos);
        int backup = pos;
        ASTNode* typeNode = parseTypeSpec();
        if (!typeNode) {
            pos = backup;
            return nullptr;
        }
        if (pos >= tokens.size() || tokens[pos].kind != lexer::TokenKind::IDENT) {
            pos = backup;
            return nullptr;
        }
        auto* identNode = new ASTNode{NodeType::Identifier};
        identNode->token = tokens[pos].text;
        pos++;
        if (pos >= tokens.size() || tokens[pos].kind != lexer::TokenKind::LP) {
            pos = backup;
            return nullptr;
        }
        pos++;
        ASTNode* paramListNode = parseParamList();
        if (pos >= tokens.size() || tokens[pos].kind != lexer::TokenKind::RP) {
            pos = backup;
            return nullptr;
        }
        pos++;
        if (pos >= tokens.size() || tokens[pos].kind != lexer::TokenKind::SEMI) {
            pos = backup;
            return nullptr;
        }
        pos++;
        auto* node = new ASTNode{NodeType::FunctionDecl};
        node->children.push_back(typeNode);
        node->children.push_back(identNode);
        if (paramListNode) node->children.push_back(paramListNode);
        debugLog("parseFunctionDecl_exit", pos);
        return node;
    }

    // 函数定义：type_spec IDENT LP param_list RP compound_stmt
    ASTNode *Parser::parseFunctionDef() {
        debugLog("parseFunctionDef", pos);
        int backup = pos;
        ASTNode* typeNode = parseTypeSpec();
        if (!typeNode) {
            pos = backup;
            return nullptr;
        }
        if (pos >= tokens.size() || tokens[pos].kind != lexer::TokenKind::IDENT) {
            pos = backup;
            return nullptr;
        }
        auto* identNode = new ASTNode{NodeType::Identifier};
        identNode->token = tokens[pos].text;
        pos++;
        if (pos >= tokens.size() || tokens[pos].kind != lexer::TokenKind::LP) {
            pos = backup;
            return nullptr;
        }
        pos++;
        ASTNode* paramListNode = parseParamList();
        if (pos >= tokens.size() || tokens[pos].kind != lexer::TokenKind::RP) {
            pos = backup;
            return nullptr;
        }
        pos++;
        ASTNode* compoundNode = parseCompoundStmt();
        if (!compoundNode) {
            pos = backup;
            return nullptr;
        }
        auto* node = new ASTNode{NodeType::FunctionDef};
        node->children.push_back(typeNode);
        node->children.push_back(identNode);
        if (paramListNode) node->children.push_back(paramListNode);
        node->children.push_back(compoundNode);
        debugLog("parseFunctionDef_exit", pos);
        return node;
    }

    // 参数列表：param param_list_tail | ε
    ASTNode *Parser::parseParamList() {
        debugLog("parseParamList", pos);
        // 如果参数列表为空，直接返回nullptr
        if (pos < tokens.size() && tokens[pos].kind == lexer::TokenKind::RP) {
            return nullptr;
        }
        int backup = pos;
        ASTNode* paramNode = parseParam();
        if (!paramNode) {
            return nullptr; // ε
        }
        ASTNode* tailNode = parseParamListTail();
        auto* node = new ASTNode{NodeType::ParamList};
        node->children.push_back(paramNode);
        if (tailNode) node->children.push_back(tailNode);
        debugLog("parseParamList_exit", pos);
        return node;
    }

    // 参数列表后续：COMMA param param_list_tail | ε
    ASTNode *Parser::parseParamListTail() {
        debugLog("parseParamListTail", pos);
        if (pos < tokens.size() && tokens[pos].kind == lexer::TokenKind::COMMA) {
            pos++;
            ASTNode* paramNode = parseParam();
            if (!paramNode) {
                return nullptr;
            }
            ASTNode* tailNode = parseParamListTail();
            auto* node = new ASTNode{NodeType::ParamList};
            node->children.push_back(paramNode);
            if (tailNode) node->children.push_back(tailNode);
            debugLog("parseParamListTail_exit", pos);
            return node;
        }
        return nullptr; // ε
    }

    // 单个参数：type_spec IDENT
    ASTNode *Parser::parseParam() {
        debugLog("parseParam", pos);
        int backup = pos;
        ASTNode* typeNode = parseTypeSpec();
        if (!typeNode) {
            pos = backup;
            return nullptr;
        }
        if (pos >= tokens.size() || tokens[pos].kind != lexer::TokenKind::IDENT) {
            pos = backup;
            return nullptr;
        }
        auto* identNode = new ASTNode{NodeType::Identifier};
        identNode->token = tokens[pos].text;
        pos++;
        auto* node = new ASTNode{NodeType::Param};
        node->children.push_back(typeNode);
        node->children.push_back(identNode);
        debugLog("parseParam_exit", pos);
        return node;
    }
}