cmake_minimum_required(VERSION 3.5)

project(clang-extension LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories("C:/Program Files/LLVM/include")
link_directories("C:/Program Files/LLVM/lib")

add_executable(clang-extension main.cpp)
target_link_libraries(clang-extension libclang)
