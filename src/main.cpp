#include <iostream>
#include "CLI/App.hpp"
#include "CLI/Formatter.hpp"
#include "CLI/Config.hpp"
#include "parser.h"
#include "formatter.h"
#include "lexer.h"



int main(const int argc, char** argv) {
    CLI::App app{"hust-formatter"};
    // 文件名参数
    std::string filename;
    app.add_option("-f,--file", filename, "Input file")->required();

    // 调试标志
    bool debug = false;
    app.add_flag("-v,--verbose", debug, "Enable debug output");

    // 输出文件参数
    std::string output;
    app.add_option("-o,--output", output, "Output file")->default_val("");


    bool lex_order = false;
    bool lex_sort = false;
    bool lex_mode = false;
    bool parse_mode = false;
    bool format_mode = false;
    bool pretty = false;
    app.add_flag("-l,--lex", lex_mode, "Only perform lexical analysis");
    app.add_flag("-p,--parse", parse_mode, "Only perform parsing");
    app.add_flag("-F,--format", format_mode, "Only perform formatting");
    app.add_flag("--lex-order", lex_order, "Print tokens in order");
    app.add_flag("--lex-sort", lex_sort, "Print tokens sorted by kind");
    app.add_flag("-P,--pretty", pretty, "Pretty print output");

    CLI11_PARSE(app, argc, argv);

    if (lex_mode) {
        std::cout << "Performing lexical analysis on file: " << filename << std::endl;
        FILE *file = fopen(filename.c_str(), "r");
        if (!file) {
            std::cerr << "Failed to open file: " << filename << std::endl;
            exit(EXIT_FAILURE);
        }
        lexer::Lexer lexer(file);
        if (pretty) {
            if (lex_sort) {
                lexer.printTokensSortedPretty();
            } else {
                lexer.printTokensOrderPretty();
            }
        } else {
            if (lex_sort) {
                lexer.printTokensSorted();
            } else {
                lexer.printTokensOrder();
            }
        }
        return 0;
    } else if (parse_mode) {
        std::cout << "Performing parsing on file: " << filename << std::endl;
        FILE *file = fopen(filename.c_str(), "r");
        if (!file) {
            std::cerr << "Failed to open file: " << filename << std::endl;
            exit(EXIT_FAILURE);
        }

        parser::Parser parser(file, debug);
        lexer::Lexer &lexer = parser.lexer;
        if (debug) {
            if (pretty) {
                if (lex_sort) {
                    lexer.printTokensSortedPretty();
                } else {
                    lexer.printTokensOrderPretty();
                }
            } else {
                if (lex_sort) {
                    lexer.printTokensSorted();
                } else {
                    lexer.printTokensOrder();
                }
            }
        }
        parser.parse();
        parser.outputAST(output);
        std::cout << "AST output to file: " << output << std::endl;
        return 0;
    } else if (format_mode) {
        if (output.empty()) {
            output = "formatted_" + filename;
        }
        std::cout << "Formatting file: " << filename << " to " << output << std::endl;
        FILE *file = fopen(filename.c_str(), "r");
        if (!file) {
            std::cerr << "Failed to open file: " << filename << std::endl;
            exit(EXIT_FAILURE);
        }
        formatter::Formatter formatter(file, debug, output);
        formatter.format();
        std::cout << "Formatted output to file: " << output << std::endl;
        return 0;
    } else {
        // 默认执行格式化
        if (output.empty()) {
            output = "formatted_output.c";
        }
        std::cout << "Formatting file: " << filename << " to " << output << std::endl;
        FILE *file = fopen(filename.c_str(), "r");
        if (!file) {
            std::cerr << "Failed to open file: " << filename << std::endl;
            exit(EXIT_FAILURE);
        }
        formatter::Formatter formatter(file, debug, output);
        formatter.format();
        std::cout << "Formatted output to file: " << output << std::endl;
        return 0;
    }
}