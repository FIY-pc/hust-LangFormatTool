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
    private:
        parser::Parser parser;
    };
}

#endif //FORMATTER_H