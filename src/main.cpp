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
    app.add_option("-f,--file", filename, "Input file");

    // 输出文件参数
    std::string output;
    app.add_option("-o,--output", output, "Output file");

    // 只做词法分析
    app.add_flag_callback("-l,--lex", [&]() {
        std::cout << "Performing lexical analysis on file: " << filename << std::endl;
        FILE *file = fopen(filename.c_str(), "r");
        if (!file) {
            std::cerr << "Failed to open file: " << filename << std::endl;
            exit(EXIT_FAILURE);
        }
        auto lexer=lexer::Lexer(file,true);
        lexer.tokenize();
        return 0;
    });

    // 只做语法分析
    bool parse_only = false;
    app.add_flag("--parse", parse_only, "Only perform syntax analysis and output AST");

    // 只格式化
    bool format_only = false;
    app.add_flag("-F,--format", format_only, "Only perform formatting");

    CLI11_PARSE(app, argc, argv);
    return 0;
}