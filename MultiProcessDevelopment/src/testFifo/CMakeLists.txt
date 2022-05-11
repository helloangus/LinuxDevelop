cmake_minimum_required(VERSION 3.0.0)
project(testFifo VERSION 0.1.0)

include(CTest)
enable_testing()



set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})
add_executable(readFifo.out readFifo.cpp)
add_executable(writeFifo.out writeFifo.cpp)
add_subdirectory(testChat)