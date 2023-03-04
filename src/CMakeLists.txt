################################################################################
# File: CMakeLists.txt
# Created Date: Sat Mar 04 2023
# Author: Nicolas IRAGNE (nicolas.iragne@alyce.fr)
# -----
# 
# -----
################################################################################

cmake_minimum_required(VERSION 3.10)

find_package(Boost COMPONENTS program_options REQUIRED)

add_library(singleton INTERFACE)
add_library(niragne::singleton ALIAS singleton)

target_include_directories(singleton INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(singleton INTERFACE cxx_std_20)

add_library(configurable INTERFACE)
add_library(niragne::configurable ALIAS configurable)

target_compile_features(configurable INTERFACE cxx_std_20)

target_sources(configurable INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/niragne/configurable.h
)

target_include_directories(configurable INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(configurable INTERFACE
    niragne::singleton
    Boost::program_options
)

# create a more generic target that can be used to link to all the niragne libraries
# if you don't want to be bothered with the details
add_library(niragne INTERFACE)
target_link_libraries(niragne INTERFACE
    niragne::singleton
    niragne::configurable
)
add_library(niragne::niragne ALIAS niragne)