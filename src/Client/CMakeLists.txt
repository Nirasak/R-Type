add_executable(r-type_client
        main.cpp
        UI/Menu.cpp
        UI/Inputbox.cpp
        UI/Core.cpp
        UI/Lobby.cpp
        UI/Textbox.cpp
	UI/Spritesheet.cpp
        UI/Data.cpp
        UI/Profile.cpp
        UI/Game.cpp
)

target_include_directories(r-type_client PRIVATE
        ../../include
)
