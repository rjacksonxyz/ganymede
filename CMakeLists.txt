cmake_minimum_required(VERSION 3.14)
project(ganymede VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set optimization flags
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g")
endif()

# Find Threads package
find_package(Threads REQUIRED)

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

# Find Google Benchmark
# Help CMake find Google Benchmark (may need to adjust the path)
list(APPEND CMAKE_PREFIX_PATH "/usr/local/lib/cmake/benchmark")
find_package(benchmark REQUIRED)
find_package(benchmark REQUIRED CONFIG)
if(NOT benchmark_FOUND)
    message(FATAL_ERROR "Google Benchmark not found. Please install it or specify its location.")
endif()

# Enable testing
enable_testing()

# Include GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

# Build examples
add_executable(example_limit examples/example_limit.cpp)
add_executable(example_market examples/example_market.cpp)
set(EXAMPLE_TARGETS example_limit example_market)
foreach(TARGET ${EXAMPLE_TARGETS})
    target_link_libraries(${TARGET} 
        PRIVATE 
            ${PROJECT_NAME}
    )
    target_include_directories(${TARGET}
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
endforeach()

# Build benchmarks
add_executable(benchmark_limit benchmarks/benchmark_limit.cpp)
add_executable(benchmark_market benchmarks/benchmark_market.cpp)
set(EXAMPLE_TARGETS benchmark_limit benchmark_market)
foreach(TARGET ${EXAMPLE_TARGETS})
    target_link_libraries(${TARGET} 
        PRIVATE 
            ${PROJECT_NAME}
            benchmark::benchmark
            Threads::Threads
    )
    target_include_directories(${TARGET}
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
endforeach()

# Build tests
add_executable(test_market tests/test_market_orders.cpp)
add_executable(test_limit tests/test_limit_orders.cpp)
set(TEST_TARGETS test_market test_limit)
foreach(TARGET ${TEST_TARGETS})
    target_link_libraries(${TARGET} 
        PRIVATE 
            ${PROJECT_NAME}
            gtest
            gtest_main
            Threads::Threads
    )
    target_include_directories(${TARGET}
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
    add_test(NAME ${TARGET} COMMAND ${TARGET})
endforeach()
