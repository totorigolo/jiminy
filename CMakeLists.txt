cmake_minimum_required(VERSION 3.6)
project(Jiminy)

set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_INCLUDE_CURRENT_DIR ON) # Qt : Find includes in corresponding build directories
set(CMAKE_AUTOMOC ON) # Qt : Instruct CMake to run moc automatically when needed
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)
SET_PROPERTY(GLOBAL PROPERTY AUTOMOC_FOLDER automoc)
set(CMAKE_CXX_STANDARD 14)

# g++ flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pipe -Wpointer-arith -Winit-self -Wl,-O1 -Wl,--discard-all -Wl,--no-undefined")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Og -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -Werror")

# Dependancies
set(EXTRA_LIBS
        pthread
        GLEW
        GL
)

### SFML
set(DEBUG_DRAW_SERVER OFF)
if (DEBUG_DRAW_SERVER MATCHES ON)
    set(EXTRA_LIBS
            ${EXTRA_LIBS}
            sfml-system
            sfml-window
    )
    add_definitions(-DDEBUG_DRAW)
endif()

### OpenGL
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLEW_INCLUDE_DIRS} )

### Bullet
find_package(Bullet REQUIRED)
include_directories( ${BULLET_INCLUDE_DIRS} )

### Qt
find_package(Qt5 REQUIRED COMPONENTS Core Widgets Gui)

# Libraries
link_directories(/usr/local/lib/)

# Include source files
add_subdirectory(src)

# Facilitates the use of the common sources
include_directories(src/)
include_directories(src/common/)

# Server executable
add_executable(jiminy_server
        ${COMMON_SRC} ${SERVER_SRC}
)
set_property(TARGET jiminy_server PROPERTY CXX_STANDARD 14)
target_link_libraries(jiminy_server ${EXTRA_LIBS})
target_link_libraries(jiminy_server ${BULLET_LIBRARIES})
target_link_libraries(jiminy_server ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )

# Client executable
qt5_add_resources(RESOURCE_ADDED ${CLIENT_QT_RESOURCES})
add_executable(jiminy_client
        ${COMMON_SRC} ${CLIENT_SRC}
        ${RESOURCE_ADDED}
)
target_link_libraries(jiminy_client ${EXTRA_LIBS})
target_link_libraries(jiminy_client ${BULLET_LIBRARIES})
target_link_libraries(jiminy_client ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )
target_link_libraries(jiminy_client Qt5::Core Qt5::Widgets Qt5::Gui)
