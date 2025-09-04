#include "lexer.h"
#include "token.h"
#include "token_translater.h"
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <algorithm>

namespace lexer {
    Lexer::Lexer(FILE *file) : file(file), line(1),column(0) {
        if (!file) {
            throw std::runtime_error("Failed to open file");
        }
    }
    Lexer::~Lexer() {
        if (file) fclose(file);
    }

    std::vector<Token> Lexer::tokenize() {
        tokens_cache.clear();
        Token token;
        do {
            token = getToken();
            tokens_cache.push_back(token);
        } while (token.kind != TokenKind::EOF_TOKEN && token.kind != TokenKind::ERROR_TOKEN);
        return tokens_cache;
    }

    void Lexer::printTokensOrder() {
        if (tokens_cache.empty()) tokenize();
        for (const auto& token : tokens_cache) {
            token.print();
        }
    }

    void Lexer::printTokensSorted() {
        if (tokens_cache.empty()) tokenize();
        std::vector<Token> sorted_tokens = tokens_cache;
        std::sort(sorted_tokens.begin(), sorted_tokens.end(), [](const Token& a, const Token& b) {
            return static_cast<int>(a.kind) < static_cast<int>(b.kind);
        });
        for (const auto& token : sorted_tokens) {
            token.print();
        }
    }

    void Lexer::printTokensOrderPretty() {
        if (tokens_cache.empty()) tokenize();
        for (const auto& token : tokens_cache) {
            std::cout << "Token(" << TokenKindToString(token.kind) << ", \"" << token.text << "\", " << token.line << ", " << token.column << ")\n";
        }
    }

    void Lexer::printTokensSortedPretty() {
        if (tokens_cache.empty()) tokenize();
        std::vector<Token> sorted_tokens = tokens_cache;
        std::sort(sorted_tokens.begin(), sorted_tokens.end(), [](const Token& a, const Token& b) {
            return static_cast<int>(a.kind) < static_cast<int>(b.kind);
        });
        for (const auto& token : sorted_tokens) {
            std::cout << "Token(" << TokenKindToString(token.kind) << ", \"" << token.text << "\", " << token.line << ", " << token.column << ")\n";
        }
    }

    void Lexer::printTokensOrderCN() {
        if (tokens_cache.empty()) tokenize();
        for (const auto& token : tokens_cache) {
            std::cout << "Token(" << TokenKindToCNString(token.kind) << ", \"" << token.text << "\", " << token.line << ", " << token.column << ")\n";
        }
    }

    void Lexer::printTokensSortedCN() {
        if (tokens_cache.empty()) tokenize();
        std::vector<Token> sorted_tokens = tokens_cache;
        std::sort(sorted_tokens.begin(), sorted_tokens.end(), [](const Token& a, const Token& b) {
            return static_cast<int>(a.kind) < static_cast<int>(b.kind);
        });
        for (const auto& token : sorted_tokens) {
            std::cout << "Token(" << TokenKindToCNString(token.kind) << ", \"" << token.text << "\", " << token.line << ", " << token.column << ")\n";
        }
    }

