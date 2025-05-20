cmake_minimum_required(VERSION 3.16)
project(Jarvis C CXX)

# Use Clang as default compiler if none specified
if(NOT CMAKE_C_COMPILER)
    set(CMAKE_C_COMPILER clang)
endif()
if(NOT CMAKE_CXX_COMPILER)
    set(CMAKE_CXX_COMPILER clang++)
endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON) # for static libs and linking

# Add llama.cpp external library (builds as static library)
add_subdirectory(external/llama.cpp EXCLUDE_FROM_ALL)

# Core library
add_library(jarvis_core STATIC
    src/core/core.c
    # Add other core source files here
)
target_include_directories(jarvis_core PUBLIC include)

# IO library
add_library(jarvis_io STATIC
    src/io/io.c
    # Add other io source files here
)
target_include_directories(jarvis_io PUBLIC include)

# LLM wrapper library (in C++, so CXX)
add_library(jarvis_llm STATIC
    src/llm/llama_wrapper.cpp
    # Add other llama wrapper files here
)
target_include_directories(jarvis_llm PUBLIC include)
target_link_libraries(jarvis_llm PRIVATE llama pthread m)

# Main executable
add_executable(jarvis
    src/main.c
)
target_include_directories(jarvis PRIVATE include)
target_link_libraries(jarvis
    jarvis_core
    jarvis_io
    jarvis_llm
)
