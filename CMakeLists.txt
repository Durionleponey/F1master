cmake_minimum_required(VERSION 3.28)
project(OS2 C)

set(CMAKE_C_STANDARD 11)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(OS2
        main.c
        include/util.c
        include/pilot.c
        include/pilot.h
)

target_link_libraries(OS2 ${CURSES_LIBRARIES})
