cmake_minimum_required(VERSION 3.14)
project(DyLib)

set(CMAKE_VERBOSE_MAKEFILE off)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

include(cmake/FindGTest.cmake)
include(cmake/FindASIO.cmake)

if (UNIX)
    add_compile_options(-Wall -Wextra -Weffc++)
else ()
    add_compile_options(/W4)
endif()

include(cmake/BuildDynLib.cmake)

if (UNIX AND NOT APPLE)
    add_compile_options(--coverage)
endif()

enable_testing()

add_executable(unit_tests
        DyLib/tests.cpp
        Network/tests.cpp
        ECS/tests.cpp)

target_include_directories(unit_tests PRIVATE .)
target_include_directories(unit_tests PRIVATE ../include)

if (UNIX AND NOT APPLE)
    target_link_libraries(unit_tests PRIVATE
            gcov
    )
endif()

if (UNIX)
    target_link_libraries(unit_tests PRIVATE
            dl
            pthread
    )
endif()

target_link_libraries(unit_tests PRIVATE
        gtest_main
        asio
)

include(GoogleTest)

gtest_discover_tests(unit_tests)

add_dependencies(unit_tests dynlib)
