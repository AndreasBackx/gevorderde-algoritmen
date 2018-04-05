
set(SOURCES
    )

set(ADDITIONAL_INPUT_FILES
    )

set(TEST_SOURCES
    )

set(ADDITIONAL_TEST_FILES
    )

### executable #################################################################

cmake_minimum_required(VERSION 3.9)
project (proj)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE debug)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra")
    set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=*;-header-filter=.*")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
endif()

if (SOURCES)
    foreach(file ${ADDITIONAL_INPUT_FILES})
        configure_file(${file} ${file} COPYONLY)
    endforeach(file)

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

    foreach(file ${ADDITIONAL_TEST_FILES})
        configure_file(${file} ${file} COPYONLY)
    endforeach(file)

    enable_testing()
    add_executable(unittests ${TEST_SOURCES})
    target_link_libraries(unittests gtest_main gmock)
    add_test(unittests unittests)

    add_custom_command(TARGET unittests
        POST_BUILD
        COMMAND $<TARGET_FILE:unittests>
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Running ${PROJECT_NAME} unittests" VERBATIM)
endif()
