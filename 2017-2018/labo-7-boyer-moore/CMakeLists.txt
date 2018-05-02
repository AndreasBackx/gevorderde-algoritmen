
set(SOURCES
    main.cpp
    woordstroom.h
    boyermoore.h
    )

set(ADDITIONAL_INPUT_FILES
    )

set(TEST_SOURCES
    boyermoore_test.cpp
    )

set(ADDITIONAL_TEST_FILES
    bible.txt
    capital.txt
    )

### executable #################################################################

cmake_minimum_required(VERSION 3.9)
project (proj)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE debug)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra -O0")
    set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=*,-boost-*,-google-*,-llvm-*;-header-filter=.*")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
endif()

if (SOURCES)
    foreach(FILE ${ADDITIONAL_INPUT_FILES})
        message("Copying file ${FILE}")
        configure_file(${FILE} ${FILE} COPYONLY)
    endforeach(FILE)

    add_executable(${PROJECT_NAME} ${SOURCES})
endif()

### unittest ###################################################################

if (TEST_SOURCES)
    # Download and unpack googletest at configure time
    configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
    execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download)

    if(result)
        message(FATAL_ERROR "CMake step for googletest failed: ${result}")
    endif()

    execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download)

    if(result)
        message(FATAL_ERROR "Build step for googletest failed: ${result}")
    endif()

    # Prevent overriding the parent project's compiler/linker
    # settings on Windows
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    # Add googletest directly to our build. This defines
    # the gtest and gtest_main targets.
    add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
        ${CMAKE_BINARY_DIR}/googletest-build
        EXCLUDE_FROM_ALL)

    # The gtest/gtest_main targets carry header search path
    # dependencies automatically when using CMake 2.8.11 or
    # later. Otherwise we have to add them here ourselves.
    if (CMAKE_VERSION VERSION_LESS 2.8.11)
        include_directories("${gtest_SOURCE_DIR}/include")
    endif()

    foreach(FILE ${ADDITIONAL_TEST_FILES})
        message("Copying file ${FILE}")
        configure_file(${FILE} ${FILE} COPYONLY)
    endforeach(FILE)

    enable_testing()

    foreach(FILE ${TEST_SOURCES})
        # TODO bekijken of we een executable kunnen maken met alle tests erin, 
        # ipv een executable per test file

        string(REGEX REPLACE "\\.[^.]*$" "" FILE_WITHOUT_EXT ${FILE})
        set(TEST_EXECUTABLE_NAME "unittest_${FILE_WITHOUT_EXT}")

        add_executable(${TEST_EXECUTABLE_NAME} ${FILE})
        target_link_libraries(${TEST_EXECUTABLE_NAME} gtest_main gmock)
        add_test("${TEST_EXECUTABLE_NAME}_ctest" ${TEST_EXECUTABLE_NAME})

        add_custom_command(TARGET ${TEST_EXECUTABLE_NAME}
            POST_BUILD
            COMMAND $<TARGET_FILE:${TEST_EXECUTABLE_NAME}>
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Running ${PROJECT_NAME} ${TEST_EXECUTABLE_NAME}" VERBATIM)
    endforeach(FILE)
endif()
