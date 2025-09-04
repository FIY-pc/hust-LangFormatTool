#ifndef LEXER_H
#define LEXER_H
#pragma once
#include <string>
#include <vector>
#include "token.h"

namespace lexer {
    class Lexer {
    public:
        explicit Lexer(FILE *file);
        ~Lexer();
        std::vector<Token> tokenize();
        void printTokensOrder(); // 顺序输出
        void printTokensSorted(); // 按种类编码排序输出
        void printTokensOrderPretty(); // 顺序美化输出
        void printTokensSortedPretty(); // 排序美化输出
        void printTokensOrderCN(); // 顺序中文输出
        void printTokensSortedCN(); // 排序中文输出
    private:
        FILE *file;
        int line;
        int column;
        std::vector<Token> tokens_cache; // 缓存token列表
        Token getToken();
        Token makeToken(TokenKind kind, const std::string& text, int col = 0) const;
    };
}

#endif //LEXER_H
