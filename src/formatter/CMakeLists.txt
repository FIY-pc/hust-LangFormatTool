add_library(formatter
        formatter.cpp
)

target_link_libraries(formatter PUBLIC parser)

target_include_directories(formatter PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)