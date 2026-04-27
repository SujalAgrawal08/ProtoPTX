cmake_minimum_required(VERSION 3.10)
project(ProtoPTX VERSION 1.0)

# Enforce C++17 Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable target
add_executable(ProtoPTX
    main.cpp
    Assembler.cpp
    VM.cpp
)
