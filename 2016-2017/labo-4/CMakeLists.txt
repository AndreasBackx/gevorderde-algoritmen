
################################################################################
# running cmake
#
#   mkdir build
#   cd build
#   cmake -G "<your preferred development environment>" ..
#
# <your preferred development environment> can be found by
#   cmake --help
#
################################################################################

cmake_minimum_required(VERSION 3.6.2 FATAL_ERROR)

project("Labo 4 - Btrees")
set(BUILD_NAME "btree")

include_directories(${CMAKE_SOURCE_DIR})

# Main #########################################################################

set(OPTIONS "-g -std=gnu++11")
set(WARNINGS "-Wall -Wpedantic -Wextra -Wconversion")
set(FLAGS "${OPTIONS} ${WARNINGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLAGS}")

add_executable( # Your source code files and headers
    ${BUILD_NAME}

    bknoop.h
    btree.h
    main.cpp
    schijf.h
)

# Testing ######################################################################

enable_testing()

find_package(Threads)

set(GOOGLETEST_DIR /home/martijn/git/googletest/googletest) # Your path to googletest main directory
include_directories(${GOOGLETEST_DIR}/include) # Your path for gtest header files
link_directories(${GOOGLETEST_DIR}/build) # Your path for libgtest.a or libtest_main.a

add_executable( # Your source code files and headers for testing
    test-${BUILD_NAME}
    bknoop.h
    schijf.h
    test/bknoop-unittest.cpp
    test/btree-unittest.cpp
)

target_link_libraries(test-${BUILD_NAME} ${CMAKE_THREAD_LIBS_INIT} gtest gtest_main)

add_test(${BUILD_NAME}-testrun test-${BUILD_NAME})

