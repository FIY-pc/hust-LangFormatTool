#include "ast.h"
#include "parser.h"

namespace parser {
    ASTNode *Parser::parseTypeSpec() {
        debugLog("parseTypeSpec", pos);
        if (pos >= tokens.size()) {
            error("type_spec: unexpected end of input, expected type keyword (int/float/char/void)");
            return nullptr;
        }
        auto kind = tokens[pos].kind;
        if (lexer::isTypeSpecifier(kind)) {
            auto* node = new ASTNode{NodeType::TypeSpec};
            node->token = tokens[pos].text;
            pos++;
            debugLog("parseTypeSpec_exit", pos);
            return node;
        } else {
            error("type_spec: expected type keyword (int/float/char/void)");
            return nullptr;
        }
    }
}
