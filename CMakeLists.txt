cmake_minimum_required(VERSION 3.10)

project(wavescript)

# Set the C standard
set(CMAKE_C_STANDARD 99)

# if(APPLE)
#     # Include directories
#     include_directories(/opt/homebrew/Cellar/portaudio/19.7.0/include)
#
#     # Link directories
#     link_directories(/opt/homebrew/Cellar/portaudio/19.7.0/lib)
# endif()


# Specify the path to the Lua library
set(LUA_LIB ${CMAKE_SOURCE_DIR}/lua/src/liblua.a)
set(LUA_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/lua/src)

# Specify the path to the PortAudio library
set(PORTAUDIO_LIB ${CMAKE_SOURCE_DIR}/portaudio/libportaudio.a)
set(PORTAUDIO_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/portaudio/include)

# Add the executable
add_executable(wavescript ./src/main.c)

# Include directories
target_include_directories(wavescript PRIVATE ${LUA_INCLUDE_DIR} ${PORTAUDIO_INCLUDE_DIR})

# Link libraries
if(APPLE)
    target_link_libraries(wavescript ${LUA_LIB} ${PORTAUDIO_LIB} m "-framework CoreAudio" "-framework AudioUnit" "-framework AudioToolbox" "-framework CoreFoundation" "-framework Carbon")
endif()

if(UNIX AND NOT APPLE)
    target_link_libraries(wavescript ${LUA_LIB} ${PORTAUDIO_LIB} m pthread asound)
endif()

# Custom command to create 'out' directory and move binary and scripts
add_custom_command(TARGET wavescript POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/out
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:wavescript> ${CMAKE_SOURCE_DIR}/out/
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/scripts ${CMAKE_SOURCE_DIR}/out/scripts
)
