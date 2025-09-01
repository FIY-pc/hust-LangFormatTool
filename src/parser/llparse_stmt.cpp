#include "ast.h"
#include "parser.h"

namespace parser {
    // 复合语句：{ 局部变量定义; 语句列表 }
    ASTNode* Parser::parseCompoundStmt() {
        debugLog("parseCompoundStmt", pos);
        int backup = pos;
        if (pos >= tokens.size() || tokens[pos].kind != lexer::TokenKind::LC) {
            return nullptr;
        }
        pos++;
        // 局部变量定义部分
        auto* varDeclList = new ASTNode{NodeType::VarDeclList};
        while (true) {
            int varBackup = pos;
            ASTNode* varDecl = parseLocalVarDecl();
            if (!varDecl) {
                pos = varBackup;
                break;
            }
            varDeclList->children.push_back(varDecl);
        }
        // 语句列表部分
        ASTNode* stmtListNode = parseStmtList();
        if (pos >= tokens.size() || tokens[pos].kind != lexer::TokenKind::RC) {
            error("compound_stmt: expected '}' at end of block");
            pos = backup;
            delete varDeclList;
            if (stmtListNode) delete stmtListNode;
            return nullptr;
        }
        pos++;
        auto* node = new ASTNode{NodeType::CompoundStmt};
        node->children.push_back(varDeclList); // 局部变量定义
        if (stmtListNode) node->children.push_back(stmtListNode); // 语句列表
        debugLog("parseCompoundStmt_exit", pos);
        return node;
    }

    // 语句列表：stmt stmt_list | ε
    ASTNode* Parser::parseStmtList() {
        debugLog("parseStmtList", pos);
        auto* node = new ASTNode{NodeType::StmtList};
        while (true) {
            int backup = pos;
            ASTNode* stmtNode = parseStmt();
            if (!stmtNode) {
                pos = backup;
                break;
            }
            node->children.push_back(stmtNode);
        }
        debugLog("parseStmtList_exit", pos);
        if (node->children.empty()) {
            delete node;
            return nullptr;
        }
        return node;
    }
}
