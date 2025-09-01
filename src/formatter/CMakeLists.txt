add_library(formatter
        formatter.cpp
)

target_include_directories(formatter PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)