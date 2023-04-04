cmake_minimum_required(VERSION 3.5)

project(NeuralNet LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-O3 -DNDEBUG)


find_package (Eigen3 3.3 REQUIRED NO_MODULE)

add_executable(NeuralNet main.cpp net.h netcrtp.h)
target_link_libraries (NeuralNet Eigen3::Eigen)
