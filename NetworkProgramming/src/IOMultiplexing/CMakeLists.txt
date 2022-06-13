cmake_minimum_required(VERSION 3.0.0)
project(IOMultiplexing VERSION 0.1.0)

include(CTest)
enable_testing()



set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

# 设置编译参数
add_subdirectory(select)
add_subdirectory(poll)