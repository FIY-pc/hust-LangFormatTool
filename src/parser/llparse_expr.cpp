#include "ast.h"
#include "parser.h"

namespace parser {
    // expr → assign_expr
    ASTNode* Parser::parseExpr() {
        debugLog("parseExpr", pos);
        ASTNode* node = parseAssignExpr();
        debugLog("parseExpr_exit", pos);
        return node;
    }

    // assign_expr → logical_or_expr | IDENT ASSIGN assign_expr
    ASTNode* Parser::parseAssignExpr() {
        debugLog("parseAssignExpr", pos);
        int backup = pos;
        // 检查是否为赋值表达式 IDENT ASSIGN assign_expr
        if (pos < tokens.size() && tokens[pos].kind == lexer::TokenKind::IDENT) {
            std::string ident = tokens[pos].text;
            int identPos = pos;
            pos++;
            if (pos < tokens.size() && tokens[pos].kind == lexer::TokenKind::ASSIGN) {
                pos++;
                ASTNode* rhs = parseAssignExpr();
                if (!rhs) {
                    error("assign_expr: expected expression after '='");
                    pos = backup;
                    return nullptr;
                }
                auto* node = new ASTNode{NodeType::AssignExpr};
                auto* identNode = new ASTNode{NodeType::Identifier};
                identNode->token = ident;
                node->children.push_back(identNode);
                node->children.push_back(rhs);
                return node;
            } else {
                pos = identPos; // 回溯，仅IDENT不是赋值
            }
        }
        // 否则为逻辑或表达式
        pos = backup;
        return parseLogicalOrExpr();
    }

    // logical_or_expr → logical_and_expr { OR logical_and_expr }
    ASTNode* Parser::parseLogicalOrExpr() {
        debugLog("parseLogicalOrExpr", pos);
        ASTNode* left = parseLogicalAndExpr();
        if (!left) return nullptr;
        while (pos < tokens.size() && tokens[pos].kind == lexer::TokenKind::OR) {
            pos++;
            ASTNode* right = parseLogicalAndExpr();
            if (!right) {
                error("logical_or_expr: expected expression after '||'");
                return nullptr;
            }
            auto* node = new ASTNode{NodeType::LogicalOrExpr};
            node->children.push_back(left);
            node->children.push_back(right);
            left = node;
        }
        debugLog("parseLogicalOrExpr_exit", pos);
        return left;
    }

    // logical_and_expr → equality_expr { AND equality_expr }
    ASTNode* Parser::parseLogicalAndExpr() {
        debugLog("parseLogicalAndExpr", pos);
        ASTNode* left = parseEqualityExpr();
        if (!left) return nullptr;
        while (pos < tokens.size() && tokens[pos].kind == lexer::TokenKind::AND) {
            pos++;
            ASTNode* right = parseEqualityExpr();
            if (!right) {
                error("logical_and_expr: expected expression after '&&'");
                return nullptr;
            }
            auto* node = new ASTNode{NodeType::LogicalAndExpr};
            node->children.push_back(left);
            node->children.push_back(right);
            left = node;
        }
        debugLog("parseLogicalAndExpr_exit", pos);
        return left;
    }

    // equality_expr → relational_expr { (EQ | NEQ) relational_expr }
    ASTNode* Parser::parseEqualityExpr() {
        debugLog("parseEqualityExpr", pos);
        ASTNode* left = parseRelationalExpr();
        if (!left) return nullptr;
        while (pos < tokens.size() &&
               (tokens[pos].kind == lexer::TokenKind::EQ || tokens[pos].kind == lexer::TokenKind::NEQ)) {
            auto op = tokens[pos].kind;
            pos++;
            ASTNode* right = parseRelationalExpr();
            if (!right) {
                error("equality_expr: expected expression after '==' or '!='");
                return nullptr;
            }
            auto* node = new ASTNode{NodeType::EqualityExpr};
            node->token = lexer::TokenKindToString(op);
            node->children.push_back(left);
            node->children.push_back(right);
            left = node;
        }
        debugLog("parseEqualityExpr_exit", pos);
        return left;
    }

