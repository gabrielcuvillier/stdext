project(stdext)

cmake_minimum_required(VERSION 3.8.2)

# Global options
set(CMAKE_CXX_STANDARD 17)

############
# libstdext
############
add_library(stdext STATIC
    src/lib/call_release_func.cpp
    src/lib/error.cpp)

# Includes
target_include_directories(stdext
    # Public
    PUBLIC src/include)
