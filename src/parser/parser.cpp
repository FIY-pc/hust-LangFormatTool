#include "parser.h"
#include "lexer.h"
#include "ast.h"
#include <utility>
#include <vector>
#include <fstream>

namespace parser {
    Parser::Parser(FILE *file, const bool debug,std::string output):
        debug(debug),output(std::move(output)), lexer(file), root(nullptr), tokens(lexer.tokenize()),pos(0)
    {
        if(this->output.empty()){
            this->output="ast.txt";
        }
    }
    Parser::Parser(lexer::Lexer &lexer, const bool debug,std::string output):
        debug(debug),output(std::move(output)), lexer(lexer), root(nullptr), tokens(lexer.tokenize()),pos(0) {}
    Parser::~Parser() {
        lexer.~Lexer();
        if (root) root->~ASTNode();
    }
    ASTNode *Parser::parse() {
        if (root) return root;
        root = parseProgram();
        return root;
    }

    // 报错
    void Parser::error(const std::string& msg) const {
        fprintf(stderr, "Parse error: %s\n", msg.c_str());
        // 打印当前 token 及上下文
        int ctx_start = std::max(0, pos - 2);
        int ctx_end = std::min((int)tokens.size() - 1, pos + 2);
        fprintf(stderr, "Context tokens (pos=%d):\n", pos);
        for (int i = ctx_start; i <= ctx_end; ++i) {
            const auto& tk = tokens[i];
            fprintf(stderr, "  [%s] '%s' (line %d, col %d)%s\n",
                lexer::TokenKindToString(tk.kind).c_str(), tk.text.c_str(), tk.line, tk.column,
                (i == pos ? " <-- current" : "")
            );
        }
        exit(EXIT_FAILURE);
    }
}

