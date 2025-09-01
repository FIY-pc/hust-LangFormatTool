#include "ast.h"
#include "parser.h"

namespace parser {
    // 语句分派
    ASTNode* Parser::parseStmt() {
        debugLog("parseStmt", pos);
        int backup = pos;
        ASTNode* node = nullptr;
        node = parseIfStmt();
        if (node) { debugLog("parseStmt_exit", pos); return node; }
        pos = backup;
        node = parseWhileStmt();
        if (node) { debugLog("parseStmt_exit", pos); return node; }
        pos = backup;
        node = parseForStmt();
        if (node) { debugLog("parseStmt_exit", pos); return node; }
        pos = backup;
        node = parseReturnStmt();
        if (node) { debugLog("parseStmt_exit", pos); return node; }
        pos = backup;
        node = parseBreakStmt();
        if (node) { debugLog("parseStmt_exit", pos); return node; }
        pos = backup;
        node = parseContinueStmt();
        if (node) { debugLog("parseStmt_exit", pos); return node; }
        pos = backup;
        node = parseCompoundStmt();
        if (node) { debugLog("parseStmt_exit", pos); return node; }
        pos = backup;
        node = parseVarDecl();
        if (node) { debugLog("parseStmt_exit", pos); return node; }
        pos = backup;
        node = parseExprStmt();
        if (node) { debugLog("parseStmt_exit", pos); return node; }
        pos = backup;
        debugLog("parseStmt_exit", pos);
        return nullptr;
    }

    // 表达式语句：expr SEMI | SEMI
    ASTNode* Parser::parseExprStmt() {
        debugLog("parseExprStmt", pos);
        int backup = pos;
        if (pos < tokens.size() && tokens[pos].kind == lexer::TokenKind::SEMI) {
            pos++;
            auto* node = new ASTNode{NodeType::ExprStmt};
            debugLog("parseExprStmt_exit", pos);
            return node;
        }
        ASTNode* exprNode = parseExpr();
        if (!exprNode) {
            pos = backup;
            debugLog("parseExprStmt_exit", pos);
            return nullptr;
        }
        if (pos >= tokens.size() || tokens[pos].kind != lexer::TokenKind::SEMI) {
            error("expr_stmt: expected ';' after expression");
            pos = backup;
            debugLog("parseExprStmt_exit", pos);
            return nullptr;
        }
        pos++;
        auto* node = new ASTNode{NodeType::ExprStmt};
        node->children.push_back(exprNode);
        debugLog("parseExprStmt_exit", pos);
        return node;
    }

    // if语句
    ASTNode* Parser::parseIfStmt() {
        debugLog("parseIfStmt", pos);
        int backup = pos;
        if (pos >= tokens.size() || tokens[pos].kind != lexer::TokenKind::IF) {
            return nullptr;
        }
        pos++;
        if (pos >= tokens.size() || tokens[pos].kind != lexer::TokenKind::LP) {
            error("if_stmt: expected '(' after 'if'");
            pos = backup;
            return nullptr;
        }
        pos++;
        ASTNode* cond = parseExpr();
        if (!cond) {
            error("if_stmt: expected condition expression");
            pos = backup;
            return nullptr;
        }
        if (pos >= tokens.size() || tokens[pos].kind != lexer::TokenKind::RP) {
            error("if_stmt: expected ')' after condition");
            pos = backup;
            return nullptr;
        }
        pos++;
        ASTNode* thenStmt = parseStmt();
        if (!thenStmt) {
            error("if_stmt: expected statement after condition");
            pos = backup;
            return nullptr;
        }
        ASTNode* node = nullptr;
        if (pos < tokens.size() && tokens[pos].kind == lexer::TokenKind::ELSE) {
            pos++;
            ASTNode* elseStmt = parseStmt();
            if (!elseStmt) {
                error("if_stmt: expected statement after 'else'");
                pos = backup;
                return nullptr;
            }
            node = new ASTNode{NodeType::IfStmt};
            node->children.push_back(cond);
            node->children.push_back(thenStmt);
            node->children.push_back(elseStmt);
        } else {
            node = new ASTNode{NodeType::IfStmt};
            node->children.push_back(cond);
            node->children.push_back(thenStmt);
        }
        debugLog("parseIfStmt_exit", pos);
        return node;
    }

