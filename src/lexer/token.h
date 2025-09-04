#ifndef TOKEN_H
#define TOKEN_H

#include <unordered_map>
#include <iostream>
#include <unordered_set>

namespace lexer {
    enum class TokenKind {
        ERROR_TOKEN,    // 错误单词
        EOF_TOKEN,      // 文件结束

        // 标识符与常量
        IDENT,          // 标识符
        INT_CONST,      // 整型常量
        LONG_CONST,     // long整型常量
        FLOAT_CONST,    // 浮点常量
        CHAR_CONST,     // 字符常量
        STRING_CONST,   // 字符串常量

        // 关键字
        INT,            // int
        FLOAT,          // float
        CHAR,           // char
        LONG,           // long
        IF,             // if
        ELSE,           // else
        WHILE,          // while
        FOR,            // for
        RETURN,         // return
        VOID,           // void
        CONTINUE,       // continue
        BREAK,          // break

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

        // 逻辑运算符
        AND,            // &&
        OR,             // ||
        NOT,            // !

        // 定界符
        LP,             // (
        RP,             // )
        LB,             // [
        RB,             // ]
        LC,             // {
        RC,             // }
        SEMI,           // ;
        COMMA,          // ,

        // 注释
        LINE_COMMENT,    // 行注释 //
        BLOCK_COMMENT,   // 块注释 /**/
    };
    static std::unordered_set<TokenKind> TypeSpecifiers = {
        TokenKind::INT,
        TokenKind::FLOAT,
        TokenKind::CHAR,
        TokenKind::LONG,
        TokenKind::VOID
    };
    inline bool isTypeSpecifier(TokenKind kind) {
        return TypeSpecifiers.find(kind) != TypeSpecifiers.end();
    }


    static const std::unordered_map<std::string, TokenKind> keywords = {
        {"int", TokenKind::INT},
        {"float", TokenKind::FLOAT},
        {"char", TokenKind::CHAR},
        {"long", TokenKind::LONG},
        {"void", TokenKind::VOID},
        {"long", TokenKind::LONG},

        {"if", TokenKind::IF},
        {"else", TokenKind::ELSE},
        {"while", TokenKind::WHILE},
        {"for", TokenKind::FOR},
        {"return", TokenKind::RETURN},

        {"continue", TokenKind::CONTINUE},
        {"break", TokenKind::BREAK},
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
