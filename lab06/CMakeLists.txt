cmake_minimum_required(VERSION 3.10)

project(sfml-platformer-iter1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SFML_DIR "C:/SFML/lib/cmake/SFML")

find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

add_executable(game
        src/main.cpp
        src/Player/Player.cpp
        src/Game/Game.cpp
        src/Map/Map.cpp
)

target_include_directories(game PRIVATE
        src
        src/Player
        src/Game
        src/Map
)

target_link_libraries(game PRIVATE
        SFML::Graphics
        SFML::Window
        SFML::System
)

add_custom_command(TARGET game POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SFML::System>
        $<TARGET_FILE:SFML::Window>
        $<TARGET_FILE:SFML::Graphics>
        $<TARGET_FILE_DIR:game>
)