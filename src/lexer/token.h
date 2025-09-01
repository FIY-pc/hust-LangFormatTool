#ifndef TOKEN_H
#define TOKEN_H

#include <unordered_map>
#include <iostream>

namespace lexer {
    enum class TokenKind {
        ERROR_TOKEN,    // 错误单词
        EOF_TOKEN,      // 文件结束

        // 标识符与常量
        IDENT,          // 标识符
        INT_CONST,      // 整型常量
        FLOAT_CONST,    // 浮点常量
        CHAR_CONST,     // 字符常量
        STRING_CONST, // 字符串常量

        // 关键字
        INT,            // int
        FLOAT,          // float
        CHAR,           // char
        IF,             // if
        ELSE,           // else
        WHILE,          // while
        FOR,            // for
        RETURN,         // return
        VOID,           // void

        // 运算符
        ASSIGN,         // =
        EQ,             // ==
        NEQ,            // !=
        LT,             // <
        GT,             // >
        LE,             // <=
        GE,             // >=
        PLUS,           // +
        MINUS,          // -
        MUL,            // *
        DIV,            // /
        MOD,            // %

        // 定界符
        LP,             // (
        RP,             // )
        LB,             // [
        RB,             // ]
        LC,             // {
        RC,             // }
        SEMI,           // ;
        COMMA,          // ,
    };

    static const std::unordered_map<std::string, TokenKind> keywords = {
        {"int", TokenKind::INT},
        {"float", TokenKind::FLOAT},
        {"char", TokenKind::CHAR},
        {"if", TokenKind::IF},
        {"else", TokenKind::ELSE},
        {"while", TokenKind::WHILE},
        {"for", TokenKind::FOR},
        {"return", TokenKind::RETURN},
        {"void", TokenKind::VOID}
    };


    struct Token {
        TokenKind kind;         // 单词类别
        std::string text;       // 单词自身值
        int line;               // 行号
        int column;             // 列号
        void print() const {
            std::cout << "Token(" << static_cast<int>(kind) << ", \"" << text << "\", " << line << ", " << column << ")\n";
        }
    };
}

#endif //TOKEN_H
