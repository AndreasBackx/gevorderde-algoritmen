
cmake_minimum_required(VERSION 3.9)
project (labo3-dynamisch-programmeren)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE debug)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra -O0")
    set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=*;-header-filter=.*")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
endif()

set(SOURCES
    main.cpp
    dnasequentie.h
    bottelmachine.h
    )

add_executable(${PROJECT_NAME} ${SOURCES})
