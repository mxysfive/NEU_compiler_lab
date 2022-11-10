cmake_minimum_required(VERSION 3.10)
project(NEUQcompiler)

set(CMAKE_CXX_STANDARD 11)

SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
add_executable(NEUQcompiler main.cpp Scanner/TokenType.h
        Scanner/Scanner.h Scanner/Scanner.cpp
        Parser/tree.h Parser/tree.cpp Parser/LLPredictor.h Parser/LLPredictor.cpp)
