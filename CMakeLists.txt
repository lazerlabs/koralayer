cmake_minimum_required(VERSION 3.10)
project(koralayer C)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Set compile flags
if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

# Detect platform and set appropriate platform-specific sources
if(UNIX AND NOT APPLE)
    set(PLATFORM_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/linux/syscalls_linux.c")
    add_compile_definitions(KORA_PLATFORM_LINUX)
elseif(APPLE)
    set(PLATFORM_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/macos/syscalls_macos.c")
    add_compile_definitions(KORA_PLATFORM_MACOS)
elseif(WIN32)
    set(PLATFORM_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/windows/syscalls_windows.c")
    add_compile_definitions(KORA_PLATFORM_WINDOWS)
endif()

# Add include directories globally
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Add source files
file(GLOB COMMON_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

# Create the main library (combine common sources with platform-specific sources)
add_library(koralayer STATIC ${COMMON_SOURCES} ${PLATFORM_SOURCES})

# Set include directories for the library
target_include_directories(koralayer
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Enable testing
enable_testing()

# Add tests subdirectory
add_subdirectory(tests)

# Platform specific configurations
if(WIN32)
    target_compile_definitions(koralayer PRIVATE WIN32_LEAN_AND_MEAN)
endif()

# Installation rules
install(TARGETS koralayer
    EXPORT koralayer-targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(DIRECTORY include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)

# Export targets
install(EXPORT koralayer-targets
    FILE koralayer-targets.cmake
    NAMESPACE koralayer::
    DESTINATION lib/cmake/koralayer
) 