    Token Lexer::getToken() {
        int c;
        // 跳过空白符并记录行列号
        while ((c = fgetc(file)) != EOF && std::isspace(c)) {
            if (c == '\n') {
                line++;
                column = 0;
            } else {
                column++;
            }
        }
        if (c == EOF) {
            return makeToken(TokenKind::EOF_TOKEN, "");
        }

        int start_col = column + 1; // 当前字符是第几个字符
        std::string text;

        // 标识符或关键字
        if (std::isalpha(c) || c == '_') {
            text += static_cast<char>(c);
            column++;
            while ((c = fgetc(file)), (std::isalnum(c) || c == '_')) {
                text += static_cast<char>(c);
                column++;
            }
            if (c != EOF) ungetc(c, file);

            // 判断是否为关键字

            auto it = keywords.find(text);
            if (it != keywords.end()) {
                return makeToken(it->second, text, start_col);
            } else {
                return makeToken(TokenKind::IDENT, text, start_col);
            }
        }

        // 整型常量和long整型常量，支持十进制、十六进制、八进制
        if (std::isdigit(c)) {
            text += static_cast<char>(c);
            column++;
            bool isHex = false, isOct = false, isFloat = false;
            int base = 10;
            int firstChar = c;
            c = fgetc(file);
            if (firstChar == '0') {
                if (c == 'x' || c == 'X') {
                    text += static_cast<char>(c);
                    column++;
                    isHex = true;
                    base = 16;
                    // 读取十六进制数字
                    while ((c = fgetc(file)), std::isxdigit(c)) {
                        text += static_cast<char>(c);
                        column++;
                    }
                } else if (std::isdigit(c)) {
                    isOct = true;
                    base = 8;
                    // 读取八进制数字
                    while (std::isdigit(c)) {
                        text += static_cast<char>(c);
                        column++;
                        c = fgetc(file);
                    }
                }
            }
            if (!isHex && !isOct) {
                // 十进制或浮点
                while (std::isdigit(c) || c == '.') {
                    if (c == '.') {
                        if (isFloat) break; // 第二个点，非法
                        isFloat = true;
                    }
                    text += static_cast<char>(c);
                    column++;
                    c = fgetc(file);
                }
            }
            // 检查是否为long整型常量（如123L）
            if (c == 'L' || c == 'l') {
                text += static_cast<char>(c);
                column++;
                return makeToken(TokenKind::LONG_CONST, text, start_col);
            }
            if (c != EOF) ungetc(c, file);
            if (isFloat)
                return makeToken(TokenKind::FLOAT_CONST, text, start_col);
            else if (isHex || isOct)
                return makeToken(TokenKind::INT_CONST, text, start_col); // 统一为INT_CONST
            else
                return makeToken(TokenKind::INT_CONST, text, start_col);
        }

        // 字符串常量
        if (c == '"') {
            text += static_cast<char>(c);
            column++;
            while ((c = fgetc(file)) != EOF && c != '"') {
                if (c == '\\') { // 处理转义字符
                    text += static_cast<char>(c);
                    column++;
                    c = fgetc(file);
                    if (c == EOF) break;
                }
                text += static_cast<char>(c);
                column++;
            }
            if (c == '"') {
                text += static_cast<char>(c);
                column++;
                return makeToken(TokenKind::STRING_CONST, text, start_col);
            } else {
                return makeToken(TokenKind::ERROR_TOKEN, text, start_col);
            }
        }

        // 注释处理
        if (c == '/') {
            int next = fgetc(file);
            if (next == '/') {
                // 行注释 //
                text = "//";
                column += 2;
                while ((c = fgetc(file)) != EOF && c != '\n') {
                    text += static_cast<char>(c);
                    column++;
                }
                if (c == '\n') {
                    line++;
                    column = 0;
                }
                return makeToken(TokenKind::LINE_COMMENT, text, start_col);
            } else if (next == '*') {
                // 块注释 /**/
                text = "/*";
                column += 2;
                bool endFound = false;
                while ((c = fgetc(file)) != EOF) {
                    text += static_cast<char>(c);
                    column++;
                    if (c == '*') {
                        int peek = fgetc(file);
                        if (peek == '/') {
                            text += '/';
                            column++;
                            endFound = true;
                            break;
                        } else if (peek != EOF) {
                            ungetc(peek, file);
                        }
                    }
                    if (c == '\n') {
                        line++;
                        column = 0;
                    }
                }
                if (!endFound) {
                    return makeToken(TokenKind::ERROR_TOKEN, text, start_col);
                }
                return makeToken(TokenKind::BLOCK_COMMENT, text, start_col);
            } else {
                if (next != EOF) ungetc(next, file);
                return makeToken(TokenKind::DIV, "/", start_col);
            }
        }

        // 单/多字符运算符、定界符
        column++;
        switch (c) {
            case '=': {
                int next = fgetc(file);
                if (next == '=') {
                    column++;
                    return makeToken(TokenKind::EQ, "==", start_col);
                } else {
                    if (next != EOF) ungetc(next, file);
                    return makeToken(TokenKind::ASSIGN, "=", start_col);
                }
            }
            case '!': {
                int next = fgetc(file);
                if (next == '=') {
                    column++;
                    return makeToken(TokenKind::NEQ, "!=", start_col);
                } else {
                    if (next != EOF) ungetc(next, file);
                    return makeToken(TokenKind::NOT, "!", start_col);
                }
            }
            case '&': {
                int next = fgetc(file);
                if (next == '&') {
                    column++;
                    return makeToken(TokenKind::AND, "&&", start_col);
                } else {
                    if (next != EOF) ungetc(next, file);
                    return makeToken(TokenKind::ERROR_TOKEN, "&", start_col);
                }
            }
            case '|': {
                int next = fgetc(file);
                if (next == '|') {
                    column++;
                    return makeToken(TokenKind::OR, "||", start_col);
                } else {
                    if (next != EOF) ungetc(next, file);
                    return makeToken(TokenKind::ERROR_TOKEN, "|", start_col);
                }
            }
            case '<': {
                int next = fgetc(file);
                if (next == '=') {
                    column++;
                    return makeToken(TokenKind::LE, "<=", start_col);
                } else {
                    if (next != EOF) ungetc(next, file);
                    return makeToken(TokenKind::LT, "<", start_col);
                }
            }
            case '>': {
                int next = fgetc(file);
                if (next == '=') {
                    column++;
                    return makeToken(TokenKind::GE, ">=", start_col);
                } else {
                    if (next != EOF) ungetc(next, file);
                    return makeToken(TokenKind::GT, ">", start_col);
                }
            }
            case '+': return makeToken(TokenKind::PLUS, "+", start_col);
            case '-': return makeToken(TokenKind::MINUS, "-", start_col);
            case '*': return makeToken(TokenKind::MUL, "*", start_col);
            case '/': return makeToken(TokenKind::DIV, "/", start_col);
            case '%': return makeToken(TokenKind::MOD, "%", start_col);
            case '(': return makeToken(TokenKind::LP, "(", start_col);
            case ')': return makeToken(TokenKind::RP, ")", start_col);
            case '[': return makeToken(TokenKind::LB, "[", start_col);
            case ']': return makeToken(TokenKind::RB, "]", start_col);
            case '{': return makeToken(TokenKind::LC, "{", start_col);
            case '}': return makeToken(TokenKind::RC, "}", start_col);
            case ';': return makeToken(TokenKind::SEMI, ";", start_col);
            case ',': return makeToken(TokenKind::COMMA, ",", start_col);
            default:
                return makeToken(TokenKind::ERROR_TOKEN, std::string(1, static_cast<char>(c)), start_col);
        }
    }

    Token Lexer::makeToken(TokenKind kind, const std::string& text, int col) const {
        return Token{kind, text, line, col ? col : column};
    }
}
