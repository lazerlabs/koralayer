# FindCMocka.cmake
# - Find cmocka
# Find the cmocka libraries
#
#  CMOCKA_INCLUDE_DIRS - where to find cmocka.h, etc.
#  CMOCKA_LIBRARIES    - List of libraries when using cmocka.
#  CMOCKA_FOUND        - True if cmocka found.

# Look for the header file
find_path(CMOCKA_INCLUDE_DIR NAMES cmocka.h
  PATHS
  /usr/local/include
  /usr/include
  /opt/homebrew/include  # For Apple Silicon Macs
  /opt/homebrew/Cellar/cmocka/*/include  # Homebrew specific path
)

# Look for the library
find_library(CMOCKA_LIBRARY NAMES cmocka
  PATHS
  /usr/local/lib
  /usr/lib
  /opt/homebrew/lib  # For Apple Silicon Macs
  /opt/homebrew/Cellar/cmocka/*/lib  # Homebrew specific path
)

# Handle the QUIETLY and REQUIRED arguments and set CMOCKA_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(CMocka
  REQUIRED_VARS CMOCKA_LIBRARY CMOCKA_INCLUDE_DIR
)

if(CMOCKA_FOUND)
  set(CMOCKA_LIBRARIES ${CMOCKA_LIBRARY})
  set(CMOCKA_INCLUDE_DIRS ${CMOCKA_INCLUDE_DIR})
  
  # Print the found paths for debugging
  message(STATUS "CMocka include directory: ${CMOCKA_INCLUDE_DIRS}")
  message(STATUS "CMocka library: ${CMOCKA_LIBRARIES}")
endif()

mark_as_advanced(CMOCKA_INCLUDE_DIR CMOCKA_LIBRARY) 