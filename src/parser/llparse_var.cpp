#include "ast.h"
#include "parser.h"

namespace parser {
    // 变量声明
    /* var_decl → type_spec IDENT SEMI
     *          | type_spec IDENT ASSIGN expr SEMI
     */
    ASTNode *Parser::parseVarDecl() {
        debugLog("parseVarDecl", pos);
        // 只有类型关键字才尝试变量声明，否则直接返回nullptr
        if (pos >= tokens.size() ||
            (tokens[pos].kind != lexer::TokenKind::INT &&
             tokens[pos].kind != lexer::TokenKind::FLOAT &&
             tokens[pos].kind != lexer::TokenKind::CHAR &&
             tokens[pos].kind != lexer::TokenKind::VOID)) {
            return nullptr;
        }
        int backup = pos;
        ASTNode* typeNode = parseTypeSpec();
        if (!typeNode) {
            error("var_decl: expected type_spec (int/float/char/void)");
            pos = backup;
            return nullptr;
        }
        // 检查标识符
        if (pos >= tokens.size() || tokens[pos].kind != lexer::TokenKind::IDENT) {
            error("var_decl: expected identifier after type_spec");
            pos = backup;
            return nullptr;
        }
        // 创建 VarDecl 节点
        auto* varNode = new ASTNode{NodeType::VarDecl};
        varNode->children.push_back(typeNode);
        // 标识符节点
        auto* identNode = new ASTNode{NodeType::Identifier};
        identNode->token = tokens[pos].text;
        varNode->children.push_back(identNode);
        pos++;
        // 检查是否有赋值
        if (pos < tokens.size() && tokens[pos].kind == lexer::TokenKind::ASSIGN) {
            pos++;
            ASTNode* exprNode = parseExpr();
            if (!exprNode) {
                error("var_decl: expected expression after '='");
                delete varNode;
                pos = backup;
                return nullptr;
            }
            varNode->children.push_back(exprNode);
        }
        // 必须以分号结尾
        if (pos >= tokens.size() || tokens[pos].kind != lexer::TokenKind::SEMI) {
            error("var_decl: expected ';' at end of declaration");
            delete varNode;
            pos = backup;
            return nullptr;
        }
        pos++;
        debugLog("parseVarDecl_exit", pos);
        return varNode;
    }

    // 局部变量声明
    /* local_var_decl → type_spec IDENT SEMI
     *                | type_spec IDENT ASSIGN expr SEMI
     */
    ASTNode *Parser::parseLocalVarDecl() {
        debugLog("parseLocalVarDecl", pos);
        if (pos >= tokens.size() ||
            (tokens[pos].kind != lexer::TokenKind::INT &&
             tokens[pos].kind != lexer::TokenKind::FLOAT &&
             tokens[pos].kind != lexer::TokenKind::CHAR &&
             tokens[pos].kind != lexer::TokenKind::VOID)) {
            return nullptr;
        }
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
        auto* varNode = new ASTNode{NodeType::LocalVarDecl};
        varNode->children.push_back(typeNode);
        auto* identNode = new ASTNode{NodeType::Identifier};
        identNode->token = tokens[pos].text;
        varNode->children.push_back(identNode);
        pos++;
        if (pos < tokens.size() && tokens[pos].kind == lexer::TokenKind::ASSIGN) {
            pos++;
            ASTNode* exprNode = parseExpr();
            if (!exprNode) {
                delete varNode;
                pos = backup;
                return nullptr;
            }
            varNode->children.push_back(exprNode);
        }
        if (pos >= tokens.size() || tokens[pos].kind != lexer::TokenKind::SEMI) {
            delete varNode;
            pos = backup;
            return nullptr;
        }
        pos++;
        debugLog("parseLocalVarDecl_exit", pos);
        return varNode;
    }
}
