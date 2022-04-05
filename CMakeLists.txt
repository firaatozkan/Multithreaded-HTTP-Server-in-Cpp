cmake_minimum_required(VERSION 3.0.0)
project(demo VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

include(CTest)
enable_testing()

file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)
add_executable(demo ${SOURCES})

target_include_directories(
        demo
        PUBLIC
        "${PROJECT_SOURCE_DIR}/inc"
)

target_link_libraries(
        demo
        PUBLIC
        pthread
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
