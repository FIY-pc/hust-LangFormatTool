cmake_minimum_required(VERSION 3.28)

# 设置项目名
set(Project_Id hust-formatter)
project(${Project_Id})

# 设置C++标准
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置编译输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/build)

# 拉取CLI11库
include(FetchContent)
FetchContent_Declare(
        cli11_proj
        QUIET
        GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
        GIT_TAG v2.3.2
)
FetchContent_MakeAvailable(cli11_proj)

add_subdirectory(src/lexer)
add_subdirectory(src/parser)
add_subdirectory(src/formatter)

add_executable(${Project_Id} src/main.cpp)

target_link_libraries(
        ${Project_Id} PRIVATE CLI11::CLI11
        lexer
        parser
        formatter
)