#include "parser.h"
#include "lexer.h"
#include "ast.h"
#include <vector>

namespace parser {
    Parser::Parser(FILE *file):
        lexer(file), root(nullptr), tokens(lexer.tokenize()), pos(0) {}
    Parser::Parser(lexer::Lexer &lexer):
        lexer(lexer), root(nullptr), tokens(lexer.tokenize()), pos(0) {}
    Parser::~Parser() {
        lexer.~Lexer();
        if (root) root->~ASTNode();
    }
    ASTNode *Parser::parse() {
        if (root) return root;
        root = parseProgram();
        return root;
    }
}
