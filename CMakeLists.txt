cmake_minimum_required(VERSION 3.24)
project(image_steganography)

set(CMAKE_CXX_STANDARD 23)

add_executable(image_steganography main.cpp flags.cpp flags.h util.cpp util.h encrypt.cpp encrypt.h)
