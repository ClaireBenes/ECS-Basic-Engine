cmake_minimum_required(VERSION 3.20)
project(ecs-game)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
## -- Raylib
set(RAYLIB_VERSION 4.5.0)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
include(FetchContent)
if (UNIX)
FetchContent_Declare(
raylib
URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
)
elseif (WIN32)
FetchContent_Declare(
raylib
URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.zip
)
endif()
FetchContent_GetProperties(raylib)
if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
set(FETCHCONTENT_QUIET NO)
FetchContent_Populate(raylib)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
endif()
endif()
# Add code
add_subdirectory( engine )
add_subdirectory( game )
# Declaring our executable
add_executable(${PROJECT_NAME} game/main.cpp)
target_link_libraries(${PROJECT_NAME} game engine)
# Copying assets to the build folder
add_custom_command(
TARGET ${PROJECT_NAME} POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_directory
${CMAKE_CURRENT_LIST_DIR}/assets
$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
COMMENT "---- Copy Assets"
)