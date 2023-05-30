cmake_minimum_required(VERSION 3.0.0)

include_directories(include)
link_directories(lib)

project(DictationMaster VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(DictationMaster main.cpp)
if(WIN32)
target_link_libraries(DictationMaster "libyaml-cpp-win.a")
ELSE()
target_link_libraries(DictationMaster "libyaml-cpp-linux.a")
ENDIF()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
