add_library(parser
        parser.cpp
        llparse_top.cpp
        llparse_var.cpp
        llparse_func.cpp
        llparse_type_spec.cpp
        llparse_expr.cpp
        llparse_stmt.cpp
        llparse_stmt_detail.cpp
        ast_display.cpp
)

target_link_libraries(parser PUBLIC lexer)

target_include_directories(parser PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)