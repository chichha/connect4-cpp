cmake_minimum_required(VERSION 3.10)
project(Connect4 VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Source files
set(SOURCES
    src/Board.cpp
    src/Game.cpp
    src/main.cpp
)

# Create executable
add_executable(connect4 ${SOURCES})

# Platform-specific settings
if(WIN32)
    # Windows specific flags
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    # Linux/Unix specific flags
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif()

# Install target
install(TARGETS connect4 DESTINATION bin)
