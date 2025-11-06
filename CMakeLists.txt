cmake_minimum_required(VERSION 3.10)
project(Connect4 VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Find SDL2
if(WIN32)
    # Windows - use vcpkg or find_package
    find_package(SDL2 CONFIG REQUIRED)
    find_package(SDL2_ttf CONFIG REQUIRED)
    set(SDL2_LIBRARIES SDL2::SDL2 SDL2::SDL2main)
    set(SDL2_TTF_LIBRARIES SDL2_ttf::SDL2_ttf)
else()
    # Linux/macOS - use pkg-config
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SDL2 REQUIRED sdl2)
    pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)
endif()

# Source files
set(SOURCES
    src/Board.cpp
    src/Game.cpp
    src/GameUI.cpp
    src/RandomAI.cpp
    src/MinimaxAI.cpp
    src/main.cpp
)

# Create executable
add_executable(connect4 ${SOURCES})

# Link SDL2
if(WIN32)
    target_link_libraries(connect4 PRIVATE ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES})
else()
    target_include_directories(connect4 PRIVATE ${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})
    target_link_libraries(connect4 PRIVATE ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES})
endif()

# Platform-specific settings
if(WIN32)
    # Windows specific flags - use GUI subsystem for release
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        set_target_properties(connect4 PROPERTIES WIN32_EXECUTABLE TRUE)
    endif()
else()
    # Linux/Unix specific flags
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif()

# Install target
install(TARGETS connect4 DESTINATION bin)
