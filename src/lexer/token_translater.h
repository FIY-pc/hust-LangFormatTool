#ifndef TRANSLATER_H
#define TRANSLATER_H

#include "token.h"

namespace lexer {
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
        if (TokenKindToStringMap.find(static_cast<int>(kind)) != TokenKindToStringMap.end()) {
            return TokenKindToStringMap[static_cast<int>(kind)];
        }
        return "ERROR_TOKEN";
    }

    static std::unordered_map<TokenKind,std::string> TokenKindToCNStringMap = {
        {TokenKind::ERROR_TOKEN, "错误单词"},
        {TokenKind::EOF_TOKEN, "文件结束"},
        {TokenKind::IDENT, "标识符"},
        {TokenKind::INT_CONST, "整型常量"},
        {TokenKind::LONG_CONST, "长整型常量"},
        {TokenKind::FLOAT_CONST, "浮点常量"},
        {TokenKind::CHAR_CONST, "字符常量"},
        {TokenKind::STRING_CONST, "字符串常量"},

        {TokenKind::INT, "int"},
        {TokenKind::FLOAT, "float"},
        {TokenKind::CHAR, "char"},
        {TokenKind::LONG, "long"},
        {TokenKind::VOID, "void"},

        {TokenKind::IF, "if"},
        {TokenKind::ELSE, "else"},
        {TokenKind::WHILE, "while"},
        {TokenKind::FOR, "for"},
        {TokenKind::RETURN, "return"},
        {TokenKind::CONTINUE, "continue"},
        {TokenKind::BREAK, "break"},

        {TokenKind::ASSIGN, "="},

        {TokenKind::EQ, "=="},
        {TokenKind::NEQ, "!="},
        {TokenKind::LT, "<"},
        {TokenKind::GT, ">"},
        {TokenKind::LE, "<="},
        {TokenKind::GE, ">="},

        {TokenKind::PLUS, "+"},
        {TokenKind::MINUS, "-"},
        {TokenKind::MUL, "*"},
        {TokenKind::DIV, "/"},
        {TokenKind::MOD, "%"},

        {TokenKind::AND, "&&"},
        {TokenKind::OR, "||"},
        {TokenKind::NOT, "!"},

        {TokenKind::LP, "("},
        {TokenKind::RP, ")"},
        {TokenKind::LB, "["},
        {TokenKind::RB, "]"},
        {TokenKind::LC, "{"},
        {TokenKind::RC, "}"},

        {TokenKind::SEMI, "分号"},
        {TokenKind::COMMA, "逗号"},

        {TokenKind::LINE_COMMENT, "行注释"},
        {TokenKind::BLOCK_COMMENT, "块注释"},
    };
    inline std::string TokenKindToCNString(TokenKind kind) {
        if (TokenKindToCNStringMap.find(kind) != TokenKindToCNStringMap.end()) {
            return TokenKindToCNStringMap[kind];
        }
        return "错误单词";
    }
}

#endif //TRANSLATER_H
