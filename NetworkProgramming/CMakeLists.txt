cmake_minimum_required(VERSION 3.0.0)
project(NetworkProgramming VERSION 0.1.0)

include(CTest)
enable_testing()



set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

# 设置编译参数
set(CMAKE_CXX_FLAGS -pthread)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/include/)
aux_source_directory(src DIRSRCS)
add_executable(NetworkProgramming.out networkProgramming_main.cpp ${DIRSRCS})

add_subdirectory(src/BaseTCPConnection)
add_subdirectory(src/MulProcessTCPConnection)
add_subdirectory(src/MulThreadTCPConnection)
add_subdirectory(src/IOMultiplexing)