# crackdown - ReXGlue Recompiled Project
#
# SDK boilerplate lives in cmake/rexglue.cmake.

cmake_minimum_required(VERSION 3.25)
project(crackdown LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/rexglue.cmake)

# Sources
set(CRACKDOWN_SOURCES
    src/main.cpp
    src/cache.cpp
)

if(WIN32)
    add_executable(crackdown WIN32 ${CRACKDOWN_SOURCES})
else()
    add_executable(crackdown ${CRACKDOWN_SOURCES})
endif()

rexglue_setup_target(crackdown GPU_PLUGINS xenos)

# Keep useful crash symbols in Release builds as well.
if(WIN32)
    target_link_options(crackdown PRIVATE -Wl,/debug,/map:crackdown.map)
endif()
