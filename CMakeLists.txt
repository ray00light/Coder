cmake_minimum_required(VERSION 3.7)
project(Coder)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp GaloisField.cpp GaloisField.h ByteGF.cpp ByteGF.h Packet.cpp Packet.h Coder.cpp Coder.h)
add_executable(Coder ${SOURCE_FILES})
