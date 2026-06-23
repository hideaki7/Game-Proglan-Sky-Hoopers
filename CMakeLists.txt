cmake_minimum_required(VERSION 3.10)
project(doodlejump)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Tambahkan include path
include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/entities
    ${CMAKE_SOURCE_DIR}/src/state
)

# Temukan SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Daftar file sumber
set(SOURCES
    src/main.cpp
    src/Game.cpp
    src/entities/Player.cpp
    src/entities/Platform.cpp
    src/entities/Buff.cpp
    src/entities/DoublePointBuff.cpp
    src/entities/RocketBuff.cpp
    src/entities/ShieldBuff.cpp
    src/entities/Portal.cpp
    src/state/MenuState.cpp
    src/state/GameState.cpp
    src/state/GuideState.cpp
)

# File header (opsional untuk IDE)
set(HEADERS
    src/Game.h
    src/entities/Player.h
    src/entities/Platform.h
    src/entities/Buff.h
    src/entities/DoublePointBuff.h
    src/entities/RocketBuff.h
    src/entities/ShieldBuff.h
    src/entities/Portal.h
    src/state/State.h
    src/state/MenuState.h
    src/state/GameState.h
    src/state/GuideState.h
    src/constants.h
)

# Build executable
add_executable(doodlejump ${SOURCES} ${HEADERS})

# Link SFML
target_link_libraries(doodlejump 
    sfml-graphics 
    sfml-window 
    sfml-system
    sfml-audio
)

# Salin folder assets setelah build
add_custom_command(TARGET doodlejump POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    ${CMAKE_BINARY_DIR}/assets
)
