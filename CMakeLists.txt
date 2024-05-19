cmake_minimum_required(VERSION 3.22.0 FATAL_ERROR)

project(CodenamePedestrian CXX)

set(PROJECT_TARGET ${PROJECT_NAME})
set(SOURCE_DIR "src")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE SOURCES "${SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE HEADERS "${SOURCE_DIR}/*.hpp")

include(FetchContent)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
    GIT_TAG 5.0
)

FetchContent_MakeAvailable(raylib)

add_executable(${PROJECT_TARGET} ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_TARGET} PRIVATE ${SOURCE_DIR})
target_link_libraries(${PROJECT_TARGET} raylib)