    // relational_expr → additive_expr { (LT | GT | LE | GE) additive_expr }
    ASTNode* Parser::parseRelationalExpr() {
        debugLog("parseRelationalExpr", pos);
        ASTNode* left = parseAdditiveExpr();
        if (!left) return nullptr;
        while (pos < tokens.size() &&
               (tokens[pos].kind == lexer::TokenKind::LT || tokens[pos].kind == lexer::TokenKind::GT ||
                tokens[pos].kind == lexer::TokenKind::LE || tokens[pos].kind == lexer::TokenKind::GE)) {
            auto op = tokens[pos].kind;
            pos++;
            ASTNode* right = parseAdditiveExpr();
            if (!right) {
                error("relational_expr: expected expression after '<', '>', '<=', '>='");
                return nullptr;
            }
            auto* node = new ASTNode{NodeType::RelationalExpr};
            node->token = lexer::TokenKindToString(op);
            node->children.push_back(left);
            node->children.push_back(right);
            left = node;
        }
        debugLog("parseRelationalExpr_exit", pos);
        return left;
    }

    // additive_expr → multiplicative_expr { (PLUS | MINUS) multiplicative_expr }
    ASTNode* Parser::parseAdditiveExpr() {
        debugLog("parseAdditiveExpr", pos);
        ASTNode* left = parseMultiplicativeExpr();
        if (!left) return nullptr;
        while (pos < tokens.size() &&
               (tokens[pos].kind == lexer::TokenKind::PLUS || tokens[pos].kind == lexer::TokenKind::MINUS)) {
            auto op = tokens[pos].kind;
            pos++;
            ASTNode* right = parseMultiplicativeExpr();
            if (!right) {
                error("additive_expr: expected expression after '+' or '-'");
                return nullptr;
            }
            auto* node = new ASTNode{NodeType::AdditiveExpr};
            node->token = lexer::TokenKindToString(op);
            node->children.push_back(left);
            node->children.push_back(right);
            left = node;
        }
        debugLog("parseAdditiveExpr_exit", pos);
        return left;
    }

    // multiplicative_expr → unary_expr { (MUL | DIV | MOD) unary_expr }
    ASTNode* Parser::parseMultiplicativeExpr() {
        debugLog("parseMultiplicativeExpr", pos);
        ASTNode* left = parseUnaryExpr();
        if (!left) return nullptr;
        while (pos < tokens.size() &&
               (tokens[pos].kind == lexer::TokenKind::MUL || tokens[pos].kind == lexer::TokenKind::DIV || tokens[pos].kind == lexer::TokenKind::MOD)) {
            auto op = tokens[pos].kind;
            pos++;
            ASTNode* right = parseUnaryExpr();
            if (!right) {
                error("multiplicative_expr: expected expression after '*', '/' or '%'");
                return nullptr;
            }
            auto* node = new ASTNode{NodeType::MultiplicativeExpr};
            node->token = lexer::TokenKindToString(op);
            node->children.push_back(left);
            node->children.push_back(right);
            left = node;
        }
        debugLog("parseMultiplicativeExpr_exit", pos);
        return left;
    }

    // unary_expr → (PLUS | MINUS | NOT) unary_expr | postfix_expr
    ASTNode* Parser::parseUnaryExpr() {
        debugLog("parseUnaryExpr", pos);
        if (pos < tokens.size() &&
            (tokens[pos].kind == lexer::TokenKind::PLUS || tokens[pos].kind == lexer::TokenKind::MINUS || tokens[pos].kind == lexer::TokenKind::NOT)) {
            auto op = tokens[pos].kind;
            pos++;
            ASTNode* expr = parseUnaryExpr();
            if (!expr) {
                error("unary_expr: expected expression after unary operator");
                return nullptr;
            }
            auto* node = new ASTNode{NodeType::UnaryExpr};
            node->token = lexer::TokenKindToString(op);
            node->children.push_back(expr);
            debugLog("parseUnaryExpr_exit", pos);
            return node;
        }
        return parsePostfixExpr();
    }

