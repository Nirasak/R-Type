set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

if(MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
endif()

add_library(plane SHARED
        Plane.cpp)

target_include_directories(plane PRIVATE
        ../../../include
)

set_target_properties(plane PROPERTIES PREFIX "")

#

add_library(cyborg SHARED
        Cyborg.cpp)

target_include_directories(cyborg PRIVATE
        ../../../include
)

set_target_properties(cyborg PROPERTIES PREFIX "")

#

add_library(block SHARED
        Block.cpp)

target_include_directories(block PRIVATE
        ../../../include
)

set_target_properties(block PROPERTIES PREFIX "")