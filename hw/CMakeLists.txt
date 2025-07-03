cmake_minimum_required(VERSION 3.5)

project(ray_tracer LANGUAGES CXX)

# Set the compiler explicitly if not relying on environment
set(CMAKE_CXX_COMPILER icpx)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find all .cpp source files recursively in src/
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Compile and link flags
target_compile_options(${PROJECT_NAME} PRIVATE
    -fsycl
    -Wall
    -pedantic
    -Werror=return-type
    -O3
)

target_link_options(${PROJECT_NAME} PRIVATE
    -fsycl
)

# ------------------------
# SDL2 (Optional for display)
# ------------------------
find_package(SDL2 REQUIRED)

if(SDL2_FOUND)
    message(STATUS "SDL2 found: ${SDL2_INCLUDE_DIRS}")
    target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES})
else()
    message(FATAL_ERROR "SDL2 not found!")
endif()
