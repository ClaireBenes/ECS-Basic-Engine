cmake_minimum_required(VERSION 3.20)
project(game)

set(CMAKE_CXX_STANDARD 20)

file(GLOB GAME_SOURCES *.cpp)
add_library(game ${GAME_SOURCES} "EcsManager.cpp"  "SceneGame.cpp" "Game.cpp")
#target_compile_definitions(game PUBLIC GDEBUG)
target_include_directories(game PUBLIC engine)
target_compile_definitions(game PUBLIC TRACY_ENABLE)
target_link_libraries(game raylib engine)