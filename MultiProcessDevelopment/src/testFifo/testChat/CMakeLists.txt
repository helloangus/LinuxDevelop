cmake_minimum_required(VERSION 3.0.0)
project(testChat VERSION 0.1.0)

include(CTest)
enable_testing()



set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

include_directories(${PROJECT_SOURCE_DIR})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})
add_executable(chatA.out chatA.cpp chatFunc.cpp mulChatFunc.cpp)
add_executable(chatB.out chatB.cpp chatFunc.cpp mulChatFunc.cpp)