cmake_minimum_required(VERSION 3.0.0)
project(epoll VERSION 0.1.0)

include(CTest)
enable_testing()



set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

# 设置编译参数
set(CMAKE_CXX_FLAGS -pthread)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})
add_executable(TestEpollServer.out testEpollServer.cpp)
add_executable(TestEpollClient.out testEpollClient.cpp)