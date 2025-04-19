cmake_minimum_required(VERSION 3.14)
project(ganymede VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# Define source files for our library
# Updated to reflect the new directory structure
file(GLOB_RECURSE LIBRARY_SOURCES
    "src/*.cpp"
)

# Define header files for our library
# Updated to reflect the new directory structure
file(GLOB_RECURSE LIBRARY_HEADERS
    "src/*.h"
    "src/*.hpp"
)

# Create a static library target
add_library(${PROJECT_NAME} STATIC
    ${LIBRARY_SOURCES}
    ${LIBRARY_HEADERS}
)

# Experimental
# Add this line to explicitly set the linker language
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

# Set up include directories for our library
# Modified to make the src directory a public include path
target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Add example executable
add_executable(example examples/example.cpp)
target_link_libraries(example 
    PRIVATE 
        ${PROJECT_NAME}
)

# Set include directories for the example
target_include_directories(example
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Option for building tests
option(BUILD_TESTS "Build the tests" OFF)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
