cmake_minimum_required(VERSION 3.10)
project(koralayer_tests C)

# Find CMocka package
find_package(CMocka REQUIRED)

# Enable testing
enable_testing()

# Set compile flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -pedantic")

# Ensure tests can find the required header files
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMOCKA_INCLUDE_DIRS}
)

# Create mock implementation for tests
set(MOCK_SYSCALLS_FILE "${CMAKE_CURRENT_SOURCE_DIR}/mock_syscalls.c")

# Set test source files
set(TEST_FILES
    test_fileinfo.c
    test_dir.c
    test_io.c
    test_putc.c
    test_unlink.c
    test_rename.c
    test_sbrk.c
    test_mmap.c
    test_spawn.c
    test_sched.c
    test_pipe.c
    test_dup.c
    test_select.c
    test_sem.c
    test_time.c
    test_stat.c
    test_signal.c
)

# Platform specific test configurations
if(WIN32)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# Create test executables
foreach(TEST_FILE ${TEST_FILES})
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_FILE})
    
    # Link against koralayer and cmocka
    target_link_libraries(${TEST_NAME}
        PRIVATE
            koralayer
            ${CMOCKA_LIBRARIES}
    )
    
    # Set include directories explicitly for each test
    target_include_directories(${TEST_NAME}
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/../include
            ${CMOCKA_INCLUDE_DIRS}
    )
    
    # Set test properties for better output
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    set_tests_properties(${TEST_NAME} PROPERTIES
        ENVIRONMENT "CMOCKA_MESSAGE_OUTPUT=stdout;CMOCKA_TEST_ABORT=1"
    )
endforeach()

# Add custom target for running tests with verbose output
add_custom_target(check
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -V
) 