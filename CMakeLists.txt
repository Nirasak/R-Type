cmake_minimum_required(VERSION 3.17)

project(R-Type)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)

set(CONAN_DISABLE_CHECK_COMPILER 1)

conan_basic_setup(TARGETS)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_BINARY_DIR})

if (UNIX)
    add_compile_options(-Wall -Wextra -Weffc++ -g3)
else ()
    add_compile_options(/W4)
endif ()

add_subdirectory(src/Client)
add_subdirectory(src/Server)

add_dependencies(r-type_client r-type_server)

set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES
        ${CMAKE_CURRENT_LIST_DIR}/r-type_client
        ${CMAKE_CURRENT_LIST_DIR}/r-type_server
)

target_include_directories(r-type_server PRIVATE CONAN_INCLUDE_DIRS_ASIO)
target_include_directories(r-type_client PRIVATE CONAN_INCLUDE_DIRS_ASIO)
target_include_directories(r-type_client PRIVATE CONAN_INCLUDE_DIRS_SFML)

if (UNIX)
    target_link_libraries(r-type_server PRIVATE
            dl
            pthread
            CONAN_PKG::asio
    )
    target_link_libraries(r-type_client PRIVATE
            pthread
            CONAN_PKG::sfml
            CONAN_PKG::asio
    )
else()
    target_link_libraries(r-type_client CONAN_PKG::sfml CONAN_PKG::asio)
    target_link_libraries(r-type_server CONAN_PKG::asio)
endif()
