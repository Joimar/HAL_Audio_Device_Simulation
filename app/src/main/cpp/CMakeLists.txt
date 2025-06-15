cmake_minimum_required(VERSION 3.22.1)

add_library( # Sets the name of the library.
        audio_hal

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        audio_hal.cpp )

# Include directories for headers
include_directories(
        ${CMAKE_SOURCE_DIR}/hardware   # Adjust path to actual location of hardware/
        ${CMAKE_SOURCE_DIR}/cutils
)

target_link_libraries(audio_hal log)