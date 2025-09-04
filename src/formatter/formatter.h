#ifndef FORMATTER_H
#define FORMATTER_H
#include <string>
#include "parser.h"

namespace formatter {
    class Formatter {
    public:
        explicit Formatter(FILE *input,bool debug=false,std::string output="formatted");
        ~Formatter();
        bool debug;
        std::string output;
        void format();
        void formatASTNode(FILE* out,  parser::ASTNode* node, int indent = 0);
        void formatExprNoSemi(FILE* out, parser::ASTNode* node);
    private:
        parser::Parser parser;
    };
}

#endif //FORMATTER_H