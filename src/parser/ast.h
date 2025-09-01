#ifndef AST_H
#define AST_H

#include <string>
#include <vector>

namespace parser {
    struct ASTNode {
        std::string type;
        std::vector<ASTNode*> children;
        std::string token;
        void print(int depth = 0) {
            for (int i = 0; i < depth; ++i) std::cout << "  ";
            std::cout << type;
            if (!token.empty()) std::cout << ": " << token;
            std::cout << std::endl;
            for (auto child : children) {
                child->print(depth + 1);
            }
        }
        ~ASTNode() {
            for (auto child : children) {
                delete child;
            }
        }
    };
}

#endif //AST_H
