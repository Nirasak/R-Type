add_executable(r-type_server
        main.cpp
        ServerCore.cpp
        GameLoop.cpp
)

target_include_directories(r-type_server PRIVATE
        ../../include
)

add_subdirectory(Entities)