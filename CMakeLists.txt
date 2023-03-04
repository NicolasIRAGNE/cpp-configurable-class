################################################################################
# File: CMakeLists.txt
# Created Date: Sat Mar 04 2023
# Author: Nicolas IRAGNE (nicolas.iragne@alyce.fr)
# -----
#
# -----
################################################################################

cmake_minimum_required(VERSION 3.10)

project(configurable_test)

add_executable(configurable_test test/main.cpp test/test.h)
target_include_directories(configurable_test PRIVATE test)

add_subdirectory(src)

target_link_libraries(configurable_test PUBLIC niragne::configurable)
