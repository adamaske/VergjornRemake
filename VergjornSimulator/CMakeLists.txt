cmake_minimum_required(VERSION 3.16)
project(VergjornSimulator CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Collect all sources
file(GLOB_RECURSE SOURCES src/*.cpp)

add_executable(vergjorn_sim ${SOURCES})

target_include_directories(vergjorn_sim PRIVATE src)

target_compile_options(vergjorn_sim PRIVATE
    -Wall -Wextra -Wpedantic
    -O2
)
