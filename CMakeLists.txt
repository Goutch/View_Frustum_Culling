cmake_minimum_required(VERSION 3.15)
project(ViewFrustumCulling)

set(CMAKE_CXX_STANDARD 14)

include_directories(GLM)
add_subdirectory(GLM)

add_executable(ViewFrustumCulling main.cpp)