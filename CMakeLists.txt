cmake_minimum_required(VERSION 3.6)
project(Jiminy)

set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)
SET_PROPERTY(GLOBAL PROPERTY AUTOMOC_FOLDER automoc)
set(CMAKE_CXX_STANDARD 14)

# g++ flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pipe -Wpointer-arith -Winit-self -Wl,-O1 -Wl,--discard-all -Wl,--no-undefined")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Og -g")
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -Werror")

# Dependancies
set(EXTRA_LIBS
        pthread
        sfml-system
        sfml-audio
        sfml-window
        sfml-graphics
        Box2D
        boost_system
        mlpack
        armadillo
        )

# Headers...
include_directories(
        "/usr/local/include/"
        "/usr/include"
)

# ... and libraries
link_directories(
        "/usr/local/lib/"
)

# Include source files
add_subdirectory(src)

# Facilitates the use of the common sources
include_directories(src/)
include_directories(src/common/)

# Server executable
add_executable(jiminy_server
        ${COMMON_SRC} ${SERVER_SRC}
)
target_link_libraries(jiminy_server ${EXTRA_LIBS})

# Client executable
add_executable(jiminy_client
        ${COMMON_SRC} ${CLIENT_SRC}
)
target_link_libraries(jiminy_client ${EXTRA_LIBS})
