cmake_minimum_required(VERSION 3.20)

project(test-cppmatplot)

execute_process (
    COMMAND bash -c "python3 -c 'import sys; print(sys.executable)'"
    OUTPUT_VARIABLE PYTHON_EXECUTABLE
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
find_package(pybind11 REQUIRED HINTS $ENV{mkPybind11Prefix})

#execute_process (COMMAND bash -c "python3 ${CMAKE_SOURCE_DIR}/../lib/meta/build.py")


add_executable(${CMAKE_PROJECT_NAME} main.cpp)

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/../lib/src)
target_link_libraries(${CMAKE_PROJECT_NAME} pybind11::embed)