#!/bin/bash
# KoraOS Layer Build Script

# Set colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[0;33m'
NC='\033[0m' # No Color

# Default configuration
BUILD_DIR="build"
BUILD_TYPE="Debug"
RUN_TESTS=false
CLEAN=false
VERBOSE=false

# Parse command line arguments
while [[ $# -gt 0 ]]; do
  case $1 in
    -h|--help)
      echo "Usage: $0 [options]"
      echo "Options:"
      echo "  -h, --help           Show this help message"
      echo "  -c, --clean          Clean build directory before building"
      echo "  -t, --test           Run tests after building"
      echo "  -r, --release        Build in Release mode (default is Debug)"
      echo "  -v, --verbose        Enable verbose output"
      echo "  -b, --build-dir DIR  Specify build directory (default: build)"
      exit 0
      ;;
    -c|--clean)
      CLEAN=true
      shift
      ;;
    -t|--test)
      RUN_TESTS=true
      shift
      ;;
    -r|--release)
      BUILD_TYPE="Release"
      shift
      ;;
    -v|--verbose)
      VERBOSE=true
      shift
      ;;
    -b|--build-dir)
      BUILD_DIR="$2"
      shift 2
      ;;
    *)
      echo -e "${RED}Error: Unknown option $1${NC}"
      exit 1
      ;;
  esac
done

# Set CMake options
CMAKE_OPTIONS="-DCMAKE_BUILD_TYPE=${BUILD_TYPE}"

# Set make options
if [ "$VERBOSE" = true ]; then
    MAKE_OPTIONS="VERBOSE=1"
else
    MAKE_OPTIONS=""
fi

# Display build configuration
echo -e "${GREEN}Build Configuration:${NC}"
echo -e "  Build Directory: ${YELLOW}${BUILD_DIR}${NC}"
echo -e "  Build Type:      ${YELLOW}${BUILD_TYPE}${NC}"
echo -e "  Clean Build:     ${YELLOW}${CLEAN}${NC}"
echo -e "  Run Tests:       ${YELLOW}${RUN_TESTS}${NC}"
echo -e "  Verbose Output:  ${YELLOW}${VERBOSE}${NC}"
echo ""

# Clean build directory if requested
if [ "$CLEAN" = true ]; then
    echo -e "${GREEN}Cleaning build directory...${NC}"
    rm -rf ${BUILD_DIR}
fi

# Create build directory if it doesn't exist
if [ ! -d ${BUILD_DIR} ]; then
    echo -e "${GREEN}Creating build directory...${NC}"
    mkdir -p ${BUILD_DIR}
fi

# Change to build directory
cd ${BUILD_DIR}

# Configure with CMake
echo -e "${GREEN}Configuring with CMake...${NC}"
cmake ${CMAKE_OPTIONS} ..

if [ $? -ne 0 ]; then
    echo -e "${RED}CMake configuration failed!${NC}"
    exit 1
fi

# Build
echo -e "${GREEN}Building...${NC}"
cmake --build . -- ${MAKE_OPTIONS}

if [ $? -ne 0 ]; then
    echo -e "${RED}Build failed!${NC}"
    exit 1
fi

# Run tests if requested
if [ "$RUN_TESTS" = true ]; then
    echo -e "${GREEN}Running tests...${NC}"
    echo ""
    
    # Run tests with colored output
    CMOCKA_MESSAGE_OUTPUT=stdout ctest -V
    
    # Check if tests passed
    if [ $? -eq 0 ]; then
        echo ""
        echo -e "${GREEN}All tests passed!${NC}"
    else
        echo ""
        echo -e "${RED}Some tests failed!${NC}"
        exit 1
    fi
fi

echo ""
echo -e "${GREEN}Build completed successfully!${NC}"
exit 0 