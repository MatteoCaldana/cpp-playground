execute_process (
    COMMAND bash -c "python3 -c 'import sys; print(sys.executable)'"
    OUTPUT_VARIABLE whichPython
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
set(PYTHON_EXECUTABLE ${whichPython})

cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
project(cpp-matplotlib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED "ON")
add_compile_options(-O2 -DNDEBUG)

find_package(pybind11 REQUIRED HINTS $ENV{mkPybind11Prefix})
add_executable(${CMAKE_PROJECT_NAME} main.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME} pybind11::embed)