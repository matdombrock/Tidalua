cmake_minimum_required(VERSION 3.10)

project(wavescript)

# Set the C standard
set(CMAKE_C_STANDARD 99)

if(APPLE)
    # Include directories
    include_directories(/opt/homebrew/Cellar/portaudio/19.7.0/include)

    # Link directories
    link_directories(/opt/homebrew/Cellar/portaudio/19.7.0/lib)
endif()

include(ExternalProject)

# Add the Lua library
ExternalProject_Add(
    lua
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/lua
    CONFIGURE_COMMAND ""
    BUILD_COMMAND make all MYCFLAGS="-fPIC" MYLDFLAGS=""
    INSTALL_COMMAND ""
    BUILD_IN_SOURCE 1
)

# Specify the path to the Lua library
set(LUA_LIB ${CMAKE_SOURCE_DIR}/lua/src/liblua.a)
set(LUA_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/lua/src)

# Add the executable
add_executable(wavescript ./src/main.c)

# Include directories
target_include_directories(wavescript PRIVATE ${LUA_INCLUDE_DIR})

# Link libraries
add_dependencies(wavescript lua)
target_link_libraries(wavescript ${LUA_LIB} portaudio m)



# Custom command to create 'out' directory and move binary and scripts
add_custom_command(TARGET wavescript POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/out
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:wavescript> ${CMAKE_SOURCE_DIR}/out/
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/scripts ${CMAKE_SOURCE_DIR}/out/scripts
)