    // while语句
    ASTNode* Parser::parseWhileStmt() {
        debugLog("parseWhileStmt", pos);
        int backup = pos;
        if (pos >= tokens.size() || tokens[pos].kind != lexer::TokenKind::WHILE) {
            return nullptr;
        }
        pos++;
        if (pos >= tokens.size() || tokens[pos].kind != lexer::TokenKind::LP) {
            error("while_stmt: expected '(' after 'while'");
            pos = backup;
            return nullptr;
        }
        pos++;
        ASTNode* cond = parseExpr();
        if (!cond) {
            error("while_stmt: expected condition expression");
            pos = backup;
            return nullptr;
        }
        if (pos >= tokens.size() || tokens[pos].kind != lexer::TokenKind::RP) {
            error("while_stmt: expected ')' after condition");
            pos = backup;
            return nullptr;
        }
        pos++;
        ASTNode* body = parseStmt();
        if (!body) {
            error("while_stmt: expected statement after condition");
            pos = backup;
            return nullptr;
        }
        auto* node = new ASTNode{NodeType::WhileStmt};
        node->children.push_back(cond);
        node->children.push_back(body);
        debugLog("parseWhileStmt_exit", pos);
        return node;
    }

    // for语句
    ASTNode* Parser::parseForStmt() {
        debugLog("parseForStmt", pos);
        int backup = pos;
        if (pos >= tokens.size() || tokens[pos].kind != lexer::TokenKind::FOR) {
            return nullptr;
        }
        pos++;
        if (pos >= tokens.size() || tokens[pos].kind != lexer::TokenKind::LP) {
            error("for_stmt: expected '(' after 'for'");
            pos = backup;
            return nullptr;
        }
        pos++;
        ASTNode* init = parseExprStmt();
        if (!init) {
            error("for_stmt: expected init expr_stmt");
            pos = backup;
            return nullptr;
        }
        ASTNode* cond = parseExprStmt();
        if (!cond) {
            error("for_stmt: expected condition expr_stmt");
            pos = backup;
            return nullptr;
        }
        ASTNode* step = parseExpr();
        if (!step) {
            error("for_stmt: expected step expression");
            pos = backup;
            return nullptr;
        }
        if (pos >= tokens.size() || tokens[pos].kind != lexer::TokenKind::RP) {
            error("for_stmt: expected ')' after for header");
            pos = backup;
            return nullptr;
        }
        pos++;
        ASTNode* body = parseStmt();
        if (!body) {
            error("for_stmt: expected statement after for header");
            pos = backup;
            return nullptr;
        }
        auto* node = new ASTNode{NodeType::ForStmt};
        node->children.push_back(init);
        node->children.push_back(cond);
        node->children.push_back(step);
        node->children.push_back(body);
        debugLog("parseForStmt_exit", pos);
        return node;
    }

    // return语句
    ASTNode* Parser::parseReturnStmt() {
        debugLog("parseReturnStmt", pos);
        int backup = pos;
        if (pos >= tokens.size() || tokens[pos].kind != lexer::TokenKind::RETURN) {
            return nullptr;
        }
        pos++;
        if (pos < tokens.size() && tokens[pos].kind == lexer::TokenKind::SEMI) {
            pos++;
            auto* node = new ASTNode{NodeType::ReturnStmt};
            debugLog("parseReturnStmt_exit", pos);
            return node;
        }
        ASTNode* exprNode = parseExpr();
        if (!exprNode) {
            error("return_stmt: expected expression after 'return'");
            pos = backup;
            return nullptr;
        }
        if (pos >= tokens.size() || tokens[pos].kind != lexer::TokenKind::SEMI) {
            error("return_stmt: expected ';' after return expression");
            pos = backup;
            return nullptr;
        }
        pos++;
        auto* node = new ASTNode{NodeType::ReturnStmt};
        node->children.push_back(exprNode);
        debugLog("parseReturnStmt_exit", pos);
        return node;
    }

    // break语句
    ASTNode* Parser::parseBreakStmt() {
        debugLog("parseBreakStmt", pos);
        int backup = pos;
        if (pos >= tokens.size() || tokens[pos].kind != lexer::TokenKind::BREAK) {
            return nullptr;
        }
        pos++;
        if (pos >= tokens.size() || tokens[pos].kind != lexer::TokenKind::SEMI) {
            error("break_stmt: expected ';' after 'break'");
            pos = backup;
            return nullptr;
        }
        pos++;
        auto* node = new ASTNode{NodeType::BreakStmt};
        debugLog("parseBreakStmt_exit", pos);
        return node;
    }

    // continue语句
    ASTNode* Parser::parseContinueStmt() {
        debugLog("parseContinueStmt", pos);
        int backup = pos;
        if (pos >= tokens.size() || tokens[pos].kind != lexer::TokenKind::CONTINUE) {
            return nullptr;
        }
        pos++;
        if (pos >= tokens.size() || tokens[pos].kind != lexer::TokenKind::SEMI) {
            error("continue_stmt: expected ';' after 'continue'");
            pos = backup;
            return nullptr;
        }
        pos++;
        auto* node = new ASTNode{NodeType::ContinueStmt};
        debugLog("parseContinueStmt_exit", pos);
        return node;
    }
}