    // postfix_expr → primary_expr | IDENT LP arg_list RP | postfix_expr LB expr RB
    ASTNode* Parser::parsePostfixExpr() {
        debugLog("parsePostfixExpr", pos);
        int backup = pos;
        // 检查是否为函数调用 IDENT LP arg_list RP
        if (pos < tokens.size() && tokens[pos].kind == lexer::TokenKind::IDENT) {
            std::string ident = tokens[pos].text;
            int identPos = pos;
            pos++;
            if (pos < tokens.size() && tokens[pos].kind == lexer::TokenKind::LP) {
                pos++;
                ASTNode* args = parseArgList();
                if (pos >= tokens.size() || tokens[pos].kind != lexer::TokenKind::RP) {
                    error("postfix_expr: expected ')' after function call arguments");
                    pos = backup;
                    return nullptr;
                }
                pos++;
                auto* node = new ASTNode{NodeType::PostfixExpr};
                auto* identNode = new ASTNode{NodeType::Identifier};
                identNode->token = ident;
                node->children.push_back(identNode);
                if (args) node->children.push_back(args);
                debugLog("parsePostfixExpr_exit", pos);
                return node;
            } else {
                pos = identPos; // 不是函数调用，回溯
            }
        }
        // 数组访问：postfix_expr LB expr RB
        ASTNode* base = nullptr;
        int arrBackup = pos;
        base = parsePrimaryExpr();
        if (!base) {
            pos = backup;
            return nullptr;
        }
        while (pos < tokens.size() && tokens[pos].kind == lexer::TokenKind::LB) {
            pos++;
            ASTNode* indexExpr = parseExpr();
            if (!indexExpr) {
                error("array_access: expected expression inside []");
                pos = arrBackup;
                return nullptr;
            }
            if (pos >= tokens.size() || tokens[pos].kind != lexer::TokenKind::RB) {
                error("array_access: expected ']' after expression");
                pos = arrBackup;
                return nullptr;
            }
            pos++;
            auto* arrNode = new ASTNode{NodeType::ArrayAccess};
            arrNode->children.push_back(base);
            arrNode->children.push_back(indexExpr);
            base = arrNode;
        }
        debugLog("parsePostfixExpr_exit", pos);
        return base;
    }

    // arg_list → expr { COMMA expr } | ε
    ASTNode* Parser::parseArgList() {
        debugLog("parseArgList", pos);
        int backup = pos;
        ASTNode* first = parseExpr();
        if (!first) return nullptr; // ε
        auto* node = new ASTNode{NodeType::ArgList};
        node->children.push_back(first);
        while (pos < tokens.size() && tokens[pos].kind == lexer::TokenKind::COMMA) {
            pos++;
            ASTNode* arg = parseExpr();
            if (!arg) {
                error("arg_list: expected expression after ','");
                pos = backup;
                return nullptr;
            }
            node->children.push_back(arg);
        }
        debugLog("parseArgList_exit", pos);
        return node;
    }

    // primary_expr → IDENT | LONG_CONST | INT_CONST | FLOAT_CONST | CHAR_CONST | STRING_CONST | LP expr RP
    ASTNode* Parser::parsePrimaryExpr() {
        debugLog("parsePrimaryExpr", pos);
        if (pos >= tokens.size()) return nullptr;
        auto kind = tokens[pos].kind;
        auto nodeType = getTypeFromTokenKind(kind);
        if (kind == lexer::TokenKind::IDENT) {
            auto* node = new ASTNode{NodeType::Identifier};
            node->token = tokens[pos].text;
            pos++;
            debugLog("parsePrimaryExpr_exit", pos);
            return node;
        } else if (isTerminalNode(nodeType)) {
            auto* node = new ASTNode{nodeType};
            node->token = tokens[pos].text;
            pos++;
            debugLog("parsePrimaryExpr_exit", pos);
            return node;
        } else if (kind == lexer::TokenKind::LP) {
            pos++;
            ASTNode* expr = parseExpr();
            if (!expr) {
                error("primary_expr: expected expression after '('");
                return nullptr;
            }
            if (pos >= tokens.size() || tokens[pos].kind != lexer::TokenKind::RP) {
                error("primary_expr: expected ')' after expression");
                return nullptr;
            }
            pos++;
            debugLog("parsePrimaryExpr_exit", pos);
            return expr;
        }
        return nullptr;
    }
}
