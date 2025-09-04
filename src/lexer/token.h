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
    static std::unordered_map<int, std::string> TokenKindToStringMap = {
        {static_cast<int>(TokenKind::ERROR_TOKEN), "ERROR_TOKEN"},
        {static_cast<int>(TokenKind::EOF_TOKEN), "EOF_TOKEN"},
        {static_cast<int>(TokenKind::IDENT), "IDENT"},
        {static_cast<int>(TokenKind::INT_CONST), "INT_CONST"},
        {static_cast<int>(TokenKind::FLOAT_CONST), "FLOAT_CONST"},
        {static_cast<int>(TokenKind::CHAR_CONST), "CHAR_CONST"},
        {static_cast<int>(TokenKind::STRING_CONST), "STRING_CONST"},

        {static_cast<int>(TokenKind::INT), "INT"},
        {static_cast<int>(TokenKind::FLOAT), "FLOAT"},
        {static_cast<int>(TokenKind::CHAR), "CHAR"},
        {static_cast<int>(TokenKind::LONG), "LONG"},
        {static_cast<int>(TokenKind::VOID), "VOID"},

        {static_cast<int>(TokenKind::IF), "IF"},
        {static_cast<int>(TokenKind::ELSE), "ELSE"},
        {static_cast<int>(TokenKind::WHILE), "WHILE"},
        {static_cast<int>(TokenKind::FOR), "FOR"},
        {static_cast<int>(TokenKind::RETURN), "RETURN"},
        {static_cast<int>(TokenKind::CONTINUE), "CONTINUE"},
        {static_cast<int>(TokenKind::BREAK), "BREAK"},

        {static_cast<int>(TokenKind::ASSIGN), "ASSIGN"},

        {static_cast<int>(TokenKind::EQ), "EQ"},
        {static_cast<int>(TokenKind::NEQ), "NEQ"},
        {static_cast<int>(TokenKind::LT), "LT"},
        {static_cast<int>(TokenKind::GT), "GT"},
        {static_cast<int>(TokenKind::LE), "LE"},
        {static_cast<int>(TokenKind::GE), "GE"},

        {static_cast<int>(TokenKind::PLUS), "PLUS"},
        {static_cast<int>(TokenKind::MINUS), "MINUS"},
        {static_cast<int>(TokenKind::MUL), "MUL"},
        {static_cast<int>(TokenKind::DIV), "DIV"},
        {static_cast<int>(TokenKind::MOD), "MOD"},

        {static_cast<int>(TokenKind::AND), "AND"},
        {static_cast<int>(TokenKind::OR), "OR"},
        {static_cast<int>(TokenKind::NOT), "NOT"},

        {static_cast<int>(TokenKind::LP), "LP"},
        {static_cast<int>(TokenKind::RP), "RP"},
        {static_cast<int>(TokenKind::LB), "LB"},
        {static_cast<int>(TokenKind::RB), "RB"},
        {static_cast<int>(TokenKind::LC), "LC"},
        {static_cast<int>(TokenKind::RC), "RC"},

        {static_cast<int>(TokenKind::SEMI), "SEMI"},
        {static_cast<int>(TokenKind::COMMA), "COMMA"},

        {static_cast<int>(TokenKind::LINE_COMMENT), "LINE_COMMENT"},
        {static_cast<int>(TokenKind::BLOCK_COMMENT), "BLOCK_COMMENT"},
    };

    inline std::string TokenKindToString(TokenKind kind) {
        return TokenKindToStringMap[static_cast<int>(kind)];
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
