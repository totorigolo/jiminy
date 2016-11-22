cmake_minimum_required(VERSION 3.6)
project(Jiminy)

add_subdirectory(common)
add_subdirectory(client)
add_subdirectory(server)

# Prepend path to source files
FUNCTION(PREPEND var prefix)
    SET(listVar "")
    FOREACH(f ${ARGN})
        LIST(APPEND listVar "${prefix}/${f}")
    ENDFOREACH(f)
    SET(${var} "${listVar}" PARENT_SCOPE)
ENDFUNCTION(PREPEND)

PREPEND(COMMON_SRC ${CMAKE_CURRENT_SOURCE_DIR}/common/ ${COMMON_SRC})
PREPEND(CLIENT_SRC ${CMAKE_CURRENT_SOURCE_DIR}/client/ ${CLIENT_SRC})
PREPEND(SERVER_SRC ${CMAKE_CURRENT_SOURCE_DIR}/server/ ${SERVER_SRC})

# Forwards to upper level
set(COMMON_SRC ${COMMON_SRC} PARENT_SCOPE)
set(CLIENT_SRC ${CLIENT_SRC} PARENT_SCOPE)
set(SERVER_SRC ${SERVER_SRC} PARENT_SCOPE)
