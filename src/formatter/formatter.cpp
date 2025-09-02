#include "formatter.h"
#include "parser.h"
#include <string>
#include <utility>

namespace formatter {
    Formatter::Formatter(FILE *input,bool debug,std::string output):
        debug(debug),output(std::move(output)),parser(input,debug)
    {
        parser.parse();
    }

    Formatter::~Formatter() {
        parser.~Parser();
    }

    void Formatter::format() {

    }
}