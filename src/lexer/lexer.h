#ifndef LEXER_H
#define LEXER_H
#pragma once
#include <string>
#include <vector>
#include "token.h"

namespace lexer {
    class Lexer {
    public:
        Lexer(FILE *file,bool printToken=false);
        ~Lexer();
        bool printToken;
        std::vector<Token> tokenize();
    private:
        FILE *file;
        int line;
        int column;
        Token getToken();
        Token makeToken(TokenKind kind, const std::string& text, int col = 0);
    };
}

#endif //LEXER_H
